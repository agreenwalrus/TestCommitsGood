#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "parser.h"
#include "scanner.yy.h"
#include "flexglobal.h"
#include "shell_system.h"
#include "bricks.h"

#define END_OF_SESSION 1
#define EXIT "exit\n"


#define BUF_SIZE 200

void* ParseAlloc(void* (*allocProc)(size_t));
void* Parse(void*, int, char*, struct list_struct**);
void* ParseFree(void*, void(*freeProc)(void*));
void ParseTrace(FILE *, char *);


int shell(char *commandLine)
{
	struct list_struct *list = NULL;
	//int i;
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
		printf("\nError of opening log.txt file");
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
	ParseTrace(NULL, zPrefix);
	fclose(fileStream);
	if(!list)
		printf("\nNULL list");

	printf("\n1");
	//printf("\n cmd: %s %s %u", list->head->toDo->cmd.command->nameOfCmd, list->head->toDo->cmd.command->args, list->head->toDo->connectionWithNextBitMask);
	printf("\n2");
	if(list)
		freeListStruct(*list);
	printf("\n3");
	if(list)
		free(list);
 
    if (-1 == lexCode) {
        perror ("\nThe scanner encountered an error."); 
    }
	ParseTrace(NULL, zPrefix);					//end of debug. Printing states of parser

    // Cleanup the scanner and parser
    yy_delete_buffer(bufferState, scanner);
    yylex_destroy(scanner);
    ParseFree(shellParser, free);
	
	return 0;
}

int main(int argc, char** argv) {
	
	char buffer[BUF_SIZE];
	
	initShell();	
	
	do {
		printf("\n>");
		fgets(buffer, BUF_SIZE, stdin);
		printf("\nbuf: %s", buffer);
		if (strcmp(buffer, EXIT) == 0)
			break;
	} while (! shell(buffer));
	
	destroyShell();
	
	getchar();
	
	return 0;
}
