void ddot_strided(LA_Dvector_ddot_stride_binop *xy, double *result)
{
  int    i, start, stride, m;
  double local_sum = 0.0, sum, *x_data, *y_data;
  
  /* Dereference the binary vector operands: */
  x = xy -> binop.x;  y = xy -> binop.y;

  /* Determine the stride based on type */
  start  = xy -> local_start; 
  stride = xy -> local_stride;
  
  /* Sum up my part (non-optimized) */
  m      = x->v->m;
  x_data = &(x->v->data[0]); y_data = &(y->v->data[0]); 
  for (i =  start; i < m; i += stride)
    local_sum += x_data[i] * y_data[i];

  /* Get the sum of all parts */
  MPI_Allreduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, xy->binop.comm);

  /* Return result */
  xy -> binop.error = 0;
  *result = sum;
}
