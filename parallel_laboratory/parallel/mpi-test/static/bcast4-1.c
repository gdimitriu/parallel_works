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
	double *pb;
	struct {int a;double **b;} value;
	MPI_Datatype mystruct;
	int blocklens[2];
	MPI_Aint indices[2];
	MPI_Datatype old_types[2];
	MPI_Comm nrproc;
	//initialization
	value.b=(double **)calloc(P,sizeof(double *));
	pb=(double *)calloc(P*P,sizeof(double));
	for(i=0;i<P;i++)
	{
 		value.b[i]=pb;
		pb+=P;
	}

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nrproc);
	if(rank==0)
	{
		//introduisez les values
		for(i=0;i<P;i++)
		for(j=0;j<P;j++)
			value.b[i][j]=j+i;
		value.a=8;
	}
	blocklens[0]=1;
	blocklens[1]=P*P;
	old_types[0]=MPI_INT;
	old_types[1]=MPI_DOUBLE;
	//l'addresse des elements
	MPI_Address(&(value.a),&indices[0]);
	MPI_Address((value.b),&indices[1]);
	//faites relatives
	indices[1]-=indices[0];
	indices[0]=0;
	MPI_Type_struct(2,blocklens,indices,old_types,&mystruct);
	MPI_Type_commit(&mystruct);
	MPI_Bcast(&value,1,mystruct,0,MPI_COMM_WORLD);
//	if(rank!=0)
//	{
		for(i=0;i<P;i++)
		{
			for(j=0;j<P;j++)
				printf("%lf ",value.b[i][j]);fflush(stdout);
			printf("\n");fflush(stdout);
		}
		printf("%d \n",value.a);fflush(stdout);
//	}
	MPI_Type_free(&mystruct);
	MPI_Finalize();
//	free(*(value.b));
//	free(value.b);
}