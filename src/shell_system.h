#ifndef SHELL_SYSTEM
#define SHELL_SYSTEM

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

#include "definitions.h"
#include "bricks.h"
#define HISTORY_FILE "shell_history.txt"

#define START_SIZE 10
#define AMOUNT_OF_SYSTEM_CMDS 6
#define CONSOLE_NAME "Shell for Windows @JR"


void resetHandles();
int initShell();	//init system variables of shell
int reallocGlobalVariableArray();
int reallocAliasArray();
void deleteAlias(char*);
void destroyShell();
void freeMemoryForVariable(struct variable_struct);
int addVariable(struct variable_struct);
int addAlias(struct variable_struct);
char* findVariable (char *);
char* getAlias(char* );
BOOL isMountedCommand(struct command_struct);
void getHistoryFilePath(char*, int);
int initHProccesses (int);
void freeHProccesses();
void WaitForMultipleProcceses();
int addHandleToHProccesses(HANDLE);
int reallocHProccesses();

#endif