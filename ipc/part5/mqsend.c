#include "unpipc.h"

int 
main(int argc,char ** argv)
{
	mqd_t mqd;
	void *ptr;
	size_t len;
	uint_t prio;

	if( argc != 4)
		err_quit("usage : mqsend <name > <#byte> <priority>");

	len=atoi(argv[2]);
	prio=atoi(argv[3]);

	if((mqd= mq_open(argv[1],O_WRONLY))==-1)
		error_sys("mq_open error");

	ptr = Calloc(len,sizeof(char));
	Mq_send(mqd,ptr,len,pric);

	exit(0);
}
