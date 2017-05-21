#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.yy.h"
#include "flexglobal.h"
#include "simpleCommand.h"
#include <Windows.h>

#define END_OF_SESSION 1


#define BUF_SIZE 200

void* ParseAlloc(void* (*allocProc)(size_t));
void* Parse(void*, int, char*, struct simpleCommand*);
void* ParseFree(void*, void(*freeProc)(void*));
void ParseTrace(FILE *, char *);


int shell(char *commandLine)
{
	struct simpleCommand commandDescription = {0};
	
	//setup scanner
	yyscan_t scanner;
    yylex_init(&scanner);
    YY_BUFFER_STATE bufferState = yy_scan_string(commandLine, scanner);
 
    // Set up the parser
    void* shellParser = ParseAlloc(malloc);
	char zPrefix[] = "\nParser state: ";
	char fileName[] = "./log.txt";
	FILE *fileStream = fopen(fileName, "w");
	
	if (!fileStream)
	{
		printf("\nError of opening file %d", GetLastError());
		return -1;
	}
	
	ParseTrace(fileStream, zPrefix);

    int lexCode;
	
	yylval.charValue = (char*)malloc(2);
	printf("\nyylval.charValue: addres %d", yylval.charValue);
	free(yylval.charValue);
	yylval.charValue = (char*)malloc(2);
		printf("\nyylval.charValue: addres %d", yylval.charValue);
			free(yylval.charValue);
  
    do {
        lexCode = yylex(scanner);
		printf("\nLexCode: %d", lexCode);
		printf("\nyylval.charValue: %s addres %d", yylval.charValue, &yylval.charValue);
		/*if (!strcmp(yylval.charValue, "Quit"))
		{
			Parse(shellParser, 0, NULL, NULL);
			break;
		}*/
        Parse(shellParser, lexCode, yylval.charValue, &commandDescription);
    }
    while (lexCode > 0);
 
    if (-1 == lexCode) {
        perror ("\nThe scanner encountered an error."); 
    }
	ParseTrace(NULL, zPrefix);
	printf("\nCommand name: %s", commandDescription.name);
    // Cleanup the scanner and parser
    yy_delete_buffer(bufferState, scanner);
	printf("\n1");
    yylex_destroy(scanner);
	printf("\n2");
    ParseFree(shellParser, free);
	printf("\n3");
	
	fclose(fileStream);
	
	if (!strcmp(commandDescription.name, "Quit") || !strcmp(commandDescription.name, "quit"))
		return END_OF_SESSION;
	
	return 0;
}

int main(int argc, char** argv) {
	
	char buffer[BUF_SIZE];
	
	do {
		printf("\n>");
		fgets(buffer, BUF_SIZE, stdin);
	} while (! shell(buffer));
	
	return 0;
}
