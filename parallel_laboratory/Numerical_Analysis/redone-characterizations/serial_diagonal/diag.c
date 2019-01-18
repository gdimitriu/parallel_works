#include<math.h>
#include<stdlib.h>
void diag(double **mat,double *ty,double *tx,long dim)
{
	long i,j,k;
	double temp;
	//find the solution for by=a
	tx[0]=ty[0]/mat[0][0];
	for(i=1;i<dim;i++)
	{
		temp=0.0;
		for(k=0;k<i;k++) temp+=mat[i][k]*ty[k];
		tx[i]=(ty[i]-temp)/mat[i][i];
	}
}