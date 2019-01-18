#include<stdio.h>
#include<stdlib.h>
int gaussJ_serial(double **mat,double *x,double *y,long dim)
{
	long k,i,j;
	double temp;
	
	for(k=0;k<dim;k++)
	{
		for(i=k+1;i<dim;i++)
		{
			temp=mat[i][k]/mat[k][k];
			for(j=k+1;j<dim;j++) mat[i][j]-=temp*mat[k][j];
			y[i]-=temp*y[k];
		}
		for(i=0;i<k;i++)
		{
			temp=mat[i][k]/mat[k][k];
			for(j=k+1;j<dim;j++) mat[i][j]-=temp*mat[k][j];
			y[i]-=temp*y[k];
		}
	}
	for(i=0;i<dim;i++) x[i]=y[i]/mat[i][i];
	return 0;
}
