#include "ourhdr.h"

#define CLI_OPEN "open"


/**
 * 要做的操作 
 * 1 打开管道 
 * 2 关闭在子进程总将fd[1]复制为STDIN_FILENO和STDOUT_FILENO
 * 3 在子进程中execl opend 服务程序
 * 4 在父进程中发送参数
 * 5 返回接收到的描述符返回 
 */
int csopen(char * name,int oflag)
{
	static int fd[2]={-1,-1};
	pid_t pid;
	struct iovec iov[3];
	char buf[10];
	int len;

	if(fd[0]<0)
	{

		if(s_pipe(fd)==-1){
			err_sys("s_pipe error");
				return -1;
		}
	
		if((pid=fork())<0){
			err_sys("fork error");
		}else if(pid==0) //子进程
		{
			//
			close(fd[0]);
			if(fd[1]!=STDIN_FILENO){
				if(dup2(fd[1],STDIN_FILENO)!=STDIN_FILENO){
					err_sys("dup2 error for stdin");
				}
			}
			if(fd[1]!=STDOUT_FILENO){
				if(dup2(fd[1],STDOUT_FILENO)!=STDOUT_FILENO){
					err_sys("dup2 error for stdout");
				}	
			}

			execl("./opend","opend",NULL);
		}	

		close(fd[1]);

	}

		//父进程发送数据
		//通过writev 比较方便组拼装数据

		sprintf(buf," %d",oflag);
		iov[0].iov_base=CLI_OPEN " ";
		iov[0].iov_len=strlen(CLI_OPEN)+1;
		iov[1].iov_base=name;
		iov[1].iov_len=strlen(name);
		iov[2].iov_base=buf;
		iov[2].iov_len=strlen(buf)+1;

		len=iov[0].iov_len+iov[1].iov_len+iov[2].iov_len;


		if(writev(fd[0],iov,3)!=len){
			err_sys("writev error");
		}

		return (recv_fd(fd[0],write));

}
