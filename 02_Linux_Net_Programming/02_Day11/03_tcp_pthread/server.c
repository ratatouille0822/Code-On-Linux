#include <headers.h>

#define SIZE 128
#define MAX 128

typedef struct _sock_info_t
{
	int sock_fd;
	struct sockaddr_in sock_addr;
}sock_info_t;

void* svr_rw(void* arg)
{
	int ret = -1;
	char buffer[SIZE];
	char ip_buffer[16];

	sock_info_t* sock_info = arg;
	while(1)
	{
		bzero(buffer, SIZE);

		ret = read(sock_info->sock_fd, buffer, SIZE); 
		if(ret <= 0)
		{
			perror("read");
			break;
		}
		if('\n' == buffer[strlen(buffer)-1])
		{
			buffer[strlen(buffer)-1]  = '\0';
		}

		if(strncmp(buffer, "quit", 4) == 0)
		{
			break;
		}
		printf("\033[34m%s:%d:\033[0m %s\n",inet_ntop(AF_INET,&sock_info->sock_addr.sin_addr.s_addr, ip_buffer, 16), ntohs(sock_info->sock_addr.sin_port),  buffer);

		ret = write(sock_info->sock_fd, buffer, strlen(buffer));
		if(ret <= 0)
		{
			perror("write");
			break;
		}
		
	}
	close(sock_info->sock_fd);
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{

	int ret = -1;
	int sockfd = -1;
	int peerfd = -1;
	int i = 0;

	pthread_t tid;
	pthread_attr_t attr;
	socklen_t len;
	sock_info_t sock_info[MAX];

	struct sockaddr_in addr;
	struct sockaddr_in from;

	char buffer[SIZE];
	char ip_buffer[16];

	// check command line imput:
	if(3 != argc)
	{
		printf("\033[031musage server x.x.x.x port\033[0m\n");
		goto err0;
	}

	//set pthread attributes:
	ret = pthread_attr_init(&attr);
	if(0 != ret)
	{
		printf("\033[032mpthread_attr_init failed...\033[0m\n");
		goto err0;
	}

	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(0 != ret)
	{
		printf("\033[032mpthread_attr_setdetachstate failed...\033[0m\n");
		goto err0;
	}

	// 1. create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		perror("socket");
		goto err1;
	}

	// 2. init structure
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	ret = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);
	if(-1 == ret)
	{
		perror("pton");
		goto err1;
	}
	addr.sin_port = htons(atoi(argv[2]));

	// 3. binding addr and port
	ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	if(-1 == ret)
	{
		perror("bind");
		goto err1;
	}

	// 4. set listening
	ret = listen(sockfd, 10);
	printf("\033[031mSocket fd = \033[0m%d, \033[031mSVR IP = \033[0m%s, \033[031mSVR_PORT = \033[0m%s\n",sockfd, argv[1], argv[2]);

	// 5. accept connection
	while(1)
	{
		len = sizeof(from);
		bzero(&from, sizeof(from));
		peerfd = accept(sockfd, (struct sockaddr*)&from, &len);
		if( peerfd <= 0)
		{
			if(EINTR == errno || ECONNABORTED == errno)
			{
				printf("errno = %d\n", errno);
				continue;
			}
			else
			{
				break;
			}
		}

		printf("\033[033mClient %s:%d connected fd = %d\033[0m\n", inet_ntop(AF_INET, &from.sin_addr.s_addr, ip_buffer, 16), ntohs(from.sin_port), peerfd);
		
		// 6. read and write socket
		bzero(&sock_info[i], sizeof(sock_info[i]));
		sock_info[i].sock_fd = peerfd;
		sock_info[i].sock_addr = from;
		ret = pthread_create(&tid, &attr, svr_rw, &sock_info[i]);
		if(0 != ret)
		{
			printf("\033[032mpthread_create failed...\033[0m\n");
			goto err1;
		}
		i++;

		if(MAX == i)
		{
			break;
		}
	}
	
	// 7. close socket
	close(sockfd);
	close(peerfd);
	pthread_attr_destroy(&attr);


    return 0;
err1:
	close(sockfd);
	pthread_attr_destroy(&attr);
err0:
	return 1;
}

