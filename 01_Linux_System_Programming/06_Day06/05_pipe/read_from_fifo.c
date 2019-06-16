#include <headers.h>
#define SIZE 128

int main()
{
	int ret = -1;
	int fd = -1;
	char buffer[SIZE];

	fd = open("./fifo", O_RDONLY);
	if(-1 == fd)
	{
		perror("open");
		return 1;
	}

	while(1)
	{
		ret = read(fd, buffer, sizeof(buffer));
		if(ret <= 0)
		{
			perror("read");
			break;
		}

		printf("%s\n", buffer);
	}

	
    return 0;
}

