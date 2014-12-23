#include <fcntl.h>
#include "open_fd.c"

#define MAXARGC 3
#define WHITE " "



void request(char *buf,int nread, int clifd, uid_t uid)
{
	int newfd;

	if(buf[nread-1]!=0){
		sprintf(errmsg,"request from uid %d not null terminated: %*.*s\n",uid,nread,nread,buf);
		send_err(clifd,-1,errmsg);
		return ;
	}

	log_msg("request : %s, from uid %d",buf ,uid);

	if(buf_args(buf,cli_args)<0){
		send_err(clifd,-1,errmsg);
		log_msg(errmsg);
		return ;
	}

	if((newfd= open(pathname,oflag))<0){
		sprintf(errmsg,"can't open %s: %s\n",pathname,strerror(errno));
		send_err(clifd,-1,errmsg);
		log_msg(err_msg);
		return ;
	}

	if(send_fd(clifd,newfd)<0)
		log_sys("send_fd error");
	log_msg("sent fd %d over fd %d for %s",newfd,clifd,pathname);

	close(newfd);

}


int buf_args(char *buf, int (*func)(int,char **))
{
	char *ptr,*argv[MAXARGC];
	int argc;

	if(strtok(buf,WHITE)==NULL)
	{
		sprintf(errmsg,"can't strtok: %s use white: %s\n",buf,WHITE);
		return -1;
	}
	argv[argc=0]= buf;
	while((ptr= strtok(NULL,WHITE))!=NULL){
		if(++argc>MAXARGC-1)
			return -1;
		argv[argc]=ptr;
	}


	argv[++argc]=NULL;

	return ((*func)(argc,argv));

}

int cli_args(int argc,char **argv)
{
	if(argc!=3||strcmp(argv[0],CL_OPEN)!=0){
		strcpy(errmsg,"usage:<pathname>,<oflag>\n");
		return -1;
	}

	pathname=argv[1];
	oflag=atoi(argv[2]);
	return (0);


}

