#include "unpipc.h"
#include "../lib/wrapunix1.c"
#include <errmethod.h>

int 
main(int argc,char **argv)
{
	int id;

	if(argc!=2)
		err_quit("ysage: shmrmid <pathname>");

	id=Shmget(Ftok(argv[1],0),0,SVSHM_MODE);
	Shmctl(id,IPC_RMID,NULL);
	exit(0);
}
