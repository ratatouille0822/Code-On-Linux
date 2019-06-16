#include <headers.h>
#include <pthread.h>

int global_num = 0;

void* fun(void* arg)
{
	while(1)
	{
		printf("here is new thread: %ld, global_num: %d, static_num: %d  \n", pthread_self(), global_num, *(int*)arg);
		global_num += 1000;
		*(int*)arg += 1000;
		sleep(1);
		if(9009 == global_num)
			break;
	}

	return NULL;
}

int main()
{
	static int static_num = 0;

	pthread_t main_thread ;
	pthread_t new_thread;

	printf("main_thread id = %ld", pthread_self());

	if(0 != pthread_create(&new_thread, NULL, fun,(void*)(&static_num)))
	{
		printf("failed to make new thread\n");
		return 1;
	}

	while(1)
	{
		printf("here is Main thread: %ld, global_num: %d, static_num: %d \n", pthread_self(), global_num, static_num);
		global_num += 1;
		static_num += 1;
		sleep(1);
		if(9009 == global_num)
			break;
	}

    return 0;
}

