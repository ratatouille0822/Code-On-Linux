#include <headers.h>
#include <sys/wait.h>

#define SIZE 1024 * 1024

int main()
{
	int ret = -1;
	int fd[2];
	char buffer[SIZE];

	pid_t pid = -1;

	ret = pipe(fd);
	if(-1 == ret)
	{
		perror("pipe");
		return 1;
	}

	pid = fork();
	if(-1 == pid)
	{
		perror("fork");
		return 2;
	}
	else if(0 == pid)
	{
		// grep bash:
		//memset(buffer, 0, SIZE);
		//ret = read(fd[0], buffer, SIZE);
		//if(-1 == ret)
		//{
		//	perror("read");
		//	return 4;
		//}
		ret = dup2(fd[0], STDIN_FILENO);
		execlp("grep", "grep", "bash",  NULL);
		//printf("%s \n", buffer);
		exit(0);
	}
	else if(pid > 0)
	{
		ret = dup2(fd[1], STDOUT_FILENO);
		if(-1 == ret)
		{
			perror("dup2");
			return 3;
		}
		execlp("ps", "ps","aux", NULL);

		wait(NULL);
	}
	
	close(fd[0]);
	close(fd[1]);
    return 0;
}

