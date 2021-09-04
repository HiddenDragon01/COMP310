//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    memorymanager.c
//

#include "memorymanager.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "kernel.h"
#include "ram.h"

char storeFileName[50];
int storeFileCount = 0;

int countTotalPages(FILE *p);
int loadPage(int pageNumber, FILE *f, int frameNumber);

int updatePageTableWithVictim(struct PCB *p, int pageNumber, int frameNumber, int victimFrame);
int updatePageTableWithoutVictim(struct PCB *p, int pageNumber, int frameNumber);

int launcher(FILE *originalFile){
    
    // 1 - Copy the entire file into the Backing Store
        
        // 1.1 - Creating the store file
            snprintf(storeFileName, sizeof(storeFileName), "BackingStore/StoreFile-%d.txt", storeFileCount);
            storeFileCount++;
            FILE *storeFile = fopen(storeFileName, "w");
            
        // 1.2 - Coping the file into the backing store
            char nextChar = fgetc(originalFile);
            while(nextChar != EOF){
                fputc(nextChar, storeFile);
                nextChar = fgetc(originalFile);
            }
        
    // 2 - Close the orginal file Pointer
        fclose(originalFile);
        fclose(storeFile);
    
    // 3 - Open the file in the Backing Store
        storeFile = fopen(storeFileName, "r");
    
    // 4 - Create the PCB and add it to the ready queue
        struct PCB *pcb = makePCB(storeFileName, countTotalPages(storeFile));
        addToReady(pcb);
    
    // 5 - Load the first two pages to RAM
        int totalPages = pcb->pages_max;
        int frame = findFrame();
        
        // Only loading the page into frame if there is space for it
        if (frame == -1){
            frame = findVictim(pcb);
            loadPage(0, storeFile, frame);
            updatePageTableWithVictim(pcb, 0, frame, frame);
        }
        else{
            loadPage(0, storeFile, frame);
            updatePageTableWithoutVictim(pcb, 0, frame);
            
            // Only adding the second page if no victim was needed to minimize swapping
            if (totalPages >= 2){
                frame = findFrame();
                if (frame != -1){
                    loadPage(1, storeFile, frame);
                    updatePageTableWithoutVictim(pcb, 1, frame);
                }
            }

        }
    fclose(storeFile);
    
    return 0;
}

# pragma mark Page and Frame Management Functions

int countTotalPages(FILE *p){
    
    float numberOfLines = 0;
    
    char line[MAX_BUFFER];
    while(fgets(line,MAX_BUFFER,p) != NULL)
        numberOfLines += 1;
    
    float pageCount = numberOfLines/MANAGER_PAGE_SIZE;
    return pageCount;
}

int loadPage(int pageNumber, FILE *f, int frameNumber){
    
    int linesToSkip = pageNumber * MANAGER_PAGE_SIZE;
    int cellNumber = frameNumber * MANAGER_PAGE_SIZE;
    
    fseek(f, 0, SEEK_SET);
    
    char line[MAX_BUFFER];
    while(fgets(line,MAX_BUFFER,f) != NULL){
        
        if(-MANAGER_PAGE_SIZE < linesToSkip && linesToSkip <= 0){
            writeCell(cellNumber, line);
            cellNumber += 1;
        }
        
        linesToSkip -= 1;
    }
    
    
    return 0;
}

int findFrame(){
    
    int currentFrame = 0;
    
    while(readCell(currentFrame) != NULL && currentFrame <= NUMBER_OF_FRAMES)
        currentFrame += MANAGER_PAGE_SIZE;
    
    currentFrame /= 4;
    
    if (currentFrame >= NUMBER_OF_FRAMES)
        return -1;
    
    return currentFrame;
}

int findVictim(struct PCB *p){
    
    int randomFrame = rand() % (NUMBER_OF_FRAMES +1);
    int attempsLeft = NUMBER_OF_FRAMES -1;
    
    while(p->pageTable[randomFrame] == 1 && attempsLeft >= 0){
        randomFrame = (randomFrame + 1) % NUMBER_OF_FRAMES;
        attempsLeft -= 1;
    }
    
    return randomFrame;
}

// Without Victim
int updatePageTableWithoutVictim(struct PCB *p, int pageNumber, int frameNumber){
    
    // Updating the page table of the pcb
    p->pageTable[pageNumber] = frameNumber;
    
    return 0;
}


// With Victim
int updatePageTableWithVictim(struct PCB *p, int pageNumber, int frameNumber, int victimFrame){
    
    // Updating the page table of the pcb
    p->pageTable[pageNumber] = frameNumber;
    
    
    // Getting the victim pcb from the kernel
    struct PCB *victimPCB = getVictimPCBAtFrame(victimFrame);
    
    // Updating its page table
    if(victimPCB == NULL)
        return 1;
    
    else
        victimPCB->pageTable[pageNumber] = -1;
    
    return 0;
}
