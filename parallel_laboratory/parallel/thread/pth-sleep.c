#include<pth.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
#include<timesec.h>


void  *test_th(void *input)
{
	struct timeval tv1,tv2;
	int in;
	int i,j;
	i=0;
	in=*(int *)input;
	for(j=0;j<100000;j++)
	{
		i++;
		if(i==1000)
		{
			printf("th %d %d\n",in,i);fflush(stdout);
			gettimeofday(&tv1,NULL);
			pth_sleep(10);
			gettimeofday(&tv2,NULL);
			timesec(tv1,tv2);
			i++;
			printf("th %d %d\n",in,i);fflush(stdout);					
		}
	}
	printf("th %d %d\n",in,i);fflush(stdout);					
	pth_exit((void *)NULL);

}

void main(void)
{
pth_t pt[2];
pth_attr_t attr;
int temp[2],i;
	pth_init();
	attr=pth_attr_new();
	pth_attr_set(attr,PTH_ATTR_JOINABLE,TRUE);
	temp[0]=1;
	pt[0]=pth_spawn(attr,test_th,&temp[0]);
	temp[1]=2;
	pt[1]=pth_spawn(attr,test_th,&temp[1]);
	for(i=0;i<2;i++)
		pth_join(pt[i],NULL);
}