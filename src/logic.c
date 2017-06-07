#include "logic.h"




void help()
{
	printf ("\n------------------------------------------ HELP ----------------------------------------------------");
	printf ("\n\n'help' - command for getting help information. Doesn't need arguments;");
	printf ("\n'cd' - command for changing directory. Doesn't need arguments;");
	printf ("\n'sleep' - command for stopping current thread. Needs arguments(number milliseconds for sleep);");
	printf ("\n'exit' - exit from shell.  Doesn't need arguments;");
	printf ("\n'getAliases' - to see current aliases");
	printf ("\n'alias name_of_alias = \"instruction_name\"' - to create alias");
	printf ("\n'unalias \"name_of_alias\"' - to delete alias");
	printf ("\n\nOther cases:");
	printf ("\nif [var_name [>, <, ==] value] then cmds_to_do else cmds_to_do;");
	printf ("\nfrom low_bound until high_bound do cmds_to_do [&]?;");
	printf ("\n\nYou can also use variables:");
	printf ("\n~ definition: var_name = value");
	printf ("\n~ boxing: echo $var_name => result: value(at the stdout)");
	printf ("\n\nOther commands:");
	printf ("\ncmd_name [argument]* ['&&' or '||' or '|' cmd_name [argument]*]*['>' or '<' or '>&' filename]{0, 3}.");
	printf ("\n\n------------------------------------------ HELP ----------------------------------------------------\n");
}

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
		printf ("Error of getting name of user %d\n", (int)GetLastError());
		return;
	}
	buf_size = (DWORD) BUFSIZ;
	if (!GetComputerName(cmpName, &buf_size))
	{
		printf ("Error of getting name of computer %d\n", (int)GetLastError());
		return;
	}
	printf("\n[%s]@[%s]\n", usrName, cmpName);
}

/*
Prints current directory
*/
void intrToTyping(char *result, int size)
{
	int len;
	if (!GetCurrentDirectory(size - 1, &result[1]))
	{
		printf ("Error of getting current directory %d\n", (int)GetLastError());
		result[0] = '\n';
		result[1] = ' ';
		result[2] = '\0';
		return;
	}
	result[0] = '\n';
	len = strlen(result);
	result[len] = '$';
	result[len + 1] = ' ';
	result[len + 2] = '\0';
}

int executeListWithFor(struct list_struct* list)
{
	HANDLE hThread;
	DWORD dwThread;

	hThread = CreateThread( 
			NULL,                   // default security attributes
			0,                      // use default stack size  
			executeForCycle,       // thread function name
			list,          // argument to thread function 
			0,                      // use default creation flags 
			&dwThread);   // returns the thread identifier 
	if (!hThread)
	{
		printf("Error of for cycle\n");
		return -1;
	}
		
	if(!list->excecAtBackGr)
		WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	return 0;
}



int execute(struct list_struct* list)
{
	if (list->size == 1)
	{
		if (!strcmp(list->head->toDo->name, IF_NAME))
		{
			if (executeIfBranch(list))
			{
				freeListStruct(*list);
				free(list);
				return -1;
			}
			freeListStruct(*list);
			free(list);
		} else if (!strcmp(list->head->toDo->name, FOR_NAME))
		{
			if (executeListWithFor(list))
				return -1;
		} else if (!strcmp(list->head->toDo->name, CMD_NAME))
		{
			if (excecuteList (list))
			{			
				freeListStruct(*list);
				free(list);
				return -1;
			} else {
				freeListStruct(*list);
				free(list);
			}
		}

	} else	
	{
		if (excecuteList (list))
		{			
			freeListStruct(*list);
			free(list);
			return -1;
		} else {
			freeListStruct(*list);
			free(list);
		}
	}

	return 0;
}


/*
Thread for for_cycle
*/
DWORD WINAPI executeForCycle(void* data)
{
	int i, from, until;
	struct list_struct *list = (struct list_struct *)data;
	struct for_cycle_struct *for_c;

	if (!list)
	{
		printf("\nError at executing for cycle\n");
		return -1;
	}

	for_c = list->head->toDo->cmd.for_cycle;
	if (for_c->from && for_c->until)
	{
		if ((!(from = atoi(for_c->from)) && strcmp( for_c->from, "0")) || (!(until = atoi (for_c->until)) && strcmp( for_c->until, "0")))
		{
			printf ("Wrong arguments. Expected integers. fom %s until %s", for_c->from, for_c->until);
			freeListStruct (*list);
			free (list);
			return -1;
		}

		if (until <= from)
			printf("Wrong diapason.\n");
		for (i = from; i < until; i++)
		{
			if (excecuteList(for_c->instractionsToDo))
			{
				freeListStruct (*list);
				free (list);
				ExitThread (-1);
			}
		}
	} else printf ("Wrong arguments. Expected integers. [from %s until %s]", for_c->from, for_c->until);
	freeListStruct (*list);
	free (list);

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
	} else if (!strcmp(cmdName, "help"))
	{
		help();
	} else if (!strcmp(cmdName, "clear"))
	{
		cleanScrean();
	} else if (!strcmp(cmdName, "getAliases"))
	{
		getAliases();
	}		
	else return -1;
	return 0;
}

