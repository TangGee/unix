#include "unpipc.h"
#include <errmethod.h>

#define MAXMSG (8192 + sizeof(long))

struct msgbuf{

	long msg_type;
	char * buf;
};

int 
main(int argc, char ** argv)
{
	int c, flag, mqid;
	long type;
	ssize_t n;
	struct msgbuf *buff;

	type = flag=0;
	while((c= getopt (argc ,argv, "nt:"))!=-1)
	{
		switch(c){
			case 'n':
				flag|=IPC_NOWAIT;
				break;
			case 't':
				type = atol(optarg);
				break;

		}
	}

	if(optind != argc-1)
		err_quit("usage : msgrcv [-n] | [-t type] <pathname>");

	mqid = msgget(ftok(argv[optind],0),O_RDONLY|O_CREAT);
	buff = malloc(MAXMSG);

	n= msgrcv(mqid,buff, MAXMSG,type,flag);
	printf("read %ld bytes ,type =%ld\n",n, buff->msg_type);

	exit(0);

}
