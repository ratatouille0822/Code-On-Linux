#include <headers.h>

int main()
{
	int fd = -1;
	fd = open("data.dat", O_WRONLY|O_CREAT, 0644);
	if(-1 == fd)
	{
		perror("open");
		return 1;
	}

	int new_fd = -1;
	new_fd = dup(fd);
	if(-1 == new_fd)
	{
		perror("dup");
		return 1;
	}
	
	printf("fd = %d\n", fd);
	printf("new_fd = %d\n", new_fd);

    return 0;
}

