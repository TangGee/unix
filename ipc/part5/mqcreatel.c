#include "unpipc.h"
#include <errmethod.h>
#include "../lib/wrapunix.c"


int main(int argc,char ** argv)
{
	int c, flags;
 	int  mqd;

	flags= O_RDWR | O_CREAT;

	while((c=Getopt(argc,argv,"e"))!=-1)
	{
		switch(c){
		case 'e':
			flags |= O_EXCL;
			break;
		}
	}

	if(optind!=argc-1)
		err_quit("usage : mqcreate [-e] name");

	if(mqd=mq_open(argv[optind],flags,FILE_MODE,NULL))
	{
		err_sys("mq_open error");
	}

	Mq_close(mqd);

	exit(0);
}
