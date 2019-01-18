#include<stdlib.h>
#include<stdio.h>
//#include"timeprint.c"
#include"rotation.c"
int main(int argc, char **argv)
{
	double **mat;
	double *pmat;
	double *spectre;
	long i,j;
	long dim;
	double **T;
	double norm,q;
	double error;
	dim=atol(argv[1]);
	error=atof(argv[2]);
	mat=(double **)calloc(dim,sizeof(double *));
	pmat=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		mat[i]=pmat;
		pmat+=dim;
	}
	T=(double **)calloc(dim,sizeof(double *));
	pmat=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		T[i]=pmat;
		pmat+=dim;
	}
	spectre=(double *)calloc(dim,sizeof(double));
	//generate the symetrical matrix
	for(i=0;i<dim;i++)
	{
		for(j=0;j<i;j++)
		{
				mat[i][j]=20000*rand()/(double)RAND_MAX;
				if((rand()/(double)RAND_MAX)<0.5) mat[i][j]=-mat[i][j];
				mat[j][i]=mat[i][j];
		}
		mat[i][i]=20000*rand()/(double)RAND_MAX;
		if((rand()/(double)RAND_MAX)<0.5) mat[i][i]=-mat[i][i];
	}
	rotation(mat,dim,spectre,error);
	free(*mat);
	free(mat);
	free(*T);
	free(T);
	return 0;
}