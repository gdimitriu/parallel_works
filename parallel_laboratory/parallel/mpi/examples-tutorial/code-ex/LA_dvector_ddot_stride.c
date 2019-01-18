double ddot_stride(LA_Dvector *x, LA_Dvector *y, int *error)
{
  int    i, start, stride, m;
  double local_sum = 0.0, sum, *x_data, *y_data;
  
  if (x->type != y->type)  /* Check for compatible types */
  {
      *error = 1;  /* FAILURE */
      return(0.0);
  }

  /* Determine the stride based on type */
  if (x->type == LA_GRID_ROW)
  {
    start = x->grid->p; stride = x->grid->P;
  }
  else
  {
    start = x->grid->q; stride = x->grid->Q;
  }

  /* Sum up my part (non-optimized) */
  m      = x->v->m;
  x_data = &(x->v->data[0]); y_data = &(y->v->data[0]); 
  for (i =  start; i < m; i += stride)
    local_sum += x_data[i] * y_data[i];

  /* Get the sum of all parts */
  MPI_Allreduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 
                x->grid->grid_comm);

  /* Return result */
  *error = MPI_SUCCESS;
  return(sum);
}
