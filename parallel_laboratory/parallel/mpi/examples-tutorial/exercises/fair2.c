#include "mpi.h"
#include <stdio.h>
#define large 128
int main(argc, argv)
int argc;
char **argv;
{
int rank, size, i, buf[1];
MPI_Status status;

MPI_Init( &argc, &argv );
MPI_Comm_rank( MPI_COMM_WORLD, &rank );
MPI_Comm_size( MPI_COMM_WORLD, &size );
if (rank == 0) {
 MPI_Request requests[large];
 MPI_Status  statuses[large];
 int         indices[large];
 int         buf[large];
 int         ndone, msgcnt;
 int         nfrom[large];
 int         j;

   for (i=1; i<size; i++) {
     MPI_Irecv( buf+i, 1, MPI_INT, i,
               MPI_ANY_TAG, MPI_COMM_WORLD, &requests[i-1] );
     nfrom[i] = 0;
   }
 msgcnt=100*(size-1);
 while(msgcnt) {
    MPI_Waitsome( size-1, requests, &ndone, indices, statuses );
    for (i=0; i<ndone; i++) {
        j = indices[i];
        printf( "Msg from %d with tag %d\n", 
                statuses[i].MPI_SOURCE, 
                statuses[i].MPI_TAG );
	if (nfrom[j]++ < 100) {
	  MPI_Irecv( buf+j, 1, MPI_INT, j,
		    MPI_ANY_TAG, MPI_COMM_WORLD, &requests[j] );
	  }
	msgcnt -= ndone;
        }
    }
  }
else {
    for (i=0; i<100; i++) 
	MPI_Send( buf, 1, MPI_INT, 0, i, MPI_COMM_WORLD );
    }
MPI_Finalize();
return 0;
}
