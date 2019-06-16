#include <headers.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SIZE 128

int main(int argc, char* argv[])
{
	int ret = -1;
	int sockfd = -1;

	struct sockaddr_in addr;

	char buffer[SIZE];

	socklen_t len;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == sockfd)
	{
		perror("socket");
		goto err0;
	}
	
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	ret = inet_pton(AF_INET, argv[1], (void*)&addr.sin_addr.s_addr);
	if(-1 == ret)
	{
		perror("inet_pton");
		goto err1;
	}

	len = sizeof(addr);

	printf("\033[33mfd = %d, ip:port =  %s:%d\n", sockfd,  inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	while(1)
	{
		bzero(buffer, SIZE);
		fgets(buffer, SIZE, stdin);
		if('\n' == buffer[strlen(buffer) - 1])
		{
			buffer[strlen(buffer) - 1] = '\0';
		}

		ret = sendto(sockfd, buffer, strlen(buffer), 0, (void*)&addr, sizeof(addr));
		if(ret <= 0)
		{
			perror("sendto");
			break;
		}

		bzero(buffer, SIZE);
		ret = recvfrom(sockfd, buffer, SIZE, 0,  (void*)&addr, &len);
		if(-1 == ret)
		{
			perror("recvfrom");
			break;
		}
		printf("\033[36mSVR: \033[0m%s\n", buffer);
	}

	close(sockfd);

    return 0;

err1:
	close(sockfd);
err0:
	return 1;
}

