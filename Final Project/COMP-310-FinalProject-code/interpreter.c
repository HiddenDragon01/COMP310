//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    interpreter.c
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "shell.h"
#include "shellmemory.h"
#include "kernel.h"
#include "memorymanager.h"
#include "DISK_driver.h"
#include "DISK_driver_problem1.h"
#include "DISK_driver_problem2.h"
#include "DISK_driver_problem3.h"

#define MAX_MULTITASKING_PROGRAMS 3

// Implemented here
int interpreter(char *parsedCommand[]);
int helpCommand(void);
int quitCommand(void);
int setCommand(char *parsedCommand[]);
int printCommand(char *parsedCommand[]);
int runCommand(char *parsedCommand[]);
int execCommand(char *parsedCommand[]);

int mountCommand(char *parsedCommand[]);
int writeCommand(char *parsedCommand[]);
int readCommand(char *parsedCommand[]);

// ***///*** Commands you need to add for Final Project ***///***
int writeCommandEXAM(char *parsedCommand[]);
int readCommandEXAM(char *parsedCommand[]);
int openCommandEXAM(char *parsedCommand[]);
int closeCommandEXAM(char *parsedCommand[]);
int seekCommandEXAM(char *parsedCommand[]);

#pragma mark Interpreter Entry Point

int interpreter(char *parsedCommand[]){
    
    // int interpreter(char *parsedCommand) returns:
    // 0, if command finished execution with success
    // 1, if command was not found
    // any int > 1, relaying the error codes of any recognized and implemented commands

    if (parsedCommand[0] == NULL)
        return 2;
    
    
    // Recognized commands
    if(strcmp(&parsedCommand[0][0], "help") == 0)
        return helpCommand();
    
    else if (strcmp(&parsedCommand[0][0], "quit") == 0)
        return quitCommand();
    
    else if(strcmp(&parsedCommand[0][0], "set") == 0)
        return setCommand(parsedCommand);
    
    else if(strcmp(&parsedCommand[0][0], "print") == 0)
        return printCommand(parsedCommand);
    
    else if(strcmp(&parsedCommand[0][0], "run") == 0)
        return runCommand(parsedCommand);
    
    else if(strcmp(&parsedCommand[0][0], "exec") == 0)
        return execCommand(parsedCommand);
    
    else if(strcmp(&parsedCommand[0][0], "mount") == 0)
        return mountCommand(parsedCommand);
    
    else if(strcmp(&parsedCommand[0][0], "write") == 0)
        return writeCommand(parsedCommand);
    
    else if(strcmp(&parsedCommand[0][0], "read") == 0)
        return readCommand(parsedCommand);

    // New commands for final project TODO
    else if(strcmp(&parsedCommand[0][0], "write_EXAM") == 0)
        return writeCommandEXAM(parsedCommand);
    
    else if(strcmp(&parsedCommand[0][0], "read_EXAM") == 0)
        return readCommandEXAM(parsedCommand);

    else if(strcmp(&parsedCommand[0][0], "open_EXAM") == 0)
        return writeCommandEXAM(parsedCommand);
    
    else if(strcmp(&parsedCommand[0][0], "close_EXAM") == 0)
        return readCommandEXAM(parsedCommand);

    else if(strcmp(&parsedCommand[0][0], "seek_EXAM") == 0)
        return seekCommandEXAM(parsedCommand);
    
    // The command was not recognized, therefore it is up to the shell to display error
    else
        return 1;

}

#pragma mark Interpreter Commmand Core Implementations

// Commands return 0, if they executed sucessfully.
// Commands return >1, if they failed to execute for a particular reason

