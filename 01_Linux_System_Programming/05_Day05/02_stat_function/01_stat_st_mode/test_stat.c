#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
	if(2 != argc)
	{
		printf("usage: ./a.out filename\n");
		return 1;
	}

	struct stat s;
	
	stat(argv[1], &s);

	printf("st_dev     = %ld\n",  s.st_dev		);
    printf("st_ino     = %ld\n",  s.st_ino      );
    printf("st_mode    = %ld\n",  s.st_mode     );
    printf("st_nlink   = %ld\n",  s.st_nlink    );
    printf("st_uid     = %ld\n",  s.st_uid      );
    printf("st_gid     = %ld\n",  s.st_gid      );
    printf("st_rdev    = %ld\n",  s.st_rdev     );
    printf("st_size    = %ld\n",  s.st_size     );
    printf("st_blksize = %ld\n",  s.st_blksize  );
    printf("st_blocks  = %ld\n",  s.st_blocks   );

	return 0;
}

