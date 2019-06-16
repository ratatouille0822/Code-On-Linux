#include <headers.h>
#include <signal.h>

int main()
{
	sigset_t set;
	int ret = -1;

	ret = sigfillset(&set);
	if(-1 == ret)
	{
		perror("sigfillset");
		return 1;
	}

	printf("--->01\n");
	for(int i = 1; i <32; i++)
	{
		if(sigismember(&set, i))
		printf("%d--", i);
	}
	printf("\n");

	ret = sigemptyset(&set);
	if(-1 == ret)
	{
		perror("sigemptyset");
		return 2;
	}

	printf("--->02\n");
	for(int i = 1; i <32; i++)
	{
		if(sigismember(&set, i))
		printf("%d--", i);
	}

	printf("--->03\n");
	ret = sigaddset(&set, SIGINT);
	for(int i = 1; i <32; i++)
	{
		if(sigismember(&set, i))
		printf("%d--", i);
	}



    return 0;
}

