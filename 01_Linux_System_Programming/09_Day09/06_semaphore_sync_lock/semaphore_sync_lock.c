#include <headers.h>
#include <semaphore.h>

typedef struct Node
{
	int data;
	struct Node* next;
}Node, *pNode;

sem_t sem1;
sem_t sem2;

pthread_mutex_t mutex;

pNode head = NULL;

void* producer(void* arg)
{
	int ret = -1;
	pNode new_node = (pNode)malloc(sizeof(Node));

	while(1)
	{
		ret = sem_wait(&sem1);
		if(-1 == ret)
		{
			perror("sem_wait");
			break;
		}

		ret = pthread_mutex_lock(&mutex);
		if(0 != ret)
		{
			printf("pthread_mutex_lock failed \n");
			break;
		}
	
		new_node->data = random()%100 + 1;
		new_node->next = head;
		head=new_node;

		printf("Producer:  %d has been produced  \n", new_node->data);

		ret = pthread_mutex_unlock(&mutex);
		if(0 != ret)
		{
			printf("pthread_mutex_unlock failed \n");
			break;
		}

		sleep(random()%3 + 1);

		ret = sem_post(&sem2);
		if(0 != ret)
		{
			printf("sem_post failed \n");
			break;
		}
	}

	pthread_exit(NULL);
}


void* customer(void* arg)
{
	int ret = -1;
	pNode tmp = head;
	while(1)
	{
		ret = sem_wait(&sem2);
		if(0 != ret)
		{
			printf("sem_wait failed \n");
			break;
		}

		ret = pthread_mutex_lock(&mutex);
		if(0 != ret)
		{
			printf("pthread_mutex_lock failed \n");
			break;
		}

		printf("Customer: consume %d\n", head->data);

		head = head->next;
		free(tmp);

		ret = pthread_mutex_unlock(&mutex);
		if(0 != ret)
		{
			printf("pthread_mutex_unlock failed \n");
			break;
		}


		ret = sem_post(&sem1);
		if(0 != ret)
		{
			printf("sem_post failed \n");
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

	ret = sem_init(&sem1, 0, 1);
	if(-1 == ret)
	{
		perror("sem_init");
		return 1;
	}

	ret = sem_init(&sem2, 0, 0);
	if(-1 == ret)
	{
		perror("sem_init");
		return 1;
	}

	ret = pthread_mutex_init(&mutex, NULL);
	if(0 != ret)
	{
		printf("pthread_mutex_init failed \n");
		return 1;
	}

	ret = pthread_create(&tid1, NULL, producer, NULL);
	if(0 != ret)
	{
		printf("pthread_create producer failed...\n");
		return 1;
	}

	ret = pthread_create(&tid2, NULL, customer, NULL);
	if(0 != ret)
	{
		printf("pthread_create customer failed...\n");
		return 1;
	}

	ret = pthread_join(tid1, NULL);
	if(0 != ret)
	{
		printf("pthread_join failed tid1\n");
		return 1;
	}
	ret = pthread_join(tid2, NULL);
	if(0 != ret)
	{
		printf("pthread_join failed tid2\n");
		return 1;
	}

	ret = pthread_mutex_destroy(&mutex);
	if(-1 == ret)
	{
		perror("pthread_mutex_destroy\n");
		return 1;
	}

	ret = sem_destroy(&sem1);
	if(-1 == ret)
	{
		perror("sem_destroy\n");
		return 1;
	}
	
	ret = sem_destroy(&sem2);
	if(-1 == ret)
	{
		perror("sem_destroy\n");
		return 1;
	}

    return 0;
}

