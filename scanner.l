/*
Here're described rules for scanning char buffers.
*/
%{
    #include "parser.h"
	#include "flexglobal.h"
	#include <string.h>
	#include <stdlib.h>
	#include <string.h>
%}

%option reentrant
%option noyywrap
%option nounput
%option noinput

%x SINGLE_QUOTED
%x DOUBLE_QUOTED


%%


"if"					{ return IF; }
"then"					{ return THEN; }
"else"					{ return ELSE; }
"fi"					{ return FI; }

"for"					{ return FOR; }
"do"					{ return DO; }
"in"					{ return IN_TOKEN; }

"while"					{ return WHILE; }

"$("					{ return DOLLARLPAREN; }
")"						{ return RPAREN; }
";"						{ return SEMICOLON; }
"="						{ return EQU; }
"=="					{ return EQUEQU; }
"["                  	{ return LBRACKET; }
"]"                   	{ return RBRACKET; } 
":"						{ return COLON; }
"\n"					{ return EOL; }
">"						{ return GREAT; }
">>"					{ return GREATGREAT; }
"<"						{ return LESS; }
">&"					{ return GREATAMPERSAND; }					

"|"                     { return PIPE; }
"&"						{ return AMPERSAND; }
"&&"					{ return ANDAND; }
"||"					{ return OROR; }



[ \t\r]                 { }

([/a-zA-Z0-9_:\.\-]+("\\\ ")*("\\\"")*("\\\#")*("\\\'")*)+       { 
	printf("\n\nscanner FILENAME: %s", yytext); 
	yylval.charValue = (char*)malloc(strlen(yytext) + 1); 
	strcpy(yylval.charValue, yytext); 
	return FILENAME; 
}	

[']                     { BEGIN(SINGLE_QUOTED); }
<SINGLE_QUOTED>[^']+    { 
	yylval.charValue = (char*)malloc(strlen(yytext) + 1);
	strcpy(yylval.charValue, yytext);
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


