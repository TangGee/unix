#include "unpipc.h"
#include <errmethod.h>

int main(int argc,char **argv)
{
	int c, oflag,mqid;

	oflag = O_RDWR | O_CREAT;

	while((c=getopt(argc,argv,"e"))!=-1)
	{
		switch(c){
			case 'e':
				oflag | IPC_EXCL;
				break;

		}

	}

	if(optind !=argc -1)
		err_quit("uusage sssss");
	mqid = msgget(ftok(argv[optind],0),oflag);
	exit(0);
}
