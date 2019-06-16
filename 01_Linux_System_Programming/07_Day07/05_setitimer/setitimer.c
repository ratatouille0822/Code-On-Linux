#include <headers.h>
#include <signal.h>
#include <sys/time.h>

void fun(int signo)
{
	static int i = 0;
	printf("%d th timer overflow, signal num = %d \n",++i, signo);
}

int main()
{
	struct itimerval timer_val = {
		.it_interval = {1, 0},
		.it_value = {3, 0}
	};

	int ret = -1;
	ret = setitimer(ITIMER_REAL, &timer_val, NULL);
	if(ret == -1)
	{
		perror("setitimer");
		return 1;
	}

	signal(SIGALRM, fun);

	getchar();

    return 0;
}

