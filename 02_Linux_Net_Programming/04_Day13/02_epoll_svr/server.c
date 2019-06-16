#include <headers.h>
#include <sys/epoll.h>

#define SIZE 128

int main( int argc, char* argv[] )
{
	int ret = -1;
	int sockfd = -1;
	int newfd = -1;
	int epollfd = -1;
	int timo = 0;
	int count = 0;

		

	struct sockaddr_in addr, from;

	char ip_buffer[16];
	char buffer[SIZE];

	socklen_t len = 0;

	struct epoll_event e;
	struct epoll_event re[2];



	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sockfd)
	{
		perror("socket");
		goto err0;
	}

	bzero(&addr, sizeof(addr));

	ret = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);
	if( -1 == ret )
	{
		perror("inet_pton");
		goto err1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));

	ret = bind(sockfd, (void*)&addr, sizeof(addr));
	if(-1 == ret)
	{
		perror("bind");
		goto err1;
	}

	ret = listen(sockfd, 10);
	if(-1 == ret)
	{
		perror("listen");
		goto err1;
	}

	printf("\033[32mListening:\033[0m %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	len = sizeof(from);
	bzero(&from, len);
	newfd = accept(sockfd, (void*)&from, &len);
	if(-1 == newfd)
	{
		perror("accept");
		goto err1;
	}
	printf("\033[31mClient %s:%d connected...\033[0m\n", inet_ntoa(from.sin_addr), ntohs(from.sin_port));


	timo = 30000;

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
	e.data.fd = newfd;
	ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, newfd, &e);
	if(-1 == ret)
	{
		perror("epoll_ctl");
		goto err2;
	}

	while(1)
	{
		ret = epoll_wait(epollfd, re, 1, 3000);
		if(-1 == ret)
		{
			perror("epoll_wait");
			break;
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
					if('\n' == buffer[strlen(buffer)-1])
					{
						buffer[strlen(buffer) - 1] = '\0';
					}
					ret = send(newfd, buffer, strlen(buffer), 0);
					if( ret<= 0 )
					{
						perror("send");
						goto err2;
					}
				}
				if(re[i].data.fd == newfd)
				{
					bzero(buffer, SIZE);
					ret = recv(newfd, buffer, SIZE, 0);
					if(ret <= 0)
					{
						close(newfd);
						epoll_ctl(epollfd, EPOLL_CTL_DEL, newfd, NULL );
						perror("recv");
						goto err2;
					}

					printf("\033[32m%s:%d:\033[0m%s\n", inet_ntop(AF_INET, &from.sin_addr.s_addr, ip_buffer, 16), ntohs(from.sin_port), buffer );

					ret = send(newfd, buffer, strlen(buffer), 0);
					if(ret <= 0)
					{
						close(newfd);
						epoll_ctl(epollfd, EPOLL_CTL_DEL, newfd, NULL );
						perror("send");
						goto err2;

					}
				}
			}
		}
	}

	close(epollfd);
	close(newfd);
	close(sockfd);

    return 0;

err2:
	close(epollfd);
err1:
	close(sockfd);
err0:
	return 1;
}

