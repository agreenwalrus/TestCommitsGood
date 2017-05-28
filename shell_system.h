#ifndef SHELL_SYSTEM
#define SHELL_SYSTEM

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

#include "definitions.h"
#include "bricks.h"

#define START_SIZE 10
#define AMOUNT_OF_SYSTEM_CMDS 2


void resetHandles();
int initShell();							//init system variables of shell
int reallocGlobalVariableArray();
void destroyShell();
void freeMemoryForVariable(struct variable_struct);
int addVariable(struct variable_struct);
char* findVariable (char *);
BOOL isMountedCommand(struct command_struct);
//int initHProccesses (int);
/*void freeHProccesses();
void WaitForMultipleProcceses();
int addHandleToHProccesses(HANDLE);
int reallocHProccesses();*/

#endif