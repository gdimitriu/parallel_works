/* Fibonacii function implemente in stthreads */
#include<st.h>
#include<st_foreign.h>
long t0,t1,t2,t3;
int pr,sr;
st_join_counter_t c[1];
long pfib_time,sfib_time;
int n;
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
void pfib(int n,int *r,st_join_counter_t *c)
{
	if(n<2)
	{
		*r=1;									//ecrire le resoudre
		st_join_counter_finish(c); 	//dire je suis termine
	} else
	{
		int a,b;
		st_join_counter_t cc[1];
		ST_POLLING();			//verifie si quelqun il veux travaille et elibere stack
		st_join_counter_init(cc,2);	//initialize join counter
		ST_THREAD_CREATE(pfib(n-1,&a,cc));	//fork fib(n-1,...)
		pfib(n-2,&b,cc);							//fork fib(n-2,...)
		st_join_counter_wait(cc);	//attende de fini le copain
		*r=a+b;		//ecrire le resoudre
		st_join_counter_finish(c);		//fire je suis termine
		ST_POLLING();			//verifie si quelqun il veux travaille et eliber stack
	}
}
void *parallel(void *a0,void *a1,void *a2,void *a3)
{
		t0=st_current_time_ms();
		st_join_counter_init(c,1);
		pfib(n,&pr,c);
		st_join_counter_wait(c);
		t1=st_current_time_ms();
		pfib_time=t1-t0;
}
//int st_main(){}
int main(void)
{
	n=33;
	// fait  le parallel fib
	stf_create_sync_worker_group(0,2,parallel,(void *)NULL,(void *)NULL,(void *)NULL,(void *)NULL);

	// fait le sequencial fib
	t0=st_current_time_ms();
	sr=fib(n);
	t1=st_current_time_ms();
	sfib_time=t1-t0;
	if(sr!=pr)
		fprintf(st_errout,"erreur\n");
	printf("pfib:%d ms on %d processors,sfib: %d ms\n",pfib_time,st_n_toplevel_workers(),sfib_time);
	return 0;
}