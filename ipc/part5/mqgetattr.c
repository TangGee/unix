#include "unpipc.h"
#include <errmethod.h>
#include "../lib/wrapunix.c"


int main(int argc,char **argv)
{
	if(argc!=2)
		err_quit("---");
	mqd_t mqd;
	struct mq_attr attr;

	if(mqd=mq_open(argv[1],O_RDONLY)<0)
		err_sys("mq_open error");

	Mq_getattr(mqd,&attr);

	printf("max #msgs = %ld\n , max #bytes/msg= %ld\n,"
			"#currently on queue = %ld\n",
			attr.mq_maxmsg, attr.mq_msgsize,attr.mq_curmsgs);


	Mq_close(mqd);
}
