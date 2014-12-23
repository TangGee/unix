#include "unpipc.h"
#include "../lib/wrapsem.c"

int 
main(int argc,char **argv)
{
	if(argc!=2)
		err_quit("usage : semunlink <name>");
	Sem_unlink(argv[1]);

	exit(0);
}

