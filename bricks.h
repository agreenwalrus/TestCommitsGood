/*
Struct for description of command
*/
#ifndef BRICKS_H
#define BRICKS_H

#define NAME_BUF_SIZE 80

union hole_command;
struct command_struct;
struct variable_struct;
struct operate_at_variabe_struct;
struct while_cycle_struct;
struct for_cycle_struct;
struct if_branch_struct;
struct list_struct;
struct node_struct;



////////////////////////////////




struct list_struct
{
	struct node_struct *head;
	struct node_struct *tail;
};





struct command_struct {
	char name[NAME_BUF_SIZE];
	int maxAmountOfArgs;
	int curAmountOfArgs;
	char **args;
	int maxAmountOfFlags;
	int curAmountOfFlags;
	char **flags;
};

struct variable_struct
{
	char *varName;
	char *varValue;
};

struct operate_at_variabe_struct
{
	char name[NAME_BUF_SIZE];
	unsigned char operation;	//bit
								//0 ==, 1 !=, 2 -=, 3 +=, 4 ++, 5 =, 6 <, 7 >
	char *value;
};

struct while_cycle_struct
{
	struct operateAtVariable *conditional;
	struct list_struct instractionsToDo;
};

struct for_cycle_struct
{
	char varName[NAME_BUF_SIZE];
	char **varStates;
	struct list_struct instractionsToDo;
};

struct if_branch_struct
{
	struct operateAtVariable *conditional;
	struct list_struct trueWay;
	struct list_struct falseWay;
};

union hole_command
{
	struct command_struct command;
	struct operate_at_variabe_struct operate_at_variable;
	struct while_cycle_struct while_cycle;
	struct for_cycle_struct for_cycle;
	struct if_branch_struct if_branch;
};

struct node_struct
{
	union hole_command toDo;
	struct node_struct *next;
	struct node_struct *prev;
};


#endif
