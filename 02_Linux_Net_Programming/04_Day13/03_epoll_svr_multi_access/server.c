#include <headers.h>
#include <sys/epoll.h>
#include <errno.h>

#define SIZE 128

typedef struct _client_t
{
	int fd;
	char ip[16];
	int port;
}client_t;

int main(int argc,  char* argv[])
{
    int ret = -1;
    int sockfd = -1;
	int newfd = -1;
	int epollfd = -1;
	int count = 0;

	struct sockaddr_in addr;
	struct sockaddr_in from;

	struct epoll_event e;
	struct epoll_event re[128];
	
	char buffer[SIZE];

	socklen_t len = 0;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sockfd)
	{
		perror("socket");
		goto err0;
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	ret = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);
	if(-1 == ret)
	{
		perror("inet_pton");
		goto err1;
	}
	addr.sin_port = htons(atoi(argv[2]));

	printf("\033[35maddr:\033[0m %s, %d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	ret = bind(sockfd, (void*)&addr, sizeof(addr));
	if(-1 == ret)
	{
		perror("bind");
		goto err1;
	}

	ret = listen(sockfd, 20);
	if(-1 == ret)
	{
		perror("listen");
		goto err1;
	}

	printf("\033[34mServer is listening\033[0m %s\033[34m:\033[0m%d \n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	epollfd = epoll_create(1);
	if(-1 == epollfd)
	{
		perror("epoll_create");
		goto err1;
	}

	client_t sockfd_info;
	memset(&sockfd_info, 0, sizeof(sockfd_info));
	sockfd_info.fd = sockfd;

	e.events = EPOLLIN;
	e.data.ptr = (void*)&sockfd_info;
	ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &e);
	if(-1 == ret)
	{
		perror("epoll_ctl");
		goto err2;
	}


	while(1)
	{    
		ret = epoll_wait(epollfd, re, 128, 300000);
		if(-1 == ret)
		{
			perror("epoll_wait");
			goto err2;
		}
		if(0 == ret)
		{
			printf("timo ...\n");
		}
		for(int i = 0; i< ret; i++)
		{
			if(re[i].events&EPOLLIN) 
			{
            
				if( ((client_t*)re[i].data.ptr)->fd == sockfd )
				{
					len = sizeof(from);
					bzero(&from, len);
					newfd = accept(sockfd, (void*)&from, &len);
					if(-1 == newfd)
					{
						perror("accept");
						goto err2;
					}

					client_t * client_info = (client_t*)malloc(sizeof(client_t));
					memset(client_info, 0, sizeof(client_t));

					client_info->fd = newfd;
					client_info->port = ntohs(from.sin_port);
					strncpy(client_info->ip, inet_ntoa(from.sin_addr), 16);

					printf("\033[35mClient connected :\033[0m%s:%d fd = %d \n", client_info->ip, client_info->port, client_info->fd);

					e.events = EPOLLIN;
					e.data.ptr = (void*)(client_info);
					ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, newfd, &e);
					if(-1 == ret)
					{
						perror("epoll_ctl");
						goto err3;
					}
				}
				else
				{
					bzero(buffer, SIZE);
					ret = recv(((client_t*)re[i].data.ptr)->fd, buffer, SIZE, 0);
					if(ret <= 0)
					{
						ret = epoll_ctl(epollfd, EPOLL_CTL_DEL, ((client_t*)re[i].data.ptr)->fd, &e);
						free(re[i].data.ptr);
						if(-1 == ret)
						{
							perror("epoll_ctl");
							break;
						}
						close(((client_t*)re[i].data.ptr)->fd);
						perror("recv");
						break;
					}
					if(buffer[strlen(buffer)-1] == '\n')
					{
						buffer[strlen(buffer)-1] = '\0';
					}
					printf("\033[36mClient %s:%d :\033[0m %s\n",((client_t*)re[i].data.ptr)->ip, ((client_t*)re[i].data.ptr)->port, buffer);

					ret = send(((client_t*)re[i].data.ptr)->fd, buffer, strlen(buffer), 0);
					if(-1 == ret)
					{
						close(((client_t*)re[i].data.ptr)->fd);
						ret = epoll_ctl(epollfd, EPOLL_CTL_DEL, ((client_t*)re[i].data.ptr)->fd, &e);
						free(re[i].data.ptr);
						if(-1 == ret)
						{
							perror("epoll_ctl");
							break;
						}
						perror("recv");
						break;
					}

					
				}
			}
		}
	}
	close(epollfd);
	close(newfd);
	close(sockfd);
	return 0;
err3:
	close(newfd);
err2:
	close(epollfd);
err1:
	close(newfd);
err0:
	return 1;
}
