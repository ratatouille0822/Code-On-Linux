#include <headers.h>

#define SIZE 512
#define B_SIZE  520

int main()
{
	int ret = -1;
	int fd1 = -1;
	int fd2 = -1;

	char buffer_write[SIZE];
	char buffer_read[SIZE];
	char send[B_SIZE];

	pid_t pid = -1;

	fd1 = open("./fifo_1", O_RDONLY);
	if(-1 == fd1)
	{
		printf("----------------1------------");
		perror("open");
		return 1;
	}
	fd2 = open("./fifo_2", O_WRONLY);
	if(-1 == fd1)
	{
		printf("----------------2------------");
		perror("open");
		return 2;
	}

	pid = fork();
	if(-1 == pid )
	{
		printf("----------------3------------");
		perror("fork");
		return 3;
	}
	else if(0 == pid)
	{
		while(1)
		{
			memset(buffer_read, 0, SIZE);
			ret = read(fd1, buffer_read, SIZE);
			if(ret < 0)
			{
				printf("ret = %d\n", ret);
				perror("read");
				return 4;
			}
			printf("%s\n", buffer_read);
		}
		exit(0);
	}
	else if( pid > 0 )
	{
		while(1)
		{
			memset(buffer_write, 0, SIZE);
			fgets(buffer_write, sizeof(buffer_write), stdin);
			if(buffer_write[strlen(buffer_write) - 1] == '\n')
			{
				buffer_write[strlen(buffer_write)- 1] = '\0';
			}
			sprintf(send, "Jerry:%s", buffer_write);
			ret = write(fd2, send, strlen(send));
			if(ret < 0)
			{
				printf("ret = %d\n", ret);
				perror("write");
				return 5;
			}

		}
	}
    return 0;
}

