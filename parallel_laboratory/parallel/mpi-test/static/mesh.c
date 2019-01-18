#include <stdio.h>
#include <math.h>
#include "mpi.h"

int main( argc, argv )
int argc;
char **argv;
{
    int rank, value, size;
    int size_mesh;
    int dims[2],periodics[2];
    int right_nbr, left_nbr;
    MPI_Comm   mesh2_comm;
    MPI_Status status;
    int coordonne[2];
    int i;

    MPI_Init( &argc, &argv );

    MPI_Comm_size( MPI_COMM_WORLD, &size );
    /* fait les setting pour le reseau */
    size_mesh=sqrt(size);
    dims[0]=dims[1]=size_mesh;
    periodics[0]=periodics[1]=0;
    MPI_Cart_create( MPI_COMM_WORLD, 2,dims,periodics, 1, &mesh2_comm );
    MPI_Cart_shift( mesh2_comm, 0, 1, &left_nbr, &right_nbr );
    MPI_Comm_rank( mesh2_comm, &rank );
    MPI_Cart_coords(mesh2_comm,rank,2,coordonne);
	 printf("process %d is %d %d\n",rank,coordonne[0],coordonne[1]);fflush(stdout);
	 if(rank!=0)
	 {
	 	MPI_Send(&rank,1,MPI_INT,0,0,MPI_COMM_WORLD);
	 }
	 if(rank==0)
	 {
	 	for(i=1;i<size;i++)
	 	{
	 		MPI_Recv(&value,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);
	 		printf("process %d\n",value);fflush(stdout);
	 	}
	 }
    MPI_Finalize( );
    return 0;
}
