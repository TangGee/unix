#include "unpipc.h"

char *
Fgets(char *ptr, int n, FILE *stream)
{
	char    *rptr;

	if ( (rptr = fgets(ptr, n, stream)) == NULL && ferror(stream))
		err_sys("fgets error");

	return (rptr);
}


void
Fputs(const char *ptr, FILE *stream)
{
	if (fputs(ptr, stream) == EOF)
		err_sys("fputs error");
}


FILE *
Popen(const char *command, const char *mode)
{
	FILE    *fp;

	if ( (fp = popen(command, mode)) == NULL)
		err_sys("popen error");
	return(fp);
}

int
Pclose(FILE *fp)
{
	int             n;

	if ( (n = pclose(fp)) == -1)
		err_sys("pclose error");
	return(n);
}


off_t
Lseek(int fd, off_t offset, int whence)
{
	off_t   pos;

	if ( (pos = lseek(fd, offset, whence)) == (off_t) -1)
		err_sys("lseek error");
	return(pos);
}


ssize_t
Read(int fd, void *ptr, size_t nbytes)
{
	        ssize_t         n;

		if ( (n = read(fd, ptr, nbytes)) == -1)
			err_sys("read error");
		return(n);
}

void
Write(int fd, void *ptr, size_t nbytes)
{
	        if (write(fd, ptr, nbytes) != nbytes)
			err_sys("write error");
}
    
