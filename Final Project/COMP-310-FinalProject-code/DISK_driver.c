//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    DISK_driver.c
//

#include "DISK_driver.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


// Helpers
FILE *partitionFile;
char filename[150];
char writebuffer[50000];
char writebuffer2[50000];

int partitionFolderExists(void);
void updateFATOnDisk(void);
void fseekToDataSection(FILE *file);
void fseekToBlock(FILE *file, int blockID);
void fseekToNextFreeBlock(FILE *file);

void initIO(){
    
    for (int i = 0; i < MAX_FILES; i++) {
        struct FAT blankFile = fat[i];
        blankFile.filename = NULL;
        blankFile.file_length = -1;
        for (int j = 0; j < MAX_BLOCKS; j++)
            blankFile.blockPtrs[j] = -1;
        blankFile.current_location = -1;
    }
    
    for (int i = 0; i < MAX_OPENFILES; i++) {
        active_file_table[i]  = NULL;
        active_file_table_owners[i] = -1;
    }
    
    currentPartition.block_size = -1;
    currentPartition.total_blocks = -1;
    currentPartition.last_free_block = -1;
}

float getBlockSize(){
    return 1.0f*currentPartition.block_size;
}

#pragma mark Mounting Functions
int partition(char *name, int blocksize, int totalblocks){
    
    // Starting fresh
    initIO();
    currentPartition.block_size = blocksize;
    currentPartition.total_blocks = totalblocks;
    currentPartition.last_free_block = 0;
    
    // Checking if "PARTITION folder exists, creating it if not"
    if(! partitionFolderExists())
        system("mkdir ./PARTITION");
    
    // Creating the file and writing the FAT to the disk
    snprintf(filename, sizeof(filename), "PARTITION/%s.txt", name);
    partitionFile = fopen(filename, "w");
    if(partitionFile == NULL)
        return 0;
    updateFATOnDisk();
    
    // Appending blocksize * totalblocks number of '0's
    for (int zeroes = totalblocks*blocksize; zeroes > 0; zeroes--)
        fputc('0', partitionFile);
    
    fclose(partitionFile);
    
    // malloc() block_buffer
    blockBuffer = malloc(currentPartition.block_size);
    
    return 1;
}
int mountFS(char *name){
    
    initIO();
    
    // Opening the file
    snprintf(filename, sizeof(filename), "PARTITION/%s.txt", name);
    partitionFile = fopen(filename, "r");
    if(partitionFile == NULL)
        return 0;
    
    // Reading the contents of currentPartition onto the file by getting the first line into write buffer
    fgets(writebuffer, 50000, partitionFile);
    sscanf(writebuffer, "%i, %i, %i\n", &currentPartition.block_size, &currentPartition.total_blocks, &currentPartition.last_free_block);
    
    // malloc() block_buffer
    blockBuffer = malloc(currentPartition.block_size);
    
    // Writting the contents of fat[MAX_FILES] to the file
    for (int i = 0; i < MAX_FILES; i++) {
        struct FAT currentFile;

        fgets(writebuffer, 50000, partitionFile);
        char *token;
        char const *nullFilename = "(null)";
        char const comma = ',';
        char const semicolon = ';';
        
        // Getting the filenames
        token = strtok(writebuffer, &comma);
        if(token == NULL)
            return 0;
        currentFile.filename = malloc(sizeof(&token));
        strcpy(currentFile.filename, token);
        if(!strcmp(currentFile.filename, nullFilename))
            currentFile.filename = NULL;
        
        // Getting the file sizes
        token = strtok(NULL, &comma);
        currentFile.file_length = atoi(token);
        
        // Getting the file pointers
        fgets(writebuffer, 50000, partitionFile);
        token = strtok(writebuffer, &semicolon);
        currentFile.blockPtrs[0] = atoi(token);
        int j = 1;
        for (; j < MAX_BLOCKS; j++){
            token = strtok(NULL, &semicolon);
            currentFile.blockPtrs[j] = atoi(token);
        }

        // Finishing off
        currentFile.current_location = 0;
        fat[i] = currentFile;
    }
    
    fclose(partitionFile);
    return 1;
}

