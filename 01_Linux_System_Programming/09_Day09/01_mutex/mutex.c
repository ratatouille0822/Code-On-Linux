#include <headers.h>

int main()
{
	int ret = -1;

	pthread_mutex_t mutex;

	ret = pthread_mutex_init(&mutex, NULL);
	{
		if(0 != ret)
		{
			printf("failed init mutex\n");
		}
		else
		{
			printf("succeded init mutex\n");
		}
	}

	pthread_mutex_destroy(&mutex);

    return 0;
}

