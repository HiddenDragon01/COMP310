//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    shell.c
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "shell.h"
#include "shellmemory.h"
#include "interpreter.h"

void parser(char *command, char *parsedCommand[]);

#pragma mark Shell UI Loop

int shellUI() {
    
    printf("Welcome to the Final Project shell!\n");
    printf("***///***<INSERT YOUR NAME AND MCGILL ID HERE>***///***\n");
    
    while(1){
        char command[MAX_BUFFER];
        char *parsedCommand[MAX_TOKENS];
        
        printf("\n$ ");
        scanf("%[^\n]%*c",command);
        
        parser(command, parsedCommand);
        int returnCode = interpreter(parsedCommand);
        
        if(returnCode == 1)
            printf("Unknown command: Type \"help\" to find a list of valid commands with their syntax.\n");
    }
    
    return 0;
}

#pragma mark Shell Parser


void parser(char *command, char *parsedCommand[]) {

    // void parser(char *command, char delimiter, char *parsedCommand[]) returns a pointer to a 2D array where:
    // parsedCommand[0][0] contains the first char of the command name
    // parsedCommand[1][0] contains the first char of the first argument
    // parsedCommand[n][0] contians the first char of the nth arugment

    char buffer[MAX_BUFFER];
    int s=0, b=0, t=0; // counters for string, buffer and tokens
    
    // Quit ahead if the line is not worth parsing
    if(!isalpha(*command))
        return;

    while(*(command+s) != '\0' && t < 6) {
        // skip leading spaces
        for(;*(command+s)==' ';s++);

        // exit if end of string
        if (*(command+s) == '\0') break;

        // otherwise, add word to buffer
        b=0;
        if(*(command+s)== '['){
            s++;
            while(*(command+s)!=']' && *(command+s)!='\n' && *(command+s)!='\0' && *(command+s)!='\r') {
                buffer[b] = *(command+s);
                s++;
                b++;
            }
        }
        else
            while(*(command+s)!=' ' && *(command+s)!=']' && *(command+s)!='\n' && *(command+s)!='\0' && *(command+s)!='\r') {
                buffer[b] = *(command+s);
                s++;
                b++;
            }
        
        
        buffer[b]='\0'; // make it a string

        // create a token
        parsedCommand[t] = strdup(buffer);
        t++;
    }
    parsedCommand[t] = NULL;
    return;
}
