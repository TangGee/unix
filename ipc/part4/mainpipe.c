#include "unpipc.h"


void client(int ,int), server(int ,int);



int main(int argc,char ** argv)
{
	int pipe1[2],pipe2[2];

	pid_t childpid;

	Pipe(pipe1);
	Pipe(pipe2);

	if((childpid=Fork())==0)
	{
		//pipe1的读
		Close(pipe1[1]);

		//pipe2的写
		Close(pipe2[0]);

		server(pipe1[0],pipe2[1]);
	}

	//pipe1的写
	Close(pipe1[0]);
	//pipe2的读
	Close(pipe2[1]);

	client(pipe2[0],pipe1[1]);

	Waitpid(childpid,NULL,0);

	exit(0);

}






