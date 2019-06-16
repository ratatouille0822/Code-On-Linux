#include <headers.h>
#include <pthread.h>

void* fun(void* arg)
{
	printf("new thread--->:\n");
	for(int i = 0; i< 10; i++)
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

	ret = pthread_join(tid, NULL);
	if( 0 != ret )
	{
		printf("failed to join detached thread\n");
	}
	else
	{
		printf("succeed joining thread\n");
	}

	getchar();

    return 0;
}

