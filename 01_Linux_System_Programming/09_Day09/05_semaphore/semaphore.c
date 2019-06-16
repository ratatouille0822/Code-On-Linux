#include <headers.h>
#include <semaphore.h>

sem_t sem;

void printer(char* str)
{
	while(*str != '\0')
	{
		sem_wait(&sem);
		putchar(*str);
		fflush(stdout);
		str++;
		usleep(500000);
		sem_post(&sem);
	}
}

void* printer1(void* arg)
{
	char* str1 = "ABCDEFGHIJKLMN";
	printer(str1);

	pthread_exit(NULL);
}


void* printer2(void* arg)
{
	usleep(10);
	char* str1 = "abcdefghijklmn";
	printer(str1);
	pthread_exit(NULL);
}


int main()
{
	int ret = -1;
	pthread_t tid1, tid2;

	ret = sem_init(&sem, 0, 1);
	if(-1 == ret)
	{
		perror("sem_init");
		return 1;
	}

	ret = pthread_create(&tid1, NULL, printer1, NULL);
	if(0 != ret)
	{
		printf("pthread_create tid1 failed...\n");
		return 1;
	}

	ret = pthread_create(&tid2, NULL, printer2, NULL);
	if(0 != ret)
	{
		printf("pthread_create tid2 failed...\n");
		return 1;
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);


	ret = sem_destroy(&sem);
	if(-1 == ret)
	{
		perror("sem_destroy");
		return 1;
	}
	
    return 0;
}

