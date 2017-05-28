#ifndef LOGIC_H
#define LOGIC_H

#include <Windows.h>
#include <stdio.h>

#include "shell_system.h"
#include "bricks.h"

#define INPUT_REDIR 0
#define OUTPUT_REDIR 1
#define ERROR_REDIR 2
#define RESERVED 3

void printIntroduction();
void intrToTyping();
int execute(struct list_struct* );
int executeIfBranch(struct list_struct*);
DWORD WINAPI executeForCycle(void* );
int executeListWithFor(struct list_struct* list);
int executeOtherCMD(struct command_struct *, HANDLE **);
int executeBuildInCMD(char *cmdName, char *args);
int executeList(struct list_struct *);
void changeDirectory (char *);
void sleep_shell(int);




#endif