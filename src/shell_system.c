#include "shell_system.h"


struct variable_struct* global_variable_array;
int global_variable_array_size;
int amount_of_global_variables;

HANDLE *hProccesses;
int amountOfProc;
int maxAmountOfHProc;

struct variable_struct* alias_array;
int alias_array_size;
int amount_of_alias_variables;

char *system_commands_shell[] = {
	"cd",
	"sleep",
	"exit",
	"help",
	"clear",
	"alias",
	"getAliases"
};
void getHistoryFilePath(char* buffer, int size)
{
	if (!GetCurrentDirectory(size, buffer))
	{
		printf ("Error of getting current directory %d\n", (int)GetLastError());
		return;
	}
	strncat(buffer, "\\\0", size);
	strncat(buffer, HISTORY_FILE, size);
}
void resetHandles()
{
	int i;
	for ( i = 0 ; i < amountOfProc; i++)
		CloseHandle(hProccesses[i]);
	amountOfProc = 0;
}

void WaitForMultipleProcceses()
{
	WaitForMultipleObjects((DWORD)amountOfProc, hProccesses, TRUE, INFINITE);
	resetHandles();
}


//Free memory


void freeHProccesses()
{
	free(hProccesses);
	hProccesses = NULL;
	amountOfProc = 0;
}


//Add handle to array

int addHandleToHProccesses(HANDLE hProc)
{
	if (amountOfProc == maxAmountOfHProc)
		if (reallocHProccesses())
			return -1;

	hProccesses[amountOfProc] = hProc;
	amountOfProc = amountOfProc + 1;

	return 0;
}

/*
Check name of command at **system_commands_shell; If it should be excecuted at current process
*/
BOOL isMountedCommand(struct command_struct cmd)
{
	int i;

	for (i = 0; i < AMOUNT_OF_SYSTEM_CMDS; i++)
		if (!strcmp(system_commands_shell[i], cmd.nameOfCmd))
			return TRUE;
	return FALSE;
}

/*
It gets the name of var. Search through global_variable_array.
Returns:
value(char*) - of variable if exist
NULL - if not
*/

char* findVariable (char *varName)
{
	int i;
	
	for (i = 0; i < amount_of_global_variables; i++)
		if (!strcmp(varName, global_variable_array[i].varName))
			if (global_variable_array[i].varValue)
				return strdup(global_variable_array[i].varValue);
	return NULL;
}

/*
function for allocation mamory of shell_system arrays and other variables

returns:
	0 if successful
	-1 if errors
*/



int initShell()
{
	SetConsoleTitle(CONSOLE_NAME);
	if (!(global_variable_array = (struct variable_struct*)(malloc(START_SIZE * sizeof(struct variable_struct)))))
	{
		printf("\nError of initialisation shell");
		return -1;
	}
	global_variable_array_size = START_SIZE;
	amount_of_global_variables = 0;

	if (!(hProccesses = (HANDLE*) malloc(sizeof(HANDLE) * START_SIZE)))
	{
		printf ("\nError of allocation memory for hProccesses (%x)", (int)GetLastError());
		return -1;
	}
	amountOfProc = 0;
	maxAmountOfHProc = START_SIZE;

	if (!(alias_array = (struct variable_struct*)(malloc(START_SIZE * sizeof(struct variable_struct)))))
	{
		printf("\nError of initialisation shell");
		return -1;
	}
	alias_array_size = START_SIZE;
	amount_of_alias_variables = 0;

	return 0;
}



//Realloc memory for hProccesses


int reallocHProccesses()
{
	int i;
	HANDLE* temp;
	
	if(!(temp = (HANDLE*)malloc((amountOfProc + START_SIZE) * sizeof(HANDLE))))
	{
		printf("\nError of reallocation in hProccesses");
		return -1;
	}
	for(i = 0; i < amountOfProc; i++)
		temp[i] = hProccesses[i];
	
	free(hProccesses);
	hProccesses = temp;	
	maxAmountOfHProc += START_SIZE;

	return 0;

}

/*
realloc memory of alias_array
*/

