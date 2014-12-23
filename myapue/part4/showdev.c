#include "apue.h"
#include <sys/stat.h>
#include <errmethod.h>
#include <sys/sysmacros.h>

#define _BSD_SOURCE             /* See feature_test_macros(7) */
#include <sys/types.h>


int main(int argc,char **argv)
{


	int i;
	struct stat statbuf;
	for(i=1;i<argc;i++)
	{
		printf("%s",argv[i]);

		if(lstat(argv[1],&statbuf)<0){
			err_sys("lstat error");
			continue;
		}

		//干=======================================================
		printf("dev = %d/%d",major(statbuf.st_dev),minor(statbuf.st_dev));
		if(S_ISCHR(statbuf.st_mode)||S_ISBLK(statbuf.st_mode)){
			printf(" %s rdev=%d/%d",S_ISCHR(statbuf.st_mode)?"char dev":"block dev", major(statbuf.st_rdev),minor(statbuf.st_rdev));
		}

		printf("\n");

	}
}
