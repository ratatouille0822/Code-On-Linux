#include <headers.h>

int get_user_permission(int flag)
{
	switch(flag & O_ACCMODE)
	{
	case O_RDWR:
		printf("RDWR\n");
		break;
	case O_RDONLY:
		printf("RDONLY\n");
		break;
	case O_WRONLY:
		printf("WRONLY\n");
		break;
	}

	
}

int main(int argc, char* argv[])
{
	// Create a new file :data.dat:
	int fd = -1;
	fd = open(argv[1], O_RDWR);
	if(-1 == fd)
	{
		perror("open");
		return 1;
	}
	printf("fd = %d\n", fd);
	 
	int ret = -1;
	char buffer[128];
	ret = read(fd, buffer, 128);
	{
		if(-1 == ret)
		{
			perror("read");
			return 1;
		}
	}
	
	printf("buffer = %s\n", buffer);

	int flag = fcntl(fd, F_GETFL);
	printf("flag = %x\n", flag);
	get_user_permission(flag);

	if(flag & O_APPEND)
	{
		printf("before set file flags\n");
		printf("appand enabled\n");
	}

	flag |= O_APPEND;
	fcntl(fd, F_SETFL, flag);

	if(flag & O_APPEND)
	{
		printf("after set file flags\n");
		printf("appand enabled\n");
	}


    return 0;
}

