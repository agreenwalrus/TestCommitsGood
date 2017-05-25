/*
Struct for description of command
*/
#ifndef BRICKS_H
#define BRICKS_H

#define NAME_BUF_SIZE 80

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "definitions.h"

#define FOR_NAME 				"for"
#define WHILE_NAME 				"while"
#define CMD_NAME 				"cammand"
#define OPER_AT_VAR_NAME 		"operate_at_var"
#define IF_NAME 				"if"

#define NEW_STDOUT_BACK 		0x01		// 0 >>, 1 >, 2 >&, 3 <
#define NEW_STDOUT				0x02
#define NEW_STDIN				0x08
#define NEW_STDERR				0x04				

#define	OPER_CMP_EQU			0x01		//0 ==, 1 !=, 2 -=, 3 +=, 4 ++, 5--, 6 =, 7 <, 8 >
#define OPER_CMP_NOT_EQU		0x02
#define OPER_MIN_EQU			0x04
#define OPER_PLUS_EQU			0x08
#define OPER_INCREM				0x10
#define OPER_DECREM				0x20
#define OPER_EQU				0x40
#define OPER_LESS				0x80
#define OPER_GRET				0x00

#define CONNECT_NO				0x00		
#define CONNECT_PIPE			0x01		//0 pipe, 1 semicolon, 2 andand, 3 ||
#define CONNECT_SEMICOLON		0x02
#define CONNECT_ANDAND			0x04
#define CONNECT_OROR			0x08

struct whole_command_struct;
struct command_struct;
struct variable_struct;
struct operate_at_variabe_struct;
struct while_cycle_struct;
struct for_cycle_struct;
struct if_branch_struct;
struct list_struct;
struct node_struct;
struct redirection_struct;
struct part_redirection_struct;

void freeVariableStruct(struct variable_struct);
void freeCommandStruct(struct command_struct);
void freeOperateAtVariableStruct(struct operate_at_variabe_struct);
void freeWhileCycleStruct(struct while_cycle_struct);
void freeForCycleStruct(struct for_cycle_struct);
void freeIfBranchStruct(struct if_branch_struct);
void freeWholeCommandStruct(struct whole_command_struct);
void freeNodeStruct(struct node_struct);
void freeListStruct(struct list_struct);
void freeRedirecionStruct(struct redirection_struct);

void addWholeCommandToList(struct list_struct*, struct whole_command_struct*);


////////////////////////////////
struct part_redirection_struct
{
	unsigned char type; 	// 0 >>, 1 >, 2 >&, 3 <
	char *fileName;
};

struct redirection_struct
{
	char *inputFile;
	char *outputClearFile;
	char *outputAtTheEndFile;
	char *errorFile;
};

struct list_struct
{
	int size;
	struct redirection_struct *redirection;
	struct node_struct *head;
	struct node_struct *tail;
	BOOL excecAtBackGr;
};

struct command_struct {
	char *nameOfCmd;
	char *args;
};

struct variable_struct
{
	char *varName;
	char *varValue;
};

struct operate_at_variabe_struct
{
	char *name;
	unsigned char operation;	//bit
								//0 ==, 1 !=, 2 -=, 3 +=, 4 ++, 5--, 6 =, 7 <, 8 >
	char *value;
};

struct while_cycle_struct
{
	struct operate_at_variabe_struct *conditional;
	struct list_struct *instractionsToDo;
};

struct for_cycle_struct
{
	char *varName;
	char *varStates;
	struct list_struct *instractionsToDo;
};

struct if_branch_struct
{
	struct operate_at_variabe_struct *conditional;
	struct list_struct *trueWay;
	struct list_struct *falseWay;
};

struct whole_command_struct
{
	char *name;
	unsigned char connectionWithNextBitMask; //0 pipe, 1 semicolon, 2 andand, 3 ||
	union {
		struct command_struct *command;
		struct operate_at_variabe_struct *operate_at_variable;
		struct while_cycle_struct *while_cycle;
		struct for_cycle_struct *for_cycle;
		struct if_branch_struct *if_branch;
	} cmd;
};

struct node_struct
{
	struct whole_command_struct *toDo;
	struct node_struct *next;
	struct node_struct *prev;
};


#endif
