#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>

#define SIZE 128

int main(int argc, char* argv[])
{
	int ret = -1;
	int sockfd = -1;
	struct sockaddr_in addr;

	char ip_buf[16];
	char write_buffer[SIZE];
	char read_buffer[SIZE];

	pid_t pid = -1;

	socklen_t len = 0;

	if(3 != argc)
	{
		perror("usage: client x.x.x.x port\n");
		return 1;
	}
	len = sizeof(struct sockaddr);
	bzero(&addr, sizeof(addr));

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
		perror("inet_pton");
		goto err1;
	}

	printf("Set client IP: %s, Port: %d, sock fd: %d\n", inet_ntop(AF_INET, &addr.sin_addr.s_addr, ip_buf, 16), addr.sin_port, sockfd);

	ret =connect(sockfd,(struct sockaddr*)&addr, len);
	if(-1 == ret)
	{
		perror("connect");
		goto err1;
	}
	
	pid = fork();
	if(-1 == pid)
	{
		perror("fork");
		goto err1;
	}
	else if(0 == pid)
	{
		while(1)
		{
			bzero(read_buffer, SIZE);
			ret = read(sockfd, read_buffer, SIZE);
			if(ret <= 0)
			{
				perror("read");
				goto err1;
			}

			printf("\033[33mServer:\033[0m %s\n", read_buffer);
		}
	}
	else if(pid > 0)
	{
		while(1)
		{
			bzero(write_buffer, SIZE);
//			printf("Please input: \n");
			fgets(write_buffer, SIZE, stdin);
			if('\n' == write_buffer[strlen(write_buffer)-1])
			{
				write_buffer[strlen(write_buffer)-1] = '\0';
			}
			if(0 == strncmp(write_buffer, "exit", 4))
			{

				break;
			}
			ret = write(sockfd, write_buffer, strlen(write_buffer));
			if(ret <= 0)
			{
				printf("client break...write\n");
				perror("write");
				break;
			}
		}
	}

	close(sockfd);
    return 0;
err1:
	close(sockfd);
	return 1;
err0:
	return 1;
}

