#include <headers.h>

int main()
{
	pid_t pid = -1;

	// 1. 创建子进程
	pid = fork();
	if(-1 == pid)
	{
		perror("fork");
		return 1;
	}
	else if( pid > 0 )
	{
		exit(0);
	}

	// 2. 创建会话
	pid = setsid();
	if(-1 == pid)
	{
		perror("setsid");
		return 2;
	}

	// 3. 更改工作目录：
	if(-1 == chdir("/home/zhangshuo"))
	{
		perror("chdir");
		return 3;
	}

	// 4. 更改文件范文掩码：
	 umask(0);

	// 5. 关闭文件描述符：
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	// 6. 主要工作循环：
	while(1)
	{
		system("echo \"hello world\" >> /tmp/txt");
		sleep(1);
	}
    return 0;
}

