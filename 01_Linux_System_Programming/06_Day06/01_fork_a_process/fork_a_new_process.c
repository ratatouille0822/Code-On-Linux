#include <headers.h>


int main()
{

	pid_t pid[3] = {0,0,0};
	static char is_child = 0;
	static char is_printed = 0;

	for(int i = 0; i< 3; i++)
	{
		if(!is_printed)
		{
			if(0 == is_child)
			{
				pid[i] = fork();
			}
			if(-1 == pid[i])
			{
				perror("fork");
				return 1;
			}

			if(0 == pid[i])
			{
				is_child = 1;
				is_printed = 1;
				printf("\n");
				printf("here is child process No. %d, its pid is: %d\n", i, getpid());
				printf("its parent is: %d\n", getppid());
				printf("\n");
			}

			if(pid[i] > 0)
			{
				is_child = 0;
				is_printed = 0;
				printf("\n");
				printf("here is parent process, its pid is %d\n",getpid());
				printf("its child is %d\n", pid[i]);
				printf("\n");
				sleep(1);
			}

		}
	}
		

    return 0;
}

