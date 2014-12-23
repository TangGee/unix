#include <errno.h>
#include <limits.h>
#ifdef OPEN_MAX
static int openmax = OPEN_MAX;
#else
static int openmax = 0;
#endif
    
#define OPEN_MAX_GUESS 256

int open_max(void){
	if(openmax == 0){
		errno=0;
		if((openmax= sysconf(_SC_OPEN_MAX))<0){
			if(errno==0)
				openmax = OPEN_MAX_GUESS;
			else
				err_sys("sysconf errno for _SC_OPEN_MAX");
		}
	}

	return openmax;
}
