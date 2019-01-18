/*	First example program */
#include<mpi.h>
#include<mpidefs.h>
int main(int argc,char **argv)
{
	int rank,size,to,from,tag,count,i,ierr;
	int src,dest;
	int st_source,st_tag,st_count;
	MPI_Status status;
	double data[100];
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	printf("\n Process %d of %d is alive.\n",rank,size);
	dest=size-1;
	src=0;
	if(rank==src)
	{
		to=dest;
		count=10;
		tag=2001;
		for(i=1;i<10;i++) data[i]=i;
		MPI_Send(data,count,MPI_DOUBLE_PRECISION,to,tag,MPI_COMM_WORLD);
	}
	else if (rank==dest)
	{
		tag=MPI_ANY_TAG;
		count=10;
		from=MPI_ANY_SOURCE;
		MPI_Recv(data,count,MPI_DOUBLE_PRECISION,from,tag,MPI_COMM_WORLD,&status);
		MPI_Get_count(&status,MPI_DOUBLE_PRECISION,&st_count);
		st_source=status.MPI_SOURCE;
		st_tag=status.MPI_TAG;
		printf("\n Status info: source=%d tag=%d count=%d.\n",st_source,st_tag,st_count);
		printf("%d received\n",rank);
		for(i=0;i<10;i++)
			printf("%d ",data[i]);
		printf("\n");
	}
	MPI_Finalize();
}