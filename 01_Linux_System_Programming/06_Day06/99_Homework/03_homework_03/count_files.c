#include <headers.h>

#define SIZE 512

int countFiles(char* path)
{
	int ret = -1;
	struct stat s;
	struct dirent* read_dir = NULL;
	DIR* dir = NULL;
	DIR* subdir = NULL;

	char temp_dir[SIZE];
	char temp[SIZE];

	static int cnt = 0;

	dir = opendir(path);
	while(1)
	{
		read_dir = readdir(dir);

		if(NULL == read_dir)
		{
			break;
		}

		if(strcmp(read_dir->d_name, ".") == 0 || strcmp(read_dir->d_name, "..")==0 )
		{
			continue;
		}
		else
		{
			if(DT_DIR == read_dir->d_type )
			{
				memset(temp_dir, 0, SIZE);
				sprintf(temp_dir, "%s/%s",getcwd(temp, SIZE), read_dir->d_name);
				printf("---%s\n", temp_dir);
				countFiles(temp_dir);
			}
			else if(DT_REG == read_dir->d_type)
			{
				cnt ++;
			}
			printf("%s \n", read_dir->d_name);
		}
	}

	return cnt;

}

int main(int argc, char* argv[])
{
	if(2 != argc)
	{
		printf("usage: ./a.out directory\n");
	}

	int i = countFiles(argv[1]);
	printf("-------------result is %d ------------------\n", i);
    return 0;
}

