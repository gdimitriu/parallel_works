/* Fibonacii function implemente in stthreads */
#include<st_foreign.h>
#include<fib.h>
#include<stdlib.h>
#include<stdio.h>
int fib(int n)
{
	if(n<2) return 1;
	else 
	{
		int a,b;
		a=fib(n-1);
		b=fib(n-2);
		return a+b;
	}
}
int main(int argc,char *argv[])
{
	n=33;
	// fait  le parallel fib
	stf_create_sync_worker_group(0,2,parallel_wrapper,(void *)NULL,(void *)NULL,(void *)NULL,(void *)NULL);

	// fait le sequencial fib
	t0=st_current_time_ms();
	sr=fib(n);
	t1=st_current_time_ms();
	sfib_time=t1-t0;
	if(sr!=pr)
		perror("erreur\n");
	printf("pfib:%d ms on %d processors,sfib: %d ms\n",pfib_time,st_n_toplevel_workers(),sfib_time);
	return 0;
}