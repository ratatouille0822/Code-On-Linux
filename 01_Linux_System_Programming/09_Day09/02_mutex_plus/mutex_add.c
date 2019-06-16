#include <headers.h>

int NUM = 0;

pthread_mutex_t mutex;

void* fun1(void* arg)
{
	for(int i = 0; i< 1000000; i++)
	{
		pthread_mutex_lock(&mutex);
		NUM++;
		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);
}

void* fun2(void* arg)
{
	for(int i = 0; i< 1000000; i++)
	{
		pthread_mutex_lock(&mutex);
		NUM++;
		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);
}

int main()
{
	int ret = -1;

	pthread_t tid1, tid2;

	ret = pthread_mutex_init(&mutex, NULL);
	if(0 != ret)
	{
		printf("failed init mutex\n");
	}

	ret = pthread_create(&tid1, NULL, fun1, NULL );
	if(0 != ret)
	{
		printf("failed create thread\n");
		return 1;
	}

	ret = pthread_create(&tid2, NULL, fun2, NULL );
	if(0 != ret)
	{
		printf("failed create thread\n");
		return 1;
	}

	ret = pthread_join(tid1, NULL);
	if(0 != ret)
	{
		printf("failed join tid1\n");
	}

	ret = pthread_join(tid2, NULL);
	if(0 != ret)
	{
		printf("failed join tid2\n");
	}

	printf("NUL = %d\n", NUM);

	pthread_mutex_destroy(&mutex);

    return 0;
}

