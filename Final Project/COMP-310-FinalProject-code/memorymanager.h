//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    memorymanager.h
//

#ifndef memorymanager_h
#define memorymanager_h

#include <stdio.h>

#include "pcb.h"

#define MANAGER_PAGE_SIZE 4
#define NUMBER_OF_FRAMES 10 // Update RAM_CAPACITY whenever this is changed such that MANAGER_PAGE_SIZE * NUMBER_OF_FRAMES = RAM_CAPACITY

int launcher(FILE *originalFile);

# pragma mark Page and Frame Management Functions

int launcher(FILE *originalFile);
int loadPage(int pageNumber, FILE *f, int frameNumber);
int updatePageTableWithVictim(struct PCB *p, int pageNumber, int frameNumber, int victimFrame);
int updatePageTableWithoutVictim(struct PCB *p, int pageNumber, int frameNumber);
int findVictim(struct PCB *p);
int findFrame(void);
int countTotalPages(FILE *p);

#endif /* memorymanager_h */
