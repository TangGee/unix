#include <termios.h>
#include "ourhdr.h"


int 
main(void)
{
	struct termios term;
	long vdisable;

	if(isatty(STDIN_FILENO)==0)
		err_quit("standard input is not a terminal deviecs");
	if((vdisable=fpathconf(STDIN_FILENO,_PC_VISABLE))<0)
		err_quit("fpathconf error oor _POSIX_VDISABLE no in effect");
	if(tcgetattr(STDIN_FILENO,&term)<0)
		err_sys("tcgetaddr error");
	term.c_cc(VINTR) = vdisable;
	term.c_cc[VEOF] = 2;

	if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&term)<0)
		err_sys("tcsetattr error");

	exit(0);
}
