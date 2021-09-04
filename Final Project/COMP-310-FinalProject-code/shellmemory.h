//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    shellmemory.h
//

#ifndef shellmemory_h
#define shellmemory_h

#define MAX_VARS 1000

# pragma mark Shell Memory Initialization Tasks
int initializeShellMemory(void);

# pragma mark Shell Memory IO Operations
int setVariable(char var[], char value[]);
char* readVariable(char var[]);

#endif /* shellmemory_h */
