#include<stdio.h>
#include<mpi.h>
int main(int argc,char **argv)
{
int rank,size;
int sum,rez;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(rank==0)
	{
		printf("myid=%d,nr task=%d\n",rank,size);fflush(stdout);
	}
	sum=rank+1;
	rez=0;
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Reduce(&sum,&rez,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	if(rank==0)
	{
		printf("Suma=%d\n",rez);fflush(stdout);
	}
	MPI_Finalize();
}