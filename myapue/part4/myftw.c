#include "apue.h"
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errmethod.h>
#include "pathalloc.c"
typedef int Myfunc(char *,const struct stat *,int);
static Myfunc func;
static int myfwt(const char *,Myfunc *);
static int dopath(Myfunc *);

static long ntot,nreg,nblk,nchr,nfifo,nlnk,nsock,ndir;

int main(int argc ,char **argv)
{
	

	if(argc!=2)
		err_quit("usage:ftw <pathname>");


	if(myfwt(argv[1],func)<0)
		err_sys("myfwt error");

	if((ntot=nreg+nblk+nchr+nfifo+nlnk+nsock+ndir)==0)
		ntot=1;

	printf("regular files=:%7ld,  %5.2f %%\n",nreg,nreg*100.0/ntot);
	printf("block files=:  %7ld,  %5.2f %%\n",nblk,nblk*100.0/ntot);
	printf("char files=:   %7ld,  %5.2f %%\n",nchr,nchr*100.0/ntot);
	printf("fifo files=:   %7ld,  %5.2f %%\n",nfifo,nfifo*100.0/ntot);
	printf("socket files=: %7ld,  %5.2f %%\n",nsock,nsock*100.0/ntot);
	printf("link files=:   %7ld,  %5.2f %%\n",nlnk,nlnk*100.0/ntot);
	printf("dir files=:    %7ld,  %5.2f %%\n",ndir,ndir*100.0/ntot);
	printf("total :        %7ld    ",ntot);

}

static char *fullpath;

static int myfwt(const char *pathname,Myfunc *func){
	
	fullpath=path_alloc(NULL);
	strcpy(fullpath,pathname);

	return (dopath(func));

}

#define FTW_NS 0
#define FTW_F  1
#define FTW_D  2
#define FTW_DNR 3

static int dopath(Myfunc *func)
{
	int ret;
	DIR *dp;
	struct dirent *dirent;
	char *ptr;

	struct stat buf;
	if(lstat(fullpath,&buf)<0)
		return func(fullpath,&buf,FTW_NS);
	if(S_ISDIR(buf.st_mode)==0)
		return func(fullpath,&buf,FTW_F);
	if((ret=func(fullpath,&buf,FTW_D))!=0)
		return ret;
	ptr=fullpath+strlen(fullpath);
	*ptr++='/';
	*ptr=0;


	if((dp=opendir(fullpath))==NULL)
		return func(fullpath,&buf,FTW_DNR);
	while((dirent=readdir(dp))!=NULL){
		if(strcmp(dirent->d_name,".")==0||
				strcmp(dirent->d_name,"..")==0)
			continue;
		//修改fullpath
		strcpy(ptr,dirent->d_name);
		if((ret=dopath(func))!=0)
			break;

	}
	ptr[-1]=0;
	if(closedir(dp)!=0)
		err_ret("can't clost dir %s",fullpath);
	return ret;
}



static int func(char *pathname,const struct stat *buf,int type)
{
	switch(type){
		case FTW_F:
			switch(buf->st_mode&S_IFMT){
				case S_IFREG:
					nreg++;
					break;
				case S_IFBLK:
					nblk++;
					break;
				case S_IFCHR:
					nchr++;
					break;
				case S_IFIFO:
					nfifo++;
					break;
				case S_IFLNK:
					nlnk++;
					break;
				case S_IFSOCK:
					nsock++;
					break;
				case S_IFDIR:
					err_dump("for S_IFDIR for %s\n",pathname);

			}

			break;

		case  FTW_NS:
			err_ret("stat error for %s\n",pathname);
			break;
		case FTW_DNR:
			err_ret("not open the file %s\n",pathname);
			break;
		case FTW_D:
			ndir++;
			break;
		default:
			err_dump("unknow type %d pathname %s\n" ,type,pathname);


	}

	return 0;


}

