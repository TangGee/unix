#include "unpipc.h"
#include <errmethod.h>

int main(int argc, char **argv)
{
	int mqid;
	size_t len;
	long type;
	struct msgbuf *ptr;

	if(argc !=4)
		err_quit("sdsadasdasd");
	len= atoi(argv[2]);
	type =atoi(argv[3]);
	mqid = msgget(ftok(argv[1],0),O_WRONLY|O_CREAT);

	ptr = calloc(sizeof(long)+len,sizeof(char));

	*((long *)ptr) = type;
	msgsnd(mqid,ptr,len,0);

	exit(0);

}
