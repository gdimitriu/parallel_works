#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
int gaussJ_serial(double **mat,double *x,double *y,long dim,long thread)
{
	long k,i,j;
	double temp;
	
	for(k=0;k<dim;k++)
	{
		for(i=0;i<dim;i++)
		{
			if(i!=k)
			{
				temp=mat[i][k]/mat[k][k];
				for(j=k+1;j<dim;j++) mat[i][j]-=temp*mat[k][j];
				y[i]-=temp*y[k];
			}
		}
		for(j=0;j<dim;j++) if(j!=k) mat[j][k]=0.0;
	}
	for(i=0;i<dim;i++) x[i]=y[i]/mat[i][i];
	return 0;
}
