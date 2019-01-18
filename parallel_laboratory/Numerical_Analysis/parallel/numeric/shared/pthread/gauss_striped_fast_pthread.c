/*
	Execute the gauss elimination on square matrix with processors on striped partioning
*/
#include<parallel/parallel-pthread.h>
/*
	data for making ring of process
*/
typedef struct
{
	int *counters;					/* counters for number of wait */
	pthread_mutex_t *mutex;		/* variable for condition signal */
	pthread_cond_t *cond;		/* condition for serialization of acces */
} spipe_fast;
/*
	data for solve in rowwise or columnwise partitioning
*/
typedef struct
{
	double **mat;		/* adjounct matrix */
	int who;				/* nr of courent thread*/
	int N;				/* dimensions of matrix */
	int P;				/* nr of threads */
	spipe_fast *pipe;	/* pointer at communications vector */
} ssystem_striped_fast;

void *thread_gauss_striped_fast_ciclic(void *p)
{
	ssystem_striped_fast data=*(ssystem_striped_fast *)p;
	int s;		/* Nr of row in partion */
	int nr;		/* average nr of row in each partition */
	int last=0;	/* if this thread have the last line */
	int proccount;	/* variable for eliminate the row=0 and last row */
	int proccount1; /* variable who reprezent the proccesor who is working */
	int replay;	/* variable for continue the elimination where it was stop */
	register int i,j,k,l;	/* counters */
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
		for(j=1;j<(data.N+1);j++)
			data.mat[0][j]=data.mat[0][j]/data.mat[0][0];
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
					for(j=(k+1);j<(data.N+1);j++)
						data.mat[l][j]-=data.mat[l][k]*data.mat[k][j];
					data.mat[l][k]=0;
				}
			}
			pthread_mutex_unlock(&data.pipe->mutex[k]);
		}
		/* make divizion step */
		if(data.pipe->counters[i-1]==1)
		{
			pthread_mutex_lock(&data.pipe->mutex[i]);
			for(j=i+1;j<(data.N+1);j++)
				data.mat[i][j]=data.mat[i][j]/data.mat[i][i];
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
					data.mat[l][data.N]-=data.mat[k][data.N]*data.mat[l][k];
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


int gauss_striped_fast(int dim,int thread,double **mat,double *x,double *y)
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
	double **workmat;
	double *pworkmat;
	register int i,j;
	pthread_attr_t attr;
	/* Making alocations for data */
	if((pt=(pthread_t *)calloc(thread,sizeof(pthread_t)))==(pthread_t *)NULL)
	{
		perror("Error alocation for pthread identifiers");
		return(-1);
	}
	if((pipe.mutex=(pthread_mutex_t *)calloc(dim,sizeof(pthread_mutex_t)))==(pthread_mutex_t *)NULL)
	{
		perror("Error alocation for vector of mutex");
		free(pt);
		return(-1);
	}
	if((pipe.cond=(pthread_cond_t *)calloc(dim,sizeof(pthread_cond_t)))==(pthread_cond_t *)NULL)
	{
		perror("Error alocation for vector of condition variable");
		free(pt);
		free(pipe.mutex);
		return(-1);
	}
	if((pipe.counters=(int *)calloc(dim,sizeof(int)))==(int *)NULL)
	{
		perror("Error alocation for vector of flags");
		free(pt);
		free(pipe.mutex);
		free(pipe.cond);
		return(-1);
	}
	if((data=(ssystem_striped_fast *)calloc(thread,sizeof(ssystem_striped_fast)))==(ssystem_striped_fast *)NULL)
	{
		perror("Error alocation for arguments of threads");
		free(pt);
		free(pipe.mutex);
		free(pipe.cond);
		free(pipe.counters);
		return(-1);
	}
	if((workmat=(double **)calloc(dim,sizeof(double *)))==(double **)NULL)
	{
		perror("Error alocation adjounct matrix");
		free(pt);
		free(data);
		free(pipe.mutex);
		free(pipe.cond);
		free(pipe.counters);
		return(-1);
	}
	if((pworkmat=(double *)calloc(dim*(dim+1),sizeof(double)))==(double *)NULL)
	{
		perror("Error alocation adjounct matrix");
		free(pt);
		free(data);
		free(pipe.mutex);
		free(pipe.cond);
		free(pipe.counters);
		return(-1);	
	}
	for(i=0;i<dim;i++)
	{
		workmat[i]=pworkmat;
		pworkmat+=dim+1;
	}
	for(i=0;i<dim;i++)
	for(j=0;j<dim;j++)
		workmat[i][j]=mat[i][j];
	for(i=0;i<dim;i++)
		workmat[i][dim]=y[i];
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
		data[i].mat=workmat;
		data[i].N=dim;
		data[i].P=thread;
		data[i].who=i;
		data[i].pipe=&pipe;
		if(pthread_create(&pt[i],&attr,thread_gauss_striped_fast_ciclic,&data[i]))
		{
			perror("Can not create threads");
			free(pt);
			free(data);
			free(*workmat);
			free(workmat);
			free(pipe.counters);
			free(pipe.mutex);
			free(pipe.cond);
			return(-2);
		}
	}
	/* Waiting for finish the threads */
	for(i=0;i<thread;i++)
		pthread_join(pt[i],NULL);
	for(i=(dim-1);i>=0;i--)
	{
		x[i]=workmat[i][dim];
	}
	free(pt);
	free(data);
	free(*workmat);
	free(workmat);
	free(pipe.counters);
	barrier_destroy(&barrier_system);
	for(i=0;i<dim;i++)
	{
		pthread_mutex_destroy(&pipe.mutex[i]);
		pthread_cond_destroy(&pipe.cond[i]);
	}
	return(0);
}
