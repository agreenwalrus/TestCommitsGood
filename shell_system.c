#include "shell_system.h"


struct variable_struct* global_variable_array;
int global_variable_array_size;
int amount_of_global_variables;

char *system_commands_shell[] = {
	"cd",
	"pwd"
};


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
	printf("\nfindVariable");
	
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
	if (!(global_variable_array = (struct variable_struct*)(malloc(START_SIZE * sizeof(struct variable_struct)))))
	{
		printf("\nError of initialisation shell");
		return -1;
	}
	global_variable_array_size = START_SIZE;
	amount_of_global_variables = 0;

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


/*
add new variable to global_variable_array
*/
int addVariable(struct variable_struct newVariable)
{
	int i;
	printf("\naddVariable %s %s %d", newVariable.varName, newVariable.varValue, amount_of_global_variables);
	
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
