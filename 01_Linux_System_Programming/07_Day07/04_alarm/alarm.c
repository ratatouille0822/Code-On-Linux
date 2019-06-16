#include <headers.h>
#include <signal.h>

void fun(int signo)
{
	printf("signal numbdf is %d\n", signo);
	abort();
}
int main()
{
	int ret = -1;

	signal(SIGALRM, fun);
	ret = alarm(10);
	while(1)
	{
		printf("ret is %d\n", ret);
		sleep(1);
	}
    return 0;
}

