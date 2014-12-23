#include "apue.h"
#include <errmethod.h>
#include <fcntl.h>


int main(int argc ,char **argv)
{
	if(open("file",O_RDWR)<0)
		err_sys("open error");

	if(unlink("file")<0)
		err_sys("unlink error");

	sleep(15);

	printf("done\n");

	exit(0);
}
