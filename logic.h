#ifndef LOGIC_H
#define LOGIC_H

#include <Windows.h>
#include <stdio.h>

#include "shell_system.h"
#include "bricks.h"

void printIntroduction();
void intrToTyping();
int execute(struct list_struct *);
int executeBuildInCMD(char *cmdName, char *args);
int executeOtherCMD(struct command_struct *, char *, char *, char *);
int excecuteList(struct list_struct *);
int excecuteNode(struct node_struct *, BOOL, BOOL, struct redirection_struct*, BOOL);
int excecuteCommand(struct command_struct *, char *, char *, char *);
void changeDirectory (char *);
void sleep_shell(int);
DWORD WINAPI executeForCycle(void* );



#endif