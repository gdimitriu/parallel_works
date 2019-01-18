#include<pthread.h>
#include<stdio.h>
int flag=0;
void *function(void *r)
{
	if(*(int *)r==1)
	{
		while(flag==0){}
		printf("je suis le thread nr %d\n",*(int *)r);fflush(stdout);
	}
	else
	{
		if(*(int *)r==2) sleep(10);
		printf("je suis le thread nr %d\n",*(int *)r);fflush(stdout);	
	}
}
int main()
{
int i;
pthread_t pth[3];
int data[3];
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
		if(i==0) flag=1;
	}
	for(i=0;i<3;i++)
		pthread_join(pth[i],NULL);
}