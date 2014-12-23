#include "unpipc.h"
#include "../lib/wrapunix1.c"

int main(int argc,char **argv)
{
	int c,oflag ,semid,nsems;

	oflag = SVSEM_MODE | IPC_CREAT;
	while((c=getopt(argc,argv,"e"))!=-1){
	
		switch(c){
			case 'e':
				oflag|IPC_EXCL;
				break;
		}
	}

	if(optind!= argc-2)
		err_quit("suage : semcreate [-e] <pathname> <nsems>");
	nsems=atoi(argv[optind+1]);

	semid= Semget(Fork(argv[optind],0),nsems,oflags);
	exit(0);
}
