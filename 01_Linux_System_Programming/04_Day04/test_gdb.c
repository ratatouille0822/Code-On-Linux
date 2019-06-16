#include <stdio.h>
#include "test_gdb_files.h"

void fun(void)
{
    printf("Hello from fun\n");
}

int main(int argc, char* argv[])
{
    printf("Hello world\n");

    for(int i = 0; i< 10; i++)
    {
        printf("---------------------------------->i is %d\n", i);
    }

    sayHello();

    fun();

    return 0;
}

