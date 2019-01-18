#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include"temps1.c"
#define dim 1000
#define numar 10
int main()
{
	int i,j,k,count;
	double mata[dim][dim],matb[dim][dim],matc[dim][dim];
	struct timeval t1,t2;
	for(i=0;i<dim;i++)
	for(j=0;j<dim;j++)
	{
		mata[i][j]=rand();
		matb[i][j]=rand();
	}
	for(count=0;count<numar;count++)
	{
		gettimeofday(&t1,NULL);
		for(i=0;i<dim;i++)
		for(j=0;j<dim;j++)
		{
			matc[i][j]=0.0;
			for(k=0;k<dim;k++) matc[i][j]+=mata[i][k]*matb[k][j];
		}
		gettimeofday(&t2,NULL);
		temps1(t1,t2);
	}
}