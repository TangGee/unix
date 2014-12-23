#include "unpipc.h"


int main(int argc,char ** argv)
{
	printf("PIPE_BUF= %ld,OPEN_MAX=%ld\n",
			pathconf(argv[1],_PC_PIPE_BUF),sysconf(_SC_OPEN_MAX));

	exit(0);
}
