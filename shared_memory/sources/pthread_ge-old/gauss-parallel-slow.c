/*
	Copyright 2006 Gabriel Dimitriu

	This file is part of scientific_computing.

    scientific_computing is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    scientific_computing is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with scientific_computing; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
*/
/*
	Execute the gauss elimination on square matrix with processors on striped partioning
*/
#include<solve-pthread.h>
/*
	data for making ring of process
*/
typedef struct
{
	long *counters;					/* counters for number of wait */
	pthread_mutex_t *mutex;		/* variable for condition signal */
	pthread_cond_t *cond;		/* condition for serialization of acces */
} spipe_fast;
/*
	data for solve in rowwise or columnwise partitioning
*/
typedef struct
{
	double **mat;		/* adjounct matrix */
	double *y;
	long who;				/* nr of courent thread*/
	long N;				/* dimensions of matrix */
	int P;				/* nr of threads */
	spipe_fast *pipe;	/* pointer at communications vector */
} ssystem_striped_fast;

void *thread_gauss_striped_fast_ciclic(void *p)
{
	ssystem_striped_fast data=*(ssystem_striped_fast *)p;
	long s;		/* Nr of row in partion */
	long nr;		/* average nr of row in each partition */
	long last=0;	/* if this thread have the last line */
	long proccount;	/* variable for eliminate the row=0 and last row */
	long proccount1; /* variable who reprezent the proccesor who is working */
	long replay;	/* variable for continue the elimination where it was stop */
	long i,j,k,l;	/* counters */
	/* 
		Make setings for partitioning 
	*/
	nr=data.N%data.P;
	s=(data.N-nr)/data.P;
	/* for the common row */
	if((data.who+1)<nr) 
		s++;
	else
	/* for the last row */
	if((data.who+1)==nr) 
	{
		s++;
		last=1;
	}
	if((nr==0) && (data.who+1)==data.P) last=1;
	
	/* 
		making alocation for vector of cicle variable 
	*/
	i=0;
	/* 
		for first row on first proccesor 
	*/
	if(data.who==0)
	{		
		/* make divizion step */
		pthread_mutex_lock(&data.pipe->mutex[0]);
		for(j=1;j<data.N;j++)
			data.mat[0][j]=data.mat[0][j]/data.mat[0][0];
		data.y[0]=data.y[0]/data.mat[0][0];
		data.mat[0][0]=1;
		data.pipe->counters[0]=1;
		pthread_cond_broadcast(&data.pipe->cond[0]);
		pthread_mutex_unlock(&data.pipe->mutex[0]);
		proccount=proccount1=data.who+data.P;
	}
	else	proccount=proccount1=data.who;
	/* until all row are eliminate */
	/* for all the row who are in this thread */
	replay=0;	
	/* i is the line who is working */
	for(i=proccount;i<data.N;i+=data.P)
	{	
		/* make elimination step */
		for(k=replay;k<i;k++)
		{
			pthread_mutex_lock(&data.pipe->mutex[k]);
			if(data.pipe->counters[k]==0) 
				pthread_cond_wait(&data.pipe->cond[k],&data.pipe->mutex[k]);
			if(data.pipe->counters[k]==1)
			{
				for(l=proccount1;l<data.N;l+=data.P)
				{
					for(j=(k+1);j<data.N;j++)
						data.mat[l][j]-=data.mat[l][k]*data.mat[k][j];
					data.y[l]-=data.y[k]*data.mat[l][k];
					data.mat[l][k]=0;
				}
			}
			pthread_mutex_unlock(&data.pipe->mutex[k]);
		}
		/* make divizion step */
		if(data.pipe->counters[i-1]==1)
		{
			pthread_mutex_lock(&data.pipe->mutex[i]);
			for(j=i+1;j<data.N;j++)
				data.mat[i][j]=data.mat[i][j]/data.mat[i][i];
			data.y[i]=data.y[i]/data.mat[i][i];
			data.mat[i][i]=1;
			/* make settings for next row */
			replay=i;
			proccount1=proccount1+data.P;
			data.pipe->counters[i]=1;
			pthread_cond_broadcast(&data.pipe->cond[i]);
			pthread_mutex_unlock(&data.pipe->mutex[i]);
		}	
	}
	/* will be sincronize before back-substitution step */
	barrier_sync(&barrier_system);
	/* 
			back-substitution 
	*/
	for(i=data.who;i<(data.N-1);i+=data.P) data.pipe->counters[i]=0;
	if(last==1)
	{
		proccount=proccount1=(data.N-1)-data.P;
		/* modification vector is reset */	
		data.pipe->counters[data.N-1]=1;
	}
	else proccount=proccount1=data.who+data.P*(s-1);
	barrier_sync(&barrier_system);
	/* resyncronization for making modification of data.pipe->counters[i] */
	/* if this is not made back-substitution work randomize because of missetting the modif */
	replay=data.N-1;
	for(i=proccount;i>=data.who;i-=data.P)
	{
		/* for all row before current row */
		for(k=replay;k>i;k--)
		{
			pthread_mutex_lock(&data.pipe->mutex[k]);
			if(data.pipe->counters[k]==0) 
				pthread_cond_wait(&data.pipe->cond[k],&data.pipe->mutex[k]);

			if(data.pipe->counters[k]==1)
			{
				for(l=proccount1;l>=data.who;l-=data.P)
				{
					/* make back-substituion */
					data.y[l]-=data.y[k]*data.mat[l][k];
				}
			}
			pthread_mutex_unlock(&data.pipe->mutex[k]);
		}
		if(data.pipe->counters[i+1]==1)
		{
			pthread_mutex_lock(&data.pipe->mutex[i]);
			data.pipe->counters[i]=1;
			replay=i;
			proccount1-=data.P;
			pthread_cond_broadcast(&data.pipe->cond[i]);
			pthread_mutex_unlock(&data.pipe->mutex[i]);
		}
	}
	pthread_exit(NULL);
}


