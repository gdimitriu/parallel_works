#include<stdio.h>
#include<sys/time.h>	/* for time functions */
#include<unistd.h>
#include<stdlib.h>
#include"acceleration.h"
#include<math.h>
#include<mpi.h>
#include<parallel/parallel-mpi-omp.h>
//#include<parallel/parallel-mpi-sth.h>
//#include<parallel/parallel-mpi-pth.h>
//#include<parallel/parallel-mpi-pthread.h>
#include<parallel/parallel-mpi.h>

#define N 100 /* Nr of lines/columns of matrix */
#define P 4	/* Nr of threads */
#define POW 4	/* the power of matrix */
double **a,**b,**c; /* matrix */
double *pa,*pb,*pc; /* pointers to matrix */
double **mattemp,**operand,**mtemp,**moutput;
double *x,*y;
double *pmattemp;
double cs[N][N];

void readfile(FILE *fp,double **mat)
{
	int i,j;
	rewind(fp);
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			fscanf(fp,"%lf",&mat[i][j]);
}
//int st_main(int argc,char **argv)
int main(int argc,char **argv)
{
	struct timeval t1s,t2s,t1p,t2p,t1,t2;
	int i,j,k,l;
	int ord;
	FILE *outs,*outs1,*outpch_mpi,*outpch_mpi_th;
	FILE *vects,*vectr;
	int numero,rank;
	//initialize MPI
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numero);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
if(rank==0)
{
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
	if((outpch_mpi_th=(FILE *)fopen("outpch_mpi_th.dat","w"))==(FILE *)NULL)
	{
		perror("error open outpch_mpi_th");
		exit(-1);
	}
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
		a[i][j]=j;
		b[i][j]=j;
	}
	x=(double *)calloc(N,sizeof(double));
	y=(double *)calloc(N,sizeof(double));
	for(i=0;i<N;i++)
		x[i]=1.0f;

if(rank==0)
{
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
}
	MPI_Barrier(MPI_COMM_WORLD);
	/* apelez rutina pe careuri a MPI*/
	for(i=0;i<N;i++)
	for(j=0;j<N;j++)
		c[i][j]=0.0;
	gettimeofday(&t1p,NULL);
	mult_sq_mat_check_mpi(N,a,b,c);
	gettimeofday(&t2p,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank==0)
	{
		printf("Checkerboard_MPI normal\n");fflush(stdout);
		acceleration(t1s,t2s,t1p,t2p,numero);
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
				fprintf(outpch_mpi,"%4.0f ",c[i][j]);
			fprintf(outpch_mpi,"\n");
		}
		fclose(outpch_mpi);
	}
	gettimeofday(&t1p,NULL);
//	mult_sq_mat_check_mpi_pth(N,a,b,c);
//	mult_sq_mat_check_mpi_sth(N,a,b,c);
//	mult_sq_mat_check_mpi_pthread(N,a,b,c);
	mult_sq_mat_check_mpi_omp(N,a,b,c);
	gettimeofday(&t2p,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank==0)
	{
		printf("Checkerboard_MPI thread\n");fflush(stdout);
		acceleration(t1s,t2s,t1p,t2p,numero);
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
				fprintf(outpch_mpi_th,"%4.0f ",c[i][j]);
			fprintf(outpch_mpi_th,"\n");
		}
		fclose(outpch_mpi_th);
	}
	free(*a);
	free(a);
	free(*b);
	free(b);
	free(*c);
	free(c);
	MPI_Finalize();
	exit(0);
}
