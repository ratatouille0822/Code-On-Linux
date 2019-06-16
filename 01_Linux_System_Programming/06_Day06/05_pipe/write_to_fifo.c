#include <headers.h>

#define SIZE 128

int main()
{
	int ret = -1;
	int fd = -1;
	char buffer[SIZE];
	int i = 0;

	ret = mkfifo("./fifo", 0644);
	if(-1 == ret)
	{
		perror("mkfifo");
		return 1;
	}

	fd = open("./fifo", O_WRONLY);
	if(-1 == fd)
	{
		perror("open");
		return 1;
	}

	while(1)
	{
		sleep(1);
		memset(buffer, 0, SIZE);
		//sprintf(buffer, "write to pipe %d times \n", i++);
		fgets(buffer, SIZE, stdin);
		if(buffer[sizeof(buffer)-1] == '\n');
		{
			buffer[sizeof(buffer)-1] = '\0';
		}
		ret = write(fd, buffer, sizeof(buffer));
		if(ret <= 0)
		{
			perror("write");
			break;
		}
		if(100 == i)
		{
			break;
		}

	}

	close(fd);
    return 0;
}

