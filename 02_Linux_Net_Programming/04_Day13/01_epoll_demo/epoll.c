#include <headers.h>
#include <sys/epoll.h>

#define SIZE 128

int main()
{
	int ret = -1;
	int epoll_fd = -1;

	struct epoll_event e;
	struct epoll_event re;

	int timo = 10000;

	char buffer[SIZE];

	epoll_fd = epoll_create(10);
	if(-1 == epoll_fd)
	{
		perror("epoll_create");
		return 1;
	}

	e.events = EPOLLIN;
	e.data.fd = STDIN_FILENO;

	ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &e);
	if(-1 == ret)
	{
		perror("epoll_ctl");
		return 1;
	}

	while(1)
	{
		ret = epoll_wait(epoll_fd, &re, 5, timo);
		if(-1 == ret)
		{
			perror("epoll_wait");
			break;
		}
		if(0 == ret)
		{
			printf("timo...\n");
		}
		else
		{
			if(re.events & EPOLLIN && re.data.fd == STDIN_FILENO)
			{
				bzero(buffer, SIZE);
				fgets(buffer, SIZE, stdin);

				printf("\033[33mBUF:\033[0m %s", buffer);

			}
		}
	}

	close(epoll_fd);
    return 0;
}

