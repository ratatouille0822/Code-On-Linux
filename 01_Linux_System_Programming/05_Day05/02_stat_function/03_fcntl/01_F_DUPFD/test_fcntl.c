#include <headers.h>

int main()
{
	int fd = -1;
	fd = open("data.dat", O_WRONLY | O_CREAT, 0644);
	if(-1 == fd)
	{
		perror("open");
		return 1;
	}
	printf("fd = %d\n", fd);

	int ret = -1;
	ret = write(fd, "123456789\n", 10);
	if(-1 == ret)
	{
		perror("write");
		return 1;
	}

	int new_fd = -1;
	new_fd = fcntl(fd, F_DUPFD, 100);
	if(-1 == new_fd)
	{
		perror("fcntl");
		return 1;
	}

	printf("new_fd = %d\n", new_fd);

	ret = write(new_fd, "abcdefg\n", 8);
	if(-1 == ret)
	{
		perror("write");
		return 1;
	}
    return 0;
}

