#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>

int main(int argc,char **argv)
{
	int i,msqid;

	for(i=0;i<10;i++)
	{
		msqid=msgget(IPC_PRIVATE,3|IPC_CREAT);
		printf("msqid=%d\n",msqid);

		msgctl(msqid,IPC_RMID,NULL);
	}
}
