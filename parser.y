/*
Grammar rules for shell.
*/


%include {
	#include<stdio.h>
	#include <stdlib.h>
	#include<assert.h>
	#include <string.h>
	#include "bricks.h"
	#include "shell_system.h"
}


%left ANDAND .
%left OROR .
%right PIPE .

%extra_argument { struct list_struct *list }

%default_type {char *}
%token_type {char *}
%type result { struct list_struct* }
%type input { struct list_struct* }
%type command_line_list { struct list_struct* }
%type command_line { struct command_struct* }
%type command { struct command_struct* }
%type redirection_list { struct redirection_struct* }
%type redirection { struct part_redirection_struct* }
%type argument_list { char* }
%type argument { char* }
%type value { char* }
%type for_cycle { struct for_cycle_struct* }
%type while_cycle { struct while_cycle_struct* }
%type variable { char* }
%type condition { struct operate_at_variabe_struct* }
%type branchig { struct if_branch_struct* }

%syntax_error {  
  printf("\nSyntax error!");  
}  

%parse_failure {
	printf("\nparse failure");
}

%parse_accept {
	printf("\nParsing's been done");
}

%stack_overflow {
	printf("\nParser stack overflow\n");
}

%start_symbol program

program ::= result(res) EOL. { list = res; }

//RESULT

result ::= .	{ printf("\nresult ::= ."); }
result ::= input . { printf("\nresult ::= input"); }
result ::= input AMPERSAND. { printf("\nresult ::= input AMPERSAND"); }
result ::= branchig . { printf("\nresult ::= branchig"); }
//result ::= branchig AMPERSAND . { printf("\nresult ::= branchig AMPERSAND"); }
result ::= variable_declaration . { printf("\nresult ::= variable_declaration"); }
result ::= for_cycle . { printf("\nresult ::= for_cycle"); }
result ::= while_cycle . { printf("\nresult ::= while_cycle"); }


//FOR_CYCLE

for_cycle ::= FOR variable IN argument_list SEMICOLON DO result. { 
	printf("\nfor_cycle ::= FOR variable IN argument_list SEMICOLON DO result"); 
} 

//WHILE_CYCLE

while_cycle ::= WHILE condition SEMICOLON DO result . { 
	printf("\nwhile_cycle ::= WHILE LBRACKET condition RBRACKET SEMICOLON DO result"); 
}

//INPUT

input(inp) ::= command_line_list(cmd_line_list) . {
	inp = cmd_line_list;
}

input(inp) ::= command_line_list(cmd_line_list) redirection_list(red_list) . {
	inp;
	cmd_line_list;
	red_list;
}


//CMDLINELIST

command_line_list(cmd_line_list) ::= command_line(cmd_line) . {
	struct whole_command_struct *whole_cmd = (struct whole_command_struct*) malloc (sizeof(struct whole_command_struct));
	whole_cmd->name = strdup(CMD_NAME);
	whole_cmd->cmd.command = cmd_line;
	whole_cmd->connectionWithNextBitMask = CONNECT_NO;
	
	cmd_line_list = (struct list_struct*) malloc (sizeof(struct list_struct));
	cmd_line_list->redirection = NULL;
	cmd_line_list->head = NULL;
	cmd_line_list->tail = NULL;
	cmd_line_list->size = 0;
	
	addWholeCommandToList(cmd_line_list, whole_cmd);	
}

command_line_list(res_cmd_line_list) ::= command_line_list(cmd_line_list) PIPE command_line(cmd_line) . {
	struct whole_command_struct *whole_cmd = (struct whole_command_struct*) malloc (sizeof(struct whole_command_struct));
	whole_cmd->name = strdup(CMD_NAME);
	whole_cmd->cmd.command = cmd_line;
	whole_cmd->connectionWithNextBitMask = CONNECT_NO;
	
	cmd_line_list->tail->toDo->connectionWithNextBitMask = CONNECT_PIPE;
	
	addWholeCommandToList(cmd_line_list, whole_cmd);
	res_cmd_line_list = cmd_line_list;
}

