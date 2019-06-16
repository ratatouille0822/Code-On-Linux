#include <headers.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	int ret = -1;

	unsigned char buffer[16];
	unsigned char ip[4];
	uint32_t ip_int = 0;

	struct in_addr addr;

	if(2 != argc)
	{
		printf("usage ./a.out ipaddr\n");
		return 1;
	}

	bzero(ip, 4);

	ret = inet_pton(AF_INET, argv[1], ip);
	if(-1 == ret)
	{
		perror("inet_pton");
		return 1;
	}

	printf("ip = %hu.%hu.%hu.%hu\n", ip[0], ip[1], ip[2], ip[3]);
	printf("------------ntop------------------\n");

	bzero(buffer, 16);
	
	if(NULL == inet_ntop(AF_INET, ip, buffer, 16))
	{
		perror("inet_ntop");
		return 1;
	}

	printf("ip = %s\n", buffer);

    return 0;
}

