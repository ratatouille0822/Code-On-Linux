#include "node.h"

pListManager ListInit()
{
	pListManager temp = (pListManager)malloc(sizeof(ListManager));
	temp->head.next = NULL;
	 
	temp->size = 0;

	return temp;
}

void AddNode(pListManager list_manager, void* data)
{
	pNode new_node = (pNode)data;

	if(NULL == list_manager || NULL == data)
	{
		printf("faild AddNode\n");
		return ;
	}
	new_node->next = list_manager->head.next;
	list_manager->head.next = new_node;
	list_manager->size += 1;
}

void Traversal(pListManager list_manager,void (*func)(void* ) )
{
	if(NULL == list_manager)
	{
		printf("Failed Traversal\n");
		return ;
	}

	pNode iter = list_manager->head.next;
	while(NULL != iter)
	{
		printf("iter is at %p\n", iter);
		func((void*)iter);
		iter = iter->next;
	}
}

void DelNode(pListManager list_manager, int pos)
{
	pNode temp = NULL;
	pNode iter = &list_manager->head;

	if(NULL == list_manager || pos > list_manager->size)
	{
		printf("failed delete\n");
		return ;
	}

	while(pos -1)
	{
		iter = iter->next;
		pos--;
	}
	temp = iter->next;
	iter->next = iter->next->next;
	temp->next = NULL;
	list_manager->size--;
}

int GetSize(pListManager list_manager)
{
	if(NULL == list_manager)
	{
		return 0;
	}
	else
	{
		return list_manager->size;
	}
}

void PushBack(pListManager list_manager, void* data)
{
	int tmp_pos = list_manager->size;
	pNode iter = &list_manager->head;
	pNode new_node = (pNode)data;

	char *p  = data;
	while(tmp_pos)
	{
		iter = iter->next;
		tmp_pos--;
	}
	iter->next = new_node;
	new_node->next = NULL;

	list_manager->size++;
}

void Pop(pListManager list_manager)
{
	DelNode(list_manager, list_manager->size);
}

void DelList(pListManager list_manager)
{
	free(list_manager);
}
