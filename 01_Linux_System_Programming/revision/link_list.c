#include <headers.h>
#include "node.h"

typedef struct UserData{
	void* ptr;
	int id;
	char name[100];
} UserData, *pUserData;

void printData(void* data)
{
	UserData print_data = *(pUserData)(data);
	printf("id = %d, name = %10s \n", print_data.id, print_data.name);
}

void test01()
{
	pListManager list_manager;
	list_manager = ListInit();

	printf("size = %d\n", GetSize(list_manager));
	UserData data1 = {NULL, 1, "str1"};
	UserData data2 = {NULL, 2, "str2"};
	UserData data3 = {NULL, 3, "str3"};
	UserData data4 = {NULL, 4, "str4"};

	AddNode(list_manager, &data1);
	AddNode(list_manager, &data2);
	AddNode(list_manager, &data3);
	AddNode(list_manager, &data4);

	Traversal(list_manager, printData );
	printf("size = %d\n", GetSize(list_manager));

	DelNode(list_manager, 2);
	Traversal(list_manager, printData );
	printf("size = %d\n", GetSize(list_manager));
	
	PushBack(list_manager, &data3);
	Traversal(list_manager, printData );
	printf("size = %d\n", GetSize(list_manager));

	Pop(list_manager);
	Traversal(list_manager, printData );
	printf("size = %d\n", GetSize(list_manager));


	DelList(list_manager);
}


int main()
{
	test01();
    return 0;
}

