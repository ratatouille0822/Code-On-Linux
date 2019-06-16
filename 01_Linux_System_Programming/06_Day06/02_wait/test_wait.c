#include <headers.h>
#include <sys/wait.h>

int main()
{
	pid_t pid = -1;
	int ret = -1;
	int status = -1;

	pid = fork();
	if(-1 == pid)
	{
		perror("fork");
		return 1;
	}
	else if(0 == pid)
	{
		for(int i = 0; i< 20; i++)
		{
			printf("here is child process:%d, time delay %d \n",getpid() , 20-i);
			sleep(1);
		}
	}
	else if( pid > 0 )
	{
		ret = wait(&status);
		printf("--- Child process %d is stopped\n", pid);
	}

	if(WIFEXITED(status))
	{
		printf("--- Child precess exited, status is %d\n", WEXITSTATUS(status));
	}
	if(WIFSIGNALED(status))
	{
		printf("--- Child precess received a signal, status is %d\n", WTERMSIG(status));
	}
	if(WIFSTOPPED(status))
	{
		printf("--- Child precess received a signal, status is %d\n", WSTOPSIG(status));
	}
	if(WIFCONTINUED(status))
	{
		printf("--- Child precess received a continue signal\n");
	}
    return 0;
}

