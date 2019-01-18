#include<pth.h>
#include<stdio.h>
int flag=0;
void *function(void *r)
{
	if(*(int *)r==1)
	{
		while(flag==0){pth_yield(NULL);}
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
pth_t pth[3];
pth_attr_t attr;
int data[3];
	pth_init();
	printf("test\n");fflush(stdout);
	attr=pth_attr_new();
	for(i=0;i<3;i++)
	{
		data[i]=i;
		pth[i]=pth_spawn(attr,function,&data[i]);
	}
	printf("les threads sont cree\n");fflush(stdout);
	while(flag==0)
	{	
		scanf("%d",&i);fflush(stdin);
		if(i==0) flag=1;
		pth_yield(NULL);
	}
	for(i=0;i<3;i++)
		pth_join(pth[i],NULL);
	pth_kill();
}