#include <headers.h>
#include <signal.h>

int main()
{
	int ret = -1;
	sigset_t set;
	sigset_t oldset;


	printf("set block signal:\n");
	if(-1 == sigemptyset(&set))
	{
		perror("sigemptyset");
		return 1;
	}
	ret = sigaddset(&set, SIGINT);
	if(-1 == ret)
	{
		printf("ret = %d", ret);
		perror("sigaddset");
		return 2;
	}
	ret = sigprocmask(SIG_BLOCK, &set, &oldset);
	if(-1 == ret)
	{
		printf("ret = %d", ret);
		perror("sigprocmask");
		return 3;
	}
	printf("signal SIGING is masked, press any key to unmask\n");
	getchar();

	if(-1 == sigprocmask(SIG_SETMASK, &oldset, NULL))
	{
		perror("sigprocmask");
		return 4;
	}

	printf("mask recovered");
	getchar();



	
    return 0;
}

