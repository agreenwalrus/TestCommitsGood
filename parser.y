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

%extra_argument { struct list_struct **list_for_return }

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
%type substitution_of_var { char * }
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

program ::= result(res) EOL. { 
	*list_for_return = res; 
}

//RESULT

result(res) ::= .	{  
	res = NULL;
}

result(res) ::= input(inp) . { 	
	if(!inp)
		printf("\nNULL list result");  
	res = inp;
	res->excecAtBackGr = FALSE;	
	printf("\n cmd: %s %s", res->head->toDo->cmd.command->nameOfCmd, res->head->toDo->cmd.command->args);
}

result(res) ::= input(inp) AMPERSAND. {
	res = inp;
	res->excecAtBackGr = TRUE;	
}


result(res) ::= branchig(branch) . {
	res = (struct list_struct*) malloc (sizeof(struct list_struct));
	res->redirection = NULL;
	res->head = NULL;
	res->tail = NULL;
	res->size = 0;
	
	
	struct whole_command_struct* whole_cmd = (struct whole_command_struct*) malloc (sizeof(struct whole_command_struct));
	whole_cmd->name = strdup(IF_NAME);
	whole_cmd->cmd.if_branch = branch;
	whole_cmd->connectionWithNextBitMask = CONNECT_NO;
	addWholeCommandToList(res, whole_cmd);
}

result(res) ::= variable_declaration . { 
	res = NULL;
}

result(res) ::= for_cycle(cycle) . { 
	res = (struct list_struct*) malloc (sizeof(struct list_struct));
	res->redirection = NULL;
	res->head = NULL;
	res->tail = NULL;
	res->size = 0;
	
	struct whole_command_struct* whole_cmd = (struct whole_command_struct*) malloc (sizeof(struct whole_command_struct));
	whole_cmd->name = strdup(FOR_NAME);
	whole_cmd->cmd.for_cycle = cycle;
	whole_cmd->connectionWithNextBitMask = CONNECT_NO;
	addWholeCommandToList(res, whole_cmd);
}

result(res) ::= while_cycle(cycle) . {
	res = (struct list_struct*) malloc (sizeof(struct list_struct));
	struct whole_command_struct* whole_cmd = (struct whole_command_struct*) malloc (sizeof(struct whole_command_struct));
	res->redirection = NULL;
	res->head = NULL;
	res->tail = NULL;
	res->size = 0;
	
	whole_cmd->name = strdup(WHILE_NAME);
	whole_cmd->cmd.while_cycle = cycle;
	whole_cmd->connectionWithNextBitMask = CONNECT_NO;
	addWholeCommandToList(res, whole_cmd);
}

//FOR_CYCLE

for_cycle(cycle) ::= FOR variable(var) IN_TOKEN argument_list(arg_list) SEMICOLON DO result(res). {
	struct variable_struct st;
	st.varName = strdup(var);
	st.varValue = NULL;
	addVariable(st);
	
	cycle = (struct for_cycle_struct*) malloc (sizeof(struct for_cycle_struct));
	
	cycle->varName = var;
	cycle->varStates = arg_list;
	cycle->instractionsToDo = res;
} 

//WHILE_CYCLE

while_cycle(while_cycle) ::= WHILE condition(cond) SEMICOLON DO result(res) . { 
	while_cycle = (struct while_cycle_struct*) malloc (sizeof(struct while_cycle_struct));
	while_cycle->conditional = cond;
	while_cycle->instractionsToDo = res;
}

//INPUT

input(inp) ::= command_line_list(cmd_line_list) . {
	inp = cmd_line_list;
	cmd_line_list->redirection = NULL;
}

input(inp) ::= command_line_list(cmd_line_list) redirection_list(red_list) . {
	cmd_line_list->redirection = red_list;
	inp = red_list;
}

//SUBSTITUTION_OF_VARIABLE

