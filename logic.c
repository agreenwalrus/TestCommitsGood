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
Thread for for_cycle
*/
DWORD WINAPI executeForCycle(void* data)
{
	printf("\nFor cycle");
	return 0;
}

/*
Excecute command
Return 0 if access
*/

int excecuteCommand(struct command_struct * cmd, char *redirInp, char *redirOutp, char *redirError)
{
	//LookAtPipes
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcInf;
	char *cmdLine;
	int size;
	if (cmd->args)
	{
		size = strlen(cmd->args) + strlen(cmd->nameOfCmd) + 3;
		cmdLine = (char*)malloc(sizeof(char) * size);
		strcpy_s(cmdLine,  size, cmd->nameOfCmd);
		strcat_s(cmdLine, size, " ");
		strcat_s(cmdLine, size, cmd->args);
	} else
	{
		size = strlen(cmd->nameOfCmd) + 3;
		cmdLine = (char*)malloc(sizeof(char) * size);
		strcpy_s(cmdLine,  size, cmd->nameOfCmd);
	}
		
	ZeroMemory( &StartupInfo, sizeof(StartupInfo) );
	StartupInfo.cb = sizeof(StartupInfo);

	ZeroMemory( &ProcInf, sizeof(ProcInf) );
		
	if( !CreateProcess( NULL,							// имя файла, который должен быть запущен
						cmdLine,					// командная строка
						NULL,							// атрибуты защиты процесса
						NULL,							// атрибуты защиты потока
						FALSE,							// признак наследования дескрипторов
						0,								// флаг, определ¤ющий тип создаваемого процесса
						NULL,							// указатель на блок памяти, содержащий переменные окружени¤
						NULL,							// рабочий каталог нового процесса
						&StartupInfo,					// параметры окна нового процесса
						&ProcInf))						// в которую будут записаны идентификаторы и дескрипторы нового процесса
	{
		printf( "CreateProcess is failed (%d)\n", GetLastError());
		return -1;
	}
	addHandleToHProccesses(ProcInf.hProcess);
	return 0;
}

int excecuteIfBranch(struct if_branch_struct *if_br)
{
	char *varValue;
	if (!(varValue = findVariable(if_br->conditional->name)))
	{
		printf ("\nNo such variable\n");
		return -1;
	}
	switch(if_br->conditional->operation)
	{
	case OPER_CMP_EQU: {
		if (!strcmp(varValue, if_br->conditional->value))
			excecuteList(if_br->trueWay);
		else excecuteList(if_br->falseWay);
		break;
	}
	case OPER_CMP_NOT_EQU: {
		if (strcmp(varValue, if_br->conditional->value))
			excecuteList(if_br->trueWay);
		else excecuteList(if_br->falseWay);
		break;
	}
	case OPER_LESS: {
		if (strcmp(varValue, if_br->conditional->value) < 0)
			excecuteList(if_br->trueWay);
		else excecuteList(if_br->falseWay);
		break;
	}
	case OPER_GRET: {
		if (strcmp(varValue, if_br->conditional->value) > 0)
			excecuteList(if_br->trueWay);
		else excecuteList(if_br->falseWay);
		break;
	}
	default: {
		printf("Unknown type of comparing at if condition\n");
		break;
	}
	}
	return 0;
}

/*
Direct next excecution
*/

int excecuteNode(struct node_struct * node, BOOL first, BOOL last, struct redirection_struct* redir)
{
	char *inp, *out, *err;
	inp = NULL;
	out = NULL;
	err = NULL;

	if (!strcmp(node->toDo->name, CMD_NAME))
	{
		if (first && last && redir)
		{
			inp = redir->inputFile;
			out = redir->outputClearFile;
			err = redir->errorFile;
		} else if (first && redir)
			inp = redir->inputFile;
		else if (last && redir)
		{
			out = redir->outputClearFile;
			err = redir->errorFile;
		}
		if (excecuteCommand(node->toDo->cmd.command, inp, out, err))
			return -1;
	} else if (!strcmp(node->toDo->name, IF_NAME))
	{
		if (excecuteIfBranch(node->toDo->cmd.if_branch))
			return -1;
	} else if (!strcmp(node->toDo->name, FOR_NAME))
	{
		HANDLE hThread;
		DWORD   dwThread;
		hThread = CreateThread( 
				NULL,                   // default security attributes
				0,                      // use default stack size  
				executeForCycle,       // thread function name
				node->toDo->cmd.for_cycle,          // argument to thread function 
				0,                      // use default creation flags 
				&dwThread);   // returns the thread identifier 
		if (!hThread)
		{
			printf("Error of for cycle\n");
			return -1;
		}

		addHandleToHProccesses(hThread);
	}
	return 0;
}

/*
Start for proccessing input
*/
int excecuteList(struct list_struct *list)
{
	struct node_struct *iter;
	int i;
	iter = list->head;

	for (i = 0; i < list->size; i++)
	{
		if (iter)
		{
			if (i == 0 && list->size == 1) excecuteNode(iter, TRUE, TRUE, list->redirection);
			else if (i == 0) excecuteNode(iter, TRUE, FALSE, list->redirection);
			else if (i == list->size - 1) excecuteNode(iter, FALSE, TRUE, list->redirection);
		

			iter = iter->next;
		} else {
			printf("Error at excecuteList\n");
			return -1;
		}
	}
	if (!list->excecAtBackGr)
		WaitForMultipleProcceses();
	return 0;
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