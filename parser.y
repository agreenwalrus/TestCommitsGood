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

%destructor result { if($$){
	freeListStruct(*$$); 
	free($$); 
}}

%destructor program { 
	if($$){
		freeListStruct(*$$); 
		free($$); 
}}
%destructor input { 
	if($$){
		freeListStruct(*$$); 
		free($$); 
}}
%destructor command_line_list { 
	if($$){
		freeListStruct(*$$); 
		free($$); 
}}
%destructor command_line { 
	if($$){
		freeCommandStruct(*$$);
		free($$);	
}}
%destructor command { 
	if($$){
		freeCommandStruct(*$$);
		free($$);	
}}
%destructor redirection_list { 
	if($$) {
		freeRedirecionStruct(*$$);
		free($$); 
	}}
%destructor redirection { if($$) free($$); }
%destructor argument_list { if($$) free($$); }
%destructor argument { if($$) free($$); }
%destructor substitution_of_var { if($$) free($$); }
%destructor value { if($$) free($$); }
%destructor for_cycle { 
	if($$) {
		freeForCycleStruct(*$$);
		free($$);
	}		
}
%destructor while_cycle { 
	if($$) {
		freeWhileCycleStruct(*$$);
		free($$);
	}		
}
%destructor variable { 
	if($$) free($$); 
}
%destructor condition  { 
	if($$) {
		freeOperateAtVariableStruct(*$$);
		free($$);
	}		
}
%destructor branchig  { 
	if($$) {
		freeIfBranchStruct(*$$);
		free($$);
	}		
}

%destructor bound  { 
	if($$) 
		free($$);	
}


%left ANDAND .
%left OROR .
%right PIPE .

%extra_argument { struct list_struct **list_for_return }

%default_type {char *}
%token_type {char *}
%type bound { char* }
%type program { struct list_struct* }
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
	printf("\nParse failure!");
}

%parse_accept {
	printf("\nParsing's been done!");
}

%stack_overflow {
	printf("\nParser stack overflow!");
}

%start_symbol program_shell

program_shell ::= program(res). {
	*list_for_return = res;
	res = NULL;
}

program(pr) ::= result(res). { 
	if (res)
	{
		res->excecAtBackGr = FALSE;
		pr = res;
	}
	res = NULL;
}

program(pr) ::= result(res) AMPERSAND. { 
	if (res)
	{
		res->excecAtBackGr = TRUE;
		pr = res;
	}
	res = NULL;
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
	inp = NULL;
}

/*result(res) ::= input(inp) AMPERSAND. {
	res = inp;
	res->excecAtBackGr = TRUE;
	inp = NULL;
}*/


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
	
	branch = NULL;
}

result(res) ::= variable_declaration . { 
	res = NULL;
}

result(res) ::= alias_declaration . {
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
	
	cycle = NULL;
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
	cycle = NULL;
}

//FOR_CYCLE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

for_cycle(cycle) ::= FOR FROM bound(low) UNTIL bound(hight) SEMICOLON DO input(res). {
		
	cycle = (struct for_cycle_struct*) malloc (sizeof(struct for_cycle_struct));
	
	cycle->from = low;
	cycle->until = hight;
	cycle->instractionsToDo = res;
	
	low = NULL;
	hight = NULL;
	res = NULL;
} 

bound(bou) ::= DIGIT(dig) . {
	bou = dig;
	dig = NULL;
}
bound(bou) ::= substitution_of_var(sub) . {
	bou = findVariable (sub);
}

//WHILE_CYCLE

while_cycle(while_cycle) ::= WHILE condition(cond) SEMICOLON DO input(res) . { 
	while_cycle = (struct while_cycle_struct*) malloc (sizeof(struct while_cycle_struct));
	while_cycle->conditional = cond;
	while_cycle->instractionsToDo = res;
	cond = NULL;
	res = NULL;
}

//INPUT

input(inp) ::= command_line_list(cmd_line_list) . {
	inp = cmd_line_list;
	cmd_line_list->redirection = NULL;
	cmd_line_list = NULL;
}

