#include <headers.h>

int main()
{
	int ret = -1;
	ret = chmod("txt_1", 0666);
	if(-1 == ret)
	{
		perror("chomod");
		return  1;
	}
	printf("修改权限成功 ！");

	STDIN_FILENO

    return 0;
}