int executeOtherCMD(struct command_struct * cmd, HANDLE ** handles)
{
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

	StartupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	StartupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	StartupInfo.hStdError =  GetStdHandle(STD_ERROR_HANDLE);

	if (handles)
	{
		if (handles[INPUT_REDIR])
			StartupInfo.hStdInput = *handles[INPUT_REDIR];
		if (handles[OUTPUT_REDIR])
			StartupInfo.hStdOutput = *handles[OUTPUT_REDIR];
		if (handles[ERROR_REDIR])
			StartupInfo.hStdError = *handles[ERROR_REDIR];
	}
	StartupInfo.dwFlags = STARTF_USESTDHANDLES;
		
	if( !CreateProcess( NULL,							// имя файла, который должен быть запущен
						cmdLine,						// командная строка
						NULL,							// атрибуты защиты процесса
						NULL,							// атрибуты защиты потока
						TRUE,							// признак наследования дескрипторов
						0,								// флаг, определ¤ющий тип создаваемого процесса
						NULL,							// указатель на блок памяти, содержащий переменные окружени¤
						NULL,							// рабочий каталог нового процесса
						&StartupInfo,					// параметры окна нового процесса
						&ProcInf))						// в которую будут записаны идентификаторы и дескрипторы нового процесса
	{
		printf( "CreateProcess is failed (%d)\n", (int)GetLastError());
		return -1;
	}
	addHandleToHProccesses(ProcInf.hProcess);
	if (handles)
	{
		if (handles[INPUT_REDIR])
		{
			CloseHandle(*handles[INPUT_REDIR]);
			free(handles[INPUT_REDIR]);
			handles[INPUT_REDIR] = NULL;
		}
		if (handles[OUTPUT_REDIR])
		{
			CloseHandle(*handles[OUTPUT_REDIR]);
			free(handles[OUTPUT_REDIR]);
			handles[OUTPUT_REDIR] = NULL;
		}
		if (handles[ERROR_REDIR])
		{
			CloseHandle(*handles[ERROR_REDIR]);
			free (handles[ERROR_REDIR]);
			handles[ERROR_REDIR] = NULL;
		}
	}

	free(cmdLine);
	return 0;
}

