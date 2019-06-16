#include <headers.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>

#define SIZE 128

int main()
{
	int ret = -1;
	int sockfd = -1;
	int new_sockfd = -1;

	struct sockaddr_in addr;
	struct sockaddr_in new_sockaddr;


	char read_buffer[SIZE];
	char write_buffer[SIZE];

	char buf_ip[16];

	pid_t pid;

	socklen_t len = 0;
	len = sizeof(addr);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if( -1 == sockfd )
	{
		perror("socket");
		goto err0;
	}
	printf("Create socket succeeded\n");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	ret = inet_pton(AF_INET, "192.168.14.83", &addr.sin_addr.s_addr);
	if(-1 == ret)
	{
		perror("inet_pton");
		goto err1;
	}
	printf("IP == %#x \n", addr.sin_addr.s_addr);

	ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("bind");
		goto err1;
	}
	printf("binding IP = 127.0.0.1, Port = 10000 succeeded\n");

	ret = listen(sockfd, 10);
	if(-1 == ret)
	{
		perror("listen");
		goto err1;
	}
	printf("server listening IP = 127.0.0.1, Port = 10000 \n");

	new_sockfd = accept(sockfd,(struct sockaddr* )&new_sockaddr, &len);
	if(-1 == new_sockfd)
	{
		perror("accept");
		goto err1;
	}
	printf("IP = %s, Port = %d connectd, socket fd = %d\n", inet_ntop(AF_INET, &new_sockaddr.sin_addr.s_addr, buf_ip, 16), new_sockaddr.sin_port, new_sockfd);

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
			bzero(write_buffer, SIZE);
			printf("Please Input: \n");
			fgets(write_buffer, SIZE, stdin);
			if('\n' == write_buffer[strlen(write_buffer)-1])
			{
				write_buffer[strlen(write_buffer)-1] = '\0';
			}

			ret = write(new_sockfd, write_buffer, strlen(write_buffer));
			if(ret <= 0)
			{
				perror("write");
				goto err1;
			}
		}

	}
	while(1)
	{
		bzero(read_buffer, SIZE);
		ret = read(new_sockfd, read_buffer, SIZE);
		if(ret <= 0)
		{
			perror("read");
			goto err1;
		}

		printf("\n");
		printf("read_buffer: %s\n", read_buffer);

	}

	close(new_sockfd);
	close(sockfd);

    return 0;

err1:
	close(new_sockfd);
	close(sockfd);
err0:
	return 1;
}


