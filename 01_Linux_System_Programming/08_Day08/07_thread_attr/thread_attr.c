#include <headers.h>
#include <pthread.h>

void* fun(void* arg)
{
	int ret = -1;

	printf("new thread--->:\n");
	ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if(0 != ret)
	{
		printf("PTHREAD_CANCEL_DISABLE\n");
	}
	else
	{
		printf("PTHREAD_CANCEL_ENABLE\n");
	}
	for(int i = 0; i< 20; i++)
	{
		printf("fun i = %d\n", i);
		sleep(1);
	}


//	pthread_exit(NULL);
}

int main()
{
	pthread_t tid = -1;
	pthread_attr_t attr ;
	int ret = -1;

	void* res;

	int detach_state;

	ret = pthread_attr_init(&attr);
	if(0 != ret)
	{
		printf("failed to init thread \n");
		return 1;
	}

	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if( 0 != ret )
	{
		printf("failed set\n");
		return 1;
	}

	ret = pthread_attr_getdetachstate(&attr, &detach_state);
	if(detach_state == PTHREAD_CREATE_DETACHED)
	{
		printf("detach_state == PTHREAD_CREATE_DETACHED \n");
	} 
	else if(detach_state == PTHREAD_CREATE_JOINABLE)
	{
		printf("detach_state == PTHREAD_CREATE_JOINABLE \n");
	}

	ret = pthread_create(&tid, NULL, fun, NULL);
	if(0 != ret)
	{
		printf("failed to create new thread \n");
	}


	
	for(int i = 0; i< 5; i++)
	{
		printf("cancel thread in %d:\n", 5-i);
		sleep(1);
	}

	ret = pthread_attr_destroy(&attr);
	if(0 != ret)
	{
		printf("fail destroy attr \n");
		return 1;
	}
	

	ret = pthread_cancel(tid);
	if(0 != ret)
	{
		printf("failed cancel thread\n");
		return 1;
	}

	ret = pthread_join(tid, &res);
	if(0 != ret)
	{
		printf("failed join \n");
	}


	if(PTHREAD_CANCELED == res)
	{
		printf("--> res = %s\n",(char*)res);
		printf("PTHREAD_CANCELED == res\n");
		return 1;
	}
	else
	{
		printf("--> res = %s\n", (char*)res);
		printf("- -!\n");
		return 1;
	}



	printf("按下回车键结束！\n");
	getchar();


    return 0;
}

