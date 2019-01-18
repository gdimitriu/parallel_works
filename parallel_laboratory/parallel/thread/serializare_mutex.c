/*
	mutex-uri
*/
#include<st.h>
#include<stdio.h>
#include<sys/wait.h>
#define NR 6
FILE *file;
st_mutex_t mt[NR]; //vector de mutex-uri
void thread_file(int nr,st_join_counter_t *joinc)
{
	st_mutex_lock(&mt[nr]);
	ST_POLLING();
	sleep(NR-nr);
	fprintf(file,"%d\n",nr);fflush(file);
	printf("%d\n",nr);fflush(stdout);	
	st_mutex_unlock(&mt[nr]);
	if(nr!=NR-1) st_mutex_unlock(&mt[nr+1]);
	st_join_counter_finish(joinc);
}

int st_main()
{
int i;
int thnr[NR];		//vector de nr de ordine thread
st_join_counter_t joinc[NR];
	file=(FILE *)fopen("test.txt","w");
	for(i=0;i<NR;i++) st_mutex_init(&mt[i]);
	for(i=0;i<NR;i++) st_mutex_lock(&mt[i]);
	st_join_counter_init(joinc,NR);
	for(i=0;i<NR;i++) 
	{
		if(i==0) st_mutex_unlock(&mt[0]);
		thnr[i]=i;
		ST_THREAD_CREATE(thread_file(thnr[i],joinc));
		ST_POLLING();
	}
	st_join_counter_wait(joinc);
	for(i=0;i<NR;i++)
		st_mutex_destroy(&mt[i]);
	fclose(file);
}