command_line_list(res_cmd_line_list) ::= command_line_list(cmd_line_list) ANDAND command_line(cmd_line) . {
	struct whole_command_struct *whole_cmd = (struct whole_command_struct*) malloc (sizeof(struct whole_command_struct));
	whole_cmd->name = strdup(CMD_NAME);
	whole_cmd->cmd.command = cmd_line;
	whole_cmd->connectionWithNextBitMask = CONNECT_NO;
	
	cmd_line_list->tail->toDo->connectionWithNextBitMask = CONNECT_ANDAND;
	
	addWholeCommandToList(cmd_line_list, whole_cmd);
	
	res_cmd_line_list = cmd_line_list;
}

command_line_list(res_cmd_line_list) ::= command_line_list(cmd_line_list) OROR command_line(cmd_line) . {
	struct whole_command_struct *whole_cmd = (struct whole_command_struct*) malloc (sizeof(struct whole_command_struct));
	whole_cmd->name = strdup(CMD_NAME);
	whole_cmd->cmd.command = cmd_line;
	whole_cmd->connectionWithNextBitMask = CONNECT_NO;
	
	cmd_line_list->tail->toDo->connectionWithNextBitMask = CONNECT_OROR;
	
	addWholeCommandToList(cmd_line_list, whole_cmd);

	res_cmd_line_list = cmd_line_list;
}
 
//CMDLIST

command_line(cmd_line) ::= command(cmd) . {
	cmd_line = cmd;
}

command_line(cmd_line) ::= command(cmd) argument_list(arg_list) . { 
	cmd->args = arg_list;
	cmd_line = cmd;
}
 
//CMD
command(cmd) ::= FILENAME(name) .	{ 
	cmd = (struct command_struct*) malloc(sizeof(struct command_struct));
	cmd->name = name;
	cmd->args = NULL;
}
command(cmd) ::= ARGUMENT(name) .	{
	cmd = (struct command_struct*) malloc(sizeof(struct command_struct));
	cmd->name = name;
	cmd->args = NULL;
}

//VARIABLE_DECLARATION

//variable_declaration ::= variable(var) EQU value(val) . { printf("\nvariable EQU value %s EQU %s", var, val); }
variable_declaration ::= variable(name) EQU argument_list(arg_list) . { 
	struct variable_struct var;
	var.varName = name;
	var.varValue = arg_list;
	addVariable(var);	
}


//REDIRECTION_LIST

redirection_list(redir_list) ::= redirection(redir) . {
	redir_list = (struct redirection_struct*) malloc (sizeof(struct redirection_struct));
	redir_list->inputFile = NULL;
	redir_list->outputClearFile = NULL;
	redir_list->outputAtTheEndFile = NULL;
	redir_list->errorFile = NULL;
	if (redir->type == NEW_STDOUT)
		redir_list->outputClearFile = redir->fileName;
	else if (redir->type == NEW_STDOUT_BACK)
		redir_list->outputAtTheEndFile = redir->fileName;
	else if (redir->type == NEW_STDIN)
		redir_list->inputFile = redir->fileName;
	else if (redir->type == NEW_STDERR)
		redir_list->errorFile = redir->fileName;
	free(redir); 
}

redirection_list(result_redir_list) ::= redirection_list(redir_list) redirection(redir) . {
	if (redir->type == NEW_STDOUT)
		redir_list->outputClearFile = redir->fileName;
	else if (redir->type == NEW_STDOUT_BACK)
		redir_list->outputAtTheEndFile = redir->fileName;
	else if (redir->type == NEW_STDIN)
		redir_list->inputFile = redir->fileName;
	else if (redir->type == NEW_STDERR)
		redir_list->errorFile = redir->fileName;
	free(redir); 
	result_redir_list = redir_list;
}

//REDIRECTION

redirection(redir) ::= GREAT argument(arg) . {
	redir = (struct part_redirection_struct*) malloc (sizeof(struct part_redirection_struct));
	redir->fileName = arg;
	redir->type = NEW_STDOUT;	
	printf("\nredirection ::= GREAT argument"); 
}

redirection(redir) ::= LESS argument(arg) . {
	redir = (struct part_redirection_struct*) malloc (sizeof(struct part_redirection_struct));
	redir->fileName = arg;
	redir->type = NEW_STDIN;	
	printf("\nredirection ::= LESS argument"); 
}

