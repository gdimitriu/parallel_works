/*
	This is the function who solve a system of linear equation using the inverse with
	Ritz method.
*/
#include<stdlib.h>
#include<memory.h>
void solve_ritz(double **tmat,double *ty,double *tx,long dim)
{
	double *x,*y;
	double temp;
	double **H;
	double *pmat;
	long i,j,k;
	long count;
	double scalar;
	x=(double *)calloc(dim,sizeof(double));
	y=(double *)calloc(dim,sizeof(double));
	H=(double **)calloc(dim,sizeof(double *));
	pmat=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		H[i]=pmat;
		pmat+=dim;
	}
	count=0;
	while(count<dim)
	{
		//make the y for inverse
		for(i=0;i<dim;i++) y[i]=0.1+(double)20000*rand()/(double)RAND_MAX;
		//make the x for inverse
		if(count==0) memcpy(y,x,dim*sizeof(double));
		else
		{
			for(i=0;i<dim;i++)
			{
				x[i]=y[i];
				for(j=0;j<dim;j++)
				{
					for(k=0;k<dim;k++)
						temp+=H[i][k]*tmat[k][j];
					x[i]-=temp*y[j];
				}
			}
		}
		//make the scalar product
		scalar=0.0;
		for(i=0;i<dim;i++)
		{
			temp=0.0;
			for(j=0;j<dim;j++)
			{
				temp+=tmat[i][j]*x[j];
			}
			scalar+=temp*x[i];
		}
		for(i=0;i<dim;i++)
			for(j=0;j<dim;j++)
				H[i][j]+=x[i]*x[j]/scalar;
		count++;
	}
	for(i=0;i<dim;i++)
	{
		tx[i]=0.0;
		for(j=0;j<dim;j++)
			tx[i]+=H[i][j]*ty[j];
	}
}