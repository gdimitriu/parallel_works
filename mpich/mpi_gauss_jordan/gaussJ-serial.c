#include<stdio.h>
#include<stdlib.h>
int gaussJ_serial(double **mat,double *x,double *y,long dim)
{
	long k,i,j;
	double temp;
	
	for(i=0;i<dim;i++)
	{
		for(k=0;k<dim;k++)
		{
			if(i!=k)
			{
				temp=mat[i][k]/mat[k][k];
				for(j=k+1;j<dim;j++) mat[i][j]-=temp*mat[k][j];
				y[i]-=temp*y[k];
			}
		}
	}
	for(i=0;i<dim;i++) x[i]=y[i]/mat[i][i];
	return 0;
}
