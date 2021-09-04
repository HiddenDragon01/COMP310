//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    kernel.c
//

#include "kernel.h"

#include <stdlib.h>

#include "shell.h"
#include "shellmemory.h"
#include "ram.h"
#include "cpu.h"
#include "pcb.h"
#include "memorymanager.h"

#define DEFAULT_QUANTA 2

struct QueueNode{
    struct PCB *PCB;
    struct QueueNode *nextNode;
};

struct Queue{
    struct QueueNode *head;
    struct QueueNode *tail;
};

struct Queue readyQueue;

void boot(void);
int kernel(void);
int initializeKernel(void);
void handlePageFault(struct PCB *pcb);

#pragma mark General Entry Point

int main(int argc, const char * argv[]) {
    int error = 0;
    boot();
    error = kernel();
    return error;
}

#pragma mark Kernel Initialization Tasks

void boot(){
    
    // Initialize data structures
    initializeKernel();
    initializeRAM();
    initializeCPU();
    initializeShellMemory();
    // IO is initialized at mounting of the volume
    
    printf("Kernel 4.0 loaded!\n");
    
    // Launch Shell
    shellUI();
}

int kernel(){
    return shellUI();
}

int initializeKernel(){
    readyQueue.head = NULL;
    readyQueue.tail = NULL;
    return 0;
}

#pragma mark Kernel Init and Kill Process


void mykill(struct QueueNode *node){
    removePCBFromRAM(node->PCB);
    free(node);
    return;
}

#pragma mark Kernel Scheduler

int scheduler(){
    
    int quanta = DEFAULT_QUANTA;
    
    if (readyQueue.head != NULL)
        setIP(readyQueue.head->PCB->PC_page * MANAGER_PAGE_SIZE, readyQueue.head->PCB->PC_offset);
    
    // Check if CPU is available (Quanta is finished or nothing is currently assigned)
    // When IP is -1, that implies an idle and available CPU
    while(readIP() != -1){
    
        // Copy the Program Counter from the PCB to the CPU Instruction Counter
        struct PCB *currentPCB = readyQueue.head->PCB;
        setIP(currentPCB->pageTable[currentPCB->PC_page]*4, currentPCB->PC_offset);
        
        char *cell = readCell(currentPCB->pageTable[currentPCB->PC_page]*4 +currentPCB->PC_offset);
//        for (int offset = 1; offset < 4; offset++) {
//            if(cell != NULL) break;
//            cell = readCell(currentPCB->pageTable[currentPCB->PC_page]*4 +currentPCB->PC_offset+offset);
//        }
        setIR(cell);
        
        int pageFault = currentPCB->pageTable[currentPCB->PC_page];
        
        if(pageFault != -1)
            // Run the program for the specified quanta (default = 2)
            // run() returns 0 if quanta terminated without page fault
            // run() returns 1 if quanta was interrupted by page fault psudo instruction
            pageFault = run(quanta);
        
        else
            pageFault = 1;
        
        if(pageFault){
            handlePageFault(currentPCB);
        }
        else{
            // Updating the PCB
            currentPCB->PC = currentPCB->PC + quanta;
            currentPCB->PC_offset = readOffset();
        }
        
        // Check if the ready queue is empty
        if (readyQueue.head->nextNode == NULL){
            
            if (currentPCB->PC_page > currentPCB->pages_max){
                // Queue is empty and all programs are done executing
                setIP(-1, 0);
                mykill(readyQueue.head);
                readyQueue.head = NULL;
                readyQueue.tail = NULL;
                return 0;
            }
            else{
                // There is only one process cycling in the queue, so let's stop interrupting it
                quanta = RAM_CAPACITY;
            }
        }
        
        else{
            // Get the next process in the ready queue and append this current one to the end
            struct QueueNode *newHead = readyQueue.head->nextNode;
            
            // If the program is done executing, kill it and liberate ressources
            if (currentPCB->PC_page > currentPCB->pages_max){
                mykill(readyQueue.head);
                readyQueue.head = newHead;
            }
            
            else {
                // Updating the queue
                readyQueue.tail->nextNode = readyQueue.head;
                readyQueue.tail = readyQueue.head;
                readyQueue.tail->nextNode = NULL;
                readyQueue.head = newHead;
            }
        }

    }
    
    // Queue is empty and all programs are done executing
    return 0;
}

#pragma mark Kernel Ready Queue Management

void addToReady(struct PCB *pcb){
    
    struct QueueNode *node = malloc(sizeof(struct QueueNode));
    node->PCB = pcb;
    node->nextNode = NULL;
    
    // If ready queue is empty
    if (readyQueue.head == NULL && readyQueue.tail == NULL){
        readyQueue.head = node;
        readyQueue.tail = node;
        return;
    }
    else{
        readyQueue.tail->nextNode = node;
        readyQueue.tail = node;
        return;
    }
    
}

void clearReadyQueue(){
    
    while(readyQueue.head != NULL){
        struct QueueNode *nextNode = readyQueue.head->nextNode;
        mykill(readyQueue.head);
        readyQueue.head = nextNode;
    }
    
    readyQueue.head = NULL;
    readyQueue.tail = NULL;
    return;
}

struct PCB* getVictimPCBAtFrame(int frame){
    
    struct QueueNode *potentialVictim = readyQueue.head;
    
    while(potentialVictim->PCB->pageTable[frame] != -1)
        potentialVictim = potentialVictim->nextNode;
    
    return potentialVictim->PCB;
}

#pragma mark Handle Page Faults

void handlePageFault(struct PCB *pcb){

    // Getting the next available page
    pcb->PC_page += 1;
    
    // Checking if the next page is already present in RAM
    if(pcb->pageTable[pcb->PC_page] != -1){
        
        // It is, then just update the Program Counter and reset the offset
        pcb->PC = pcb->pageTable[pcb->PC_page];
        pcb->PC_offset = 0;
    }
    else{
        // It is not and needs to be fetched from the Backing Store
        
        // Attempting to find a free cell
        int frameNumber = findFrame();
        
        // Checking if a victim is needed
        if(frameNumber == -1){
            frameNumber = findVictim(pcb);
            updatePageTableWithVictim(pcb, pcb->PC_page, frameNumber, frameNumber);
        }
        else
            updatePageTableWithoutVictim(pcb, pcb->PC_page, frameNumber);
        
        // Loading the page to RAM
        FILE *storeFile = fopen(pcb->filename, "r");
        loadPage(pcb->PC_page, storeFile, frameNumber);
        fclose(storeFile);
        
        // Udate the Program Counter and reset the offset
        pcb->PC = pcb->pageTable[pcb->PC_page];
        pcb->PC_offset = 0;
    }
    
    return;
}
