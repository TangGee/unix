#ifndef  __ourhdr_h
#define __ourhdr_h

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errmethod.h>

#define FIFO_MODE (S_ISUSE | S_IWUSR | S_IRGRP | S_IROTH)

typedef void Sigfunc(int);


#include "signal.c"



#endif
