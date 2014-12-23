#include "unpipc.h"

void
Door_call(int fd, door_arg_t *arg)
{
	if (door_call(fd, arg) == -1)
		err_sys("door_call error");
}

