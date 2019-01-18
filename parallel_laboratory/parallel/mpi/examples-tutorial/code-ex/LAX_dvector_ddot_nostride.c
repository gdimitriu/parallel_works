void ddot_nostride(LA_Dvector_binop *xy, double *result)
{
  int    i, m;
  double local_sum = 0.0, sum, *x_data, *y_data;

  /* Dereference the binary vector operands: */
  x = xy -> x;  y = xy -> y; 

  /* Sum up my portion of the vector (non-optimized) */
  m = x->v->m;
  x_data = &(x->v->data[0]);
  y_data = &(y->v->data[0]); 
  for (i = 0; i < m; i++) 
    local_sum += x_data[i] * y_data[i];

  MPI_Allreduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM,
		xy->comm);

  /* Return the result */
  xy -> error = 0;
  *result = sum;
}

void ddot_nostride_blas(LA_Dvector_binop *xy, double *result)
{
  double local_sum = 0.0, sum;
  int intone = 1;

  /* Dereference the binary vector operands: */
  x = xy -> x;  y = xy -> y;

  /* Sum up my portion of the vector */
  blas_ddot(&(x->v->m), &(x->v->data[0]), &intone,
	    &(y->v->data[0]), &intone);

  MPI_Allreduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM,
		xy->comm);

  /* Return the result */
  xy -> error = 0;
  *result = sum;
}
