double weighted_norm(LA_dvector *v, LA_dvector *weights)
{
    int i;
    int m = v -> v . m;
    double *v_ptr, *wt_ptr;
    double temp, vmax_local = 0.0, vmax;
    double sum_local = 0.0, sum;

    /* point to local data */
    v_ptr  = v -> v . data;
    wt_ptr = weights -> v . data;

    /* compute local weighted maximum: */
    for(i = 0; i < m; i++, v_ptr++, wt_ptr++)
    {
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
    if(vmax > 0.0)
    {
        v_ptr  = v -> v . data;
        wt_ptr = weights -> v . data;

        for(i = 0; i < m; i++, v_ptr++, wt_ptr++)
        {
            temp = ((*vec_ptr)/(*wt_ptr)) / vmax;
            sum_local += temp * temp;
        }

        if(v -> type == LA_ROW)
           MPI_Allreduce(&sum_local, &sum, 1, MPI_DOUBLE, MPI_SUM, 
              v -> grid -> row_comm);
        else
           MPI_Allreduce(&sum_local, &sum, 1, MPI_DOUBLE, MPI_SUM, 
              v -> grid -> col_comm);

	/* sum now contains the global sum, rescale it: */
	sum = vmax * sqrt(sum/((double)(v -> m)));
    }
    else
	sum = 0.0;

    return(sum);
}

