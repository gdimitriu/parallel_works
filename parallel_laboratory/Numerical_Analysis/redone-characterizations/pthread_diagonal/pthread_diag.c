/*
	Solve a diagonal matrix
*/
#include<pthread.h>
#include<math.h>
/*
	data for solve in rowwise or columnwise partitioning
*/
barrier_t barrier_system;
typedef struct
{
	double **mat;		/* adjounct matrix */
	double *y;			/* free term vectors */
	long who;				/* nr of courent thread*/
	long N;				/* dimensions of matrix */
	int P;				/* nr of threads */
	int *counters;
	pthread_mutex_t *mutex;
} ssystem_striped_fast;

void *thread_diag_striped_fast_ciclic(void *p)
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
			back-substitution 
	*/
	for(i=data.who;i<data.N;i+=data.P) data.counters[i]=0;
	if(last==1)
	{
		proccount=data.N-1-data.P;
		/* modification vector is reset */	
		data.counters[data.N-1]=1;
		for(i=data.who;i<data.N-1;i+=data.P)
			pthread_mutex_lock(&data.mutex[i]);
	}
	else
	{
		proccount=data.who+data.P*(s-1);
		for(i=data.who;i<data.N;i+=data.P)
			pthread_mutex_lock(&data.mutex[i]);
	}
	barrier_sync(&barrier_system);
	replay=data.N-1;
	for(i=proccount;i>=data.who;i-=data.P)
	{
		/* for all row before current row */
		for(k=replay;k>i;k--)
		{
			if(data.counters[k]==0) 
			{
				pthread_mutex_lock(&data.mutex[k]);
				pthread_mutex_unlock(&data.mutex[k]);
			}
			data.y[i]-=data.y[k]*data.mat[i][k];
		}	
		data.counters[i]=1;
		pthread_mutex_unlock(&data.mutex[i]);
		proccount1=i-data.P;
		for(k=replay;k>i;k--)
		{
			for(l=proccount1;l>=data.who;l-=data.P)
			{
				/* make back-substituion */
				data.y[l]-=data.y[k]*data.mat[l][k];
			}
		}
		replay=i;
	}
	pthread_exit(NULL);
}


int pthread_diag(long dim,int thread,double **mat,double *x,double *y)
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
	pthread_mutex_t *mutex;
	int *counters;
	long i;
	pthread_attr_t attr;
	/* Making alocations for data */
	pt=(pthread_t *)calloc(thread,sizeof(pthread_t));
	mutex=(pthread_mutex_t *)calloc(dim,sizeof(pthread_mutex_t));
	counters=(int *)calloc(dim,sizeof(int));
	data=(ssystem_striped_fast *)calloc(thread,sizeof(ssystem_striped_fast));
	barrier_init(&barrier_system,thread);
	
	for(i=0;i<dim;i++)
	{
		counters[i]=0.0;
		pthread_mutex_init(&mutex[i],NULL);
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
		data[i].mutex=mutex;
		data[i].counters=counters;
		pthread_create(&pt[i],&attr,thread_diag_striped_fast_ciclic,&data[i]);
	}
	/* Waiting for finish the threads */
	for(i=0;i<thread;i++)
		pthread_join(pt[i],NULL);
	free(pt);
	free(data);
	free(counters);
	barrier_destroy(&barrier_system);
	for(i=0;i<dim;i++) x[i]=y[i];
	for(i=0;i<dim;i++)
		pthread_mutex_destroy(&mutex[i]);
	return(0);
}
