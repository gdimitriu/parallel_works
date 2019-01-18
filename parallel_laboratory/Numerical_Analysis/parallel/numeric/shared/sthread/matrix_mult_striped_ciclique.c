/*
	Matrix multiplications with output matrix in rowwise or colmnwise stiping partion
*/
#include<parallel/parallel-sth.h>
#include<math.h>

void thread_mult_sq_mat_row_ciclique(double **mata,double **matb,double **matc,long who,long N,long P,st_join_counter_t *joinc)
{
	long i,j,k;
	for(i=who;i<N;i+=P)
	{
		ST_POLLING();
		for(j=0;j<N;j++)
		{
			//ST_POLLING();
			matc[i][j]=0.0;
			for(k=0;k<N;k++)
				matc[i][j]+=mata[i][k]*matb[k][j];
		}
		
	}
	st_join_counter_finish(joinc);
}

void thread_mult_sq_mat_col_ciclique(double **mata,double **matb,double **matc,long who,long N,long P,st_join_counter_t *joinc)
{
	long i,j,k;
	for(j=who;j<N;j+=P)
	{
		ST_POLLING();
		for(i=0;i<N;i++)
		{
			//ST_POLLING();
			matc[i][j]=0.0;
			for(k=0;k<N;k++)
				matc[i][j]+=mata[i][k]*matb[k][j];
		}
	}	
	st_join_counter_finish(joinc);
}



int mult_sq_mat_striped_ciclique_sth(long mat,int thread,double **a,double **b,double **c,int type)
{
/*
	mat is the size of square matrix
	thread is the nr of threads which will be use for computation
	a and b are the input matrix
	c is the output matrix
	type is 1 for rowwise or 0 for columnwise partitioning
*/
	long i;
	st_join_counter_t *join_c;
	/* Making alocations for structures */
	if((join_c=(st_join_counter_t *)calloc(thread,sizeof(st_join_counter_t)))==(st_join_counter_t *)NULL) 
	{
		perror("Error alocation data for counter identifier");
		return(-1);
	}
	/*
		Create threads
	*/
	st_join_counter_init(join_c,thread);
	for(i=0;i<thread;i++)
	{
		switch(type)
		{
			case 0:
			{
				ST_THREAD_CREATE(thread_mult_sq_mat_col_ciclique(a,b,c,i,mat,thread,join_c));
				ST_POLLING();
				break;
			}
			case 1:
			{
				ST_THREAD_CREATE(thread_mult_sq_mat_row_ciclique(a,b,c,i,mat,thread,join_c));
				ST_POLLING();
				break;
			}
			default:
			{
				ST_THREAD_CREATE(thread_mult_sq_mat_row_ciclique(a,b,c,i,mat,thread,join_c));
				ST_POLLING();
			}
		}
	}
	/*
		Waiting to finish the threads
	*/
	st_join_counter_wait(join_c);
	/* dealocating the memory */
	free(join_c);
	return(0);
}
