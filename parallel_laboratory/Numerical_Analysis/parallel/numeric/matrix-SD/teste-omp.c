#include<stdio.h>
#include<sys/time.h>	/* for time functions */
#include<unistd.h>
#include<stdlib.h>
#include"acceleration.h"
#include<math.h>
#include<zlib.h>
#include<parallel/parallel-omp.h>
#define P 9
int N; /* Nr of lines/columns of matrix */
double **a,**b,**c; /* matrix */
double *pa,*pb,*pc; /* pointers to matrix */
double **rez,*prez;	/* matrix resoudre */
double **cs,*pcs;
char numea[128],numeb[128];
gzFile *fp;
double temp;

void readfile(char *nume,double **mat)
{
int i,j;
	fp=(gzFile *)gzopen(nume,"r");
	gzrewind(fp);
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			gzread(fp,&mat[i][j],sizeof(double));
	gzclose(fp);
}
//int st_main(int argc,char **argv)
int main(int argc,char **argv)
{
struct timeval t1s,t2s,t1p,t2p,t1,t2;
int i,j,k,l,m;
int ord;
FILE *outs,*outs1,*outpch_mpi,*outpch_mpi_1,*outpch_mpi_1th;
int size,rank;
int nr;
	if((outs1=(FILE *)fopen("outs1.dat","w"))==(FILE *)NULL)
	{
		perror("error open outs1");
		exit(-1);
	}
	if((outpch_mpi=(FILE *)fopen("outpch_mpi.dat","w"))==(FILE *)NULL)
	{
		perror("error open outpch_mpi");
		exit(-1);
	}
	if((outpch_mpi_1=(FILE *)fopen("outpch_mpi_1.dat","w"))==(FILE *)NULL)
	{
		perror("error open outpch_mpi_1");
		exit(-1);
	}
	if((outpch_mpi_1th=(FILE *)fopen("outpch_mpi_1th.dat","w"))==(FILE *)NULL)
	{
		perror("error open outpch_mpi_th_1th");
		exit(-1);
	}
	for(i=0;i<128;i++)
	{
		numea[i]=0;
		numeb[i]=0;
	}
	printf("Introduceti numele fisierului a:");fflush(stdout);
	gets(numea);
	printf("Introduceti numele fisierului b:");fflush(stdout);
	gets(numeb);
	fp=(gzFile *)gzopen(numea,"r");	
	N=0;
	gzrewind(fp);
	while(!gzeof(fp))
	{
		gzread(fp,&temp,sizeof(double));
		N++;
	}
	N--;
	N=(int)sqrt(N)+1;
	gzclose(fp);
	/* fait allocation pour matrices dans le master */
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
	cs=(double **)calloc(N,sizeof(double *));
	pcs=(double *)calloc(N*N,sizeof(double));
	for(j=0;j<N;j++)
	{
		cs[j]=pcs;
		pcs+=N;
	}
	/*
		multiplicity secventials for computing acceleration factor
	*/
	/* de ce point nous contorize le temps */
	gettimeofday(&t1s,NULL);
	readfile(numea,a);
	readfile(numeb,b);
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
/*--------------------------------
	 POUR MP
--------------------------------*/
	for(i=0;i<N;i++)
   	for(j=0;j<N;j++)
   		cs[i][j]=0.0;
   gettimeofday(&t1p,NULL);
   mult_sq_mat_check_omp(N,P,a,b,cs,0);
	gettimeofday(&t2p,NULL);
   printf("MP\n");fflush(stdout);
   acceleration(t1s,t2s,t1p,t2p,P);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outpch_mpi,"%4.0f ",cs[i][j]);
		fprintf(outpch_mpi,"\n");
	}
	fclose(outpch_mpi);
	free(*cs);
	free(cs);
	free(*b);
	free(b);
	free(*a);
	free(a);
	exit(0);
}
