double weighted_norm_II(LA_dvector *v, LA_dvector *weights)
{   /* test for norm problems resulting from parallel summation */
    int i, scanlen;
    int m = v -> v . m; /* local length of vector */
    double *v_ptr, *wt_ptr;
    double temp, vmax_local = 0.0, vmax;
    double sum_local = 0.0, sum, scanval, *scanarray;
    /* point to local data */
    v_ptr  = v -> v . data;
    wt_ptr = weights -> v . data;
    /* compute local weighted maximum: */
    for(i = 0; i < m; i++, v_ptr++, wt_ptr++) {
        temp = fabs((*v_ptr)/(*wt_ptr));
        vmax_local = max(temp, vmax_local);
    }
    if(v -> type == LA_ROW)
       MPI_Allreduce(&vmax_local, &vmax, 1, MPI_DOUBLE, MPI_MAX, 
                  v -> grid -> row_comm);
    else
       MPI_Allreduce(&vmax_local, &vmax, 1, MPI_DOUBLE, MPI_MAX, 
                  v -> grid -> col_comm);
    /* vmax is now the maximum overall */
    if(vmax > 0.0) {
        v_ptr  = v -> v . data;
        wt_ptr = weights -> v . data;
	/* still trust local summations in this version... */
        for(i = 0; i < m; i++, v_ptr++, wt_ptr++) {
            temp = ((*vec_ptr)/(*wt_ptr)) / vmax;
            sum_local += temp * temp;
        }
        if(v -> type == LA_ROW) {
           MPI_Scan(&sum_local, &scanval, 1, MPI_DOUBLE, MPI_SUM, 
              v -> grid -> row_comm);
	   scanarray = NEW(double, v -> grid -> q);
	   MPI_Allgather(&scanval, 1, MPI_DOUBLE, scanarray, 1, MPI_DOUBLE,
			 v -> grid -> row_comm);

	   scanlen = v -> grid -> q;
        } else {
           MPI_Scan(&sum_local, &scanval, 1, MPI_DOUBLE, MPI_SUM, 
              v -> grid -> col_comm);
	   scanarray = NEW(double, v -> grid -> p);
	   MPI_Allgather(&scanval, 1, MPI_DOUBLE, scanarray, 1, MPI_DOUBLE,
			 v -> grid -> col_comm);

	   scanlen = v -> grid -> p;
        }
	/* fill in a fancy test on the scanarray[] to see if cat. cancellation
	   is going on... */

	/* sum now contains the global sum, rescale it: */
	sum = vmax * sqrt(sum/((double)(v -> m)));
        free(scanarray);
    }
    else
	sum = 0.0;
    return(sum);
}
