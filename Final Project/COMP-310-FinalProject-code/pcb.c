//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    pcb.c
//

#include <stdlib.h>
#include <stdio.h>

#include "pcb.h"
#include "memorymanager.h"
#include "ram.h"

#pragma mark PCB Initialization Tasks

struct PCB* makePCB(char *filename, int numberOfPages){

    struct PCB *newPCB = malloc(sizeof(struct PCB));
    
    newPCB->PC = 0;
    newPCB->PC_page = 0;
    newPCB->PC_offset = 0;
    newPCB->pages_max = numberOfPages;
    
    for (int i = 0; i < NUMBER_OF_FRAMES ; i++)
        newPCB->pageTable[i] = -1;
    
    newPCB->filename = (char*)malloc(50);
    strcpy(newPCB->filename, filename);
    
    return newPCB;
}

int removePCBFromRAM(struct PCB *pcb){
    
    // Remove all pages owned by pcb from RAM
    for (int i = 0; i < NUMBER_OF_FRAMES; i++)
        if(pcb->pageTable[i] != -1)
            removeFromRAM(pcb->pageTable[i], pcb->pageTable[i] + (MANAGER_PAGE_SIZE -1));
    
    // Liberating the storage malloced to the data structure
    free(pcb->filename);
    free(pcb);
    return 0;
}
