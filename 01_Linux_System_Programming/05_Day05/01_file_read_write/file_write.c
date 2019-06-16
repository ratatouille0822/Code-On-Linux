#include <headers.h>

int main()
{
	// 1. 打开文件
	int fd = -1;
	int ret = -1;
	fd = open("txt", O_WRONLY|O_CREAT|O_EXCL, 0644);
	if( -1 == fd )
	{
		perror("open");
		return 1;
	}

	printf("fd = %d \n", fd);

	// 2. 写文件
	ret = write(fd, "Hello World \n", strlen("Hello World \n"));
	if(-1 == ret)
	{
		perror("write");
		return 1;
	}
	// 3. 关闭文件
	close(fd);
    return 0;
}

