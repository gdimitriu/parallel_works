#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include"temps1.c"
#define dim 1000
#define numar 10
int main()
{
	struct timeval t1,t2;
	int i,j,k,l;
	double temp,mat[dim][dim+1],x[dim],rez[dim];
	
	for(i=0;i<dim;i++) rez[i]=(double)i;
	for(l=0;l<numar;l++)
	{
		for(i=0;i<dim;i++)
		{
			for(j=0;j<dim;j++) mat[i][j]=(double)rand();
			temp=0.0;
			for(j=0;j<dim;j++) if(j!=i) temp+=abs(mat[i][j]);
			mat[i][i]+=temp;
		}
		for(i=0;i<dim;i++)
		{
			mat[i][dim]=0.0;
			for(j=0;j<dim;j++) mat[i][dim]+=mat[i][j]*rez[j];
		}
		gettimeofday(&t1,NULL);
		for(k=0;k<dim-1;k++)
		for(i=k+1;i<dim;i++)
		{
			temp=mat[i][k]/mat[k][k];
			for(j=k+1;j<dim;j++) mat[i][j]-=temp*mat[k][j];
			mat[i][dim]-=temp*mat[k][dim];
		}
		for(i=dim-1;i>=0;i--)
		{
			temp=0.0;
			x[i]=0.0;
			for(j=i+1;j<dim;j++) temp+=mat[i][j]*x[j];
			x[i]=(mat[i][dim]-temp)/mat[i][i];
		}
		gettimeofday(&t2,NULL);
		temps1(t1,t2);
//		for(i=0;i<dim;i++) printf("%lf=%lf\n",rez[i],x[i]);
	}
}
