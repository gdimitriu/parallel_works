#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<string.h>
#include"timeprint.c"
#include <mpi.h>
#include"gauss_sc_mpi_pthread2.c"
#define numar 10
int main(int argc,char **argv)
{
	struct timeval t1,t2;
	double **mat,*x,*rez,*temp1,temp,*y;
	int i,l,j;
	int dim;
	int rank;
	FILE *fp;
	int thread;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&thread);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	dim=atoi(argv[1]);
	MPI_Bcast(&dim,1,MPI_INT,0,MPI_COMM_WORLD);
	if(rank==0)
	{
		y=(double *)calloc(dim,sizeof(double));
		rez=(double *)calloc(dim,sizeof(double));
		for(i=0;i<dim;i++) rez[i]=(double)i;
		//here is the parallel zone
		mat=(double **)calloc(dim,sizeof(double *));
		temp1=(double *)calloc(dim*dim,sizeof(double));
		for(i=0;i<dim;i++)
		{
			mat[i]=temp1;
			temp1+=dim;
		}
	}
	x=(double *)calloc(dim,sizeof(double));
	if(rank==0)
	{
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
		gettimeofday(&t1,NULL);
	}
	for(l=0;l<numar;l++)
	{
		pthread_gauss_MPI(mat,y,x,dim);
	}
	if(rank==0) 
	{
		gettimeofday(&t2,NULL);
		if(rank==0) for(i=0;i<dim;i++) {if(fabs(rez[i]-x[i])>1E-5) printf("%lf=%lf\n",rez[i],x[i]); fflush(stdout);}
		fp=fopen("time-par.dat","a");
		timeprint(t1,t2,numar,dim,fp,thread);
		fclose(fp);
		free(*mat);
		free(mat);
		free(rez);
		free(y);
	}		
	free(x);
	MPI_Finalize();
	return 0;
}
