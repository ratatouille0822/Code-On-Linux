#include <headers.h>
#include <sys/epoll.h>

#define SIZE 128

int main(int argc, char* argv[])
{
	int ret = -1;
	int sockfd  = -1;
	int epollfd = -1;
	int count = 0;

	struct epoll_event e;
	struct epoll_event re[2];

	struct sockaddr_in addr;

	char buffer[SIZE];
	char ip_buffer[16];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sockfd)
	{
		perror("socket");
		goto err0;

	}

	ret = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);
	if(-1 == ret)
	{
		perror("inet_pton");
		goto err1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));

	ret = connect(sockfd, (void*)&addr, sizeof(addr));
	if(-1 == ret)
	{
		perror("connect");
		goto err1;
	}

	printf("\033[35mConnect to \033[0m%s:%s\n", argv[1], argv[2]);

	epollfd = epoll_create(1);
	if(-1 == epollfd)
	{
		perror("epoll_create");
		goto err1;
	}

	e.events = EPOLLIN;
	e.data.fd = STDIN_FILENO;
	ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &e);
	if(-1 == ret)
	{
		perror("epoll_ctl");
		goto err2;
	}

	e.events = EPOLLIN;
	e.data.fd = sockfd;
	ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &e);
	if(-1 == ret)
	{
		perror("epoll_ctl");
		goto err2;
	}

	while(1)
	{
		ret = epoll_wait(epollfd, re, 2, 3000);
		if(ret< 0)
		{
			perror("epoll_wait");
			goto err2;
		}
		if(0 == ret)
		{
			printf("timo...\n");
		}

		count = ret;
		for(int i = 0; i< count; i++)
		{
			if(re[i].events & EPOLLIN)
			{
				if(re[i].data.fd == STDIN_FILENO)
				{
					bzero(buffer, SIZE);
					fgets(buffer, SIZE, stdin);
					if('\n' == buffer[strlen(buffer) - 1])
					{
						buffer[strlen(buffer) - 1] = '\0';
					}
					ret = send(sockfd, buffer, strlen(buffer), 0);
					if(ret<=0)
					{
						perror("send");
						goto err2;
					}
				}

				if(re[i].data.fd == sockfd)
				{
					bzero(buffer, SIZE);
					ret = recv(sockfd, buffer, SIZE, 0);
					if(ret <= 0)
					{
						perror("recv");
						goto err2;
					}
					printf("\033[34mret = %d SVR: \033[0m%s\n",ret,  buffer);
				}
			}
		}
	}
	



	close(epollfd);
	close(sockfd);
	
    return 0;
err2:
	close(epollfd);
err1:
	close(sockfd);
err0:
	return 1;
}

