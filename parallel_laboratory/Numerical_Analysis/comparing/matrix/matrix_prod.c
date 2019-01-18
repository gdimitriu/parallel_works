#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include"temps1.c"
#define dim 1000
int main()
{
	int i,j,count;
	double mata[dim][dim],matb[dim][dim],matc[dim][dim];
	struct timeval t1,t2;
	for(i=0;i<dim;i++)
	for(j=0;j<dim;j++)
	{
		mata[i][j]=(double)rand();
		matb[i][j]=(double)rand();
	}
	for(count=0;count<10;count++)
	{
		gettimeofday(&t1,NULL);
		for(i=0;i<dim;i++)
		for(j=0;j<dim;j++) matc[i][j]=mata[i][j]*matb[i][j];
		gettimeofday(&t2,NULL);
		temps1(t1,t2);
	}
}