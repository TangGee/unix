#include "unpipc.h"

int
Fcntl(int fd, int cmd, void *arg)
{
	        int     n;

		        if ( (n = fcntl(fd, cmd, arg)) == -1)
				                err_sys("fcntl error");
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

void
Ftruncate(int fd, off_t length)
{
	if (ftruncate(fd, length) == -1)
		err_sys("ftruncate error");
}

long
Sysconf(int name)
{
	long    val;

	errno = 0;              /* in case sysconf() does not change this */
	if ( (val = sysconf(name)) == -1) {
		if (errno != 0)
			err_sys("sysconf error");
		
		else
			err_sys("sysconf: %d not defined", name);
	}
	return(val);
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


void *
Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	void    *ptr;

	if ( (ptr = mmap(addr, len, prot, flags, fd, offset)) == MAP_FAILED)
		err_sys("mmap error");
	return(ptr);
}




int
Shm_open(const char *pathname, int oflag, mode_t mode)
{
	int             fd;

	if ( (fd = shm_open(pathname, oflag, mode)) == -1)
		err_sys("shm_open error for %s", pathname);
	return(fd);
}

void
Shm_unlink(const char *pathname)
{
	if (shm_unlink(pathname) == -1)
		err_sys("shm_unlink error");
}

void
Fstat(int fd, struct stat *ptr)
{
	        if (fstat(fd, ptr) == -1)
			                err_sys("fstat error");
}

key_t
Ftok(const char *pathname, int id)
{
	key_t   key;

	if ( (key = ftok(pathname, id)) == -1)
		err_sys("ftok error for pathname \"%s\" and id %d", pathname, id);
	return(key);
}

int
Shmget(key_t key, size_t size, int flags)
{
	int             rc;

	if ( (rc = shmget(key, size, flags)) == -1)
		err_sys("shmget error");
	return(rc);
}

void *
Shmat(int id, const void *shmaddr, int flags)
{
	void    *ptr;

	if ( (ptr = shmat(id, shmaddr, flags)) == (void *) -1)
		err_sys("shmat error");
	return(ptr);
}


void
Shmctl(int id, int cmd, struct shmid_ds *buff)
{
	if (shmctl(id, cmd, buff) == -1)
		err_sys("shmctl error");
}





