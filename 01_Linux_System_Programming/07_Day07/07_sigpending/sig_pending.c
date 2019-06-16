#include <headers.h>

int main()
{
	sigset_t set, oldset;
	sigset_t pend;

	int ret = -1;
	int i = 0;

	ret = sigemptyset(&set);
	if(-1 == ret)
	{
		perror("sigemptyset");
		return 1;
	}

	ret = sigaddset(&set, SIGINT);
	if(-1 == ret)
	{
		perror("sigaddset");
		return 1;
	}
	ret = sigaddset(&set, SIGQUIT);
	if(-1 == ret)
	{
		perror("sigaddset");
		return 1;
	}
	ret = sigaddset(&set, SIGALRM);
	if(-1 == ret)
	{
		perror("sigaddset");
		return 1;
	}

	ret = sigprocmask(SIG_BLOCK, &set, &oldset);

	while(1)
	{
		ret = sigpending(&pend);
		if(-1 == ret)
		{
			perror("sigpending");
			return 2;
		}
		for(int i = 1; i<32; ++i)
		{
			if(sigismember(&pend, i))
			{
				printf("1");

			}
			else if(0 == sigismember(&pend, i))
			{
				printf("0");
			}
		}

		printf("\n");
		sleep(1);
		 ++i;

		 if(i>10)
		 {
			 printf("cancel blocking...\n");
			 sigprocmask(SIG_SETMASK, &oldset, NULL);
		 }


	


	}



	


    return 0;
}