input(inp) ::= command_line_list(cmd_line_list) redirection_list(red_list) . {
	cmd_line_list->redirection = red_list;
	inp = cmd_line_list;
	cmd_line_list = NULL;
	red_list = NULL;
}

//SUBSTITUTION_OF_VARIABLE

substitution_of_var(subs) ::= DOLLARLPAREN FILENAME(ptr) RPAREN .{
	subs = ptr;
	
	ptr = NULL;
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
	
	cmd_line = NULL;
}

command_line_list(res_cmd_line_list) ::= command_line_list(cmd_line_list) PIPE command_line(cmd_line) . {
	struct whole_command_struct *whole_cmd = (struct whole_command_struct*) malloc (sizeof(struct whole_command_struct));
	whole_cmd->name = strdup(CMD_NAME);
	whole_cmd->cmd.command = cmd_line;
	whole_cmd->connectionWithNextBitMask = CONNECT_NO;
	
	cmd_line_list->tail->toDo->connectionWithNextBitMask = CONNECT_PIPE;
	
	addWholeCommandToList(cmd_line_list, whole_cmd);
	res_cmd_line_list = cmd_line_list;
	cmd_line_list = NULL;
	cmd_line = NULL;
}

command_line_list(res_cmd_line_list) ::= command_line_list(cmd_line_list) ANDAND command_line(cmd_line) . {
	struct whole_command_struct *whole_cmd = (struct whole_command_struct*) malloc (sizeof(struct whole_command_struct));
	whole_cmd->name = strdup(CMD_NAME);
	whole_cmd->cmd.command = cmd_line;
	whole_cmd->connectionWithNextBitMask = CONNECT_NO;
	
	cmd_line_list->tail->toDo->connectionWithNextBitMask = CONNECT_ANDAND;
	
	addWholeCommandToList(cmd_line_list, whole_cmd);
	
	res_cmd_line_list = cmd_line_list;
	cmd_line_list = NULL;
	cmd_line = NULL;
}

command_line_list(res_cmd_line_list) ::= command_line_list(cmd_line_list) OROR command_line(cmd_line) . {
	struct whole_command_struct *whole_cmd = (struct whole_command_struct*) malloc (sizeof(struct whole_command_struct));
	whole_cmd->name = strdup(CMD_NAME);
	whole_cmd->cmd.command = cmd_line;
	whole_cmd->connectionWithNextBitMask = CONNECT_NO;
	
	cmd_line_list->tail->toDo->connectionWithNextBitMask = CONNECT_OROR;
	
	addWholeCommandToList(cmd_line_list, whole_cmd);

	res_cmd_line_list = cmd_line_list;
	cmd_line_list = NULL;
	cmd_line = NULL;
}
 
//CMDLIST

command_line(cmd_line) ::= command(cmd) . {
	cmd_line = cmd;
	cmd = NULL;
}

command_line(cmd_line) ::= command(cmd) argument_list(arg_list) . { 
	cmd->args = arg_list;
	cmd_line = cmd;
	cmd = NULL;
	arg_list = NULL;
}
 
//CMD
command(cmd) ::= FILENAME(name) .	{ 
	cmd = (struct command_struct*) malloc(sizeof(struct command_struct));
	cmd->nameOfCmd = name;
	cmd->args = NULL;
	name = NULL;
}

command(cmd) ::= ARGUMENT(name) .	{
	cmd = (struct command_struct*) malloc(sizeof(struct command_struct));
	cmd->nameOfCmd = name;
	cmd->args = NULL;
	name = NULL;
}

//ALIAS_DECLARATION

alias_declaration ::= ALIAS variable(name) EQU ARGUMENT(arg) . {
	struct variable_struct var;
	var.varName = name;
	var.varValue = arg;
	addAlias(var);
	name = NULL;
	arg = NULL;
}

//VARIABLE_DECLARATION

variable_declaration ::= variable(name) EQU argument_list(arg_list) . { 
	struct variable_struct var;
	var.varName = name;
	var.varValue = arg_list;
	addVariable(var);
	name = NULL;
	arg_list = NULL;
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
	result_redir_list = redir_list;
	redir_list = NULL;
}

//REDIRECTION

