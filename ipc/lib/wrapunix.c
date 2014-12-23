#include "unpipc.h"
#include <mqueue.h>

typedef void    Sigfunc_rt(int, siginfo_t *, void *);


void Pipe(int *fp)
{
	if(pipe(fp)<0)
		err_sys("pipe error");
}


pid_t Fork()
{
	pid_t pid;

	if((pid=fork())==-1)
		err_sys("fork error");

	return pid;
}

void Close(int fd)
{
	if(close(fd)==-1)
		err_sys("close error");
}

pid_t
Waitpid(pid_t pid, int *iptr, int options)
{
	pid_t   retpid;

	if ( (retpid = waitpid(pid, iptr, options)) == -1)
		err_sys("waitpid error");
	return(retpid);
}


void
Write(int fd, void *ptr, size_t nbytes)
{
	if (write(fd, ptr, nbytes) != nbytes)
		err_sys("write error");
}

ssize_t
Read(int fd, void *ptr, size_t nbytes)
{
	ssize_t         n;

	if ( (n = read(fd, ptr, nbytes)) == -1)
		err_sys("read error");
	return(n);
}


int
Open(const char *pathname, int oflag, ...)
{
	int             fd;
		if ( (fd = open(pathname, oflag)) == -1)
			err_sys("open error for %s", pathname);
	return(fd);
}




int
Getopt(int argc, char *const *argv, const char *str)
{
	int             opt;

	if ( ( opt = getopt(argc, argv, str)) == '?')
		exit(1);                /* getopt() has already written to stderr */
	return(opt);
}


/*
mqd_t
Mq_open(const char *pathname, int oflag, ...)
{
	mqd_t   mqd;
	va_list ap;
	mode_t  mode;
	struct mq_attr  *attr;

	if (mqd = mq_open(pathname, oflag)) == (mqd_t) -1)
			err_sys("mq_open error for %s", pathname);
	}

	return(mqd);
}
*/

void
Mq_close(mqd_t mqd)
{
	        if (mq_close(mqd) == -1)
			                err_sys("mq_close error");
}

void
Mq_unlink(const char *pathname)
{
	if (mq_unlink(pathname) == -1)
		err_sys("mq_unlink error");
}

void
Mq_getattr(mqd_t mqd, struct mq_attr *mqstat)
{
	        if (mq_getattr(mqd, mqstat) == -1)
			                err_sys("mq_getattr error");
}






Sigfunc_rt *
signal_rt(int signo, Sigfunc_rt *func)
{
	struct sigaction	act, oact;

	act.sa_sigaction = func;		/* must store function addr here */
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;		/* must specify this for realtime */
	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;	/* SunOS 4.x */
#endif
	} else {
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;		/* SVR4, 44BSD */
#endif
	}
	if (sigaction(signo, &act, &oact) < 0)
		return((Sigfunc_rt *) SIG_ERR);
	return(oact.sa_sigaction);
}
/* end signal_rt */

Sigfunc_rt *
Signal_rt(int signo, Sigfunc_rt *func)
{
	Sigfunc_rt	*sigfunc;

	if ( (sigfunc = signal_rt(signo, func)) == (Sigfunc_rt *) SIG_ERR)
		err_sys("signal_rt error");
	return(sigfunc);
}


void
Sigaddset(sigset_t *set, int signo)
{
	if (sigaddset(set, signo) == -1)
		err_sys("sigaddset error");
}


void
Sigemptyset(sigset_t *set)
{
	if (sigemptyset(set) == -1)
		err_sys("sigemptyset error");
}




void
Sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
	if (sigprocmask(how, set, oset) == -1)
		err_sys("sigprocmask error");
}

void
Sigqueue(pid_t pid, int signo, const union sigval val)
{
	if (sigqueue(pid, signo, val) == -1)
		err_sys("sigqueue error");
}

