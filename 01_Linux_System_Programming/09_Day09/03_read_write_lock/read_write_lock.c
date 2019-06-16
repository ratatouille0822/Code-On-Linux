#include <headers.h>

pthread_rwlock_t rw_lock;
int NUM = 0;

void* fun_read(void* arg)
{
	while(1)
	{
		pthread_rwlock_rdlock(&rw_lock);
		printf("thread %ld:NUM = %d\n", (long)arg, NUM);
		pthread_rwlock_unlock(&rw_lock);
		usleep(500000);
	}
}

void* fun_write(void* arg)
{
	while(1)
	{
		pthread_rwlock_wrlock(&rw_lock);
		NUM++;
		printf("thread %ld is going to write\n", (long)arg);
		pthread_rwlock_unlock(&rw_lock);
		usleep(500000);
	}
}

int main()
{
	int ret = -1;
	pthread_t tid[8];

	ret = pthread_rwlock_init(&rw_lock, NULL);
	if(0 != ret)
	{
		printf("failed to init rw_lock\n");
		return 1;
	}

	for(int i = 0; i<8; i++)
	{
		if(i<5)
		{
			ret = pthread_create(&tid[i], NULL, fun_read, (void*)(long)i);
			if(0 != ret)
			{
				printf("failed create thread tid[%d]\n", i);
				return 1;
			}
		}
		else
		{
			ret = pthread_create(&tid[i], NULL, fun_write, (void*)(long)i);
			if(0 != ret)
			{
				printf("failed create thread tid[%d]\n", i);
				return 1;
			}
		}
	}

	printf("press any key to stop...\n");
	getchar();




	ret = pthread_rwlock_destroy(&rw_lock);
	if(0 != ret)
	{
		printf("failed to destroy\n");
		return 1;
	}
    return 0;
}

