//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    ram.c
//

#include <stdlib.h>
#include <string.h>

#include "ram.h"

char *ram[RAM_CAPACITY];
int nextFreeCell;

# pragma mark RAM Initialization tasks

int initializeRAM(){
    
    for (int i = 0; i < RAM_CAPACITY; i++)
        ram[i] = NULL;
    nextFreeCell = 0;
    
    // Reseting the backing store
    system("rm -r ./BackingStore");
    system("mkdir ./BackingStore");
    
    return 0;
}

# pragma mark RAM Whole File IO Operations

int removeFromRAM(int start, int end){
    
    if (start > end|| start < 0 || end < 0 || start >= RAM_CAPACITY || end >= RAM_CAPACITY){
        printf("RAM cell bulk delete request is out of bounds\n");
        return 1;
    }
    
    for (int i = start; i <= end; i++)
        ram[i] = NULL;
    
    return 0;
}

#pragma mark RAM Individual Cell IO Operations


char* readCell(int cell){
    
    if(cell >= RAM_CAPACITY || cell < 0){
        printf("RAM cell read request is out of bounds\n");
        return NULL;
    }
    
    return ram[cell];
}

int writeCell(int cell, char *buffer){
    
    if(cell >= RAM_CAPACITY || cell < 0){
        printf("RAM cell write request is out of bounds\n");
        return 1;
    }
    
    ram[cell] = strdup(buffer);
    return 0;
}

int freeCell(int cell){
    
    if(cell >= RAM_CAPACITY || cell < 0){
        printf("RAM cell delete request is out of bounds\n");
        return 1;
    }
    
    ram[cell] = NULL;
    return 0;
}