int helpCommand(){
    printf("\n\t\t\t**Help **\n\t\t\t\tVersion 4.0, April 2021 Release\n\nThe following commands are available to you whenever your cursor is blinking next to the \"$\" prompt:\n\nRemember, all input in this shell is case-sensitive, including command names, variable names, and file names.\n\n----------------------------------------------------------------------------------------------------------------\n|\tCommand\t\t|\t\t\t\tDescription\t\t\t\t\t\t|\n---------------------------------------------------------------------------------------------------------------\n|\thelp\t\t| The help command displays this help menu.\t\t\t\t\t\t|\n|---------------------------------------------------------------------------------------------------------------\n|\tquit\t\t| The quit command terminates the shell and returns your previous shell.\t\t|\n|---------------------------------------------------------------------------------------------------------------\n|\tset\t\t| The set command sets the value of a shell variable with the following syntax: \t|\n|\t\t\t| \"set VARIABLENAME STRING\"\t\t\t\t\t\t\t\t|\n|---------------------------------------------------------------------------------------------------------------\n|\tprint\t\t| The print command prints the value of a shell variable with the following syntax: \t|\n|\t\t\t| \"print VARIABLENAME\"\t\t\t\t\t\t\t\t\t|\n|---------------------------------------------------------------------------------------------------------------\n|\trun\t\t| The run command will execute a list of instructions saved on a text file with \t|\n|\t\t\t| the following syntax: \"run SCRIPT.txt\"\t\t\t\t\t\t|\n----------------------------------------------------------------------------------------------------------------\n|\texec\t\t| The exec command will load into RAM and execute conccurently up to 3 programs with \t|\n|\t\t\t| the following syntax: \"exec PROG1.txt PROG2.txt PROG3.txt\"\t\t\t\t|\n----------------------------------------------------------------------------------------------------------------\n|\tmount\t\t| The mount command will mount an existing partition or create a new partition if the \t|\n|\t\t\t| partition name does not exist. The mount command requires a partion name, a number \t|\n|\t\t\t| of blocks, and block size as arguments in this order. It has the following syntax: \t|\n|\t\t\t| \"mount partitionname 20 10\"\t\t\t\t\t\t\t\t|\n----------------------------------------------------------------------------------------------------------------\n|\twrite\t\t| The write command writes a string to a file. It requires a filename and a string \t|\n|\t\t\t| enclosed by \"[]\" as arguments, in this order. It has the following syntax: \t\t|\n|\t\t\t| \"write filename [hello world]\"\t\t\t\t\t\t\t|\n----------------------------------------------------------------------------------------------------------------\n|\tread\t\t| The read command reads the contents of a file into a shell variable. It requires a \t|\n|\t\t\t| filename and a shell variable name as arguments. It has the following syntax: \t|\n|\t\t\t| \"read filename var\"\t\t\t\t\t\t\t\t\t|\n----------------------------------------------------------------------------------------------------------------\n|\tread_EXAM\t| TODO \t\t\t\t\t\t\t\t\t\t\t|\n----------------------------------------------------------------------------------------------------------------\n|\twrite_EXAM\t| TODO \t\t\t\t\t\t\t\t\t\t\t|\n----------------------------------------------------------------------------------------------------------------\n|\topen_EXAM\t| TODO \t\t\t\t\t\t\t\t\t\t\t|\n----------------------------------------------------------------------------------------------------------------\n|\tclose_EXAM\t| TODO \t\t\t\t\t\t\t\t\t\t\t|\n----------------------------------------------------------------------------------------------------------------\n|\tseek_EXAM\t| TODO \t\t\t\t\t\t\t\t\t\t\t|\n----------------------------------------------------------------------------------------------------------------\n");
    
    return 0;
}

int quitCommand(){
    
    printf("Bye!\n \n");
    exit(0);
    
    return 0;
}

int setCommand(char *parsedCommand[]){
    
    if (parsedCommand[1] == NULL || parsedCommand[2] == NULL ){
        printf("The set command sets the value of a shell variable with the following syntax \"set VARIABLENAME STRING\"\n");
        return 6;
    }
    if(!isalnum(*parsedCommand[2]) || !isalnum(*parsedCommand[1])){
        printf("The set command sets the value of an alphanumeric shell variable to an alphanumeric string without spaces.\n");
        return 6;
    }
    
    char *var = &parsedCommand[1][0];
    char *value = &parsedCommand[2][0];
    
    int returnCode = setVariable(var, value);
    
    if (returnCode == 5)
        printf("Failed to set variable. Out of memory. \n");
        
    return returnCode;
}

int printCommand(char *parsedCommand[]){
    
    if (parsedCommand[1] == NULL){
        printf("The print command prints the value of a shell variable with the following syntax:\"print VARIABLENAME\"\n");
        return 6;
    }
    
    char *value = readVariable(parsedCommand[1]);

    if (isalnum(*value))
        printf("%s\n",value);
    else
        printf("Variable does not exist\n");
    
    return 0;
}

int runCommand(char *parsedCommand[]){
    
    // Pointers and setting up of working vairables
    FILE *file;
    char *filename = parsedCommand[1];
    char *readable = "r";
    
    // Opening the file
    file = fopen(filename, readable);
    
    // Checking if the file is present
    if(file == NULL) {
          perror("Script not found");
          return 2;
    }

    // fgets() in a while loop reads the script file line by line until the end of file character is found
    char command[MAX_BUFFER];
    while(fgets(command,MAX_BUFFER,file) != NULL){
        
        char *parsedCommand[5];
        parser(command, parsedCommand);
        int returnCode = interpreter(parsedCommand);
        
        if(returnCode == 1)
            printf("Unknown command: Type \"help\" to find a list of valid commands with their syntax.\n");
    }
    
    fclose(file);
    return 0;
}

int execCommand(char *parsedCommand[]){
    
    // Argument Count Error handeling
    if (parsedCommand[1] == NULL ){ //|| parsedCommand[4] != NULL 
        printf("Wrong syntax. The exec command requires between 1 and 3 valid filenames as arguments.\n");
        return 1;
    }
    
    char *filenames[MAX_MULTITASKING_PROGRAMS];
    char *readable = "r";
    
    for (int i = 1; i <= MAX_MULTITASKING_PROGRAMS; i++) {
        
        if(parsedCommand[i] == NULL)
            break;
        
        filenames[i-1] = parsedCommand[i];
        
        // Checking if the file is available
        FILE *file = fopen(filenames[i-1], readable);
        if (file == NULL){
            printf("File: ");
            printf("%s", filenames[i-1]);
            printf(" has not been found.\n");
            for (int i = 0; i < MAX_MULTITASKING_PROGRAMS; i++)
                filenames[i] = NULL;
            clearReadyQueue();
            return 2;
        }
        
        // Checking if myinit() completed sucessfully
        // Call to launcher() from memorymanager.c replaces all functioality from myinit() from kernel.c
        if(launcher(file) != 0)
            return 3;
    }
    
    // Starting execution of programs
    scheduler();
    
    return 0;
}


