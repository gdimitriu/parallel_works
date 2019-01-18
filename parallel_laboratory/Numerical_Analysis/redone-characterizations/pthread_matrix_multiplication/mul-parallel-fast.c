/*
	Execute the gauss elimination on square matrix with processors on striped partioning
*/
#include<solve-pthread.h>
/*
	data for solve in rowwise or columnwise partitioning
*/
typedef struct
{
	double **mat;
	double **mat2;
	long who;				/* nr of courent thread*/
	long N;				/* dimensions of matrix */
	int P;				/* nr of threads */
} ssystem_striped_fast;

void *thread_mul_striped_fast_ciclic(void *p)
{
	ssystem_striped_fast data=*(ssystem_striped_fast *)p;
	long nr;
	long last=0;	/* if this thread have the last line */
	long i,j,k,l;	/* counters */
	/* 
		Make setings for partitioning 
	*/
	nr=data.N%data.P;
	/* for the last row */
	if((data.who+1)==nr) 
		last=1;
	if((nr==0) && (data.who+1)==data.P) last=1;
	
	/* 
		making alocation for vector of cicle variable 
	*/
	for(j=data.who;j<data.N;j+=data.P)
	for(k=0;k<data.N;k++) data.mat2[j][k]=data.mat[j][k];
	for(i=0;i<3;i++)
	{
		for(j=data.who;j<data.N;j+=data.P)
		for(k=0;k<data.N;k++) data.mat2[j][k]=data.mat2[j][k]*data.mat[j][k];
	}
	pthread_exit(NULL);
}


int mul_striped_fast(long dim,int thread,double **mat,double **mat2)
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
	long i;
	pthread_attr_t attr;
	/* Making alocations for data */
	pt=(pthread_t *)calloc(thread,sizeof(pthread_t));
	data=(ssystem_striped_fast *)calloc(thread,sizeof(ssystem_striped_fast));
	pthread_attr_init(&attr);
	/*
		Create threads
	*/
	for(i=0;i<thread;i++)
	{
		data[i].mat=mat;
		data[i].mat2=mat2;
		data[i].N=dim;
		data[i].P=thread;
		data[i].who=i;
		pthread_create(&pt[i],&attr,thread_mul_striped_fast_ciclic,&data[i]);
	}
	/* Waiting for finish the threads */
	for(i=0;i<thread;i++)
		pthread_join(pt[i],NULL);
	free(pt);
	free(data);
	return(0);
}
