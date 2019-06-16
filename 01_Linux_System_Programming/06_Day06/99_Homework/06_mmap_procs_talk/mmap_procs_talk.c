#include <headers.h>

#define SIZE 512

int main()
{
	int fd = -1;
	int ret = -1;

	void* addr = NULL;

	char buffer_read[SIZE];
	char buffer_write[SIZE];

	int fd_file = -1;

	pid_t pid = -1;

	fd_file = open("mem_file", O_RDWR|O_CREAT, 0644);
	if(-1 == fd_file)
	{
		perror("open");
		return 1;
	}
	ret = truncate("./mem_file", 1024*1024);
	if(-1 == ret)
	{
		perror("truncate");
		return 2;
	}

	addr = mmap(NULL,1024, PROT_WRITE | PROT_READ, MAP_SHARED, fd_file, 0);
	if(NULL == addr)
	{
		perror("mmap");
		return 3;
	}
	
	close(fd_file);

	pid = fork();
	if(-1 == pid)
	{
		perror("fork");
		return 4;
	}
	else if( 0 == pid )
	{
		// child process does:
		while(1)
		{
			memset(buffer_read, 0, SIZE);
			memcpy(buffer_read, addr, SIZE);
			printf("%s\n", buffer_read);
		}
		exit(0);
	}

	while(1)
	{
		memset(buffer_write, 0, SIZE);
		fgets(buffer_write, sizeof(buffer_write), stdin);
		if(buffer_write[strlen(buffer_write)-1] == '\n');
		{
			buffer_write[strlen(buffer_write)-1] = '\0';
		}
		memcpy(addr, buffer_write, SIZE);
	}
	//p_write_done = 1;

	wait(NULL);
	munmap(addr, 1024);





    return 0;
}

