#include <headers.h>

#define SIZE 512

int main(int argc, char* argv[])
{
	int ret = -1;
	int ret_write = -1;
	int fd = -1;
	int new_fd = -1;
	char* src = argv[1];
	char* dest= argv[2];
	char buffer[SIZE];

	if(3 != argc)
	{
		printf("usage: mycp src dest");
		return 1;
	}
	
	fd = open(src, O_RDONLY);
	if(-1 == fd)
	{
		perror("open");
		goto err1;
	}
	
	new_fd = open(dest, O_WRONLY|O_CREAT, 0666);
	if(-1 == new_fd)
	{
		perror("open");
		goto err2;
	}


	while(1)
	{
		ret = read(fd, buffer, SIZE);
		if(ret <= 0)
		{
			printf("%d\n",ret);
			perror("read");
			break;
		}
		ret_write = write(new_fd, buffer, ret);
		if(ret_write <= 0)
		{
			perror("write");
			break;
		}
	};

	close(fd);
	close(new_fd);

err2:
	close(fd);
err1: 
	return 1;
}

