#include <stdio.h>
#include <errmethod.h>
void pr_stdio(const char *,FILE *);

int main(void)
{
	FILE *fp;

	fputs("enter any character\n",stdout);
	if(getchar()==EOF)
		err_sys("getchar error");

	fputs("one line to standard error\n",stderr);

	pr_stdio("stdin",stdin);
	pr_stdio("stdout",stdout);
	pr_stdio("stderr",stderr);

	if((fp= fopen("/etc/motd","r"))==NULL)
		err_sys("fopen error");
	if(getc(fp)==EOF)
		err_sys("getc error");

	pr_stdio("/etc/motd",fp);
	exit(0);

}

void pr_stdio(const char *name,FILE *fp)
{
	printf("stream = %s ",name);

	if(fp->_flags&_IONBF) printf("  unbuffered");
	if(fp->_flags&_IOLBF) printf("  line buffered");
	else printf("  full buffered");

	printf(", buffer size = %ld\r", fp->_IO_buf_end -fp->_IO_buf_base);
}
