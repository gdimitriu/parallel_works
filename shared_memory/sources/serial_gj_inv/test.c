#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include"timeprint.c"
#define numar 10
int main(int argc,char **argv)
{
	struct timeval t1,t2;
	long i,l,j;
	FILE *fp;
	printf("first=%d ",gettimeofday(&t1,NULL));
	sleep(6000);
	printf("second=%d\n",gettimeofday(&t2,NULL));
	fp=fopen("test.dat","a");
	timeprint(t1,t2,1,1,fp,1);
	fclose(fp);
	return 0;
}
