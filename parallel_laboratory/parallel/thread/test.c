#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include<unistd.h>
int test;
pthread_mutex_t mutex;
typedef struct
{
	int *temp;
	int *flag;
	int *flag1;
} datas;

void *thread(void *input)
{
datas data=*(datas *)input;
	while(*data.flag==0)
	{
		if(*data.flag1==0)
		{
			*data.temp=(int)rand();
			test++;
			pthread_mutex_lock(&mutex);	
			*data.flag1=1;
			pthread_mutex_unlock(&mutex);
		}
		else
		{
//			usleep(10);
		}
	}
}
int main(void)
{
pthread_t pth;
datas data;
int i,temp,flag,flag1,j;
	i=1;
	flag=0;
	flag1=0;
	test=0;
	data.flag=&flag;
	data.temp=&temp;
	data.flag1=&flag1;
	pthread_mutex_init(&mutex,NULL);
	pthread_create(&pth,NULL,thread,&data);
	while(i<10)
	{
		if(flag1==1)
		{
			pthread_mutex_lock(&mutex);
			flag1=0;
			pthread_mutex_unlock(&mutex);
			printf("temp=%d %d %d\n",temp,i,test);fflush(stdout);
			i++;
		}
		else
		{
//			usleep(10);
		}
	}	
	flag=1;
	pthread_mutex_destroy(&mutex);
}