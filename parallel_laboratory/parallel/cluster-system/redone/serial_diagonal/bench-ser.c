#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<sys/time.h>
#include"timeprint.c"
#include "diag.c" 
#define numar 10
int main(int argc, char **argv)
{
	double **mat;
	double *pmat;
	long i,j;
	long dim;
	double *x,*y,*rez;
	FILE *fp;
	struct timeval t1,t2;
	dim=atol(argv[1]);
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
	for(i=0;i<dim;i++) rez[i]=(double)i+1.0;
	//generate the diagonal matrix
	for(i=0;i<dim;i++)
	{
		for(j=0;j<i;j++) mat[i][j]=0.0;
		for(j=i;j<dim;j++)
		{
//				mat[i][j]=2000.0*((double)rand()/(double)RAND_MAX);
				mat[i][j]=(double)rand();
				if((rand()/(double)RAND_MAX)<0.5) mat[i][j]=-mat[i][j];
		}
	}
	//generate the free term
	for(i=0;i<dim;i++)
	{
		y[i]=0.0;
		x[i]=0.0;
		for(j=i;j<dim;j++) y[i]+=mat[i][j]*rez[j];
	}
	gettimeofday(&t1,NULL);
	for(i=0;i<numar;i++)
		diag(mat,y,x,dim);
	gettimeofday(&t2,NULL);
	for(i=0;i<dim;i++) {if(fabs(rez[i]-x[i])>1E-5) printf("%lf=%lf\n",rez[i],x[i]); fflush(stdout);}
	fp=fopen("time-ser.dat","a");
	timeprint(t1,t2,numar,dim,fp,1);
	fclose(fp);
	free(*mat);
	free(mat);
	return 0;
}