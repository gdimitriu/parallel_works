/*
	Program in which we learn to transmite diferent data with bcast
*/
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define P 4

int main(int argc,char **argv)
{
	int rank,i,j;
	double *pb,*pc;
	double **b,**c;
	int test,temp,data;
	MPI_Comm nrproc;
	//initialization
	b=(double **)calloc(P,sizeof(double *));
	pb=(double *)calloc(P*P,sizeof(double));
	for(i=0;i<P;i++)
	{
 		b[i]=pb;
		pb+=P;
	}
	c=(double **)calloc(P,sizeof(double *));
	pc=(double *)calloc(P*P,sizeof(double));
	for(i=0;i<P;i++)
	{
 		c[i]=pc;
		pc+=P;
	}

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nrproc);
	if(rank==0)
	{
		//introduisez les values
		for(i=0;i<P;i++)
		for(j=0;j<P;j++)
			if(i==(P-1)) b[i][j]=j+i+1.002;
			else	b[i][j]=j+i;
		MPI_Bcast(*b,P*P,MPI_DOUBLE,0,MPI_COMM_WORLD);
		for(i=0;i<P;i++)
		{
			for(j=0;j<P;j++)
				printf("%lf ",b[i][j]);fflush(stdout);
			printf("\n");fflush(stdout);
		}
	}
	else 
	{
		MPI_Bcast(*c,P*P,MPI_DOUBLE,0,MPI_COMM_WORLD);
		printf("normal\n");fflush(stdout);
		for(i=0;i<P;i++)
		{
			for(j=0;j<P;j++)
				printf("%lf ",c[i][j]);fflush(stdout);
			printf("\n");fflush(stdout);
		}
	}
	test=0;
	data=rank;
	if(rank==0) printf("NR PROC %d\n",nrproc);
	for(i=0;i<nrproc;i++)
	{
		temp=data;
		MPI_Bcast(&temp,1,MPI_INT,i,MPI_COMM_WORLD);
		test=test+temp;
	}
	printf("Process %d a %d\n",rank,test);fflush(stdout);
	MPI_Finalize();
	free(*b);
	free(b);
	free(*c);
	free(c);
}