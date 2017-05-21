/*
Grammar rules for shell.
*/


%include {
	#include<stdio.h>
	#include <stdlib.h>
	#include"parser.h"
	#include<assert.h>
	#include <string.h>
	#include "simpleCommand.h"
}

%extra_argument { struct simpleCommand * commandDescription }

%default_type {char *}
%token_type {char *}

%destructor FILENAME { printf ("\ndestructorFILENAME %s", $$); free((void*)$$); }
%destructor command { printf ("\ndestructorCMD %s", $$); free((void*)$$); }
%destructor argument { printf ("\ndestructorARG %s", $$); free((void*)$$); }
%destructor AMPERSAND { free((void*)$$); printf ("\ndestructorARG"); }
%destructor EOL { free((void*)$$); printf ("\ndestructorEOL"); }

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

result ::= QUIT . { printf("\nresult ::= QUIT"); }
result ::= input . { printf("\nresult ::= input"); }
result ::= result AMPERSAND. { printf("\nresult ::= input AMPERSAND"); }

//INPUT

input ::= . { printf("\ninput ::= ."); }
input ::= command_line_list .{ printf("\ninput ::= command_line_list"); }
input ::= command_line_list redirection_list . { printf("\ninput ::= input redirection"); }
//input ::= input AMPERSAND .{ printf("\ninput ::= input AMPERSAND"); }

//CMDLINELIST

command_line_list ::= command_line . { printf("\ncommand_list ::= command"); }
command_line_list ::= command_line_list PIPE command_line . { printf("\ncommand_list ::= command_list PIPE command"); }
 
//CMDLIST

command_line ::= command . { printf("\ncommand_line ::= command"); }
command_line ::= command argument_list . { printf("\ncommand_line ::= command argument_list"); }
 
 //CMD
command(cmd) ::= FILENAME(ptr) . { cmd = ptr; printf("\ncommand ::= FILENAME %s", cmd); strcpy(commandDescription->name, cmd); }
//command ::= FILENAME(B) argument_list . { strcpy(commandDescription, B); printf("\ncommand ::= FILENAME %s argument_list", B); }

//REDIRECTION_LIST

redirection_list ::= redirection . { printf("\nredirection_list ::= redirection"); }
redirection_list ::= redirection_list redirection . { printf("\nredirection_list ::= redirection_list redirection"); }

//REDIRECTION

redirection ::= GREAT argument . { printf("\nredirection ::= GREAT argument"); }
redirection ::= LESS argument . { printf("\nredirection ::= LESS argument"); }
redirection ::= GREATAMPERSAND argument . { printf("\nredirection ::= GREATAMPERSAND argument"); }
redirection ::= GREATGREAT argument . { printf("\nredirection ::= GREATGREAT argument"); }

//ARGLIST

argument_list ::= argument . { printf("\nargument_list ::= argument"); }
argument_list ::= argument_list argument . { printf("\nargument_list ::= argument_list argument"); }

//ARG

argument(arg) ::= FILENAME(ptr) . { arg = ptr; printf("\nargument ::= FILENAME %s", arg); }
argument(arg) ::= FLAG(ptr) . { arg = ptr; printf("\nargument ::= FLAG %s", arg); }
argument(arg) ::= ARGUMENT(ptr) . { arg = ptr; printf("\nargument ::= ARGUMENT %s", arg); }













