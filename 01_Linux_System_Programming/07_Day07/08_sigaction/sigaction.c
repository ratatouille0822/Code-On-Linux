#include <headers.h>

void myfun(int sig)
{
	printf("sig = %d\n", sig);
	sleep(5);
	printf("wake up...\n");
}

int main()
{
	int ret = -1;
	int fd = -1;

	sigval_t test;
	struct sigaction act= {
		//.sa_sigaction = fun,
		.sa_flags = 0,
		.sa_handler = myfun
	};


	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGQUIT);

	ret = sigaction(SIGINT, &act, NULL);

	while(1);
    return 0;
}