int executeIfBranch(struct list_struct *list)
{
	struct if_branch_struct *if_br = list->head->toDo->cmd.if_branch;
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
		{
			if (if_br->trueWay) execute(if_br->trueWay);
		}else if (if_br->falseWay) execute(if_br->falseWay);
		break;
	}
	case OPER_LESS: {
		if (strcmp(varValue, if_br->conditional->value) < 0)
		{
			if (if_br->trueWay) execute(if_br->trueWay);
		} else if (if_br->falseWay) execute(if_br->falseWay);
		break;
	}
	case OPER_GRET: {
		if (strcmp(varValue, if_br->conditional->value) > 0)
		{
			if (if_br->trueWay) execute(if_br->trueWay);
		} else if (if_br->falseWay) execute(if_br->falseWay);
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
Start for proccessing input
*/
int excecuteList(struct list_struct *list)
{
	BOOL errorFlag = 0;
	HANDLE *handles[4];
	SECURITY_ATTRIBUTES secAtr;
	HANDLE hPipeR, hPipeW;
	struct node_struct *iter;
	int i;
	secAtr.nLength = sizeof(SECURITY_ATTRIBUTES);
	secAtr.lpSecurityDescriptor = NULL;
	secAtr.bInheritHandle = TRUE;

	iter = list->head;

	for (i = 0; i < 4; i++)
		handles[i] = NULL;

	for (i = 0; i < list->size; i++)
	{
		if (iter)
		{
			if (isMountedCommand(*iter->toDo->cmd.command))
			{
				if (executeBuildInCMD(iter->toDo->cmd.command->nameOfCmd, iter->toDo->cmd.command->args))
					return -1;
				iter = iter->next;
				continue;
			}
			if (0 == i && list->redirection && list->redirection->inputFile)
			{
				if (!(handles[INPUT_REDIR] = (HANDLE*)malloc(sizeof(HANDLE))))
				{
					printf ("\nNo memory");
					
					return -1;
				
				}
				*handles[INPUT_REDIR] = CreateFile(list->redirection->inputFile, GENERIC_READ, FILE_SHARE_READ, &secAtr, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, NULL);
				if (*handles[INPUT_REDIR] == INVALID_HANDLE_VALUE)
				{
					printf("\nError of opening input redirection file: '%s' error(%d)", list->redirection->inputFile, (int)GetLastError());
					return -1;
				}
			}
			if (list->size - 1 == i && list->redirection && list->redirection->outputClearFile)
			{
				if (!(handles[OUTPUT_REDIR] = (HANDLE*)malloc(sizeof(HANDLE))))
				{
					printf ("\nNo memory");
					return -1;
				}
				*handles[OUTPUT_REDIR] = CreateFile(list->redirection->outputClearFile, GENERIC_WRITE, FILE_SHARE_WRITE, &secAtr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if (*handles[OUTPUT_REDIR] == INVALID_HANDLE_VALUE)
				{
					printf("\nError of opening output redirection file: '%s' error(%d)",  list->redirection->outputClearFile, (int)GetLastError());
					return -1;
				}
			}
			if (list->size - 1 == i && list->redirection && list->redirection->errorFile)
			{
				if (!(handles[ERROR_REDIR] = (HANDLE*)malloc(sizeof(HANDLE))))
				{
					printf ("\nNo memory");
					return -1;
				}
				*handles[ERROR_REDIR] = CreateFile(list->redirection->errorFile, GENERIC_WRITE, FILE_SHARE_WRITE, &secAtr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if (*handles[ERROR_REDIR] == INVALID_HANDLE_VALUE)
				{
					printf("\nError of opening error redirection file %d", (int)GetLastError());
					return -1;
					
				}
			}

			if (handles[RESERVED])
			{
				if (!SetHandleInformation(*handles[RESERVED], 0, HANDLE_FLAG_INHERIT))
				{
					printf("\nError SetHandleInformation %d", (int)GetLastError());
					return -1;
				}
				handles[INPUT_REDIR] = handles[RESERVED];
				handles[RESERVED] = NULL;
			}

			switch(iter->toDo->connectionWithNextBitMask)
			{
			case CONNECT_ANDAND:
			case CONNECT_NO:
			case CONNECT_SEMICOLON:
				{
					if (executeOtherCMD(iter->toDo->cmd.command, handles))
					{
						WaitForMultipleProcceses();
						return -1;
					}
					break;
				}
			case CONNECT_OROR:
				{
					if (!executeOtherCMD(iter->toDo->cmd.command, handles))
					{
						WaitForMultipleProcceses();
						return 0;
					}
					break;
				}
			case CONNECT_PIPE:
				{
					if (!CreatePipe(&hPipeR, &hPipeW, &secAtr, 0))
					{
						printf("\nError of creation pipe %d", (int)GetLastError());
						return -1;
					}

					if (!(handles[OUTPUT_REDIR] = (HANDLE*)malloc(sizeof(HANDLE))))
					{
						printf ("\nNo memory");
						return -1;
					}

					if (!(handles[RESERVED] = (HANDLE*)malloc(sizeof(HANDLE))))
					{
						printf ("\nNo memory");
						return -1;
					}

					if (!SetHandleInformation(hPipeW, 0, HANDLE_FLAG_INHERIT || !SetHandleInformation(hPipeR, HANDLE_FLAG_INHERIT, 0)))
					{
						printf("\nError SetHandleInformation %d", (int)GetLastError());
						return -1;
					}

					*handles[OUTPUT_REDIR] = hPipeW;
					*handles[RESERVED] = hPipeR;

					if (executeOtherCMD(iter->toDo->cmd.command, handles))
					{
						WaitForMultipleProcceses();
						return -1;
					}

					break;
				}
			}

			iter = iter->next;
		} else {
			printf("Error at excecuteList\n");
			return -1;
		}
	}
	WaitForMultipleProcceses();
	return 0;
}

/*
Mounted clear
*/

void cleanScrean()
{
	if (system("CLS")) system("clear");
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
