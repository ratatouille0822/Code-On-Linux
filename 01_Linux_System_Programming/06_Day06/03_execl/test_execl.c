#include <headers.h>

int main()
{
	char* env[] = {"key = value", NULL};

	printf("hello from start\n");
	//execl("/bin/ls", "ls", "-al", NULL);
	//execlp("ls", "ls","-al", NULL);
	execle("/bin/ls", "ls", NULL, env);
	
	printf("hello from end \n");
    return 0;
}

