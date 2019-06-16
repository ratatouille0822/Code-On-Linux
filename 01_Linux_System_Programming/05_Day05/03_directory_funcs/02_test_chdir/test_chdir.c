#include <headers.h>

int main()
{
	char buffer[128];

	getcwd(buffer, 128);
	printf("pwd = %s\n", buffer);

	char* path = "/home/zhangshuo/01_Lessons/01_Linux_System_Programming/05_Day05/03_directory_funcs/01_test_getcwd";
	chdir(path);

	getcwd(buffer, 128);
	printf("pwd = %s\n", buffer);
    return 0;
}

