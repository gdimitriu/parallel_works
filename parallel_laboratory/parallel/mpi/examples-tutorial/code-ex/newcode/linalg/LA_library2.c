#define N_DIMS 2
LA_Grid_2d *la_grid_2d_new_II(MPI_Comm comm, int P, Q)
{
  LA_Grid_2d *grid;
  MPI_Comm  comm_2d, row, col;
  int       my_rank, p, q; 
  int dims[N_DIMS],        /* hold dimensions */
      local[N_DIMS],       /* local position */
      period[N_DIMS],      /* aperiodic flags */
      remain_dims[N_DIMS]; /* flags for sub-dimension computations */
  /* Generate a new communicator with virtual topology added: */
  dims[0] = P; period[0] = FALSE;
  dims[1] = Q; period[1] = FALSE;
  MPI_Cart_create(comm, N_DIMS, dims, period, TRUE, &comm_2d);
  /* map back to topology coordinates: */
  MPI_Comm_rank(comm, &my_rank);  
  MPI_Cart_coords(comm_2d, my_rank, N_DIMS, &local);
  p = local[0]; q = local[1];  /* this is "my" grid location */
  /* Use cartesian sub-topology mechanism to get row/col comms: */
  remain_dims[0] = TRUE; remain_dims[1] = FALSE;
  MPI_Cart_sub(comm_2d, remain_dims, &row);
  remain_dims[0] = FALSE; remain_dims[1] = TRUE;
  MPI_Cart_sub(comm_2d, remain_dims, &col);
  grid = NEW(LA_Grid_2d, 1);  /* Make new grid */

  /* rest of the code is the same as before */
}
