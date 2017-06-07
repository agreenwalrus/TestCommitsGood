#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>


#define READLINE_LIBRARY

#include "parser.h"
#include "scanner.yy.h"
#include "flexglobal.h"
#include "shell_system.h"
#include "bricks.h"
#include "logic.h"
#include "readline/readline.h"
#include "readline/history.h"

#define END_OF_SESSION 1
#define EXIT "exit"




#define BUF_SIZE 200


void* ParseAlloc(void* (*allocProc)(size_t));
void* Parse(void*, int, char*, struct list_struct**);
void* ParseFree(void*, void(*freeProc)(void*));
//void ParseTrace(FILE *, char *);

int shell(char *commandLine)
{
	struct list_struct *list = NULL;
	yyscan_t scanner;
    yylex_init(&scanner);
    YY_BUFFER_STATE bufferState = yy_scan_string(commandLine, scanner);
	int lexCode;						//for returned lex code
 
    // Set up the parser
    void* shellParser = ParseAlloc(malloc);

	//Data for debug
	
	/*char zPrefix[] = "\nParser state: ";
	char fileName[] = "./log.txt";
	FILE *fileStream = fopen(fileName, "w");
	
	
	
	if (!fileStream)
	{
		printf("\nError of opening log.txt file");
		return -1;
	}*/
	
	
	//ParseTrace(fileStream, zPrefix);			//Start of debuging. Printing states of parser to file "log.txt"
	
	//end Data for debug
	
    do {
        lexCode = yylex(scanner);
        Parse(shellParser, lexCode, yylval.charValue, &list);
    }
    while (lexCode > 0);
	
	//ParseTrace(NULL, zPrefix);
	//fclose(fileStream);
	
	if(list)
		execute(list);

    if (-1 == lexCode) 
        printf("Input: %s", commandLine); 
    
	//ParseTrace(NULL, zPrefix);					//end of debug. Printing states of parser

    // Cleanup the scanner and parser
    yy_delete_buffer(bufferState, scanner);
    yylex_destroy(scanner);
    ParseFree(shellParser, free);
	
	return 0;
}

int main(int argc, char** argv) {
	
	char *buffer = NULL, introduction[BUF_SIZE + BUF_SIZE + 1], histotyFile[BUF_SIZE], temp[BUF_SIZE];
	
	using_history ();
	
	getHistoryFilePath(histotyFile, BUF_SIZE);

	initShell();	
	read_history(histotyFile);
	printIntroduction();
	while(1) {
		if (buffer)
		{
			free(buffer);
			buffer = NULL;
		}
		intrToTyping(introduction, INTROD_SIZE);

		buffer = readline(introduction);
		if (buffer && *buffer)
		{
			add_history(buffer);
			write_history(histotyFile);
			
			
			if (!strcmp(buffer, EXIT))
			{
				printf("\nAre you sure? y/n ");
				scanf("%s", temp);
				if (!strcmp(temp, "y"))
					break;
				else continue;
			}
			
			shell(buffer);
		}

	}
	
	free(buffer);
	destroyShell();
	getchar();
	
	return 0;
}
