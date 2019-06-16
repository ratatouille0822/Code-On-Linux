#include <headers.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	int ret = -1;

	if(2 != argc)
	{
		printf("usage ./a.out ipaddr\n");
		return 1;
	}

	unsigned char ip[4];
	ret = inet_pton(AF_INET, argv[1], ip);
	if(-1 == ret)
	{
		perror("inet_pton");
		return 1;
	}

	printf("ip = %hu.%hu.%hu.%hu\n", ip[0], ip[1], ip[2], ip[3]);




    return 0;
}

