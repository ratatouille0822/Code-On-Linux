#include <headers.h>
#include <pthread.h>

void* fun(void* arg)
{
	for(int i = 0; i< 20; i++)
	{
		printf("fun: i = %d, tid = %ld\n", i + 1, pthread_self());
		sleep(1);
	}

	pthread_exit(NULL);
}

int main()
{
	pthread_t tid = -1;
	pthread_t main_tid = -1;


	int ret = -1;

	ret = pthread_create(&tid, NULL, fun, NULL);
	if(0 != ret)
	{
		printf("failed to create new thread \n");
	}

	main_tid = pthread_self();
	for(int i = 0; i< 5; i++)
	{
		printf("main: i = %d, tid = %ld\n", i + 1, main_tid);
		sleep(1);
	}

	pthread_exit(NULL);

	ret = pthread_join(tid, NULL);
	if(0 != ret)
	{
		printf("join failure\n");
	}
	else 
	{
		printf("join succeeded\n");
	}



    return 0;
}

