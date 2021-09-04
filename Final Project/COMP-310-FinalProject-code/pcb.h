//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    pcb.h
//

#ifndef pcb_h
#define pcb_h

#include <stdio.h>
#include <string.h>

#include "ram.h"
//#include "memorymanager.h"

struct PCB{
    int PC;
    
    // Added for Assignment 3
    int pageTable[10]; // Should be AT MOST equal to NUMBER_OF_FRAMES
    int PC_page;
    int PC_offset;
    int pages_max;
    
    char *filename; // Filename of the file in the Backing Store associated with this PCB
};

#pragma mark PCB Tasks

struct PCB* makePCB(char *filename, int numberOfPages);
int removePCBFromRAM(struct PCB *pcb);

#endif /* pcb_h */
