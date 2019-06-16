#include <headers.h>

typedef struct _node_t
{
	int data;
	struct _node_t* next;
}node_t, *p_node_t;

pthread_cond_t cond;
pthread_mutex_t mutex;

p_node_t head = NULL;

void* producer(void* arg)
{
	int ret = -1;

	while(1)
	{
		sleep(random()%5+1);
		p_node_t new_node = (p_node_t)malloc(sizeof(node_t));
		if(NULL == new_node)
		{
			printf("failed malloc\n");
			break;
		}
		ret = pthread_mutex_lock(&mutex);
		if(0 != ret)
		{
			printf("lock failed ------> producer!\n");
			break;
		}

		new_node->data = random()%100 + 1;
		new_node->next = head;
		head = new_node;

		printf("the number is %d-------------> Producer!\n", new_node->data);

		pthread_mutex_unlock(&mutex);
		if(0 != ret)
		{
			printf("unlock failed -----> customer! \n");
			break;
		}

		ret = pthread_cond_signal(&cond);
	}

	pthread_exit(NULL);

}

void* customer(void* arg)
{
	int ret = -1;
	p_node_t temp = NULL;
	while(1)
	{
		sleep(random()%5);
		ret = pthread_mutex_lock(&mutex);
		if(0 != ret)
		{
			printf("lock failed ----> customer \n");
			break;
		}

		if(NULL == head)
		{
			printf("wait for the producer to make...\n");
			ret = pthread_cond_wait(&cond, &mutex);
			if(0 != ret)
			{
				printf("failed pthread_cond_wait\n");
				break;
			}
		}

		printf("the number produced is: %d------------> Customer\n",head->data);

		temp = head;
		head = head->next;

		free(temp);

		pthread_mutex_unlock(&mutex);
		if(0 != ret)
		{
			printf("unlock failed \n");
			break;
		}
	}

	pthread_exit(NULL);

}

int main()
{
	int ret = -1;
	pthread_t tid1, tid2;

	srandom(1);

	ret = pthread_mutex_init(&mutex, NULL);
	if(0 != ret)
	{
		printf("init mutex failed...\n");
		return 1;
	}
	ret = pthread_cond_init(&cond, NULL);
	if(0 != ret)
	{
		printf("init cond failed...\n");
		return 1;
	}

	ret = pthread_create(&tid1, NULL, producer, NULL);
	if(0 != ret)
	{
		printf("create tid1 producer failed...\n");
		return 1;
	}

	ret = pthread_create(&tid2, NULL, customer, NULL);
	if(0 != ret)
	{
		printf("create tid2 customer failed...\n");
		return 1;
	}

	printf("press any key to end \n");
	getchar();

	ret = pthread_mutex_destroy(&mutex);
	if(0 != ret)
	{
		printf("destroy mutex failed...\n");
		return 1;
	}

	ret = pthread_cond_destroy(&cond);
	if(0 != ret)
	{
		printf("destroy cond failed...\n");
		return 1;
	}


    return 0;
}

