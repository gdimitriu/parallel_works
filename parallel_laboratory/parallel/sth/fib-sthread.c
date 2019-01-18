/* Fibonacii functionimplemente in stthreads */
#include<st.h>
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
int st_main(int argc,char **argv)
{
	int n=33;
	long pfib_time,sfib_time;
	{
		long t0,t1,t2,t3;
		int pr,sr;
		st_join_counter_t c[1];
	
		// fait  le parallel fib
		t0=st_current_time_ms();
		st_join_counter_init(c,1);
		pfib(n,&pr,c);
		st_join_counter_wait(c);
		t1=st_current_time_ms();
		pfib_time=t1-t0;
	
		// fait le sequencial fib
		t0=st_current_time_ms();
		sr=fib(n);
		t1=st_current_time_ms();
		sfib_time=t1-t0;
		if(sr!=pr)
		{
			fprintf(st_errout,"erreur\n");
//			st_stack_trace_and_die();
		}
		printf("argc=%d argv[0]=%s\n",argc,argv[0]);
	}
	printf("pfib:%d ms on %d processors,sfib: %d ms\n",pfib_time,st_n_toplevel_workers(),sfib_time);
	return 0;
}