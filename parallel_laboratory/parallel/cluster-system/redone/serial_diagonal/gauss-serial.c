#include<stdio.h>
#include<stdlib.h>
int gauss_serial(double **mat,double *y,double *x,long dim)
{
	int k,i,j;
	double temp;

	for(k=0;k<dim-1;k++)
	for(i=k+1;i<dim;i++)
	{
		temp=mat[i][k]/mat[k][k];
		for(j=k+1;j<dim;j++) mat[i][j]-=temp*mat[k][j];
		y[i]-=temp*y[k];
	}
	
	for(i=dim-1;i>=0;i--)
	{
		temp=0.0;
		x[i]=0.0;
		for(j=i+1;j<dim;j++) temp+=mat[i][j]*x[j];
		x[i]=(y[i]-temp)/mat[i][i];
	} 
	
	return 0;
}
