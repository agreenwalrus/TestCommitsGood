#include "bricks.h"

void freeVariableStruct(struct variable_struct st)
{
	printf ("\nfreeVariableStruct %s %s", st.varName, st.varValue);
	if (st.varName)
		free(st.varName);
	if	(st.varValue)
		free(st.varValue);
}

void freeCommandStruct(struct command_struct st)
{
	printf("\nfreeCommandStruct %s %s", st.nameOfCmd, st.args);
	if(st.args)
		free(st.args);
	if(st.nameOfCmd)
		free(st.nameOfCmd);
}

void freeOperateAtVariableStruct(struct operate_at_variabe_struct st)
{
	printf("\nfreeOperateAtVariableStruct %s %s", st.name, st.value);
	if(st.name)
		free(st.name);
	if(st.value)
		free(st.value);
}


void freeWhileCycleStruct(struct while_cycle_struct st)
{
	printf("\nfreeWhileCycleStruct ");
	if (st.conditional)
	{
		freeOperateAtVariableStruct(*st.conditional);
		free(st.conditional);
	}
	if (st.instractionsToDo)
	{
		freeListStruct(*st.instractionsToDo);
		free(st.instractionsToDo);
	}
}

void freeForCycleStruct(struct for_cycle_struct st)
{
	printf("\nfreeForCycleStruct %s %s", st.varName, st.varStates);
	if (st.varName)
		free(st.varName);
	if (st.varStates)
		free(st.varStates);
	if (st.instractionsToDo)
	{
		freeListStruct(*st.instractionsToDo);
		free(st.instractionsToDo);
	}
}

void freeIfBranchStruct(struct if_branch_struct st)
{
	printf("\nfreeIfBranchStruct");
	if (st.conditional)
	{
		freeOperateAtVariableStruct(*st.conditional);
		free(st.conditional);
	}
	if (st.falseWay)
	{
		freeListStruct(*st.falseWay);
		free(st.falseWay);
	}
	if (st.trueWay)
	{
		freeListStruct(*st.trueWay);
		free(st.trueWay);
	}
}

void freeWholeCommandStruct(struct whole_command_struct st)
{
	printf("\nfreeWholeCommandStruct %s", st.name);
	if (st.name)
	{
		if (!strcmp(st.name, FOR_NAME))
		{
			freeForCycleStruct(*st.cmd.for_cycle);
			free(st.cmd.for_cycle);
		} else if (!strcmp(st.name, WHILE_NAME))
		{
			freeWhileCycleStruct(*st.cmd.while_cycle);
			free(st.cmd.while_cycle);
		} else if (!strcmp(st.name, CMD_NAME))
		{
			freeCommandStruct(*st.cmd.command);
			free(st.cmd.command);
		} else if (!strcmp(st.name, OPER_AT_VAR_NAME))
		{
			freeOperateAtVariableStruct(*st.cmd.operate_at_variable);
			free(st.cmd.operate_at_variable);
		} else if (!strcmp(st.name, IF_NAME))
		{
			freeIfBranchStruct(*st.cmd.if_branch);
			free(st.cmd.if_branch);
		}
		free(st.name);
	}
}

void freeNodeStruct(struct node_struct st)
{
	printf("\nfreeNodeStruct");
	if (st.toDo)
	{
		freeWholeCommandStruct(*st.toDo);
		free(st.toDo);
	}
}

void freeListStruct(struct list_struct st)
{
	struct node_struct *iter;
	iter = st.head;	
	printf("\nfreeListStruct");
	while (iter)
	{
		freeNodeStruct(*iter);
		iter = iter->next;
	}
	if (st.redirection)
	{
		freeRedirecionStruct(*st.redirection);
		free(st.redirection);
	}
}

void freeRedirecionStruct(struct redirection_struct st)
{
	printf("\nfreeRedirecionStruct");
	if (st.errorFile)
		free(st.errorFile);
	if(st.inputFile)
		free(st.inputFile);
	if(st.outputAtTheEndFile)
		free(st.outputAtTheEndFile);
	if(st.outputClearFile)
		free(st.outputClearFile);
}

void addWholeCommandToList(struct list_struct* list, struct whole_command_struct* whole_command)
{
	struct node_struct *node;
	if (!whole_command || !list)
		return;
	if (!(node = (struct node_struct*)malloc(sizeof(struct node_struct))))
	{
		printf ("\nNo memory briks.c addWholeCommandToList");
		return;
	}
	node->prev = NULL;
	node->next = NULL;
	node->toDo = whole_command;
	if (!list->head)
		list->head = list->tail = node;
	else if (list->head == list->tail)
	{
		list->tail = node;
		node->prev = list->head;
		list->head->next = node;
	}
	else {
		list->tail->next = node;
		node->prev = list->tail;
		list->tail = node;
	}
	list->size = list->size + 1;
}
