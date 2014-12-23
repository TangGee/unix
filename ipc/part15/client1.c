#include "unpipc.h"
#include "../lib/wrapunix1.c"
#include <errmethod.h>
int main(int argc,char **argv)
{
	int fd;
	long ival,oval;
	door_arg_t arg;

	if(argc!=3)
		err_quit("usage: client1 <server-name> <interger-value>");
	fd= Open(argv[1],O_RDWR);

	ival=atoi(argv[2]);
	arg.data_ptr=(char *)&ival;
	arg.data_size= sizeof(long);
	arg.desc_ptr=NULL;
	arg.desc_num=0;
	arg.rbuf=(char *)&oval;
	arg.rsize=sizeof(long);
	
	Door_call(fd,&arg);
	printf("result: %ld\n",oval);

	exit(0);
}
