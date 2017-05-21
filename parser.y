/*
Grammar rules for shell.
*/


%include {
	#include<stdio.h>
	#include <stdlib.h>
	#include<assert.h>
	#include <string.h>
	#include "simpleCommand.h"
}

%extra_argument { struct simpleCommand * commandDescription }

%default_type {char *}
%token_type {char *}

%destructor command { printf ("\ndestructorCMD %s", $$); free((void*)$$); }
%destructor argument { printf ("\ndestructorARG %s", $$); free((void*)$$); }
%destructor	variable	{ printf("\nsestructorVAR %s", $$); free((void*)$$); }

%syntax_error {  
  printf("\nSyntax error!");  
}  

%parse_failure {
	printf("\nparse failure");
}

%parse_accept {
	printf("\nParsing's been done");
}


%start_symbol program

program ::= result EOL. { printf("\nprogram ::= result"); }

//RESULT

result ::= input . { printf("\nresult ::= input"); }
result ::= input AMPERSAND. { printf("\nresult ::= input AMPERSAND"); }
result ::= branchig . { printf("\nresult ::= branchig"); }
result ::= branchig AMPERSAND . { printf("\nresult ::= branchig AMPERSAND"); }

//INPUT

input ::= . { printf("\ninput ::= ."); }
input ::= command_line_list .{ printf("\ninput ::= command_line_list"); }
input ::= command_line_list redirection_list . { printf("\ninput ::= input redirection"); }
//input ::= input AMPERSAND .{ printf("\ninput ::= input AMPERSAND"); }

//CMDLINELIST

command_line_list ::= command_line . { printf("\ncommand_line_list ::= command_line"); }
command_line_list ::= command_line_list PIPE command_line . { printf("\ncommand_line_list ::= command_line_list PIPE command_line"); }
 
//CMDLIST

command_line ::= command . { printf("\ncommand_line ::= command"); }
command_line ::= command argument_list . { printf("\ncommand_line ::= command argument_list"); }
 
 //CMD
command(cmd) ::= FILENAME(ptr) .	{ 
	cmd = ptr; 
	printf("\ncommand ::= FILENAME %s", cmd); 
	strcpy(commandDescription->name, cmd); 
}
command(cmd) ::= ARGUMENT(ptr) .	{
	cmd = ptr;
	strcpy(commandDescription->name, ptr); 
	printf("\ncommand ::= ARGUMENT %s", ptr); 
}

command(cmd) ::= WORD(ptr) .	{ 
	cmd = ptr; 
	printf("\ncommand ::= WORD %s", cmd); 
	strcpy(commandDescription->name, cmd); 
}

//REDIRECTION_LIST

redirection_list ::= redirection . { printf("\nredirection_list ::= redirection"); }
redirection_list ::= redirection_list redirection . { printf("\nredirection_list ::= redirection_list redirection"); }

//REDIRECTION

redirection ::= GREAT argument . { printf("\nredirection ::= GREAT argument"); }
redirection ::= LESS argument . { printf("\nredirection ::= LESS argument"); }
redirection ::= GREATAMPERSAND argument . { printf("\nredirection ::= GREATAMPERSAND argument"); }
redirection ::= GREATGREAT argument . { printf("\nredirection ::= GREATGREAT argument"); }

///BRANCHING

branchig ::= IF condition SEMICOLON THEN result ELSE result FI . { printf("\nbranchig ::= IF condition SEMICOLON THEN result ELSE result FI"); }
branchig ::= IF condition SEMICOLON THEN COLON ELSE result FI . { printf("\nbranchig ::= IF condition SEMICOLON THEN COLON ELSE result FI"); }
branchig ::= IF condition SEMICOLON THEN result ELSE COLON FI.	{ printf("\nbranchig ::= IF condition SEMICOLON THEN result ELSE COLON FI"); }

//CONDITION

condition ::= variable LESS value . { printf("\ncondition ::= variable LESS value"); }
condition ::= variable GREAT value . { printf("\ncondition ::= variable GREAT value"); }

//VARIABLE
variable(var) ::= WORD(ptr) . { var = ptr; printf("\nvariable ::= WORD %s", var);}

//VALUE

value ::= DIGIT . { printf("\nvalue ::= DIGIT"); }
value ::= WORD . { printf("\nvalue ::= WORD"); }

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
	strcpy(commandDescription->args[commandDescription->curAmountOfArgs], arg);
	commandDescription->curAmountOfArgs = commandDescription->curAmountOfArgs + 1;
}
	
argument(arg) ::= FLAG(ptr) . 		{ 
	arg = ptr; 
	printf("\nargument ::= FLAG %s", arg); 
	strcpy(commandDescription->args[commandDescription->curAmountOfArgs], arg);
	commandDescription->curAmountOfArgs = commandDescription->curAmountOfArgs + 1;
}

argument(arg) ::= ARGUMENT(ptr) .	{ 
	arg = ptr; 
	printf("\nargument ::= ARGUMENT %s", arg);
	strcpy(commandDescription->args[commandDescription->curAmountOfArgs], arg);
	commandDescription->curAmountOfArgs = commandDescription->curAmountOfArgs + 1;
}

argument(arg) ::= WORD(ptr) .		{ 
	arg = ptr; 
	printf("\nargument ::= WORD %s", arg);
	strcpy(commandDescription->args[commandDescription->curAmountOfArgs], arg);
	commandDescription->curAmountOfArgs = commandDescription->curAmountOfArgs + 1;
}













