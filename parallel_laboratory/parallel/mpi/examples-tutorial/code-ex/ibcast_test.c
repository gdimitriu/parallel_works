/* for testing purposes */
#include "ibcast.h"
main ( argc, argv )
int    argc;
char **argv;
{
  Ibcast_handle **handle;
  int rank, size, *ranks;
  int i, errors = 0;

  MPI_Init ( &argc, &argv );
  MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
  MPI_Comm_size ( MPI_COMM_WORLD, &size );

  printf("[%d of %d] Initialized.\n",rank,size);

  handle = (Ibcast_handle **)malloc(sizeof(Ibcast_handle *) * size);
  ranks  = (int *)malloc(sizeof(int) * size);

  for (i=0; i<size; i++)
    ranks[i] = -999;
  ranks[rank] = rank;

  for (i=0; i<size; i++) 
    Ibcast ( &ranks[i], 1, MPI_INT, i, MPI_COMM_WORLD, &handle[i]);

  for (i=0; i<size; i++) {
    Ibcast_wait ( &handle[i] );
    if (ranks[i] != i) {
      printf("[%d] error, ranks[%d] = %d\n",rank,i,ranks[i]);
      errors++;
    }
  }
  
  free(ranks);
  free(handle);

  if (errors)
    printf("[%d] Errors.\n",rank);
  else
    printf("[%d] Successful.\n",rank);

  MPI_Finalize();
}


