//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    shellmemory.c
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shellmemory.h"

struct MEM{
    char *var;
    char *value;
};

struct MEM variables[MAX_VARS];

# pragma mark Shell Memory Initialization Tasks

int initializeShellMemory(){
    for (int i = 0; i < MAX_VARS; i++){
        variables[i].var = malloc(sizeof(char*));
        variables[i].value = malloc(sizeof(char*));
    }
    
    return 0;
}

# pragma mark Shell Memory IO Operations

int setVariable(char var[], char value[]){
    
    // int setVariable(char var[], char value[]) returns:
    // 0 if the variable was correctly assigned or overwritten
    // 5 if there is no more space for variables
    
    int i = 0;
    
    while(*variables[i].var != (char)NULL && strcmp(variables[i].var, var) != 0){
        if(i == MAX_VARS -1)
            return 5;
        i++;
    }
    
    variables[i].var = strcpy(variables[i].var, var);
    variables[i].value = strcpy(variables[i].value, value);

    return 0;
}

char* readVariable(char var[]){
    
    // char* readVariable(char var[]) returns:
    // char* to the string if the variable was found
    // NULL pointer if the variable was not found
    
    int i = 0;
    
    while(strcmp(variables[i].var, var) != 0 && *variables[i].var != (char)NULL){
        if(i == MAX_VARS -1)
            return NULL;
        i++;
    }
    
    return variables[i].value;
}
