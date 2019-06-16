#include <headers.h>

#define SIZE 1024

int main()
{
	//1. 打开文件
	char buffer[SIZE];
	int fd = -1;

	STDIN_FILENO

	fd = open("txt", O_RDONLY);
	if(-1 == fd)
	{
		perror("open");
		return 1;
	}
	printf("fd = %d\n", fd);

	//2. 读文件
	int ret = -1;
	memset(buffer, 0, SIZE);
	ret = read(fd, buffer, SIZE);

	if(-1 == ret)
	{
		perror("read");
		return 1;
	}

	printf("SIZE = %ld, ret = %d\n", strlen(buffer), ret);
	printf("buffer = %s\n", buffer);
	//3. 关闭文件
	close(fd);
}

