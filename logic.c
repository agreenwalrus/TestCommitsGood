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
Start execute
*/

int execute(struct list_struct *list)
{
	printf("\nexecute(struct list_struct *list)");
	excecuteList(list);
	
	if (!list->excecAtBackGr)
		WaitForMultipleProcceses();
	else resetHandles();
	
	freeListStruct(*list);
	free(list);

	return 0;
}

/*
Thread for for_cycle
*/
DWORD WINAPI executeForCycle(void* data)
{
	int i, from, until;
	struct for_cycle_struct *for_c = (struct for_cycle_struct *)data;
	
	if ((!(from = atoi(for_c->from)) && strcmp( for_c->from, "0")) || (!(until = atoi (for_c->until)) && strcmp( for_c->until, "0")))
	{
		printf ("Wrong arguments. Expected integers.");
	}
	
	for (i = from; i < until; i++)
	{
		if (excecuteList(for_c->instractionsToDo))
			return -1;
	}
	return 0;
}

int executeBuildInCMD(char *cmdName, char *args)
{
	int millisec;
	if (!strcmp(cmdName, "cd"))
		if (!args)
		{
			printf ("'cd' neeeds arguments.");
			return -1;
		} else changeDirectory(args);
	else if (!strcmp(cmdName, "sleep"))
	{
		if (!args)
		{
			printf ("'sleep' neeeds arguments.");
			return -1;
		} else if (!(millisec = atoi(args)))
		{
			printf ("Wrong arguments for 'sleep'");
			return -1;
		} else sleep_shell(millisec);

	}
}

int executeOtherCMD(struct command_struct * cmd, char *redirInp, char *redirOutp, char *redirError)
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
		strncpy(cmdLine, cmd->nameOfCmd, size);
		strncat(cmdLine, " ", size);
		strncat(cmdLine, cmd->args, size);
	} else
	{
		size = strlen(cmd->nameOfCmd) + 3;
		cmdLine = (char*)malloc(sizeof(char) * size);
		strncpy(cmdLine, cmd->nameOfCmd,  size);
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

/*
Excecute command
Return 0 if access
*/

int excecuteCommand(struct command_struct * cmd, char *redirInp, char *redirOutp, char *redirError)
{
	if (isMountedCommand(*cmd))
	{
		if (executeBuildInCMD(cmd->nameOfCmd, cmd->args))
			return -1;
	}
	else if (executeOtherCMD(cmd, redirInp, redirOutp, redirError))
		return -1;
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

int excecuteNode(struct node_struct * node, BOOL first, BOOL last, struct redirection_struct* redir, BOOL execAtBG)
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
		printf("\nCreating for_cycle thread (%d)", execAtBG);
		struct for_cycle_struct *temp = node->toDo->cmd.for_cycle;
		HANDLE hThread;
		DWORD   dwThread;
		hThread = CreateThread( 
				NULL,                   // default security attributes
				0,                      // use default stack size  
				executeForCycle,       // thread function name
				temp,          // argument to thread function 
				0,                      // use default creation flags 
				&dwThread);   // returns the thread identifier 
		if (!hThread)
		{
			printf("Error of for cycle\n");
			return -1;
		}
		
		if(!execAtBG)
			WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		
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
			if (i == 0 && list->size == 1) excecuteNode(iter, TRUE, TRUE, list->redirection, list->excecAtBackGr);
			else if (i == 0) excecuteNode(iter, TRUE, FALSE, list->redirection, list->excecAtBackGr);
			else if (i == list->size - 1) excecuteNode(iter, FALSE, TRUE, list->redirection, list->excecAtBackGr);
		

			iter = iter->next;
		} else {
			printf("Error at excecuteList\n");
			return -1;
		}
	}
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

/*
Build-in 'sleep'
*/
void sleep_shell(int millisec)
{
	Sleep(millisec);
}