int gauss_striped_fast(long dim,int thread,double **mat,double *x,double *y)
{
	/*
		dim is dimension of data matrix
		thread is the numbers of threads
		mat is data matrix
		x is unknown vector
		y is free vector
	*/
	pthread_t *pt;
	ssystem_striped_fast *data;
	spipe_fast pipe;
	long i,j;
	pthread_attr_t attr;
	/* Making alocations for data */
	if((pt=(pthread_t *)calloc(thread,sizeof(pthread_t)))==(pthread_t *)NULL)
	{
		perror("Error alocation for pthread identifiers");
		return(-1);
	}
	pipe.mutex=(pthread_mutex_t *)calloc(dim,sizeof(pthread_mutex_t));
	pipe.cond=(pthread_cond_t *)calloc(dim,sizeof(pthread_cond_t));
	pipe.counters=(long *)calloc(dim,sizeof(long));
	data=(ssystem_striped_fast *)calloc(thread,sizeof(ssystem_striped_fast));
	barrier_init(&barrier_system,thread);
	for(i=0;i<dim;i++)
	{
		pthread_mutex_init(&pipe.mutex[i],NULL);
		pthread_cond_init(&pipe.cond[i],NULL);
		pipe.counters[i]=0;
	}
	pthread_attr_init(&attr);
	/*
		Create threads
	*/
	for(i=0;i<thread;i++)
	{
		data[i].mat=mat;
		data[i].y=y;
		data[i].N=dim;
		data[i].P=thread;
		data[i].who=i;
		data[i].pipe=&pipe;
		pthread_create(&pt[i],&attr,thread_gauss_striped_fast_ciclic,&data[i]);
	}
	/* Waiting for finish the threads */
	for(i=0;i<thread;i++)
		pthread_join(pt[i],NULL);
	for(i=(dim-1);i>=0;i--)
		x[i]=y[i];
	free(pt);
	free(data);
	free(pipe.counters);
	barrier_destroy(&barrier_system);
	for(i=0;i<dim;i++)
	{
		pthread_mutex_destroy(&pipe.mutex[i]);
		pthread_cond_destroy(&pipe.cond[i]);
	}
	return(0);
}
