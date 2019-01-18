#include<stdio.h>
#include<stdlib.h>
int mul_serial(double **mat,double **mat2,long dim)
{
	long k,i,j;
	double temp;

	for(j=0;j<dim;j++)
	for(k=0;k<dim;k++) mat2[j][k]=mat[j][k];
	for(i=0;i<3;i++)
	{
		for(j=0;j<dim;j++)
		for(k=0;k<dim;k++) mat2[j][k]=mat2[j][k]*mat[j][k];
	}
	return 0;
}
