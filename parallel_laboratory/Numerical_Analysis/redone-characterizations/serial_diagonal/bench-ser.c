#include<stdlib.h>
#include<stdio.h>
#include<math.h>
//#include"timeprint.c"
#include "diag.c"
int main(int argc, char **argv)
{
	double **mat;
	double *pmat;
	long i,j;
	long dim;
	double *x,*y,*rez;
	FILE *fp;
	//dim=atol(argv[1]);
	scanf("%ld",&dim);
	mat=(double **)calloc(dim,sizeof(double *));
	pmat=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		mat[i]=pmat;
		pmat+=dim;
	}
	rez=(double *)calloc(dim,sizeof(double));
	x=(double *)calloc(dim,sizeof(double));
	y=(double *)calloc(dim,sizeof(double));
	for(i=0;i<dim;i++) rez[i]=i+1.0;
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
	//generate the free term
	for(i=0;i<dim;i++)
	{
		y[i]=0.0;
		x[i]=0.0;
		for(j=0;j<dim;j++) y[i]+=mat[i][j]*rez[j];
	}
	diag(mat,y,x,dim);
	for(i=0;i<dim;i++) {if(fabs(rez[i]-x[i])>1E-5) printf("%lf=%lf\n",rez[i],x[i]); fflush(stdout);}
	fp=fopen("time-ser.dat","a");
	fclose(fp);
	free(*mat);
	free(mat);
	return 0;
}