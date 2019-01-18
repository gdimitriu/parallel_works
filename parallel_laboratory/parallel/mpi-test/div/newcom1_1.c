/*
	program for testing new send,recive and bcast
	il est preemptive et il ne marche pas
*/
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<mpi.h>
#include<parallel/parallel-pthread.h>
typedef struct
{
	int *input;
	int count;
	MPI_Datatype datatype;
	int dest;
	int source;
	int *output;
	int size;
} datas;
barrier_t barrier_test;

void *bcastp(void *p)
{
int rank,i;
int sync=10;
MPI_Request request;
MPI_Status status;
datas arg=*(datas *)p;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	barrier_sync(&barrier_test);

	if(arg.source==rank)
	{
		for(i=0;i<arg.size;i++)
		{
//			printf("rank %d source %d to %d\n",rank,arg.source,i);fflush(stdout);
			sync=rank;
			if(i!=rank)
			{
//				printf("send rank %d source %d to %d with tag %d have %d\n",rank,arg.source,i,rank,sync);fflush(stdout);
				MPI_Send(&sync,1,MPI_INT,i,arg.source,MPI_COMM_WORLD);
			}
		}
	}
	if(arg.source!=rank)
	{
//		printf("before rank %d source %d from %d with tag %d\n",rank,arg.source,arg.source,arg.source);fflush(stdout);
		MPI_Irecv(&sync,1,MPI_INT,arg.source,arg.source,MPI_COMM_WORLD,&request);
		MPI_Waitall(1,&request,&status);
//		MPI_Recv(&sync,1,MPI_INT,arg.source,arg.source,MPI_COMM_WORLD,&status);
		printf("recv rank %d source %d have %d\n",rank,arg.source,sync);fflush(stdout);
	}
/*
	if(arg.source==rank)
		MPI_Bcast(&sync,1,MPI_INT,rank,MPI_COMM_WORLD);
	else
		MPI_Bcast(&sync,1,MPI_INT,arg.source,MPI_COMM_WORLD);
*/
	barrier_sync(&barrier_test);
	pthread_exit(NULL);
}


int main(int argc,char **argv)
{
int i;
int rank,size;
datas *data;
pthread_t *pth;
int buff1[2],buff2[2];
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	pth=(pthread_t *)calloc(size,sizeof(pthread_t));
	data=(datas *)calloc(size,sizeof(datas));
	buff1[0]=rank;
	buff1[1]=rank+1;
	barrier_init(&barrier_test,size);
	for(i=0;i<size;i++)
	{
		data[i].count=2;
		data[i].datatype=MPI_INT;
		data[i].input=buff1;
		data[i].output=buff2;
		data[i].source=i;
		data[i].size=size;
		pthread_create(&pth[i],NULL,bcastp,&data[i]);
	}
	for(i=0;i<size;i++)
		pthread_join(pth[i],NULL);
	printf("rank %d c'est fini\n",rank);fflush(stdout);
	MPI_Barrier(MPI_COMM_WORLD);
	printf("rank %d c'est fini apres barrier\n",rank);fflush(stdout);
	barrier_destroy(&barrier_test);
	MPI_Finalize();
	free(pth);
	free(data);
}

