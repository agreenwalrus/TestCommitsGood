/*
Struct for description of command
*/
#ifndef BRICKS_H
#define BRICKS_H

#define NAME_BUF_SIZE 80

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FOR_NAME "for"
#define WHILE_NAME "while"
#define CMD_NAME "cammand"
#define OPER_AT_VAR_NAME "operate_at_var"
#define IF_NAME "if"

struct whole_command_struct;
struct command_struct;
struct variable_struct;
struct operate_at_variabe_struct;
struct while_cycle_struct;
struct for_cycle_struct;
struct if_branch_struct;
struct list_struct;
struct node_struct;

void freeVariableStruct(struct variable_struct);
void freeCommandStruct(struct command_struct);
void freeOperateAtVariableStruct(struct operate_at_variabe_struct);
void freeWhileCycleStruct(struct while_cycle_struct);
void freeForCycleStruct(struct for_cycle_struct);
void freeIfBranchStruct(struct if_branch_struct);
void freeWholeCommandStruct(struct whole_command_struct);
void freeNodeStruct(struct node_struct);
void freeListStruct(struct list_struct);

void addWholeCommandToList(struct list_struct*, struct whole_command_struct*);


////////////////////////////////

struct list_struct
{
	
	struct node_struct *head;
	struct node_struct *tail;
};

struct command_struct {
	char *name;
	int maxLengthOfArgs;
	int curLengthOfArgs;
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
								//0 ==, 1 !=, 2 -=, 3 +=, 4 ++, 5 =, 6 <, 7 >
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
	int maxAmountOfStates;
	int currentAmountOfStates;
	char **varStates;
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
	unsigned char connectionWithNextBitMask; //0 pipe, 1 semicolon, 2 andand, 3 ||
	struct whole_command_struct *toDo;
	struct node_struct *next;
	struct node_struct *prev;
};




#endif
