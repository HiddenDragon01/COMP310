//    Final Project COMP 310
//    Your Name:
//    Your McGill ID: 
//
//    shell.h
//

#ifndef shell_h
#define shell_h

#define MAX_TOKENS 10
#define MAX_BUFFER 100

#pragma mark Shell UI Loop
int shellUI(void);

#pragma mark Shell Parser
void parser(char *command, char *parsedCommand[]);

#endif /* shell_h */
