/*
	Program in which we learn to transmite diferent data with bcast
*/
#include<stdio.h>
#include<mpi.h>

int main(int argc,char **argv)
{
	int rank;
	struct {int a;double b;int c;} value;
	MPI_Datatype mystruct;
	int blocklens[3];
	MPI_Aint indices[3];
	MPI_Datatype old_types[3];
	int nrproc;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nrproc);
	
	/* make data which we transmite */
	/* nr of data */
	blocklens[0]=1;blocklens[1]=1;blocklens[2]=1;
	/* type of data */
	old_types[0]=MPI_INT;old_types[1]=MPI_DOUBLE;old_types[2]=MPI_INT;
	/* location of elements */
	MPI_Address(&value.a,&indices[0]);
	MPI_Address(&value.b,&indices[1]);
	MPI_Address(&value.c,&indices[2]);
	/* make relatives */
	indices[2]-=indices[0];
	indices[1]-=indices[0];
	indices[0]=0;
	MPI_Type_struct(3,blocklens,indices,old_types,&mystruct);
	MPI_Type_commit(&mystruct);
	
	do
	{
		if(rank==0)
		{
			printf("Enter the data\n");fflush(stdout);
			scanf("%d %lf %d",&value.a,&value.b,&value.c);fflush(stdin);
		}
		MPI_Bcast(&value,1,mystruct,0,MPI_COMM_WORLD);
		printf("Process %d got %d %lf %d\n",rank,value.a,value.b,value.c);fflush(stdout);
		MPI_Barrier(MPI_COMM_WORLD);
	} while(value.a>=0);
	MPI_Type_free(&mystruct);
	MPI_Finalize();
	return 0;
}