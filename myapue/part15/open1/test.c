#include <stdio.h>


int test(int r,int s)
{
	int u;

	printf("%d,%d\n",r,s);

	if(r<=0|| s<=0)
		u=r+s;
	else if(r>s)
		u=test(r-3,s-2);
	else 
		u=test(r-2,s-3);

	printf("%d\n",u);

	return u;


		
}

int main(void)
{
	test(7,7);
	return ;
}
