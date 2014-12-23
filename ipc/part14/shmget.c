#include "unpipc.h"
#include "../lib/wrapunix1.c"
#include <errmethod.h>

int  main(int argc,char **argv)
{
	int c,id,oflag;
	char *ptr;
	size_t length;

	oflag=SVSHM_MODE | IPC_CREAT;

	while((c=getopt(argc,argv,"e"))!=-1){
		switch(c)
		{
			case 'e':
				oflag|=IPC_EXCL;
				break;

		}
	}
	if(optind!=argc-2)
		err_quit("usgae: shmget [-e] <pathname> <length>");
	length=atoi(argv[optind+1]);
	id=Shmget(Ftok(argv[optind],0),length,oflag);
	ptr = Shmat(id,NULL,0);
	exit(0);
}
