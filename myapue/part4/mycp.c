#include "apue.h"
#include <fcntl.h>
#include <errmethod.h>

int main(int argc,char **argv)
{
	int o_fd,n_fd;
	char c;

	if(argc !=3)
		err_quit("usage:mycp <srcpathname> <descpathname>");
	if((o_fd=open(argv[1],O_RDONLY))==-1)
		err_quit("open srcfile error");
	if((n_fd=open(argv[2],O_RDWR|O_CREAT|O_EXCL))==-1)
	{
		if(errno==EEXIST){
			printf("are you overrite the destfile?y or n");
			scanf("%c",&c);
			switch(c)
			{
				case 'y':
					if((n_fd=open(argv[2],O_RDWR|O_CREAT))==-1)
						err_quit("open desc file error");

					break;
				case 'n':
					exit(1);
					break;
			
			
			}
		}else{
			err_quit(" open dest file error");
		}

	}

	while(read(o_fd,&c,1)==1){
		if(c!=0)
		write(n_fd,&c,1);
	}

	close(n_fd);
	close(o_fd);
}
