#include "unpipc.h"

int 
main(int argc, char ** argv)
{
	int msgid;
	struct msqid_ds info;
	struct msgbuf buf;

	msqid msgget(IPC_PRIVATE,SVMSG_MODE| IPC_CREATE);
	buf.mtype=1;
	buf.mtex[0]=1;
	msgsnd(msqid,IPC_STAT,&info);
	printf ("read0write:%03o,cbytes = %lu,
	,qnum = %lu\n, qbytes = %lu",info.msg_perm.mode&0777,
	(ulong_t)info.msg_cbytes,
	        (ulong_t)info.msg_qnum,(ulong_t)info.msg_qbytes);

	system("ipcs -q");

	msgctl(msqid, IPC_RMID,NULL);
	exit(0);
	
}
