/*
	Execute the calcul of find the extreme (maximum or minimum) of a vector of 
	numbers.
*/

#include<parallel/parallel-pthread.h>
typedef struct 
{
	int type;			/* type of extreme */
	/*
		1 for maximum;
		0 for minumum;
	*/
	double *vector;	/* pointer to data vector */
	int N;				/* number of elements */
	int P;				/* number of threads */
	int who;				/* nr of thread which it is */
	double *extreme;	/* value of extreme */
	int *position;		/* position of extreme */
} sextreme;

void *pthread_extreme(void *p)
{
	sextreme data=*(sextreme *)p;
	int start;			/*	point of start */
	int stop;			/* point of stop */
	int nr;				/* nr of elements per thread */
	int i;				/* counter */
  	/*
  		Make setings for partitioning
  	*/
	nr=data.N%data.P;
	if((data.who!=(data.P-1))&&(data.who!=0))
	{
		start=data.who*nr;
		stop=start+nr;
	}
	if(data.who==0)
	{	
		start=0;
		stop=start+nr;
	}
	if(data.who==(data.P-1))
	{
		start=data.who*nr;
		stop=data.N;
		nr=stop-start+1;
	}
	*(data.extreme)=data.vector[start];
	*(data.position)=start;
	for(i=start;i<stop;i++)
	{
		if(data.vector[i]<*(data.extreme))
		{
			if(data.type==0)
			{
				*(data.extreme)=data.vector[i];
				*(data.position)=i;
			}
		}
		else
		{
			if(data.type==1)
			{
				*(data.extreme)=data.vector[i];
				*(data.position)=i;
			}
		}
	}
	pthread_exit(NULL);
}

int extreme(int dim,int thread,double *vector,int type,double *extremeout,int *position)
{
	/*
		dim is dimension of vector of numbers
		thread is number of threads
		vector is a pointer to vector of numbers
		type is 0 for minimum and 1 for maximum
		extremeout is the output data
		position is the position of extreme
	*/
	register int i;
	pthread_t *pt;
	sextreme *data;
	
	/*
		make alocation for variable
	*/
	if((pt=(pthread_t *)calloc(thread,sizeof(pthread_t *)))==(pthread_t *)NULL)
	{
		perror("Error alocation for vector of pthreadid");
		return(-1);
	}
	if((data=(sextreme *)calloc(thread,sizeof(sextreme)))==(sextreme *)NULL)
	{
		perror("Error alocation for vector of data");
		free(pt);
		return(-1);
	}
	/*
		Making threads
	*/
	for(i=0;i<thread;i++)
	{
		data[i].P=thread;
		data[i].N=dim;
		data[i].who=i;
		data[i].vector=vector;
		data[i].type=type;
		data[i].extreme=(double *)calloc(1,sizeof(double));
		data[i].position=(int *)calloc(1,sizeof(int));
		if(pthread_create(&pt[i],NULL,pthread_extreme,&data[i]))
		{
			perror("Can't create threads");
			free(pt);
			free(data);
			return(-2);
		}
	}
	/*
		Waiting for finish the threads
	*/
	for(i=0;i<thread;i++)
		pthread_join(pt[i],NULL);
	*extremeout=*(data[0].extreme);
	*position=*(data[0].position);
	for(i=0;i<thread;i++)
	{
		if(*(data[i].extreme)<(*extremeout))
		{
			if(type==0)
			{
				*extremeout=*(data[i].extreme);
				*position=*(data[i].position);
			}
		}
		else
		{
			if(type==1)
			{
				*extremeout=*(data[i].extreme);
				*position=*(data[i].position);
			}
		}
	}
	for(i=0;i<thread;i++)
	{
		free(data[i].extreme);
		free(data[i].position);
	}
	free(pt);
	free(data);
	return(0);
}