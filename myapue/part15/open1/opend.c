#include <syslog.h>
#include "opend.h"
#include "deamon_init.c"
#include "loop.c"

int debug;
char errmsg[MAXLINE];
int oflag;
char *pathname;
Client *client=NULL;
int client_size;

int 
main(int argc,char **argv)
{
	char c;

	log_open("open.serv",LOG_PID,LOG_USER);

	while((c=getopt(argc,argv,"d"))!=EOF)
	{
		switch(c){
			case 'd':
				debug=1;
				break;
			case '?':
				err_quit("unrecognized option -c%\n",optopt);
		}
	}

	if(debug==0)
		deamon_init();

	loop();
}

