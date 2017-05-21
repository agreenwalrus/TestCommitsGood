/*
Struct for description of command

*/
#ifndef SIMPLE_COMMAND_H
#define SIMPLE_COMMAND_H

#define NAME_BUF_SIZE 80

struct simpleCommand {
	char name[NAME_BUF_SIZE];
	int maxAmountOfArgs;
	int curAmountOfArgs;
	char args[NAME_BUF_SIZE][NAME_BUF_SIZE];
	//char **args;
};

#endif
