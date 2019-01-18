#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<string.h>
#include"timeprint.c"
#include <mpi.h>
#include <math.h>
#include"gaussJ_inv_mpi.c"
#define numar 10
int main(int argc,char **argv)
{
	struct timeval t1,t2;
	double **mat,**inv,*temp1,temp,**mat1;
	int i,l,j,k;
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
		//here is the parallel zone
		mat=(double **)calloc(dim,sizeof(double *));
		temp1=(double *)calloc(dim*dim,sizeof(double));
		for(i=0;i<dim;i++)
		{
			mat[i]=temp1;
			temp1+=dim;
		}
		inv=(double **)calloc(dim,sizeof(double *));
		temp1=(double *)calloc(dim*dim,sizeof(double));
		for(i=0;i<dim;i++)
		{
			inv[i]=temp1;
			temp1+=dim;
		}
	}
	if(rank==0)
	{
		for(i=0;i<dim;i++)
		{
			for(j=0;j<dim;j++) mat[i][j]=(double)rand();
			temp=0.0;
			for(j=0;j<dim;j++) if(j!=i) temp+=abs(mat[i][j]);
			mat[i][i]+=temp;
		}
		gettimeofday(&t1,NULL);
	}
	for(l=0;l<numar;l++)
		gj_inv_MPI(mat,inv,dim);
	if(rank==0) 
	{
		gettimeofday(&t2,NULL);
		fp=fopen("time-par.dat","a");
		timeprint(t1,t2,numar,dim,fp,thread);
		mat1=(double **)calloc(dim,sizeof(double *));
		temp1=(double *)calloc(dim*dim,sizeof(double));
		for(i=0;i<dim;i++)
		{
			mat1[i]=temp1;
			temp1+=dim;
		}
		for(i=0;i<dim;i++)
		for(j=0;j<dim;j++)
		{
			mat1[i][j]=0.0;
			for(k=0;k<dim;k++) mat1[i][j]+=mat[i][k]*inv[k][j];
		}
		for(i=0;i<dim;i++) 
			if(fabs(mat1[i][i]-1.0)>1e-6)
			{
				 printf("error at %d %lf\n",i,mat1[i][i]);
				 fflush(stdout);
			 }
		fclose(fp);
		free(*mat);
		free(mat);
		free(*mat1);
		free(mat1);
		free(*inv);
		free(inv);
	}		
	MPI_Finalize();
	return 0;
}
