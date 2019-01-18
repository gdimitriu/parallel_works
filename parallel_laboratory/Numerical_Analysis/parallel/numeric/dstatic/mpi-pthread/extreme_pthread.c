/* 
	file for function of extrem in MPI implementation
*/
#include<stdio.h>
#include<parallel/parallel-mpi-pthread.h>
typedef struct
{
	int *type;
	int dela;
	int *pos;
	double *val;
} internal_extreme;
pthread_mutex_t mutex_extreme;
void *psend(void *input)
{
internal_extreme *dataint=(internal_extreme *)input;
	MPI_Send(dataint->pos,1,MPI_INT,0,0,MPI_COMM_WORLD);
	MPI_Send(dataint->val,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
}
void *precv(void *input)
{
internal_extreme *dataint=(internal_extreme *)input;
struct 
{
	int pos;
	double val;
} temp;
MPI_Status status;
		MPI_Recv(&(temp.pos),1,MPI_INT,dataint->dela,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&(temp.val),1,MPI_DOUBLE,dataint->dela,0,MPI_COMM_WORLD,&status);
		pthread_mutex_lock(&mutex_extreme);
		if(temp.val<*(dataint->val))
		{
			if(*(dataint->type)==0)
			{
				*(dataint->val)=temp.val;
				*(dataint->pos)=temp.pos;
			}
		}
		else
		{
			if(*(dataint->type)==1)
			{
				*(dataint->val)=temp.val;
				*(dataint->pos)=temp.pos;
			}
		}
		pthread_mutex_unlock(&mutex_extreme);
}
int extreme_mpi_pthread(int dim,double *vector,double *output,int *position,int type)
{
int thread;		/* le numeros des processors */
int numero;		/* le numero de processor */
int start;		/* le point de partir	 */
int stop;		/* le point de arrete */
int nr;			/* le numero des elements dans le thread */
int i;			/* le contor */
pthread_t *pth_recv;
pthread_t pth_send;
internal_extreme *data;
MPI_Status status;
int rank,size;
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Comm_size(MPI_COMM_WORLD,&size);
if(rank==0)
	data=(internal_extreme *)calloc(size,sizeof(internal_extreme));
else
	data=(internal_extreme *)calloc(1,sizeof(internal_extreme));
	numero=rank-1;
	thread=size-1;
   /* calcule le constants de partagement */
	nr=dim%thread;
	if((numero!=(thread-1))&&(numero!=0))
	{
		start=numero*nr;
		stop=start+nr;
	}
	if(numero==0)
	{
		start=0;
		stop=start+nr;
	}
	if(numero==(thread-1))
	{
		start=numero*nr;
		stop=dim;
		nr=stop-start+1;
	}
	*output=vector[start];
	*position=start;
	/* zone pour le extrem (zone parallele) */
	for(i=start;i<stop;i++)
	{
		if(vector[i]<*output)
		{
			if(type==0)
			{
				*output=vector[i];
				*position=i;
			}
		}
		else
		{
			if(type==1)
			{
				*output=vector[i];
				*position=i;
			}
		}
	}

	if(rank==0)
	{
		pth_recv=(pthread_t *)calloc(size,sizeof(pthread_t));
		pthread_mutex_init(&mutex_extreme,NULL);
		for(i=1;i<size;i++)
		{
			data[i].val=output;
			data[i].pos=position;
			data[i].dela=i;
			data[i].type=&type;
			pthread_create(&pth_recv[i],NULL,precv,(void *)&data[i]);
		}
		for(i=1;i<size;i++)
			pthread_join(pth_recv[i],NULL);
		free(pth_recv);
		pthread_mutex_destroy(&mutex_extreme);
	}
	else
	{
		data[0].val=output;
		data[0].pos=position;
		data[0].dela=rank;
		pthread_create(&pth_send,NULL,psend,(void *)data);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	free(data);
	return(0);
}