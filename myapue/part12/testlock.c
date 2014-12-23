#include "ourhdr.h"
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>


int 
main(void)
{
	int fd;
	pid_t pid;
	char buff[5];
	struct stat statbuf;

	if((fd=open("templock",O_RDWR| O_CREAT | O_TRUNC, FILE_MODE))<0)
		err_sys("open error");

	if(write(fd,"avcdef",6)!=6)
		err_sys("write error");
	if(fstat(fd,&statbuf)<0)
		err_sys("fstat error");
	if(fchmod(fd,(statbuf.st_mode  & ~S_IXGRP)| S_ISGID)<0)
		err_sys("fstat error");
	
	TELL
}
