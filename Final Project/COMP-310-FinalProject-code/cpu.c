//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    cpu.c
//

#include "cpu.h"

#include <string.h>
#include <stdlib.h>

#include "ram.h"
#include "shell.h"
#include "memorymanager.h"
#include "interpreter.h"

struct CPU{
    // When IP is -1, that implies an idle and available CPU
    int IP;
    int offset; // Added for Assignment 3
    char IR[1000];
    int quanta;
};

struct CPU cpu;

#pragma mark CPU Initialization Tasks

int initializeCPU(){
    
    cpu.IP = -1;
    cpu.quanta = 2;
    cpu.offset = 0;
    
    return 0;
}

#pragma mark CPU IO Functions

int readIP(){
    return cpu.IP + cpu.offset;
}

int readOffset(){
    return cpu.offset;
}

void setIP(int ip, int offset){
    cpu.IP = ip;
    cpu.offset = offset;
}

void setQuanta(int quanta){
    cpu.quanta = quanta;
}

void setIR(char *instruction){
    if (instruction != NULL)
        strcpy(cpu.IR, instruction);
}

#pragma mark Page Fault Pseudo Instruction

int pageFault(){
    return 1; // Kernel will handle page fault operation from here onwards
}

#pragma mark CPU Run Loop

int run(int quanta){
    
    setQuanta(quanta);
    
    // CPU will run until either the quanta runs out OR cpu.offset == 4, triggering a page fault
    while(cpu.quanta > 0 && cpu.offset < MANAGER_PAGE_SIZE){
        
        printf("%s", cpu.IR);
        
        char **parsedCommand;
        parsedCommand = malloc(5 * sizeof(char*));
        
        parser(cpu.IR, parsedCommand);
        int returnCode = 1;
        //int returnCode = interpreter(parsedCommand);

        if(returnCode == 1)
            printf("Illegal instruction at RAM address: %i \n", readIP());
        
        setQuanta(cpu.quanta -1);
        cpu.offset++;
        
        if(cpu.offset >= MANAGER_PAGE_SIZE)
            return pageFault();
        
        if(cpu.IP < RAM_CAPACITY){
            char *cell = readCell(readIP());
            if(cell == NULL)
                return pageFault();
            setIR(cell);
        }
    }
    

    return 0;
}
