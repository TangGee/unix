#include "unpipc.h"
#include <errmethod.h>


int main(int argc,char ** argv)
{
	if(argc!=2)
		err_quit("usage : mq name");

	mq_unlink(argv[1]);

	exit(0);
}
