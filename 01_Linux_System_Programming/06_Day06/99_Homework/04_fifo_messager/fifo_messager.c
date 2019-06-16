#include <headers.h>

#define SIZE 512
#define B_SIZE 520

int main()
{
	int ret = -1;
	int fd1 = -1;
	int fd2 = -1;

	pid_t pid = -1;

	char buffer[SIZE];
	char buffer_read[SIZE];
	char send[B_SIZE];
	ret = mkfifo("./fifo_1", 0644);
	if(-1 == ret)
	{
		perror("mkfifo");
		return 1;
	}

	ret = mkfifo("./fifo_2", 0644);
	if(-1 == ret)
	{
		perror("mkfifo");
		return 2;
	}

	fd1 = open("./fifo_1", O_WRONLY);
	if(fd1 <= 0)
	{
		perror("open");
		return 3;
	}

	fd2 = open("./fifo_2", O_RDONLY);
	if(fd2 <= 0)
	{
		perror("open");
		return 4;
	}

	pid = fork();
	if(-1 == pid)
	{
		perror("fork");
		return 1;
	}
	else if( 0 == pid )
	{
		while(1)
		{
			memset(buffer, 0, SIZE);
			fgets(buffer, sizeof(buffer), stdin);
			if(buffer[strlen(buffer) - 1] == '\n')
			{
				buffer[strlen(buffer)- 1] = '\0';
			}
			sprintf(send, "Tom:%s", buffer);
			ret = write(fd1, send, strlen(send));
			if(ret < 0)
			{
				printf("ret = %d, pid = %d", ret, getpid());
				perror("write");
				return 5;
			}
		}
	
		exit(0);
	}
	else if(pid > 0)
	{
		while(1)
		{
			memset(buffer_read, 0, SIZE);
			ret = read(fd2, buffer_read, SIZE);
			if(ret < 0)
			{
				printf("ret = %d, pid = %d", ret, getpid());
				perror("read");
				
				return 6;
			}
			printf("%s\n", buffer_read);
		}
	}
    return 0;
}

