#include<stdio.h>
#include<sys/time.h>	/* for time functions */
#include<unistd.h>
#include<stdlib.h>
#include"acceleration.h"
#include<math.h>
#include<parallel/parallel-pthread.h>

#define N 160 /* Nr of lines/columns of matrix */
#define P 4	/* Nr of threads */
#define POW 4	/* the power of matrix */
double **a,**b,**c; /* matrix */
double *pa,*pb,*pc; /* pointers to matrix */
double **mattemp,**operand,**mtemp,**moutput;
double *x,*y;
double *pmattemp;
double cs[N][N];

int main(int argc,char **argv)
{
	struct timeval t1s,t2s,t1p,t2p,t1,t2;
	int i,j,k,l;
	int ord;
	FILE *outs1,*outpch,*outpc,*outpr;
	if((outs1=(FILE *)fopen("outs1.dat","w"))==(FILE *)NULL)
	{
		perror("error open outs1");
		exit(-1);
	}
	if((outpch=(FILE *)fopen("outpch.dat","w"))==(FILE *)NULL)
	{
		perror("error open outpch");
		exit(-1);
	}
	if((outpc=(FILE *)fopen("outpc.dat","w"))==(FILE *)NULL)
	{
		perror("error open outpc");
		exit(-1);
	}
	if((outpr=(FILE *)fopen("outpr.dat","w"))==(FILE *)NULL)
	{
		perror("error open outpr");
		exit(-1);
	}
	a=(double **)calloc(N,sizeof(double *));
	pa=(double *)calloc(N*N,sizeof(double));
	for(j=0;j<N;j++)
	{
		a[j]=pa;
		pa+=N;
	}
	b=(double **)calloc(N,sizeof(double *));
	pb=(double *)calloc(N*N,sizeof(double));
	for(j=0;j<N;j++)
	{
		b[j]=pb;
		pb+=N;
	}
	c=(double **)calloc(N,sizeof(double *));
	pc=(double *)calloc(N*N,sizeof(double));
	for(j=0;j<N;j++)
	{
		c[j]=pc;
		pc+=N;
	}	
	mattemp=(double **)calloc(N,sizeof(double *));
	pmattemp=(double *)calloc(N*N,sizeof(double));
	for(j=0;j<N;j++)
	{
		mattemp[j]=pmattemp;
		pmattemp+=N;
	}	
	for(i=0;i<N;i++)
	for(j=0;j<N;j++)
	{
		a[i][j]=1.0;
		b[i][j]=1.0;
	}
	x=(double *)calloc(N,sizeof(double));
	y=(double *)calloc(N,sizeof(double));
	for(i=0;i<N;i++)
		x[i]=1.0f;

	/*
		multiplicity secventials for computing acceleration factor
	*/

	gettimeofday(&t1s,NULL);
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			cs[i][j]=0.0;
			for(k=0;k<N;k++)
				cs[i][j]+=a[i][k]*b[k][j];
		}
	gettimeofday(&t2s,NULL);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outs1,"%4.0f ",cs[i][j]);
		fprintf(outs1,"\n");
	}
	fclose(outs1);

	/* apelez rutina pe careuri in pth*/
	for(i=0;i<N;i++)
	for(j=0;j<N;j++)
		c[i][j]=0.0;
	gettimeofday(&t1p,NULL);
	mult_sq_mat_check(N,P,a,b,c);
	gettimeofday(&t2p,NULL);
	acceleration(t1s,t2s,t1p,t2p,P);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outpch,"%4.0f ",c[i][j]);
			fprintf(outpch,"\n");
	}
	fclose(outpch);
	/* apelez rutina pe rinduri in pth*/
	for(i=0;i<N;i++)
	for(j=0;j<N;j++)
		c[i][j]=0.0;
	gettimeofday(&t1p,NULL);
	mult_sq_mat_striped(N,P,a,b,c,1);
	gettimeofday(&t2p,NULL);
	acceleration(t1s,t2s,t1p,t2p,P);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outpr,"%4.0f ",c[i][j]);
			fprintf(outpr,"\n");
	}
	fclose(outpr);
	/* apelez rutina pe coloane in pth*/
	for(i=0;i<N;i++)
	for(j=0;j<N;j++)
		c[i][j]=0.0;
	gettimeofday(&t1p,NULL);
	mult_sq_mat_striped(N,P,a,b,c,0);
	gettimeofday(&t2p,NULL);
	acceleration(t1s,t2s,t1p,t2p,P);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outpc,"%4.0f ",c[i][j]);
			fprintf(outpc,"\n");
	}
	fclose(outpc);
	free(*a);
	free(a);
	free(*b);
	free(b);
	free(*c);
	free(c);
	exit(0);
}
