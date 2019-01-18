/*
	Execute the gauss elimination on square matrix with processors on striped partioning
*/
#include<solve-openmp.h>
/*
	data for solve in rowwise or columnwise partitioning
*/

int mul_striped_fast(long dim,int thread,double **mat,double **mat2)
{
	/*
		dim is dimension of data matrix
		thread is the numbers of threads
		mat is data matrix
	*/
	long i,j,k;
	int who,nr,last=0;
	/* Making alocations for data */
	#pragma omp parallel private(i,j,k,who,nr,last)
	{
		who=omp_get_thread_num();
		nr=dim%thread;
		if((who+1)==nr) last=1;
		if((nr==0) && ((who+1)==thread)) last=1;
		for(j=who;j<dim;j+=thread)
		{
			for(k=0;k<dim;k++) mat2[j][k]=mat[j][k];
		}
		for(i=0;i<3;i++)
		{
			for(j=who;j<dim;j+=thread)
			{
				for(k=0;k<dim;k++) mat2[j][k]=mat2[j][k]*mat[j][k];
			}
		}
	}
	return(0);
}
