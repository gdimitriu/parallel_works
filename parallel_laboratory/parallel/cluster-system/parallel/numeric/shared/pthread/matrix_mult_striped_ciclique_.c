/*
	Matrix multiplications with output matrix in rowwise or colmnwise stiping partion
*/
#include<parallel/parallel-pthread.h>
#include<math.h>
typedef struct
{
	long N;				/* nr of rows/colummns */
	int P;				/* nr of threads */
	int who; 			/* thread nr */
	double **a,**b;	/* matrix for computing */
	double **c;			/* matrix result */
} smult_sq_mat_striped;

void *thread_mult_sq_mat_row_ciclique(void *p)
{
	smult_sq_mat_striped data=*(smult_sq_mat_striped *)p;
	long i,j,k;
	for(i=data.who;i<data.N;i+=data.P)
		for(j=0;j<data.N;j++)
		{
			data.c[i][j]=0.0;
			for(k=0;k<data.N;k++)
				data.c[i][j]+=data.a[i][k]*data.b[k][j];
		}
	pthread_exit(NULL);
}

void *thread_mult_sq_mat_col_ciclique(void *p)
{
	smult_sq_mat_striped data=*(smult_sq_mat_striped *)p;
	long i,j,k;
	for(j=data.who;j<data.N;j+=data.P)
		for(i=0;i<data.N;i++)
		{
			data.c[i][j]=0.0;
			for(k=0;k<data.N;k++)
				data.c[i][j]+=data.a[i][k]*data.b[k][j];
		}
	pthread_exit(NULL);
}



int mult_sq_mat_striped_ciclique(long mat,int thread,double **a,double **b,double **c,int type)
{
/*
	mat is the size of square matrix
	thread is the nr of threads which will be use for computation
	a and b are the input matrix
	c is the output matrix
	type is 1 for rowwise or 0 for columnwise partitioning
*/
	long i;
	pthread_t *pt;
	smult_sq_mat_striped *data;
	/* Making alocations for structures */
	if((pt=(pthread_t *)calloc(thread,sizeof(pthread_t)))==(pthread_t *)NULL) 
	{
		perror("Error alocation data for pthread identifier");
		return(-1);
	}
	if((data=(smult_sq_mat_striped *)calloc(thread,sizeof(smult_sq_mat_striped)))==(smult_sq_mat_striped *)NULL) 
	{
		perror("Error alocation data for pthread identifier");
		free(pt);
		return(-1);
	}
	/*
		Create threads
	*/
	for(i=0;i<thread;i++)
	{
		data[i].P=thread;
		data[i].N=mat;
		data[i].who=i;
		data[i].a=a;
		data[i].b=b;
		data[i].c=c;
		switch(type)
		{
			case 0:
			{
				if(pthread_create(&pt[i],NULL,thread_mult_sq_mat_col_ciclique,&data[i])) 
				{
				perror("Can not create threads");
				free(pt);
				free(data);
				return(-1);
				}
				break;
			}
			case 1:
			{
				if(pthread_create(&pt[i],NULL,thread_mult_sq_mat_row_ciclique,&data[i])) 
				{
				perror("Can not create threads");
				free(pt);
				free(data);
				return(-1);
				}
				break;
			}
			default:
			{
				if(pthread_create(&pt[i],NULL,thread_mult_sq_mat_row_ciclique,&data[i])) 
				{
				perror("Can not create threads");
				free(pt);
				free(data);
				return(-1);
				}
			}
		}
	}
	/*
		Waiting to finish the threads
	*/
	for(i=0;i<thread;i++)
		pthread_join(pt[i],NULL);
	/* dealocating the memory */
	free(pt);
	free(data);
	return(0);
}
