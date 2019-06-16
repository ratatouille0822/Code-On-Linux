#include <headers.h>
#include <sys/wait.h>

void say_hello()
{
	printf("hello----------------------------------------------------------------- \n");
}

void judge_status(int status)
{
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
}

int main()
{
	pid_t pid[3] = {-1,-1,-1};
	pid_t wait_pid = 10;
	int ret = -1;
	int status = 10;
	char wild_pointer;

	for(int i = 0; i< 3; i++)
	{
		pid[i] = fork();
		if(-1 == pid[i])
		{
			perror("fork");
			return 1;
		}
		else if(0 == pid[i])
		{
			if(0 == i)
			{
				printf("this is child process 1 : pid = %d\n", getpid());
				system("ps aux | grep mysql");
			}
			else if(1 == i)
			{
				printf("this is child process 2 : pid = %d\n", getpid());
				say_hello();
			}
			else if(2 == i)
			{
				printf("this is child process 3 : pid = %d\n", getpid());
				free(&wild_pointer);
			}
			exit(0);
		}
		else if( pid[i] > 0 )
		{
			wait_pid = waitpid(-1, &status, 0);
			judge_status(status);
			printf("----\n");
			printf("wait_pid = %d\n", wait_pid);
			printf("status = %d \n", status);
			printf("----\n");
		}

	}

    return 0;
}

