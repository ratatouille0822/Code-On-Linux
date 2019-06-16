#include <headers.h>

#define SIZE 512

int main()
{
	int fd = -1;
	int fd_read = -1;
	pid_t pid = -1;
	int ret = -1;
	int child_read_ret = -1;
	char buffer[SIZE];
	char read_buffer[SIZE];
	static char child_write_done = 0;

	fd = open("txt", O_RDWR|O_CREAT|O_TRUNC, 0666);
	if(-1 == fd)
	{
		perror("open");
		return 1;
	}

	fd_read = open("/etc/passwd", O_RDONLY);
	if(-1 == fd)
	{
		perror("open");
		return 1;
	}

	pid = fork();
	if(-1 == pid)
	{
		perror("fork");
		return -1;
	}
	else if(0 == pid)
	{
		memset(buffer, 0, SIZE);
		while(1)
		{
			ret = read(fd_read, buffer, SIZE);
			if(ret<= 0)
			{
				perror("read");
				break;
			}
			ret = write(fd, buffer, ret);
		}
	}
	else if(pid > 0 )
	{
		printf("going to sleep:\n");
		sleep(1);
		printf("woke up\n");
		memset(read_buffer, 0, SIZE);
		lseek(fd,0, SEEK_SET);
		while(1)
		{
			ret = read(fd, read_buffer, SIZE);
			printf("ret = %d, fd = %d\n", ret, fd);
			if(ret <= 0)
			{
				perror("read");
				break;
			}
			printf("%s \n", read_buffer);
		}

	}
    return 0;
}

