#include "logic.h"


/*
It's an introduction for starting program
*/
void printIntroduction()
{
	char usrName[BUFSIZ + 1], cmpName[BUFSIZ];
	DWORD buf_size = (DWORD) BUFSIZ;

	printf ("Opensource project Shell for Windows.\nCreated by the sudent of BSUIR 550502\nJulia Runova.\n");
	if (!GetUserName(usrName, &buf_size))
	{
		printf ("Error of getting name of user %d\n", GetLastError());
		return;
	}
	buf_size = (DWORD) BUFSIZ;
	if (!GetComputerName(cmpName, &buf_size))
	{
		printf ("Error of getting name of computer %d\n", GetLastError());
		return;
	}
	printf("\n[%s]@[%s]\n", usrName, cmpName);
}

/*
Prints current directory
*/
void intrToTyping()
{
	char curDir[BUFSIZ + 1];
	if (!GetCurrentDirectory(BUFSIZ, curDir))
	{
		printf ("Error of getting current directory %d\n", GetLastError());
		return;
	}
	printf("%s$", curDir);
}

/*
Start for proccessing input
*/
int excecute(struct list_struct *list)
{
	struct node_struct *iter;
	if (list->size == 1)
	{
		
			
	} else {
	}
}

/*
Mounted "cd" cmd
*/
void changeDirectory (char *newPuth)
{
	if (!SetCurrentDirectory(newPuth))
		printf("%s No such file or directory.\n", newPuth);
	return;
}