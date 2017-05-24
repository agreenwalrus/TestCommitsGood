#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "parser.h"
#include "scanner.yy.h"
#include "flexglobal.h"
#include "shell_system.h"
#include "bricks.h"

#define END_OF_SESSION 1


#define BUF_SIZE 200

void* ParseAlloc(void* (*allocProc)(size_t));
void* Parse(void*, int, char*, struct list_struct*);
void* ParseFree(void*, void(*freeProc)(void*));
void ParseTrace(FILE *, char *);


int shell(char *commandLine)
{
	struct list_struct list = {0};
	int i;
	//setup scanner
	yyscan_t scanner;
    yylex_init(&scanner);
    YY_BUFFER_STATE bufferState = yy_scan_string(commandLine, scanner);
	int lexCode;						//for returned lex code
 
    // Set up the parser
    void* shellParser = ParseAlloc(malloc);

	//Data for debug
	char zPrefix[] = "\nParser state: ";
	char fileName[] = "./log.txt";
	FILE *fileStream = fopen(fileName, "w");
	//end Data for debug
	
	if (!fileStream)
	{
		printf("\nError of opening file %d", GetLastError());
		return -1;
	}
	
	ParseTrace(fileStream, zPrefix);			//Start of debuging. Printing states of parser to file "log.txt"
  
    do {
        lexCode = yylex(scanner);
		printf("\nLexCode: %d", lexCode);
		printf("\nyylval.charValue: %s ", yylval.charValue);
        Parse(shellParser, lexCode, yylval.charValue, &list);
    }
    while (lexCode > 0);
	
	freeListStruct(list);
 
    if (-1 == lexCode) {
        perror ("\nThe scanner encountered an error."); 
    }
	ParseTrace(NULL, zPrefix);					//end of debug. Printing states of parser

    // Cleanup the scanner and parser
    yy_delete_buffer(bufferState, scanner);
    yylex_destroy(scanner);
    ParseFree(shellParser, free);
	
	fclose(fileStream);
	
	return 0;
}

int main(int argc, char** argv) {
	
	char buffer[BUF_SIZE];
	
	initShell();	
	
	do {
		printf("\n>");
		fgets(buffer, BUF_SIZE, stdin);
		if (!strcmp(buffer, "exit"))
			break;
	} while (! shell(buffer));
	
	destroyShell();
	
	return 0;
}
