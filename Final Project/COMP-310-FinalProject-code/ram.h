//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    ram.h
//

#ifndef ram_h
#define ram_h

#include <stdio.h>

#define RAM_CAPACITY 40 // Update NUMBER_OF_FRAMES whenever this is changed such that MANAGER_PAGE_SIZE * NUMBER_OF_FRAMES = RAM_CAPACITY
#define MAX_BUFFER 100

#pragma mark RAM Initialization tasks
int initializeRAM(void);

#pragma mark RAM Whole File IO Operations
//int addToRAM(FILE *p, int *start, int *end);
int removeFromRAM(int start, int end);

#pragma mark RAM Individual Cell IO Operations
char* readCell(int cell);
int writeCell(int cell, char *buffer);
int freeCell(int cell);

#endif /* ram_h */
