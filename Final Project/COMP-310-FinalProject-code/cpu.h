//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    cpu.h
//

#ifndef cpu_h
#define cpu_h

#include <stdio.h>

#pragma mark CPU Initialization Tasks

int initializeCPU(void);

#pragma mark CPU IO Functions
void setQuanta(int quanta);
void setIR(char *instruction);
void setIP(int ip, int offset);
int readIP(void);
int readOffset(void);

#pragma mark CPU Run Loop
int run(int quanta);

#endif /* cpu_h */
