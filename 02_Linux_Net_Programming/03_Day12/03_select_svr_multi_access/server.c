#include <headers.h>
#include <sys/select.h>

#define SIZE 128

int main(int argc, char* argv[])
{

	int ret = -1;
	int sockfd = -1;
	int newfd = -1;
	int maxfd = 0;
	int i = 0;

	struct sockaddr_in addr;
	struct sockaddr_in from;
	struct timeval timo;

	char buffer[SIZE];
	char ip_buffer[16];

	fd_set readfds;
	fd_set savefds;
	socklen_t len = 0;


	if(3 != argc)
	{
		printf("\033[32mUsage: x.x.x.x port\033[0m\n");
		goto err0;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if( -1 == sockfd )
	{
		perror ("socket");
		goto err0;
	}

	bzero(&addr, sizeof(addr));
	ret = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);
	if(-1 == ret)
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

	printf("\033[33mServer listening %s:%s sockfd = %d\033[0m\n", argv[1], argv[2], sockfd);

	FD_ZERO(&readfds);
	FD_SET(sockfd, &savefds);
	maxfd = sockfd + 1;

	while(1)
	{
		readfds = savefds;
		timo.tv_sec = 300;
		timo.tv_usec = 0;
		ret = select(maxfd, &readfds, NULL, NULL, &timo);
		if(ret < 0)
		{
			perror("select");
			break;
		}
		else if(0 == ret)
		{
			printf("timo...\n");
		}
		else
		{
			if(FD_ISSET(sockfd, &readfds))
			{
				len = sizeof(from);
				bzero(&from, len);
				newfd = accept(sockfd, (void*)&from, &len);
				if(-1 == newfd)
				{
					perror("accept");
					break;
				}

				printf("\033[31m%s:%d connected\033[0m\n", inet_ntop(AF_INET, &from.sin_addr.s_addr, ip_buffer, 16), ntohs(from.sin_port));

				FD_SET(newfd, &savefds);
				maxfd = newfd >= maxfd ? newfd + 1: maxfd;
			}//if


			for(i = sockfd + 1; i< maxfd; i++)
			{
				if(FD_ISSET(i, &readfds))
				{

					bzero(buffer, SIZE);
					ret = recv(i, buffer, SIZE, 0);
					if(ret <= 0)
					{
						perror("recv");
						FD_CLR(i, &savefds);

						if(i == maxfd - 1)
						{
							maxfd -= 1;
						}//fi

						continue;
					}//fi
					if('\n' == buffer[strlen(buffer)-1])
					{
						buffer[strlen(buffer) - 1] = '\0';
					}//fi
					printf("\033[34mrecv: \033[0m%s\n", buffer);

					ret = send(i, buffer, sizeof(buffer), 0);
					if(ret <= 0)
					{
						perror("send");
						FD_CLR(i, &savefds);

						if(i == maxfd - 1)
						{
							maxfd -= 1;
						}//fi

						continue;
					}//fi
				}//fi
			}//for
		}//esle
	}//while(1)

	close(newfd);
	close(sockfd);
    return 0;

err2:
	close(newfd);
err1:
	close(sockfd);
err0:
	return 1;
}

