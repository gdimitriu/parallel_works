#include<stdio.h>
#include<st.h>
#include<math.h>
#include<unistd.h>
int test;
st_mutex_t mutex[1];

int thread(int *temp,int *flag,int *flag1)
{
	while(*flag==0)
	{
		if(*flag1==0)
		{
			*temp=(int)rand();
			test++;
			st_mutex_lock(mutex);	
			*flag1=1;
			st_mutex_unlock(mutex);
		}
		else
		{
			ST_POLLING();
			usleep(10);
		}
	}
}
int st_main(void)
{
int i,temp,flag,flag1,j;
	i=1;
	flag=0;
	flag1=0;
	test=0;
	st_mutex_init(mutex);
	ST_THREAD_CREATE(thread(&temp,&flag,&flag1));
	while(i<10)
	{
		if(flag1==1)
		{
			st_mutex_lock(mutex);
			flag1=0;
			st_mutex_unlock(mutex);
			printf("temp=%d %d %d\n",temp,i,test);fflush(stdout);
			i++;
		}
		else
		{
			ST_POLLING();
			usleep(10);
		}
	}	
	flag=1;
	st_mutex_destroy(mutex);
}