int reallocAliasArray()
{
	int i;
	struct variable_struct* temp;
	
	if(!(temp = (struct variable_struct*)malloc((alias_array_size + START_SIZE) * sizeof(struct variable_struct))))
	{
		printf("\nError of reallocation in reallocAliasArray");
		return -1;
	}
	for(i = 0; i < alias_array_size; i++)
		temp[i] = alias_array[i];
	
	free(alias_array);
	alias_array = temp;	
	alias_array_size += START_SIZE;

	return 0;
}

/*
realloc memory of global_variable_array
*/

int reallocGlobalVariableArray()
{
	int i;
	struct variable_struct* temp;
	
	if(!(temp = (struct variable_struct*)malloc((global_variable_array_size + START_SIZE) * sizeof(struct variable_struct))))
	{
		printf("\nError of reallocation in reallocGlobalVariableArray");
		return -1;
	}
	for(i = 0; i < global_variable_array_size; i++)
		temp[i] = global_variable_array[i];
	
	free(global_variable_array);
	global_variable_array = temp;	
	global_variable_array_size += START_SIZE;

	return 0;
}

/*
It frees memory for global_variable_array
*/

void destroyShell()
{
	int i;

	for(i = 0; i < amount_of_global_variables; i++)
		freeMemoryForVariable(global_variable_array[i]);
	
	free(global_variable_array);
	free(hProccesses);

	
	for(i = 0; i < amount_of_alias_variables; i++)
		freeMemoryForVariable(alias_array[i]);

	free(alias_array);
}

/*
It frees memory of struct variable_struct
*/

void freeMemoryForVariable(struct variable_struct variable)
{
	if (variable.varName)
		free(variable.varName);
	if (variable.varValue)
		free(variable.varValue);
	//free(variable);
}

char* getAlias(char* aliasName)
{
	int i;
	for (i = 0; i < amount_of_alias_variables; i++)
		if (!strcmp(alias_array[i].varName, aliasName))
		{			
			if (alias_array[i].varValue)
				return strdup(alias_array[i].varValue);
			else return NULL;
		}
	return NULL;
}


/*
add new variable to global_variable_array
*/
int addVariable(struct variable_struct newVariable)
{
	int i;
	
	if (amount_of_global_variables == global_variable_array_size)
		if (reallocGlobalVariableArray())
			return -1;
	for(i = 0; i < amount_of_global_variables; i++)
	{
		if (!strcmp(newVariable.varName, global_variable_array[i].varName))
		{
			freeMemoryForVariable(global_variable_array[i]);
			global_variable_array[i].varName = newVariable.varName;
			global_variable_array[i].varValue = newVariable.varValue;
			return 0;
		}
	}
	if (i == amount_of_global_variables)
		global_variable_array[i] = newVariable;
	amount_of_global_variables += 1;
	return 0;
}


/*
add new Alias
*/
int addAlias(struct variable_struct newVariable)
{
	int i;
	
	if (amount_of_alias_variables == alias_array_size)
		if (reallocAliasArray())
			return -1;
	for(i = 0; i < amount_of_alias_variables; i++)
	{
		if (!strcmp(newVariable.varName, alias_array[i].varName))
		{
			freeMemoryForVariable(alias_array[i]);
			alias_array[i].varName = newVariable.varName;
			alias_array[i].varValue = newVariable.varValue;
			return 0;
		}
	}
	if (i == amount_of_alias_variables)
		alias_array[i] = newVariable;
	amount_of_alias_variables += 1;
	return 0;
}

void deleteAlias(char* aliasName)
{
	int i, j;
	for (i = 0; i < amount_of_alias_variables; i++)
		if (!strcmp(alias_array[i].varName, aliasName))
		{
			if (alias_array[i].varValue)
				free(alias_array[i].varValue);
			if (alias_array[i].varName)
				free(alias_array[i].varName);

			for (j = i; j < amount_of_alias_variables - 1; j++)
				alias_array[j] = alias_array[j + 1];

			amount_of_alias_variables--;
		}
}

void getAliases ()
{
	int i;

	printf ("Shell aliases:\n\n");
	if (!amount_of_alias_variables)
	{
		printf("No aliases.\n");
		return;
	}
	for (i = 0; i < amount_of_alias_variables; i++)
		printf("%s - %s\n", alias_array[i].varName, alias_array[i].varValue);
	return;
}








