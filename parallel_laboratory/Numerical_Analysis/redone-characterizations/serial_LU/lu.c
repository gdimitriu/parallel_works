#include<math.h>
#include<stdlib.h>
void lu(double **mat,double *ty,double *tx,long dim)
{
	long i,j,k;
	double **b,*y;
	double *tmat,temp;
	b=(double **)calloc(dim,sizeof(double *));
	tmat=(double *)calloc(dim*dim,sizeof(double));
	y=(double *)calloc(dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		b[i]=tmat;
		tmat+=dim;
	}
	//factorization of the matrix
	b[0][0]=sqrt(mat[0][0]);
	for(i=1;i<dim;i++)
		b[i][0]=mat[i][0]/b[0][0];
	for(j=1;j<dim;j++)
	{
		temp=0.0;
		for(i=0;i<j;i++) temp+=b[j][i]*b[j][i];
		b[j][j]=sqrt(mat[j][j]-temp);
		for(i=j+1;i<dim;i++)
		{
			temp=0.0;
			for(k=0;k<j;k++) temp+=b[i][k]*b[j][k];
			b[i][j]=(mat[i][j]-temp)/b[j][j];
		}
	}
	//find the solution for by=a
	y[0]=ty[0]/b[0][0];
	for(i=1;i<dim;i++)
	{
		temp=0.0;
		for(k=0;k<i;k++) temp+=b[i][k]*ty[k];
		y[i]=(ty[i]-temp)/b[i][i];
	}
	//find the solution for b*x=y
	tx[dim-1]=y[dim-1]/b[dim-1][dim-1];
	for(i=dim-2;i>=0;i--)
	{
		temp=0.0;
		for(j=i+1;j<dim;j++) temp+=b[j][i]*tx[j];
		tx[i]=(y[i]-temp)/b[i][i];
	}
	//free the memory
	free(*b);
	free(b);
	free(y);
}