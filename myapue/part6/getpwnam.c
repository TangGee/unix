#include <errmethod.h>
#include <sys/types.h>
#include <pwd.h>
#include <stddef.h>
#include <string.h>

struct passwd *getpwnam(char *name)
{
	struct passwd *ptr;

	setpwent();

	while((ptr=getpwent())!=NULL){
		if(strcmp(name,ptr->pw_name)==0)
			break;
	}

	endpwent();

	return ptr;
}


int main(int argc,char **argv)
{
	struct passwd * pw;
	if(argc!=2)
		err_sys("usage : username");

	pw=getpwnam(argv[1]);

	printf("pw name = %s",pw->pw_name);



}


