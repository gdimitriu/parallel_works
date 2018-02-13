/*
	Matrix multiplications with output matrix in rowwise or colmnwise stiping partion
*/
#include<stdio.h>
#include<stdlib.h>
#include<parallel.h>
#include<math.h>

void *thread_mult_sq_mat_row(void *p)
{
	smult_sq_mat_striped data=*(smult_sq_mat_striped *)p;
	int s,i,j,k;
	s=data.N/data.P;
	for(i=s*data.who;i<s*(data.who+1);i++)
		for(j=0;j<data.N;j++)
		{
			data.c[i][j]=0.0;
			for(k=0;k<data.N;k++)
				data.c[i][j]+=data.a[i][k]*data.b[k][j];
		}
	pthread_exit(NULL);
}

void *thread_mult_sq_mat_col(void *p)
{
	smult_sq_mat_striped data=*(smult_sq_mat_striped *)p;
	int s,i,j,k;
	s=data.N/data.P;
	for(j=s*data.who;j<s*(data.who+1);j++)
		for(i=0;i<data.N;i++)
		{
			data.c[i][j]=0.0;
			for(k=0;k<data.N;k++)
				data.c[i][j]+=data.a[i][k]*data.b[k][j];
		}
	pthread_exit(NULL);
}



int mult_sq_mat_striped(int mat,int thread,double **a,double **b,double **c,int type)
{
/*
	mat is the size of suare matrix
	thread is the nr of threads which will be use for computnation
	a and b are the input matrix
	c is the output matrix
	type is 1 for rowwise or 0 for columnwise partitioning
*/
	int i;
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
				if(pthread_create(&pt[i],NULL,thread_mult_sq_mat_col,&data[i])) 
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
				if(pthread_create(&pt[i],NULL,thread_mult_sq_mat_row,&data[i])) 
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
				if(pthread_create(&pt[i],NULL,thread_mult_sq_mat_row,&data[i])) 
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
