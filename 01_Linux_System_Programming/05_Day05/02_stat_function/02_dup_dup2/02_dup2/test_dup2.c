#include <headers.h>

int main()
{
	int fd = -1;
	fd = open("data.dat", O_WRONLY | O_CREAT , 0644);
	if(-1 == fd)
	{
		perror("open");
		return 1;
	}
	printf("fd = %d\n", fd);

	int ret = -1;
	ret = write(fd, "ABCDEFG\n", 8);
	if(-1 == ret)
	{
		perror("write");
		return 1;
	}

	int new_fd = -1;
	new_fd = dup2(fd, 1);
	if(-1 == new_fd)
	{
		perror("dup2");
		return 1;
	}

	printf("new_fd = %d\n", new_fd);

	ret = write(new_fd, "1234567890\n", 11);
	if(-1 == ret)
	{
		perror("write");
		return 1;
	}
    return 0;
}

