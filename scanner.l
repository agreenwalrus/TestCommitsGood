/*
Here're described rules for scanning char buffers.
*/
%{
	#include <string.h>
	#include <stdlib.h>
    #include "parser.h"
	#include "shell_system.h"
	#include "flexglobal.h"

	#define LS_CHANGE "dir"
	#define GREP_CHANGE "findstr"
	#define PS_CHANGE "tasklist"
	#define KILL_CHANGE "taskkill"
	#define DIFF_CHANGE "fc"
	#define MAN_CHANGE	"help"
	#define INFO_CHANGE	"help"
	#define LESS_CHANGE	"more"
	#define FREE_CHANGE	"mem"
%}

%option reentrant
%option noyywrap
%option nounput
%option noinput

%x SINGLE_QUOTED
%x DOUBLE_QUOTED


%%

"alias"					{ return ALIAS; }
"ALIAS"					{ return ALIAS; }
"free"					{ yylval.charValue = strdup(FREE_CHANGE); return FILENAME; }
"less"					{ yylval.charValue = strdup(LESS_CHANGE); return FILENAME; }
"info"					{ yylval.charValue = strdup(INFO_CHANGE); return FILENAME; }
"man"					{ yylval.charValue = strdup(MAN_CHANGE); return FILENAME; }
"diff"					{ yylval.charValue = strdup(DIFF_CHANGE); return FILENAME; }
"ls"					{ yylval.charValue = strdup(LS_CHANGE); return FILENAME;  }
"grep"					{ yylval.charValue = strdup(GREP_CHANGE); return FILENAME; }
"ps"					{ yylval.charValue = strdup(PS_CHANGE); return FILENAME; }
"kill"					{ yylval.charValue = strdup(KILL_CHANGE); return FILENAME; }	

"if"					{ return IF; }
"then"					{ return THEN; }
"else"					{ return ELSE; }
"fi"					{ return FI; }

"for"					{ return FOR; }
"do"					{ return DO; }
"from"					{ return FROM; }
"until"					{ return UNTIL; }

"while"					{ return WHILE; }

"$("					{ return DOLLARLPAREN; }
")"						{ return RPAREN; }
";"						{ return SEMICOLON; }
"="						{ return EQU; }
"=="					{ return EQUEQU; }
"["                  	{ return LBRACKET; }
"]"                   	{ return RBRACKET; } 
":"						{ return COLON; }
">"						{ return GREAT; }
">>"					{ return GREATGREAT; }
"<"						{ return LESS; }
">&"					{ return GREATAMPERSAND; }					

"|"                     { return PIPE; }
"&"						{ return AMPERSAND; }
"&&"					{ return ANDAND; }
"||"					{ return OROR; }



[ \t\r\n]                 { }

"-"?[1-9][0-9]*			{
	
	yylval.charValue = strdup(yytext);
	return DIGIT; 
}

"0"	{
	yylval.charValue = strdup(yytext);	
	return DIGIT; 
}

([/a-zA-Z0-9_:\.\-]+("\\\ ")*("\\\"")*("\\\#")*("\\\'")*)+       {
	char *temp;
	if (!(temp = getAlias(yytext)))
		yylval.charValue = strdup(yytext);
	else yylval.charValue = temp;
	printf("Value: %s", yylval.charValue);
	return FILENAME; 
}	

[']                     { BEGIN(SINGLE_QUOTED); }
<SINGLE_QUOTED>[^']+    { 
	yylval.charValue = strdup(yytext);
}
<SINGLE_QUOTED>[']      { 
	BEGIN(INITIAL);  
	return ARGUMENT; 
}
<SINGLE_QUOTED><<EOF>>  { return -1; }

["]                     { BEGIN(DOUBLE_QUOTED); }
<DOUBLE_QUOTED>[^"]+    { 
	yylval.charValue = (char*)malloc(strlen(yytext) + 1); 
	strcpy(yylval.charValue, yytext); 
}
<DOUBLE_QUOTED>["]      { 
	BEGIN(INITIAL);
	return ARGUMENT; 
}

<DOUBLE_QUOTED><<EOF>>  { return -1; }

%%


