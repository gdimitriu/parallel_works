#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/time.h>
#include"solve-omp.h"
#include"speedup-b.c"
#define numar 10
#define PRECIZIE 1E-5
int main(int argc,char **argv)
{
	struct timeval t1s[numar],t2s[numar],t1p[numar],t2p[numar];
	long i,l,j;
	long dim;
	int thread;
	double **mat,*x,*rez,*temp1,temp,*y;
	dim=atol(argv[1]);
	thread=atoi(argv[2]);
	mat=(double **)calloc(dim,sizeof(double *));
	temp1=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		mat[i]=temp1;
		temp1+=dim;
	}
	x=(double *)calloc(dim,sizeof(double));
	y=(double *)calloc(dim,sizeof(double));
	rez=(double *)calloc(dim,sizeof(double));
	for(i=0;i<dim;i++) rez[i]=(double)i;
	for(l=0;l<numar;l++)
	{
		for(i=0;i<dim;i++) for(j=0;j<dim;j++) mat[i][j]=0.0;
		for(i=0;i<dim;i++)
		{
			for(j=0;j<dim;j++) mat[i][j]=(double)rand();
			temp=0.0;
			for(j=0;j<dim;j++) if(j!=i) temp+=abs(mat[i][j]);
			mat[i][i]+=temp;
		}
		for(i=0;i<dim;i++)
		{
			y[i]=0.0;
			x[i]=0.0;
			for(j=0;j<dim;j++) y[i]+=mat[i][j]*rez[j];
		}
		gettimeofday(&t1s[l],NULL);
		jacobi_serial(mat,x,y,dim,PRECIZIE);
		gettimeofday(&t2s[l],NULL);
		for(i=0;i<dim;i++) {if(abs(rez[i]-x[i])>PRECIZIE) printf("ser %lf=%lf\n",rez[i],x[i]); fflush(stdout);}
		//d'ici c'est la zone parallele

		for(i=0;i<dim;i++) for(j=0;j<dim;j++) mat[i][j]=0.0;
		for(i=0;i<dim;i++)
		{
			for(j=0;j<dim;j++) mat[i][j]=(double)rand();
			temp=0.0;
			for(j=0;j<dim;j++) if(j!=i) temp+=abs(mat[i][j]);
			mat[i][i]+=temp;
		}
		for(i=0;i<dim;i++)
		{
			y[i]=0.0;
			x[i]=0.0;
			for(j=0;j<dim;j++) y[i]+=mat[i][j]*rez[j];
		}
		gettimeofday(&t1p[l],NULL);
		jacobi_fast_omp(dim,thread,mat,x,y,PRECIZIE);
		gettimeofday(&t2p[l],NULL);
		for(i=0;i<dim;i++) {if(abs(rez[i]-x[i])>PRECIZIE) printf("par %lf=%lf\n",rez[i],x[i]); fflush(stdout);}
		printf("a trecut de ciclul %ld\n",l);fflush(stdout);
	}
	speedup(t1s,t2s,t1p,t2p,thread,numar,dim);
	free(x);
	free(rez);
	free(y);
	return 0;
}
