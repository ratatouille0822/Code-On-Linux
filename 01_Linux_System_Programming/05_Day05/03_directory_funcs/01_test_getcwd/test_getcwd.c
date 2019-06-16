#include <headers.h>

int main()
{
	char buffer[128];

	getcwd(buffer, 128);
	printf("pwd = %s\n", buffer);
    return 0;
}

