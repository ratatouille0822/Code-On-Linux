#include <headers.h>
#include <dirent.h>

//struct dirent
//  {
//    __ino_t d_ino;
//    __off_t d_off;
//    unsigned short int d_reclen;
//    unsigned char d_type;
//    char d_name[256];		/* We must not include limits.h! */
//  };
int get_d_type(unsigned int type)
{
	switch(type)
	{
	case DT_BLK:
		printf("DT_BLK\n");
		break;
	case DT_CHR:
		printf("DT_CHR\n");
		break;
	case DT_DIR:
		printf("DT_DIR\n");
		break;
	case DT_LNK:
		printf("DT_LNK\n");
		break;
	case DT_FIFO:
		printf("DT_FIFO\n");
		break;
	case DT_REG:
		printf("DT_REG\n");
		break;
	case DT_SOCK:
		printf("DT_SOCK\n");
		break;
	default:
		printf("UNKNOW\n");
		break;
	}
}

int main()
{
	char buffer[128];
	char* path = "/home/zhangshuo/01_Lessons/01_Linux_System_Programming/05_Day05/";
	 
	DIR * dir = opendir(path);

	struct dirent * read_dir = NULL;
	
	do
	{
		read_dir = readdir(dir);
		printf("%10ld, %10ld, %5u,%25s ", read_dir->d_ino, read_dir->d_off, read_dir->d_reclen, read_dir->d_name);

		get_d_type(read_dir->d_type);
	}
	while(NULL != read_dir);
	return 0;
}

