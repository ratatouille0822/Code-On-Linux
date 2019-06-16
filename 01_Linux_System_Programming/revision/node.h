#pragma once
#include <headers.h>

typedef struct Node
{
	struct Node * next;
} Node, *pNode;

typedef struct ListManager
{
	Node head;
	int size;
} ListManager, *pListManager;

pListManager ListInit();
void AddNode(pListManager list_manager, void* data);
void Traversal(pListManager list_manager, void (*func)(void*));
void DelNode(pListManager list_manager, int pos);
int GetSize(pListManager list_manager);
void PushBack(pListManager list_manager, void* data);
void Pop(pListManager list_manager);

void DelList(pListManager list_manager);
