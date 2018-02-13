#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<sys/time.h>
#include"timeprint.c"
#include"omp_gj_inv.c"
#define numar 10
int main(int argc,char **argv)
{
	struct timeval t1,t2;
	long i,l,j,k;
	long dim;
	int thread;
	FILE *fp;
	double **mat,*temp1,temp,**mat1,**inv;
	dim=atol(argv[1]);
	thread=atol(argv[2]);
	mat=(double **)calloc(dim,sizeof(double *));
	temp1=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		mat[i]=temp1;
		temp1+=dim;
	}
	mat1=(double **)calloc(dim,sizeof(double *));
	temp1=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		mat1[i]=temp1;
		temp1+=dim;
	}
	inv=(double **)calloc(dim,sizeof(double *));
	temp1=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		inv[i]=temp1;
		temp1+=dim;
	}
	for(i=0;i<dim;i++)
	{
		for(j=0;j<dim;j++) mat[i][j]=(double)rand();
		temp=0.0;
		for(j=0;j<dim;j++) if(j!=i) temp+=abs(mat[i][j]);
		mat[i][i]+=temp;
	}
	gettimeofday(&t1,NULL);
	for(l=0;l<numar;l++)
	{
#pragma omp parallel for private(j)
		for(i=0;i<dim;i++)
			for(j=0;j<dim;j++) mat1[i][j]=mat[i][j];
		omp_gj_inv(mat1,inv,dim,thread);
	}
	gettimeofday(&t2,NULL);
	for(i=0;i<dim;i++)
		for(j=0;j<dim;j++)
		{
			mat1[i][j]=0.0;
			for(k=0;k<dim;k++) mat1[i][j]+=mat[i][k]*inv[k][j];
		}
	for(i=0;i<dim;i++) 
		if(fabs(mat1[i][i]-1.0)>1e-6)
		{
		 printf("error at %d %lf",i,mat1[i][i]);
		 fflush(stdout);
		 }
	fp=fopen("time-par.dat","a");
	timeprint(t1,t2,numar,dim,fp,thread);
	fclose(fp);
	free(*mat);
	free(mat);
	free(*mat1);
	free(mat1);
	free(*inv);
	free(inv);
	return 0;
}
