#include <headers.h>
#include <signal.h>

typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);


void fun(int signo)
{
	printf("signal number is %d\n", signo);
}

int main()
{
	int ret = -1;
	pid_t pid = -1;
	int i = 0;

	if(SIG_ERR == signal(SIGINT, fun))
	{
		perror("signal");
		return 3;
	}

	pid = fork();
	if(-1 == pid)
	{
		perror("fork");
		return 1;
	}
	else if(0 == pid)
	{
		while(1)
		{
			printf("child is live %d\n", i++);
			printf("father is %d\n", getppid());
			sleep(1);
			if(10 == i)
			{
				abort();
			}
		}
		
		exit(0);
	}

	sleep(1);
	printf("print any key to kill child process\n");
	getchar();

	ret = kill(pid, SIGINT);
	if(-1 == ret)
	{
		perror("kill");
		return 2;
	}



    return 0;
}

