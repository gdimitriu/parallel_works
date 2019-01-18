#include<stdio.h>
#include<sys/time.h>	/* for time functions */
#include<unistd.h>
#include<stdlib.h>
#include"acceleration.h"
#include<math.h>
#include<mpi.h>
#include<zlib.h>
#include<parallel/parallel-mpi.h>
//#include<parallel/parallel-mpi-omp.h>
//#include<parallel/parallel-mpi-sth.h>
//#include<parallel/parallel-mpi-pth.h>
#include<parallel/parallel-mpi-pthread.h>

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
int dims[2];		/* les dimensions */
int periodicite[2]; /* la periodicite de reseau */
int coordonne[2];
MPI_Comm mesh2_comm;
int partition,procs;
MPI_Status status;
//initialize MPI
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
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
	write(1,"Introduceti numele fisiserului a:",33*sizeof(char));
	gets(numea);
	write(1,"Introduceti numele fisiserului b:",33*sizeof(char));
	gets(numeb);
}
	MPI_Bcast(numea,128,MPI_CHAR,0,MPI_COMM_WORLD);
	MPI_Bcast(numeb,128,MPI_CHAR,0,MPI_COMM_WORLD);
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
	cs=(double **)calloc(N,sizeof(double *));
	pcs=(double *)calloc(N*N,sizeof(double));
	for(j=0;j<N;j++)
	{
		cs[j]=pcs;
		pcs+=N;
	}
	readfile(numea,a);
	readfile(numeb,b);
if(rank==0)
{
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
}
	/* apelez rutina pe careuri a MPI*/
	for(i=0;i<N;i++)
	for(j=0;j<N;j++)
		c[i][j]=0.0;
	MPI_Barrier(MPI_COMM_WORLD);
	/* de ce point nous contorize le temps */
	gettimeofday(&t1p,NULL);
	readfile(numea,a);
	readfile(numeb,b);
	mult_sq_mat_check_mpi(N,a,b,c);
if(rank==0)	gettimeofday(&t2p,NULL);
if(rank==0)
{
	printf("Checkerboard_MPI normal\n");fflush(stdout);
	acceleration(t1s,t2s,t1p,t2p,size);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outpch_mpi,"%4.0f ",c[i][j]);
		fprintf(outpch_mpi,"\n");
	}
	fclose(outpch_mpi);
}
	free(*a);
	free(a);
	free(*b);
	free(b);
	free(*c);
	free(c);
	free(*cs);
	free(cs);
	/* ----------------------------------------------------------- */
	/* 
			appele la routine de calcule pour matrix a check cannon
	*/
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
if(rank==0)
{
	rez=(double **)calloc(N,sizeof(double *));
	prez=(double *)calloc(N*N,sizeof(double));
	for(j=0;j<N;j++)
	{
		rez[j]=prez;
		prez+=N;
	}	
}
	procs=sqrt(size);
	partition=N/procs;
	a=(double **)calloc(partition,sizeof(double *));
	pa=(double *)calloc(partition*partition,sizeof(double));
	for(j=0;j<partition;j++)
	{
		a[j]=pa;
		pa+=partition;
	}
	b=(double **)calloc(partition,sizeof(double *));
	pb=(double *)calloc(partition*partition,sizeof(double));
	for(j=0;j<partition;j++)
	{
		b[j]=pb;
		pb+=partition;
	}
	c=(double **)calloc(partition,sizeof(double *));
	pc=(double *)calloc(partition*partition,sizeof(double));
	for(j=0;j<partition;j++)
	{
		c[j]=pc;
		pc+=partition;
	}	
	MPI_Barrier(MPI_COMM_WORLD);
	/* de ce point nous contorize le temps */
if(rank==0)	gettimeofday(&t1p,NULL);
	dims[0]=dims[1]=procs;
	periodicite[0]=periodicite[1]=1;
	MPI_Cart_create(MPI_COMM_WORLD,2,dims,periodicite,1,&mesh2_comm);
if(rank==0)	readfile(numea,rez);
	diffusion_donne_check_mpi(mesh2_comm,size,rank,rez,a,partition);
if(rank==0) readfile(numeb,rez);
	diffusion_donne_check_mpi(mesh2_comm,size,rank,rez,b,partition);
	mult_sq_mat_check_mpi_cannon(N,a,b,c,mesh2_comm,procs,partition);
	// unifie la matrice de resoudre
if(rank==0)
{
	for(i=0;i<partition;i++)
	for(j=0;j<partition;j++)
		rez[i][j]=c[i][j];
	for(k=1;k<procs;k++)
	{
		MPI_Cart_coords(mesh2_comm,k,2,coordonne);
		j=partition*coordonne[1];
		for(l=0;l<partition;l++)
		{
			i=partition*coordonne[0]+l;
			MPI_Recv(&rez[i][j],partition,MPI_DOUBLE,k,0,MPI_COMM_WORLD,&status);
		}
	}
}
else
{
	for(l=0;l<partition;l++)
		MPI_Send(&c[l][0],partition,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
}
if(rank==0)	gettimeofday(&t2p,NULL);
if(rank==0)
{
	printf("Checkerboard_MPI cannon\n");fflush(stdout);
	acceleration(t1s,t2s,t1p,t2p,size);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outpch_mpi_1,"%4.0f ",rez[i][j]);
		fprintf(outpch_mpi_1,"\n");
	}
	fclose(outpch_mpi_1);
}
	free(*a);
	free(a);
	free(*b);
	free(b);
	free(*c);
	free(c);
if(rank==0)
{
	free(*rez);
	free(rez);
}
	/* 
			appele la routine de calcule pour matrix a check cannon avec threads
	*/
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
if(rank==0)
{
	rez=(double **)calloc(N,sizeof(double *));
	prez=(double *)calloc(N*N,sizeof(double));
	for(j=0;j<N;j++)
	{
		rez[j]=prez;
		prez+=N;
	}	
	readfile(numea,rez);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outpch_mpi_1th,"%4.0f ",rez[i][j]);
		fprintf(outpch_mpi_1th,"\n");
	}
	fclose(outpch_mpi_1th);
	free(*rez);
	free(rez);
}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Comm_free(&mesh2_comm);
	MPI_Finalize();
	exit(0);
}
