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

%x SINGLE_QUOTED
%x DOUBLE_QUOTED


%%

"\n"					{ printf("\n\nscanner: %s", yytext); yylval.charValue = (char*)malloc(strlen(yytext) + 1); strcpy(yylval.charValue, yytext); return EOL; }
">"						{ return GREAT; }
">>"					{ return GREATGREAT; }
"<"						{ return LESS; }
">&"					{ return GREATAMPERSAND; }					

"|"                     { return PIPE; }
"&"						{ printf("\n\nscanner: %s", yytext); yylval.charValue = (char*)malloc(strlen(yytext) + 1);  strcpy(yylval.charValue, yytext); return AMPERSAND; }

[ \t\r]                 { }

"-"[a-zA-Z]{1,2}		{ printf("\n\nscanner FLAG: %s", yytext); yylval.charValue = (char*)malloc(strlen(yytext) + 1);  strcpy(yylval.charValue, yytext); return FLAG; }	

[a-zA-Z0-9_\.\-]+       { printf("\n\nscanner FILENAME: %s", yytext); yylval.charValue = (char*)malloc(strlen(yytext) + 1); strcpy(yylval.charValue, yytext); return FILENAME; }	

[']                     { BEGIN(SINGLE_QUOTED); }
<SINGLE_QUOTED>[^']+    { }
<SINGLE_QUOTED>[']      { BEGIN(INITIAL); printf("\n\nscanner: %s", yytext); yylval.charValue = (char*)malloc(strlen(yytext) + 1);  strcpy(yylval.charValue, yytext); return ARGUMENT; }
<SINGLE_QUOTED><<EOF>>  { return -1; }

["]                     { BEGIN(DOUBLE_QUOTED); }
<DOUBLE_QUOTED>[^"]+    { }
<DOUBLE_QUOTED>["]      { BEGIN(INITIAL); printf("\n\nscanner: %s", yytext); yylval.charValue = (char*)malloc(strlen(yytext) + 1); strcpy(yylval.charValue, yytext); return ARGUMENT; }
<DOUBLE_QUOTED><<EOF>>  { return -1; }

[^ \t\r\n|'"]+          { printf("\n\nscanner ARGUMENT: %s", yytext); yylval.charValue = (char*)malloc(strlen(yytext) + 1); strcpy(yylval.charValue, yytext); return ARGUMENT; }



%%