#pragma mark General IO Functions
int openFile(char *name){
    
    // Checking if there is a free block in the data section to create a new file
    if(currentPartition.last_free_block > currentPartition.total_blocks)
        return -1;
    
    // Lookup on the FAT
    int FATindex = 0;
    for (; FATindex < MAX_FILES; FATindex++)
        if (fat[FATindex].filename == NULL || strcmp(fat[FATindex].filename, name) == 0)
            break;
    
    // Checking if the file is not already open
    int i = 0;
    for (; i < MAX_OPENFILES; i++)
        if(active_file_table_owners[i] == FATindex)
            return FATindex;
    
    // Opening the partition file
    int activeFileIndex = 0;
    for(; activeFileIndex < MAX_OPENFILES; activeFileIndex++)
        if(active_file_table[activeFileIndex] == NULL)
            break;
    if(activeFileIndex == MAX_OPENFILES)
        return -1; // Too many files open at the same time

    active_file_table[activeFileIndex]  = fopen(filename, "r+");
    active_file_table_owners[activeFileIndex] = FATindex;
    
    if(FATindex == MAX_FILES && fat[MAX_FILES -1].filename != NULL){
        // File DNE on the FAT and FAT is full
        fclose(active_file_table[activeFileIndex]);
        active_file_table[activeFileIndex] = NULL;
        active_file_table_owners[activeFileIndex] = -1;
        
        return -1;
    }
    else if(fat[FATindex].filename == NULL){
        // Make a new file
        fat[FATindex].filename = malloc(sizeof(500));
        strcpy(fat[FATindex].filename, name);
        fat[FATindex].current_location = 0;
        fat[FATindex].file_length = 0;
        
        // READ:
        // ALL Seeking is done when doing IO, since the file system is NOT contigous and files do not have a pre-allocated size.
        // Space in the data section will be allocated ON WRITE, so we are done here
        
        // Seeking to the data section
        //fseekToDataSection(active_file_table[activeFileIndex]);
        // Seeking to first block
    }
    else{
        // Open a current file
        // Data structures already initialized
        fat[FATindex].current_location = 0;
        
        // READ:
        // ALL Seeking is done when doing IO, since the file system is NOT contigous and files do not have a pre-allocated size.
        
        // Seeking to the data section
        //fseekToDataSection(active_file_table[activeFileIndex]);
        // Seeking to first block
        
    }
    
    return FATindex;
}

void closeFile(int file){
    
    // Checking if the file is not already open
    int i = 0;
    for (; i < MAX_OPENFILES; i++)
        if(active_file_table_owners[i] == file)
            break;
    
    if(i == MAX_OPENFILES)
        return; // File not acutally open
    
    fclose(active_file_table[i]);
    active_file_table_owners[i] = -1;
    
    return;
}

