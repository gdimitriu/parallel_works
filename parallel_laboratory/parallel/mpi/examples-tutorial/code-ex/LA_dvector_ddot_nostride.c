double ddot_nostride(LA_Dvector *x, *y, int *error)
{
  int    i, m;
  double local_sum = 0.0, sum, *x_data, *y_data;

  if (x->type != y->type)  /* Check for compatible types */
  {  
      *error = MPI_FAILURE;
      return(0.0);
  }
  /* Sum up my portion of the vector (non-optimized) */
  m = x->v->m;
  x_data = &(x->v->data[0]); y_data = &(y->v->data[0]); 
  for (i = 0; i < m; i++) 
    local_sum += x_data[i] * y_data[i];

  /* Get row or column sum based on type */
  if (x->type == LA_GRID_ROW)
    MPI_Allreduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM,
                  x->grid->row_comm);
  else                  
    MPI_Allreduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM,
                  x->grid->col_comm);

  /* Return the result */
  *error = MPI_SUCCESS;
  return (sum);
}
