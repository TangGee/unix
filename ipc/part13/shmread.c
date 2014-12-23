#include "unpipc.h"
#include <errmethod.h>
#include "../lib/wrapunix1.c"

int main(int argc,char **argv)
{
	int i,fd;
	unsigned char c,*ptr;
	struct stat stat;
	
	if(argc!=2)
		err_quit("usgae: shmread <name>");
	fd=Shm_open(argv[1],O_RDONLY,FILE_MODE);
	Fstat(fd,&stat);
	ptr=Mmap(NULL,stat.st_size,PROT_READ,MAP_SHARED,fd,0);

	Close(fd);

	for(i=0;i<stat.st_size;i++)
		if((c=*ptr++)!=(i%256))
			err_ret("ptr[%d] =%d",i,c);
	exit(0);
}
