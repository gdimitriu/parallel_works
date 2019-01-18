#include<stdio.h>
#include<parallel.h>
#include<stdlib.h>

void *thread_mult_sq_mat_vect_row(void *p)
{
	smult_sq_mat_vect_striped data=*(smult_sq_mat_vect_striped *)p;
	int s,i,j;
	s=data.N/data.P;
	for(i=s*data.who;i<(data.who+1)*s;i++)
	{
		data.y[i]=0.0f;
		for(j=0;j<data.N;j++)
			data.y[i]+=data.a[i][j]*data.x[j];
	}
	pthread_exit(NULL);
}

int mult_sq_mat_vect_striped(int mat,int thread,double **a,double *x,double *y)
{
/*
	N nr of row/column for matrix
	thread nr of threads for computing
	a input matrix
	x input vector
	y input vector
*/
	smult_sq_mat_vect_striped *data;
	int i;
	pthread_t *pt;
	/*	Make alocation */
	if((data=(smult_sq_mat_vect_striped *)calloc(thread,sizeof(smult_sq_mat_vect_striped)))==(smult_sq_mat_vect_striped *)NULL)
	{
		perror("Error alocation identification");
		return(-1);
	}
	if((pt=(pthread_t *)calloc(thread,sizeof(pthread_t)))==(pthread_t *)NULL)
	{
		perror("Error alocation indics vector");
		return(-1);
	}
	/* Create threads */
	for(i=0;i<thread;i++)
	{
		data[i].a=a;
		data[i].x=x;
		data[i].y=y;
		data[i].who=i;
		data[i].N=mat;
		data[i].P=thread;
		if(pthread_create(&pt[i],NULL,thread_mult_sq_mat_vect_row,&data[i]))
		{
			perror("Can not create threads");
			free(pt);
			free(data);
			return(-1);
		}
	}
	/* Wait all thread to finish */
	for(i=0;i<thread;i++)
		pthread_join(pt[i],NULL);
	/* dealocating memory */
	free(pt);
	free(data);
	return(0);
}
