#include<stdio.h>
#include<stdlib.h>
#include<pth.h>
pth_t pt[2];
void *test_th(void *intput)
{
pth_t pths;
	pth_write(0,"in\n",3*sizeof(char));fflush(stdout);
	pth_suspend(pt[0]);
	printf("thread out\n");fflush(stdout);
}

void *test_dummy(void *intput)
{
pth_write(0,"dummy\n",6*sizeof(char));fflush(stdout);
pth_sleep(2);
printf("out dummy\n");fflush(stdout);
}

int main(void)
{

pth_attr_t attr;
int i;
	pth_init();
	attr=pth_attr_new();
	pth_attr_set(attr,PTH_ATTR_JOINABLE,TRUE);
	pt[0]=pth_spawn(attr,test_th,(void *)NULL);
	pt[1]=pth_spawn(attr,test_dummy,(void *)NULL);
	printf("Input data\n");fflush(stdout);
	pth_yield(pt[0]);
	pth_yield(pt[1]);
	i=10;
	while(i!=0)
	{
		scanf("%d",&i);
		fflush(stdin);
	}
	pth_resume(pt[0]);
	for(i=0;i<2;i++)
		pth_join(pt[i],NULL);
	pth_kill();
	exit(0);
}