#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errmethod.h>
static void f1(void),f2(void);

int main(void )
{
	printf("main pid %d\n",getpid());
	f1();
	f2();

	printf("man getpid=%d\n",getpid());
	_exit(0);
}

static void f1(void)
{
	pid_t pid;

	if(pid=vfork()<0)
	{
		err_sys("vfork error");
	}
}

static void f2(void)
{
	char buf[1000];
	int i;

	printf("f2 pid_t = %d\n",getpid());

	for(i-0;i<sizeof(buf);i++)
	{
		buf[i]=0;
	}
}