redirection(redir) ::= GREATAMPERSAND argument(arg) . {
	redir = (struct part_redirection_struct*) malloc (sizeof(struct part_redirection_struct));
	redir->fileName = arg;
	redir->type = NEW_STDERR;
	printf("\nredirection ::= GREATAMPERSAND argument"); 
}

redirection(redir) ::= GREATGREAT argument(arg) . {
	redir = (struct part_redirection_struct*) malloc (sizeof(struct part_redirection_struct));
	redir->fileName = arg;
	redir->type = NEW_STDOUT_BACK;
	printf("\nredirection ::= GREATGREAT argument"); 
}

//BRANCHING

branchig(branch) ::= IF condition(cond) THEN result(trueRes) ELSE result(falseRes) FI . { 
	branch = (struct if_branch_struct*) malloc(sizeof(struct if_branch_struct));
	branch->conditional = cond;
	branch->trueWay = trueRes;
	branch->falseWay = falseRes;
	printf("\nbranchig ::= IF condition THEN result ELSE result FI"); 
}

branchig(branch) ::= IF condition(cond) THEN COLON ELSE result(res) FI . {
	branch = (struct if_branch_struct*) malloc(sizeof(struct if_branch_struct));
	branch->conditional = cond;
	branch->trueWay = NULL;
	branch->falseWay = res;
	printf("\nbranchig ::= IF condition THEN COLON ELSE result FI"); 
}

branchig(branch) ::= IF condition(cond) THEN result(res) ELSE COLON FI.	{
	branch = (struct if_branch_struct*) malloc(sizeof(struct if_branch_struct));
	branch->conditional = cond;
	branch->trueWay = res;
	branch->falseWay = NULL;
	printf("\nbranchig ::= IF condition THEN result ELSE COLON FI"); 
}

//CONDITION

condition(cond) ::= LBRACKET variable(var) LESS argument(arg) RBRACKET . {
	cond = (struct operate_at_variabe_struct*) malloc (sizeof(struct operate_at_variabe_struct));
	cond->name = var;
	cond->value = arg;
	cond->operation = OPER_LESS;	
	printf("\ncondition ::= LBRACKET variable LESS value RBRACKET"); 
}
condition(cond) ::= LBRACKET variable(var) GREAT argument(arg) RBRACKET . {
	cond = (struct operate_at_variabe_struct*) malloc (sizeof(struct operate_at_variabe_struct));
	cond->name = var;
	cond->value = arg;
	cond->operation = OPER_GRET;
	printf("\ncondition ::= LBRACKET variable GREAT value RBRACKET"); 
}
condition(cond) ::= LBRACKET variable(var) EQUEQU argument(arg) RBRACKET . { 
	cond = (struct operate_at_variabe_struct*) malloc (sizeof(struct operate_at_variabe_struct));
	cond->name = var;
	cond->value = arg;
	cond->operation = OPER_CMP_EQU;
	printf("\nLBRACKET variable EQUEQU argument RBRACKET"); 
}

//VARIABLE
variable(var) ::= WORD(ptr) . { 
	var = ptr; 
	printf("\nvariable ::= WORD %s", var);
}

//ARGLIST

argument_list(arg_list) ::= argument(arg) . {
	arg_list = arg;
	printf("\nargument_list ::= argument"); 
}

argument_list(new_arg_list) ::= argument_list(arg_list) argument(arg) . {
	new_arg_list = (char*) malloc ((strlen(arg) + strlen(arg_list) + 2) * sizeof(char));
	stpcpy(new_arg_list, arg_list);
	strcat(new_arg_list, " \0");
	strcat(new_arg_list, arg);
	printf("\nargument_list ::= argument_list argument %s", new_arg_list);
	free(arg_list);
	free(arg);
}

//ARG

argument(arg) ::= FILENAME(ptr) .	{ 
	arg = ptr; 
	printf("\nargument ::= FILENAME %s", arg); 
}

argument(arg) ::= ARGUMENT(ptr) .	{ 
	arg = ptr; 
	printf("\nargument ::= ARGUMENT %s", arg);
}












