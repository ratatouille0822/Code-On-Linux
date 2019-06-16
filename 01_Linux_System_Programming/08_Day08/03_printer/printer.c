#include <headers.h>
#include <pthread.h>

void* printer1(void* arg)
{
	int ret = -1;
	int fd = *(int*)(long)arg;
	printf("----> fd = %d\n", fd);
	
	for(char c = 'a'; c<= 'z'; c++)
	{
		usleep(100 * 1000);
		ret = write(fd, &c, 1);
		if(-1 == ret)
		{
			perror("write");
			pthread_exit(NULL);
		}
	}
	pthread_exit(NULL);
}

void* printer2(void* arg)
{
	int ret = -1;
	int fd = *(int*)(long)arg;
	printf("----> fd = %d\n", fd);

	for(char c = 'A'; c<= 'Z'; c++)
	{
		usleep(100 * 1000);
		ret = write(fd, &c, 1);
		if(-1 == ret)
		{
			perror("write");
			pthread_exit(NULL);
		}
	}
	pthread_exit(NULL);
}

int main()
{
	int ret = -1;
	pthread_t tid_1;
	pthread_t tid_2;

	int fd = -1;
	fd = open("txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if( -1 == fd )
	{
		perror("open");
		return 2;
	}

	printf("----> fd = %d\n", fd);

	ret = pthread_create(&tid_1, NULL, printer1, (void*)(long)&fd);
	if(0 != ret)
	{
		printf("create 1 failed \n");
		return 1;
	}
	ret = pthread_create(&tid_1, NULL, printer2, (void*)(long)&fd);
	if(0 != ret)
	{
		printf("create 2 failed \n");
		return 1;
	}

	printf("waiting for threads to end\n");

	pthread_join(tid_1, NULL);
	pthread_join(tid_2, NULL);

	printf("threads ended\n");
	close(fd);
    return 0;
}

