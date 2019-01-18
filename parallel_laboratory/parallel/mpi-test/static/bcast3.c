/* Program which make Bcast with pack */
#include<stdio.h>
#include<mpi.h>
#define SIZE 100
int main(int argc,char **argv)
{
	int rank;
	int packsize,position;
	int a;
	double b;
	int c;
	char packbuf[SIZE];
	MPI_Comm nrprocs;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nrprocs);
	do
	{
		if(rank==0)
		{
			printf("Enter the numbers:\n");fflush(stdout);
			scanf("%d %lf %d",&a,&b,&c);fflush(stdin);
			packsize=0;
			MPI_Pack(&a,1,MPI_INT,packbuf,SIZE,&packsize,MPI_COMM_WORLD);
			MPI_Pack(&b,1,MPI_DOUBLE,packbuf,SIZE,&packsize,MPI_COMM_WORLD);
			MPI_Pack(&c,1,MPI_INT,packbuf,SIZE,&packsize,MPI_COMM_WORLD);
		}
		MPI_Bcast(&packsize,1,MPI_INT,0,MPI_COMM_WORLD);
		MPI_Bcast(packbuf,packsize,MPI_PACKED,0,MPI_COMM_WORLD);
		if(rank!=0)
		{
			position=0;
			MPI_Unpack(packbuf,packsize,&position,&a,1,MPI_INT,MPI_COMM_WORLD);
			MPI_Unpack(packbuf,packsize,&position,&b,1,MPI_DOUBLE,MPI_COMM_WORLD);
			MPI_Unpack(packbuf,packsize,&position,&c,1,MPI_INT,MPI_COMM_WORLD);
		}
		printf("Process %d got %d %lf %d\n",rank,a,b,c);fflush(stdout);
		MPI_Barrier(MPI_COMM_WORLD);
	} while(a>=0);
	MPI_Finalize();
	return 0;
}