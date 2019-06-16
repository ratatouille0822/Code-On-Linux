#include <headers.h>
#include <pthread.h>

void* fun(void* arg)
{
	printf("new thread--->:\n");
	for(int i = 0; i< 20; i++)
	{
		printf("fun i = %d\n", i);
		sleep(1);
	}

	pthread_exit(NULL);
}

int main()
{
	pthread_t tid = -1;

	int ret = -1;

	ret = pthread_create(&tid, NULL, fun, NULL);
	if(0 != ret)
	{
		printf("failed to create new thread \n");
	}

	ret = pthread_detach(tid);
	if(0 != ret)
	{
		printf("failed \n");
		return 1;
	}

	for(int i = 0; i< 5; i++)
	{
		printf("cancel thread in %d:\n", 5-i);
		sleep(1);
	}

	ret = pthread_cancel(tid);
	if(0 != ret)
	{
		printf("failed cancel thread\n");
		return 1;
	}


    return 0;
}

