#include <stdio.h>
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "my_div.h"

int main()
{
    int x = 1, y = 1;

    printf("x + y = %d\n", add(x, y));
    printf("x - y = %d\n", sub(x, y));
    printf("x * y = %d\n", mul(x, y));
    printf("x / y = %d\n", my_div(x, y));
    printf("Hello world\n");
    return 0;
}

