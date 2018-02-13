#include<math.h>
#include<stdlib.h>
void diag(double **mat,double *ty,double *tx,long dim)
{
	long i,j,k;
	double temp;
	for(i=dim-1;i>=0;i--)
	{
		temp=0.0;
		for(k=i+1;k<dim;k++) temp+=mat[i][k]*tx[k];
		tx[i]=(ty[i]-temp)/mat[i][i];
	}
}