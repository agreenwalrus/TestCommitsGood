#ifndef LOGIC_H
#define LOGIC_H

#include <Windows.h>
#include <stdio.h>

#include "shell_system.h"
#include "bricks.h"

void printIntroduction();
void intrToTyping();
int excecuteList(struct list_struct *);
int excecuteNode(struct node_struct *, BOOL, BOOL, struct redirection_struct*);
int excecuteCommand(struct command_struct *, char *, char *, char *);
void changeDirectory (char *);
DWORD WINAPI executeForCycle(void* );



#endif