#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int jacobi_serial(double **mat,double *x,double *y,long dim,double eroare)
{
	int k,i,j;
	double temp;
	double *oldx;//old solution
	int flag;
	oldx=(double *)calloc(dim,sizeof(double));
	for(i=0;i<dim;i++) x[i]=y[i];
	flag=0;
	while(flag!=dim)
	{
		flag=0.0;
		for(i=0;i<dim;i++) oldx[i]=x[i];
		for(i=0;i<dim;i++)
		{
			temp=0.0;
			for(j=0;j<dim;j++) if(j!=i) temp=temp+mat[i][j]*oldx[j];
			x[i]=(y[i]-temp)/mat[i][i];
			if(abs(x[i]-oldx[i])<=eroare) flag++;
		}
	}
	free(oldx);
	return 0;
}
