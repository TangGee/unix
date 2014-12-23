#include <errmethod.h>
#include "unpipc.h"
#include "../lib/wrapunix1.c"

int 
main(int argc,char **argv)
{
	int i,id;
	struct shmid_ds buff;
	unsigned char *ptr;

	if(argc!=2)
		err_quit("usage : shmwrite <pathname>");
	id=Shmget(Ftok(argv[1],0),0,SVSHM_MODE);
	ptr = Shmat(id,NULL,0);
	Shmctl(id,IPC_STAT,&buff);

	for(i=0;i<buff.shm_segsz;i++)
	{
		*ptr++=i%256;
	}
	exit(0);
}
