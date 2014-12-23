#include "unpipc.h"

#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"

int main(int argc,char **argv)
{
	int readfd,writefd;
	pid_t childpid;


	if(mkfifo(FIFO1,FILE_MODE)<0 && errno!=EEXIST)
		err_sys("can't creadt %s",FIFO1);
	if(mkfifo(FIFO2,FILE_MODE)<0&& errno!=EEXIST)
	{
		err_sys("can't create %s",FIFO2);
		unlink(FIFO1);
	}

	if((childpid=Fork())==0)
	{
		readfd=Open(FIFO1,O_RDONLY,0);
		writefd=Open(FIFO2,O_WRONLY,0);

		server(readfd,writefd);

		exit(0);

	}

	writefd=Open(FIFO1,O_WRONLY,0);
	readfd=Open(FIFO2,O_RDONLY,0);

	client(readfd,writefd);

	Waitpid(childpid,NULL,0);

	Close(readfd);
	Close(writefd);

	unlink(FIFO1);
	unlink(FIFO2);

	exit(0);



}
