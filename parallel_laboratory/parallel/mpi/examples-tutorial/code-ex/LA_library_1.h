typedef struct la_grid_2d
{
  int P, Q;  /* global shape of grid */
  int p, q;  /* local position on grid */
  
  MPI_Comm grid_comm; /* parent communicator */
  MPI_Comm row_comm;  /* row    communicator */
  MPI_Comm col_comm;  /* column communicator */

} LA_Grid_2d;
