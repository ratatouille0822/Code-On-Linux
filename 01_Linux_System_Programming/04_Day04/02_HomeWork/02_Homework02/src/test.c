#include <stdio.h>
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"


int main()
{
	int x = 100, y = 50;
	printf("x+y = %d\n", add(x,y));
	printf("x-y = %d\n", sub(x,y));
	printf("x*y = %d\n", mul(x,y));
	printf("x/y = %d\n", mydiv(x,y));
    return 0;
}

