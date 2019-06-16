#include <headers.h>
#include <sys/select.h>

#define SIZE 128

int main(int argc, char* argv[])
{

	int ret = -1;
	int sockfd = -1;
	int maxfd = 0;

	char buf[SIZE];
	char ip_buf[16];

	struct sockaddr_in addr;
	struct timeval time_out;

	fd_set wrfds;
	fd_set rdfds;
	
	if(3 != argc)
	{
		printf("usage: client x.x.x.x port\n");
		goto err0;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if( -1 == sockfd )
	{
		perror("socket");
		goto err0;
	}

	addr.sin_family = AF_INET;
	ret = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);
	if(-1 == ret)
	{
		perror("inet_pton");
		goto err1;
	}
	addr.sin_port = htons(atoi(argv[2]));
	
	ret = connect(sockfd,(void*)&addr, sizeof(addr));
	if(-1 == ret)
	{
		perror("connect");
		goto err1;
	}
	 
	maxfd = sockfd > STDIN_FILENO ? sockfd + 1: STDIN_FILENO + 1;

	while(1)
	{
		FD_SET(STDIN_FILENO, &rdfds);
		FD_SET(sockfd, &rdfds);

		time_out.tv_sec = 30;
		time_out.tv_usec = 0;

		ret = select(maxfd, &rdfds, NULL, NULL, &time_out);
		if(-1 == ret)
		{
			perror("select");
			goto err1;
		}
		else if(0 == ret)
		{
			printf("time out ...\n");
		}
		else
		{
			if(FD_ISSET(STDIN_FILENO, &rdfds))
			{
				memset(buf, 0, SIZE);
				fgets(buf, SIZE, stdin);
				if('\n' == buf[strlen(buf)-1])
				{
					buf[strlen(buf)-1] = '\0';
				}

				ret = send(sockfd, buf, strlen(buf), 0);
				if(-1 == ret)
				{
					perror("send");
					goto err1;
				}
			}
			if(FD_ISSET(sockfd, &rdfds))
			{
				memset(buf, 0, SIZE);
				ret = recv(sockfd, buf, SIZE, 0);
				if(ret<=0)
				{
					perror("recv");
					goto err1;
				}

				printf("\033[34m%s:%d: \033[0m%s\n", inet_ntop(AF_INET, &addr.sin_addr.s_addr, ip_buf, 16), addr.sin_port, buf);
			}
		}
	}
	close(sockfd);
    return 0;
err1:
	close(sockfd);
err0:
	return 1;
}

