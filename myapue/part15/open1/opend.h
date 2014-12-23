#include <sys/types.h>
#include <errno.h>
#include "ourhdr.h"
#include "my_log.h"

#define CL_OPEN "open"
#define CS_OPEN "/home/tlinux/open"

extern int debug;
extern char errmsg[];
extern oflag;
extern char *pathname;
typedef struct{
	int fd;
	uid_t uid;

} Client;

extern Client * client;

extern int client_size;

int cli_args(int ,char **);
int client_add(int ,uid_t);
void loop(void);
void request(char *,int ,int , uid_t);


