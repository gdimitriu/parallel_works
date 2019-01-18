LA_Grid_2d *la_grid_2d_new(MPI_Comm comm, int P, int Q)
{
  LA_Grid_2d *grid;
  MPI_Comm  row, col;
  int       my_rank, p, q;
  
  /* Determine row and column position */
  MPI_Comm_rank(comm, &my_rank);
  p = my_rank / Q;
  q = my_rank % Q;   /* pick a row-major mapping */

  /* Split comm into row and col comms */
  MPI_Comm_split(comm, p, q, &row); /* color by row, rank by column */
  MPI_Comm_split(comm, q, p, &col); /* color by column, rank by row */
  
  /* Make new grid */
  grid = (LA_Grid_2d *)malloc(sizeof(LA_Grid_2d));
  /* Fill in new grid structure */
  grid->grid_comm = comm;
  grid->row_comm  = row;
  grid->col_comm  = col;
  grid->P         = P;
  grid->Q         = Q;
  grid->p         = p;
  grid->q         = q;
  /* Return the newly built grid */
  return (grid);
}
