#include<st.h>
#include<stdio.h>
#include"barrier-sthread.h"
#define TH 1
int flag=0;
st_cond_t cond[1];
st_mutex_t mutex[1];
st_join_counter_t *counter;
barrier_t barrier;
void function(int r)
{
	if(r==0)
	{
//		while(flag==0){ST_POLLING();}
		st_mutex_lock(mutex);
		st_cond_wait(cond,mutex);
		st_mutex_unlock(mutex);
//		st_mutex_lock(mutex);
//		barrier_sync(&barrier);
		printf("je suis le thread nr %d\n",r);fflush(stdout);
//		st_mutex_unlock(mutex);
	}
	else
	{
		printf("je suis le thread nr %d\n",r);fflush(stdout);	
//		sleep(5);
		printf("j'avais dort %d\n",r);fflush(stdout);
	}
	st_join_counter_finish(counter);
}
int st_main()
{
int i;
	printf("test\n");fflush(stdout);
//	mutex=(st_mutex_t *)calloc(1,sizeof(st_mutex_t));
//	cond=(st_cond_t *)calloc(1,sizeof(st_cond_t));
	counter=(st_join_counter_t *)calloc(3,sizeof(st_join_counter_t));
	barrier_init(&barrier,2);
	st_join_counter_init(counter,TH);
	st_cond_init(cond);
	st_mutex_init(mutex);
//	st_mutex_lock(mutex);
	for(i=0;i<TH;i++)
		ST_THREAD_CREATE(function(i));
	printf("les threads sont cree\n");fflush(stdout);
//	ST_POLLING();
	while(flag==0)
	{
		scanf("%d",&i);fflush(stdin);
		printf("tu a apuie %d\n",i);fflush(stdout);
		if(i==0)
		{
			flag=1;
		st_mutex_lock(mutex);
		st_cond_signal(cond);	
		st_mutex_unlock(mutex);
//		barrier_sync(&barrier);
		}
	}
	st_join_counter_wait(counter);
}