char *readBlock(int file){
    
    if(file < 0)
        return NULL; // File open opreation probably failed before this call. We should never get here.

    if(fat[file].current_location >= MAX_BLOCKS)
        return NULL;
    
    // Lookup the block to read from
    int blockToRead = fat[file].blockPtrs[fat[file].current_location];
    
    // Seeing the block is valid
    if(blockToRead == -1)
        return NULL;
        
    // Get the file pointer
    FILE *filePtr = NULL;
    int i = 0;
    for (; i < MAX_OPENFILES; i++)
        if(active_file_table_owners[i] == file){
                filePtr = active_file_table[i];
            break;
        }
    if(i == MAX_OPENFILES)
        return NULL; // File was not open before this call
    
    // Seek to the block
    fseekToBlock(filePtr, blockToRead);
    

    // Reading the block char by char into the buffer
    i = 0;
    for (; i < currentPartition.block_size; i++) {
        char c = fgetc(filePtr);
        if(c == '0'){
            blockBuffer[i] = '\0';
            break;
        }
        blockBuffer[i] = c;
    }
    blockBuffer[i] = '\0';
    
    fat[file].current_location += 1;
    
    return blockBuffer;
}
int writeBlock(int file, char *data){
    
    if(file < 0)
        return 0; // File open opreation probably failed before this call. We should never get here.
    
    if(fat[file].current_location >= MAX_BLOCKS)
        return 0; // No more blocks are available for the file
    
    // Appending by default
    fat[file].current_location = fat[file].file_length;
    
    // Get the file pointer
    FILE *filePtr = NULL;
    int i = 0;
    for (; i < MAX_OPENFILES; i++)
        if(active_file_table_owners[i] == file){
            filePtr = active_file_table[i];
            break;
        }
    if(i == MAX_OPENFILES)
        return 1; // File was not open before this call
    
    // Seek to next free block
    fseekToNextFreeBlock(filePtr);
    
    // Writing the block char by char into the buffer
    for (int i = 0; i < currentPartition.block_size; i++) {
        char c = *(data+i);
        if(c == 0)
            c = '0';
        fputc(c, filePtr);
    }
    
    // Updating the file's blockPtr array
    fat[file].blockPtrs[fat[file].current_location] = currentPartition.last_free_block;
    
    currentPartition.last_free_block += 1;
    fat[file].current_location += 1;
    fat[file].file_length +=1;
    if(fat[file].current_location < MAX_BLOCKS)
        fat[file].blockPtrs[fat[file].current_location] = -1;
    
    // Update the FAT on disk
    partitionFile = fopen(filename, "r+");
    updateFATOnDisk();
    fclose(partitionFile);
    
    return 1;
}

#pragma mark Private Helper Functions

int partitionFolderExists(){
    
    struct stat partitionFolder;
    int error = stat("./PARTITION", &partitionFolder);
    
    if (error != 0)
        return 0;

    if(partitionFolder.st_mode & S_IFDIR)
        return 1;
    else
        return 0;
}

void fseekToDataSection(FILE *file){
    
    fseek(file, 0, SEEK_SET);
    
    for (int i = MAX_FILES*2 +1; i > 0; i--)
        fgets(writebuffer, 50000, file);
    return;
}

void fseekToBlock(FILE *file, int blockID){
    
    fseekToDataSection(file);
    
    for (int i = blockID * currentPartition.block_size; i > 0; i--)
        fgetc(file);
    
    fseek(file, 0, SEEK_CUR );
    return;
}


void fseekToNextFreeBlock(FILE *file){
     
    fseekToBlock(file, currentPartition.last_free_block);
    return;
}


void updateFATOnDisk(){
    // This function will simply seek to the begining of the partition file and copy
    // the whole FAT again leaving the file pointer at the begining of the data section
    
    // Seeking to the begining
    fseek(partitionFile, 0, SEEK_SET);
    
    // Writting the contents of currentPartition onto the file
    sprintf(writebuffer, "%i, %i, %i\n", currentPartition.block_size, currentPartition.total_blocks, currentPartition.last_free_block);
    fputs(writebuffer, partitionFile);
    
    // Writting the contents of fat[MAX_FILES] to the file
    for (int i = 0; i < MAX_FILES; i++) {
        struct FAT currentFile = fat[i];
        sprintf(writebuffer, "%i", currentFile.blockPtrs[0]);
        for (int j = 1; j < MAX_BLOCKS; j++)
            sprintf(writebuffer, "%s; %i", writebuffer, currentFile.blockPtrs[j]);
        sprintf(writebuffer2, "%s, %i\n %s\n", currentFile.filename, currentFile.file_length, writebuffer);
        fputs(writebuffer2, partitionFile);
    }
    
    return;
}
