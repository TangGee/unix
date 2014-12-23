#include "unpipc.h"
#include "../lib/wrapunix1.c"
#include <errmethod.h>
int main(int argc, char **argv)
{
	printf("SEM_NSEMS_MAX= %ld,SEN_VALUE_MAX = %ld\n",
		sysconf(_SC_SEM_NSEMS_MAX),sysconf(_SC_SEM_VALUE_MAX));
	exit(0);
}