int mountCommand(char *parsedCommand[]){
    
    // Argument Count Error handeling
    if (parsedCommand[1] == NULL && parsedCommand[2] == NULL && parsedCommand[3] == NULL ){
        printf("Wrong syntax. The mount command requires a partion name, a number of blocks, and block size as arguments.\n");
        return 1;
    }
    
    // Check if partition exists
    char filename[70];
    snprintf(filename, sizeof(filename), "PARTITION/%s.txt", parsedCommand[1]);
    int blockSize = atoi(parsedCommand[3]);
    int totalBlocks = atoi(parsedCommand[2]);
    
    FILE *file = fopen(filename, "r");
    int status;

    // If file does not exists
    if (file == NULL){
        // Create it with partition()
        //fclose(file);
        status = partition(parsedCommand[1], blockSize, totalBlocks);
    }
    else{
        // Else, call mountFS();
        fclose(file);
        status = mountFS(parsedCommand[1]);
    }
    
    if(status == 0)
        printf("Could not mount volume with name %s.\n" , parsedCommand[1]);
    
    return 0;
}

int writeCommand(char *parsedCommand[]){
    
    // Argument Count Error handeling
    if (parsedCommand[1] == NULL && parsedCommand[2] == NULL){
        printf("Wrong syntax. The write command requires a filename and a string enclosed by \"[]\" as arguments.\n");
        return 1;
    }
    
    // Open file only opens files once, so there is no need to check if it is open already
    int fileID = openFile(parsedCommand[1]);
    
    // Write the string to the file
    float strLenght = strlen(parsedCommand[2]);
    float blockSize = getBlockSize();
    int blocksToWrite = (strLenght / blockSize) + 1;
    
    for (int i = 0; i < blocksToWrite; i++) {
        int ptrMovement = i * getBlockSize();
        char *ptr = (parsedCommand[2] + ptrMovement);
        writeBlock(fileID, ptr);
    }
    
    closeFile(fileID);
    return 0;
}

int readCommand(char *parsedCommand[]){
    
    // Argument Count Error handeling
    if (parsedCommand[1] == NULL && parsedCommand[2] == NULL ){
        printf("Wrong syntax. The read command requires a filename and a shell variable name as arguments.\n");
        return 1;
    }
    
    // Open file only opens files once, so there is no need to check if it is open already
    int fileID = openFile(parsedCommand[1]);
    if(fileID == -1){
        printf("Failed to open file.\n");
        return 2;
    }
    
    // Read the string from the file
    char *ptr = readBlock(fileID);
    if(ptr ==  NULL){
        printf("Read command failed.\n");
        return 2; // Read block failed
    }
    
    char *finalStr = malloc(MAX_BUFFER);
    strcpy(finalStr, ptr);
    
    while(ptr != NULL){
        ptr = readBlock(fileID);
        if (ptr == NULL)
            break;
        strcat(finalStr, ptr);
    }
    
    
    // Write the string to the shell variable
    setVariable(parsedCommand[2], finalStr);
    closeFile(fileID);
    
    return 0;
}



// ***///*** Commands you need to add for Final Project ***///***

int writeCommandEXAM(char *parsedCommand[]){
    // Implement the functionality in the files
    // DISK_driver_problem1.c [.h] that were provided to you

    // You can make visible any data structures that you 
    // might need, by importing the appropriate headers
    // and making needed data structures visible.
    printf("TODO\n");
    return 0;
}

int readCommandEXAM(char *parsedCommand[]){
    // Implement the functionality in the files
    // DISK_driver_problem1.c [.h] that were provided to you

    // You can make visible any data structures that you 
    // might need, by importing the appropriate headers
    // and making needed data structures visible.
    printf("TODO\n");
    return 0;
}

int openCommandEXAM(char *parsedCommand[]){
    // Implement the functionality in the files
    // DISK_driver_problem1.c [.h] that were provided to you

    // You can make visible any data structures that you 
    // might need, by importing the appropriate headers
    // and making needed data structures visible.
    printf("TODO\n");
    return 0;
}

int closeCommandEXAM(char *parsedCommand[]){
    // Implement the functionality in the files
    // DISK_driver_problem2.c [.h] that were provided to you

    // You can make visible any data structures that you 
    // might need, by importing the appropriate headers
    // and making needed data structures visible.

    printf("TODO\n");
    return 0;
}

int seekCommandEXAM(char *parsedCommand[]){
    // Implement the functionality in the files
    // DISK_driver_problem3.c [.h] that were provided to you
    
    // You can make visible any data structures that you 
    // might need, by importing the appropriate headers
    // and making needed data structures visible.

    printf("TODO\n");
    return 0;    
}