substitution_of_var(subs) ::= ANDLPAREN FILENAME(ptr) RPAREN .{
	subs = ptr;
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
	cmd->nameOfCmd = name;
	cmd->args = NULL;
}

command(cmd) ::= ARGUMENT(name) .	{
	cmd = (struct command_struct*) malloc(sizeof(struct command_struct));
	cmd->nameOfCmd = name;
	cmd->args = NULL;
}

//VARIABLE_DECLARATION

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
}

redirection(redir) ::= LESS argument(arg) . {
	redir = (struct part_redirection_struct*) malloc (sizeof(struct part_redirection_struct));
	redir->fileName = arg;
	redir->type = NEW_STDIN;	
}

redirection(redir) ::= GREATAMPERSAND argument(arg) . {
	redir = (struct part_redirection_struct*) malloc (sizeof(struct part_redirection_struct));
	redir->fileName = arg;
	redir->type = NEW_STDERR; 
}

redirection(redir) ::= GREATGREAT argument(arg) . {
	redir = (struct part_redirection_struct*) malloc (sizeof(struct part_redirection_struct));
	redir->fileName = arg;
	redir->type = NEW_STDOUT_BACK;
}

//BRANCHING

branchig(branch) ::= IF condition(cond) THEN result(trueRes) ELSE result(falseRes) FI . { 
	branch = (struct if_branch_struct*) malloc(sizeof(struct if_branch_struct));
	branch->conditional = cond;
	branch->trueWay = trueRes;
	branch->falseWay = falseRes;
}

branchig(branch) ::= IF condition(cond) THEN COLON ELSE result(res) FI . {
	branch = (struct if_branch_struct*) malloc(sizeof(struct if_branch_struct));
	branch->conditional = cond;
	branch->trueWay = NULL;
	branch->falseWay = res;
}

branchig(branch) ::= IF condition(cond) THEN result(res) ELSE COLON FI.	{
	branch = (struct if_branch_struct*) malloc(sizeof(struct if_branch_struct));
	branch->conditional = cond;
	branch->trueWay = res;
	branch->falseWay = NULL;
}

//CONDITION

condition(cond) ::= LBRACKET variable(var) LESS argument(arg) RBRACKET . {
	cond = (struct operate_at_variabe_struct*) malloc (sizeof(struct operate_at_variabe_struct));
	cond->name = var;
	cond->value = arg;
	cond->operation = OPER_LESS;	
}

condition(cond) ::= LBRACKET variable(var) GREAT argument(arg) RBRACKET . {
	cond = (struct operate_at_variabe_struct*) malloc (sizeof(struct operate_at_variabe_struct));
	cond->name = var;
	cond->value = arg;
	cond->operation = OPER_GRET;
}

condition(cond) ::= LBRACKET variable(var) EQUEQU argument(arg) RBRACKET . { 
	cond = (struct operate_at_variabe_struct*) malloc (sizeof(struct operate_at_variabe_struct));
	cond->name = var;
	cond->value = arg;
	cond->operation = OPER_CMP_EQU;
}

//VARIABLE
variable(var) ::= WORD(ptr) . { 
	var = ptr; 
}

//ARGLIST

argument_list(arg_list) ::= argument(arg) . {
	arg_list = arg; 
}

argument_list(new_arg_list) ::= argument_list(arg_list) argument(arg) . {
	new_arg_list = (char*) malloc ((strlen(arg) + strlen(arg_list) + 2) * sizeof(char));
	stpcpy(new_arg_list, arg_list);
	strcat(new_arg_list, " \0");
	strcat(new_arg_list, arg);
	free(arg_list);
	free(arg);
}

//ARG

argument(arg) ::= FILENAME(ptr) .	{ 
	arg = ptr;
}

argument(arg) ::= ARGUMENT(ptr) .	{ 
	arg = ptr; 
}

argument (arg) ::= substitution_of_var(subs) . {
	arg = findVariable (subs);
	free(subs);
}












