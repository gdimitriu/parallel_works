#include<pthread.h>
#include<stdio.h>
int flag=0;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex;
void *function(void *r)
{
	if(*(int *)r==1)
	{
//		while(flag==0){}
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
		pthread_mutex_unlock(&mutex);
		printf("je suis le thread nr %d\n",*(int *)r);fflush(stdout);
	}
	else
	{
		printf("je suis le thread nr %d\n",*(int *)r);fflush(stdout);	
	}
}
int main()
{
int i;
pthread_t pth[3];
int data[3];
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	printf("test\n");fflush(stdout);
	for(i=0;i<3;i++)
	{
		data[i]=i;
		pthread_create(&pth[i],NULL,function,&data[i]);
	}
	printf("les threads sont cree\n");fflush(stdout);
	while(flag==0)
	{
		scanf("%d",&i);fflush(stdin);
		if(i==0)
		{
			flag=1;
			pthread_mutex_lock(&mutex);
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&mutex);
		}
	}
	for(i=0;i<3;i++)
		pthread_join(pth[i],NULL);
		
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}