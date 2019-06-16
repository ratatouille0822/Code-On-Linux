#include <headers.h>
#include <sys/select.h>

#define SIZE 128

int main(int argc, char* argv[])
{

	int ret = -1;
	int sockfd = -1;
	int newfd = -1;
	int maxfd = 0;

	struct sockaddr_in addr;
	struct sockaddr_in from;
	struct timeval timo;

	char buffer[SIZE];
	char ip_buffer[16];

	fd_set readfds;
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

	ret = listen(sockfd, 10);
	if(-1 == ret)
	{
		perror("listen");
		goto err1;
	}

	printf("\033[33mServer listening %s:%s\033[0m\n", argv[1], argv[2]);

	len = sizeof(from);
	bzero(&from, sizeof(from));
	newfd = accept(sockfd, (void*)&from, &len);
	if(-1 == newfd)
	{
		perror("accept");
		goto err2;
	}

	maxfd = newfd + 1;

	while(1)
	{
		FD_ZERO(&readfds);
		FD_SET(STDIN_FILENO, &readfds);
		FD_SET(newfd, &readfds);

		timo.tv_sec = 10;
		timo.tv_usec = 0;

		ret = select(maxfd, &readfds, NULL, NULL, &timo);
		if(-1 == ret)
		{
			perror("select");
			break;
		}
		if(0 == ret)
		{
			printf("timo !\n");
			continue;
		}

		if(FD_ISSET(newfd, &readfds))
		{
			bzero(buffer, SIZE);
			ret = recv(newfd, buffer, SIZE, 0);
			if(ret <= 0)
			{
				perror("recv");
				break;
			}

			if('\n' == buffer[strlen(buffer)-1])
			{
				buffer[strlen(buffer)-1] = '\0';
			}

			bzero(ip_buffer, 16);
			printf("\033[034m%s:%d:\033[0m %s\n", inet_ntop(AF_INET, &from.sin_addr.s_addr, ip_buffer, 16 ), from.sin_port, buffer);
			
			ret = send(newfd, buffer, sizeof(buffer), 0);
			if(ret <= 0)
			{
				perror("send");
				break;
			}
		}

		if(FD_ISSET(STDIN_FILENO, &readfds))
		{
			bzero(buffer, SIZE);
			fgets(buffer, SIZE, stdin);
			if('\n' == buffer[strlen(buffer)-1])
			{
				buffer[strlen(buffer)-1] = '\0';
			}
			bzero(ip_buffer, 16);
			printf("\033[034mSVR:\033[0m %s\n", buffer);
			
			ret = send(newfd, buffer, sizeof(buffer), 0);
			if(ret <= 0)
			{
				perror("send");
				break;
			}
		}

	}

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

