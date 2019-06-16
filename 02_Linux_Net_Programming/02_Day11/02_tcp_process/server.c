#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define SIZE 128

void rec_child(int signo)
{
	printf("child process close in %d\n", signo);
	while(-1 != wait(NULL))
		;
}

int main(int argc, char* argv[])
{
	int ret = -1;
	int sockfd = -1;
	int peerfd = -1;

	struct sockaddr_in addr;
	struct sockaddr_in from;

	char buffer[SIZE];
	char ip_buffer[16];

	pid_t pid;
	socklen_t len = -1;

	struct sigaction act;

	if(3 != argc)
	{
		printf("usage: server x.x.x.x port\n");
		goto err1;
	}

	memset(&act, 0, sizeof(act));
	act.sa_handler = rec_child;

	ret = sigemptyset(&act.sa_mask);
	if(-1 == ret)
	{
		perror("sig_emptyset");
		goto err0;
	}

	ret = sigaddset(&act.sa_mask, SIGINT);
	if(-1 == ret)
	{
		perror("sigaddset");
		goto err0;
	}
	ret = sigaddset(&act.sa_mask, SIGQUIT);
	if(-1 == ret)
	{
		perror("sigaddset");
		goto err0;
	}

	ret = sigaction(SIGCHLD, &act, NULL );
	if(-1 == ret)
	{
		perror("sigaction");
		goto err0;
	}
	
	memset(&addr, 0, sizeof(addr));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sockfd)
	{
		perror("socket");
		goto err0;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	ret = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);
	if(-1 == ret)
	{
		perror("pton");
		goto err1;
	}

	ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
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

	while(1)
	{
		len = sizeof(from);
		memset(&from, 0, len);
		peerfd = accept(sockfd, (struct sockaddr*)&from, &len);

		if(-1 == peerfd)
		{
			perror("accept");
			if(EINTR == errno || ECONNABORTED == errno)
			{
				if(EINTR == errno)
				{
					printf("errno == EINTR\n");
				}
				else if(ECONNABORTED == errno)
				{
					printf("errno == ECONNABORTED\n");
				}
				continue;
			}
			else
			{
				break;
			}
		}

		printf("Client %s:%d is connected\n", inet_ntop(AF_INET, &from.sin_addr.s_addr, ip_buffer, 16), ntohs(from.sin_port));

		pid = fork();
		if(-1 == pid)
		{
			perror("fork");
			goto err1;
		}
		if(0 == pid)
		{
			close(sockfd);
			while(1)
			{
				bzero(buffer, SIZE);
				ret = read(peerfd, buffer, SIZE);
				if(-1 == ret)
				{
					perror("read");
					goto err1;
				}
				else if( 0 == ret )
				{
					break;
				}
				if(strncmp(buffer, "quit", 4)==0)
				{
					break;
				}
				printf("\033[036m%s:%d:\033[0m %s\n",inet_ntop(AF_INET, &from.sin_addr.s_addr, ip_buffer, 16), ntohs(from.sin_port), buffer);

				ret = write(peerfd, buffer, strlen(buffer));
				if(-1 == ret)
				{
					perror("write");
					goto err1;
				}
				else if( 0 == ret )
				{
					goto err0;
				}
			}
			close(peerfd);
			exit(0);
		}
	}

	close(sockfd);

    return 0;

err1:
	close(sockfd);
err0:
	return 1;
}

