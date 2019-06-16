#include <headers.h>
#include <signal.h>

typedef void (*sighandler_t) (int);

void fun(int sig_num)
{
	printf("The signal number is %d \n", sig_num);
}

int main()
{
	int i = 0;
	sighandler_t sig_ret = NULL;

	sig_ret = signal(SIGINT, fun);
	if(SIG_ERR == sig_ret)
	{
		printf("---1---\n");
		perror("signal");
		return 1;
	}

	sig_ret = signal(SIGQUIT, fun);
	if(SIG_ERR == sig_ret)
	{
		printf("---2---\n");
		perror("signal");
		return 2;
	}
	while(1)
	{
		sleep(1);
		printf("delay %d times \n", i++);
	}

    return 0;
}

