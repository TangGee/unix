#include <sys/times.h>
#include <unistd.h>
#include <stdlib.h>
#include <errmethod.h>
#include "prexit.c"

static void 
pr_times(clock_t real,struct tms *tmsstart,struct tms *tmsend);
static void do_cmd(const char *);

int 
main(int argc,char **argv)
{
	int i;
	for(i=1;i<argc;i++){
		do_cmd(argv[i]);
	}
	exit(0);

}

static void do_cmd(const char *cmd)
{
	struct tms tmsstart,tmsend;
	clock_t start,end;
	int status;

	fprintf(stderr, "\ncommand %s\n",cmd);

	if((start=times(&tmsstart))==-1)
		err_sys("times error");
	if((status=system(cmd))<0)
		err_sys("system error");
	if((end=times(&tmsend))==-1)
		err_sys("times error");

	pr_times(end-start,&tmsstart,&tmsend);

	pr_exit(status);

}



static void 
pr_times(clock_t real,struct tms *tmsstart,struct tms *tmsend)
{
	/**
	 * 每秒钟滴答数
	 */
	static long  clktck=0;

	printf("clktck --- %ld",clktck);
	if(clktck==0)
		if((clktck=sysconf(_SC_CLK_TCK))<0)
			err_sys("sysconf error");
	fprintf(stderr,"  real: %7.2f\n", real/(double)clktck);
	fprintf(stderr,"  user: %7.2f\n", 
		(tmsend->tms_utime-tmsstart->tms_utime)/(double)clktck);
	fprintf(stderr,"  sys: %7.2f\n", 
		(tmsend->tms_stime-tmsstart->tms_stime)/(double)clktck);
	fprintf(stderr,"  child user: %7.2f\n", 
		(tmsend->tms_cutime-tmsstart->tms_cutime)/(double)clktck);
	fprintf(stderr,"  child sys: %7.2f\n", 
		(tmsend->tms_cstime-tmsstart->tms_cstime)/(double)clktck);
}
