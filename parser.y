/*
Grammar rules for shell.
*/


%include {
	#include<stdio.h>
	#include <stdlib.h>
	#include<assert.h>
	#include <string.h>
	#include "bricks.h"
}


%extra_argument { struct list_struct *list }

%default_type {char *}
%token_type {char *}
%type result { struct list_struct* }
%type input { struct list_struct* }
%type command_line_list { struct list_struct* }
%type command_line { struct command_struct* }
%type command { struct command_struct* }
%type redirection { struct redirection_struct* }
%type argument_list { char* }
%type argument { char* }
%type value { char* }
%type for_cycle { struct for_cycle_struct* }
%type while_cycle { struct while_cycle_struct* }
%type variable { char* }
%type condition { struct operate_at_variabe_struct* }
%type branchig { struct struct if_branch_struct* }

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

program ::= result EOL. { printf("\nprogram ::= result"); }

//RESULT

result ::= .	{ printf("\nresult ::= ."); }
result ::= input . { printf("\nresult ::= input"); }
result ::= input AMPERSAND. { printf("\nresult ::= input AMPERSAND"); }
result ::= branchig . { printf("\nresult ::= branchig"); }
//result ::= branchig AMPERSAND . { printf("\nresult ::= branchig AMPERSAND"); }
result ::= variable_declaration . { printf("\nresult ::= variable_declaration"); }
result ::= for_cycle . { printf("\nresult ::= for_cycle"); }
result ::= while_cycle . { printf("\nresult ::= while_cycle"); }

//INPUT

//input ::= . { printf("\ninput ::= ."); }
input ::= command_line_list .{ printf("\ninput ::= command_line_list"); }
input ::= command_line_list redirection_list . { printf("\ninput ::= input redirection"); }
//input ::= input AMPERSAND .{ printf("\ninput ::= input AMPERSAND"); }

//CMDLINELIST

//command_line_list ::= .
command_line_list ::= command_line . { printf("\ncommand_line_list ::= command_line"); }
command_line_list ::= command_line_list PIPE command_line . { printf("\ncommand_line_list ::= command_line_list PIPE command_line"); }
 
//CMDLIST

command_line ::= command . { printf("\ncommand_line ::= command"); }
command_line ::= command argument_list . { printf("\ncommand_line ::= command argument_list"); }
 
 //CMD
command(cmd) ::= FILENAME(ptr) .	{ 
	cmd = ptr; 
	printf("\ncommand ::= FILENAME %s", cmd); 
}
command(cmd) ::= ARGUMENT(ptr) .	{
	cmd = ptr;
	printf("\ncommand ::= ARGUMENT %s", ptr); 
}

command(cmd) ::= WORD(ptr) .	{ 
	cmd = ptr; 
	printf("\ncommand ::= WORD %s", cmd); 

}

//VARIABLE_DECLARATION

//variable_declaration ::= variable(var) EQU value(val) . { printf("\nvariable EQU value %s EQU %s", var, val); }
variable_declaration ::= variable EQU argument_list . { printf("\nvariable_declaration ::= variable EQU argument_list"); }


//REDIRECTION_LIST

redirection_list ::= redirection . { printf("\nredirection_list ::= redirection"); }
redirection_list ::= redirection_list redirection . { printf("\nredirection_list ::= redirection_list redirection"); }

//REDIRECTION

redirection ::= GREAT argument . { printf("\nredirection ::= GREAT argument"); }
redirection ::= LESS argument . { printf("\nredirection ::= LESS argument"); }
redirection ::= GREATAMPERSAND argument . { printf("\nredirection ::= GREATAMPERSAND argument"); }
redirection ::= GREATGREAT argument . { printf("\nredirection ::= GREATGREAT argument"); }

//BRANCHING

branchig ::= IF condition THEN result ELSE result FI . { printf("\nbranchig ::= IF condition THEN result ELSE result FI"); }
branchig ::= IF condition THEN COLON ELSE result FI . { printf("\nbranchig ::= IF condition THEN COLON ELSE result FI"); }
branchig ::= IF condition THEN result ELSE COLON FI.	{ printf("\nbranchig ::= IF condition THEN result ELSE COLON FI"); }

//CONDITION

condition ::= LBRACKET variable LESS argument RBRACKET . { printf("\ncondition ::= LBRACKET variable LESS value RBRACKET"); }
condition ::= LBRACKET variable GREAT argument RBRACKET . { printf("\ncondition ::= LBRACKET variable GREAT value RBRACKET"); }
condition ::= LBRACKET variable EQUEQU argument RBRACKET . { printf("\nLBRACKET variable EQUEQU argument RBRACKET"); }

//VARIABLE
variable(var) ::= WORD(ptr) . { var = ptr; printf("\nvariable ::= WORD %s", var);}

//ARGLIST

argument_list ::= argument . { 
	printf("\nargument_list ::= argument"); 
}

argument_list ::= argument_list argument . { 
	printf("\nargument_list ::= argument_list argument"); 
}

//ARG

argument(arg) ::= FILENAME(ptr) .	{ 
	arg = ptr; 
	printf("\nargument ::= FILENAME %s", arg); 
}
	
argument(arg) ::= FLAG(ptr) . 		{ 
	arg = ptr; 
	printf("\nargument ::= FLAG %s", arg); 
	
}

argument(arg) ::= ARGUMENT(ptr) .	{ 
	arg = ptr; 
	printf("\nargument ::= ARGUMENT %s", arg);
	
}

argument(arg) ::= WORD(ptr) .		{ 
	arg = ptr; 
	printf("\nargument ::= WORD %s", arg);
	
}

argument(arg) ::= DIGIT(ptr) .		{ 
	arg = ptr; 
	printf("\nargument ::= DIGIT %s", arg);
	
}

//FOR_CYCLE

for_cycle ::= FOR variable IN argument_list SEMICOLON DO result. { 
	printf("\nfor_cycle ::= FOR variable IN argument_list SEMICOLON DO result"); 
} 

//WHILE_CYCLE

while_cycle ::= WHILE condition SEMICOLON DO result . { 
	printf("\nwhile_cycle ::= WHILE LBRACKET condition RBRACKET SEMICOLON DO result"); 
}