redirection(redir) ::= GREAT argument(arg) . {
	redir = (struct part_redirection_struct*) malloc (sizeof(struct part_redirection_struct));
	redir->fileName = arg;
	redir->type = NEW_STDOUT;
	arg = NULL;
}

redirection(redir) ::= LESS argument(arg) . {
	redir = (struct part_redirection_struct*) malloc (sizeof(struct part_redirection_struct));
	redir->fileName = arg;
	redir->type = NEW_STDIN;	
	arg = NULL;
}

redirection(redir) ::= GREATAMPERSAND argument(arg) . {
	redir = (struct part_redirection_struct*) malloc (sizeof(struct part_redirection_struct));
	redir->fileName = arg;
	redir->type = NEW_STDERR;
	arg = NULL;
}

redirection(redir) ::= GREATGREAT argument(arg) . {
	redir = (struct part_redirection_struct*) malloc (sizeof(struct part_redirection_struct));
	redir->fileName = arg;
	redir->type = NEW_STDOUT_BACK;
	arg = NULL;
}

//BRANCHING

branchig(branch) ::= IF condition(cond) THEN result(trueRes) ELSE result(falseRes) FI . { 
	branch = (struct if_branch_struct*) malloc(sizeof(struct if_branch_struct));
	branch->conditional = cond;
	branch->trueWay = trueRes;
	branch->falseWay = falseRes;
	trueRes = NULL;
	falseRes = NULL;
	cond = NULL;
}

branchig(branch) ::= IF condition(cond) THEN COLON ELSE result(res) FI . {
	branch = (struct if_branch_struct*) malloc(sizeof(struct if_branch_struct));
	branch->conditional = cond;
	branch->trueWay = NULL;
	branch->falseWay = res;
	res = NULL;
	res = NULL;
}

branchig(branch) ::= IF condition(cond) THEN result(res) ELSE COLON FI.	{
	branch = (struct if_branch_struct*) malloc(sizeof(struct if_branch_struct));
	branch->conditional = cond;
	branch->trueWay = res;
	branch->falseWay = NULL;
	res = NULL;
	res = NULL;
}

//CONDITION

condition(cond) ::= LBRACKET variable(var) LESS argument(arg) RBRACKET . {
	cond = (struct operate_at_variabe_struct*) malloc (sizeof(struct operate_at_variabe_struct));
	cond->name = var;
	cond->value = arg;
	cond->operation = OPER_LESS;
	var = NULL;
	arg = NULL;
}

condition(cond) ::= LBRACKET variable(var) GREAT argument(arg) RBRACKET . {
	cond = (struct operate_at_variabe_struct*) malloc (sizeof(struct operate_at_variabe_struct));
	cond->name = var;
	cond->value = arg;
	cond->operation = OPER_GRET;
	var = NULL;
	arg = NULL;
}

condition(cond) ::= LBRACKET variable(var) EQUEQU argument(arg) RBRACKET . { 
	cond = (struct operate_at_variabe_struct*) malloc (sizeof(struct operate_at_variabe_struct));
	cond->name = var;
	cond->value = arg;
	cond->operation = OPER_CMP_EQU;
	var = NULL;
	arg = NULL;
}

//VARIABLE
variable(var) ::= argument(ptr) . { 
	var = ptr;
	ptr = NULL;	
}

//ARGLIST

argument_list(arg_list) ::= argument(arg) . {
	arg_list = arg;
	arg = NULL;
}

argument_list(new_arg_list) ::= argument_list(arg_list) argument(arg) . {
	new_arg_list = (char*) malloc ((strlen(arg) + strlen(arg_list) + 2) * sizeof(char));
	strcpy(new_arg_list, arg_list);
	strcat(new_arg_list, " \0");
	strcat(new_arg_list, arg);
}

//ARG

argument(arg) ::= FILENAME(ptr) .	{ 
	arg = ptr;
	ptr = NULL;
}

argument(arg) ::= ARGUMENT(ptr) .	{ 
	arg = ptr;
	ptr = NULL;
}

argument (arg) ::= substitution_of_var(subs) . {
	arg = findVariable (subs);
}

argument(arg) ::= DIGIT(ptr) .	{ 
	arg = ptr;
	ptr = NULL;
}












