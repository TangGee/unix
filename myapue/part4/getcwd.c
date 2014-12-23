#include "apue.h"
#include "pathalloc.c"
#include <errmethod.h>

int main(void)
{
	char *ptr;
	size_t size;

	if(chdir("/tmp/spool/123")<0)
		err_sys("chdir faild");

	ptr=path_alloc(&size);
	if(getcwd(ptr,size)<0)
		err_sys("getcwd fauled");

	printf("cwd =%s\n",ptr);
	exit(0);
}
