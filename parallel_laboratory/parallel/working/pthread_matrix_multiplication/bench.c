#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/time.h>
#include"solve-pthread.h"
#include"timeprint.c"
#define numar 10

int main(int argc,char **argv)
{
	struct timeval t1,t2;
	double **mat,*temp1,temp,**mat1;
	long i,l,j;
	long dim;
	int thread;
	FILE *fp,*fp2;
	dim=atol(argv[1]);
	thread=atoi(argv[2]);
	//here is the parallel zone
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
	for(i=0;i<dim;i++)
		for(j=0;j<dim;j++) mat[i][j]=j;
	gettimeofday(&t1,NULL);
	for(l=0;l<numar;l++)
		mul_striped_fast(dim,thread,mat,mat1);
	gettimeofday(&t2,NULL);
	fp=fopen("time-par.dat","a");
	timeprint(t1,t2,numar,dim,fp,thread);
	fclose(fp);
/*	fp2=fopen("data.dat","a");
	for(i=0;i<dim;i++)
	{
		for(j=0;j<dim;j++)
			fprintf(fp2,"%lf",mat1[i][j]);
		fprintf(fp2,"\n");
	}
	fclose(fp2); */
	free(*mat);
	free(mat);		
	free(*mat1);
	free(mat1);
	return 0;
}
