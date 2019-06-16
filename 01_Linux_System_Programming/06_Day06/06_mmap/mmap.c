#include <headers.h>
#include <sys/mman.h>

int main()
{
	int ret = -1;
	int fd = -1;
	char * buffer = "test to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\ntest to write mmap\n";

	void* addr = NULL;

	fd = open("txt", O_RDWR);
	if(-1 == fd)
	{
		perror("open");
		return 1;
	}

	addr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(MAP_FAILED == addr)
	{
		perror("mmap");
		return 1;
	}

	close(fd);

	memcpy(addr, buffer, strlen(buffer));

	munmap(addr, 1024);


    return 0;
}

