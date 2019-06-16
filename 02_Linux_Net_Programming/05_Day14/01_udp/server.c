#include <headers.h>
#include <sys/socket.h>

#define SIZE 128

int main(int argc, char* argv[])
{
	int sockfd = -1;
	int ret = -1;

	struct sockaddr_in addr;
	struct sockaddr_in from;

	char buffer[SIZE];
	char ip_buffer[16];

	socklen_t len = 0;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == sockfd)
	{
		perror("socket");
		goto err0;
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	ret = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);
	if(-1 == ret)
	{
		perror("inet_pton");
		goto err1;
	}

	ret = bind(sockfd, (void*)&addr, sizeof(addr));
	if(-1 == ret)
	{
		perror("bind");
		goto err1;
	}

	len = sizeof(from);

	while(1)
	{
		bzero(buffer, SIZE);
		ret = recvfrom(sockfd, buffer, SIZE, 0, (void* )&from, &len);
		if(-1 == ret)
		{
			perror("recvfrom");
			goto err1;
		}

		printf("\033[32mClient %s:%d:\033[0m %s\n", inet_ntoa(from.sin_addr), ntohs(from.sin_port), buffer);

		ret = sendto(sockfd, buffer, strlen(buffer), 0, (void*)&from, sizeof(from));
		if(ret <= 0)
		{
			perror("sendto");
			goto err1;
		}

	}

err1:
	close(sockfd);
err0:
	return 1;
    return 0;
}

