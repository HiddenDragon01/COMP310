//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    DISK_driver.h
//

#ifndef DISK_driver_h
#define DISK_driver_h

#include <stdio.h>


# define MAX_FILES 20
# define MAX_BLOCKS 10
# define MAX_OPENFILES 50


struct PARTITION{
    int block_size;
    int total_blocks;
    
    // This allows to dynamically allocate space as files grow
    int last_free_block;
} currentPartition;

struct FAT{
    char *filename;
    int file_length;
    int blockPtrs[MAX_BLOCKS];
    int current_location;
} fat[MAX_FILES];

char *blockBuffer;
FILE *active_file_table[MAX_OPENFILES];
int active_file_table_owners[MAX_OPENFILES];

void initIO(void);
float getBlockSize(void);

#pragma mark Mounting Functions
int partition(char *name, int blocksize, int totalblocks);
int mountFS(char *name);

#pragma mark General IO Functions
int openFile(char *name);
void closeFile(int file);
char *readBlock(int file);
int writeBlock(int file, char *data);

#endif /* DISK_driver_h */
