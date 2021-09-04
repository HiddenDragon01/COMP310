//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    kernel.h
//

#ifndef kernel_h
#define kernel_h

#include <stdio.h>
#include "pcb.h"

#pragma mark Kernel Init Process
int myinit(char *filename);

#pragma mark Kernel Scheduler
int scheduler(void);

#pragma mark Kernel Ready Queue Management
void addToReady(struct PCB *pcb);
void clearReadyQueue(void);
struct PCB* getVictimPCBAtFrame(int frame);

#endif /* kernel_h */
