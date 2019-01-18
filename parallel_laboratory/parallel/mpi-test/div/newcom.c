/*
	program for testing new send,recive and bcast
*/
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<mpi.h>
#include<parallel.h>
typedef struct
{
	int dest;
	int source;
	int size;
} datas;
barrier_t barrier_test;
void *sendp(void *p)
{
int rank;
datas arg=*(datas *)p;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Send(&rank,1,MPI_INT,arg.dest,arg.source,MPI_COMM_WORLD);
	printf("send from %d to %d %d\n",arg.source,arg.dest,rank);fflush(stdout);
	pthread_exit(NULL);
}
void *recvp(void *p)
{
int rank;
MPI_Status status;
datas arg=*(datas *)p;
	MPI_Recv(&rank,1,MPI_INT,arg.source,arg.source,MPI_COMM_WORLD,&status);
	printf("recv at %d from %d data %d\n",arg.dest,arg.source,rank);fflush(stdout);
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
	
		data[0].source=rank;
		data[0].dest=rank+1;
		if(data[0].dest==size) data[0].dest=0;
		data[0].size=size;
		pthread_create(&pth[0],NULL,&sendp,&data[0]);
//		pthread_detach(pth[0]);
		data[1].source=rank-1;
		if(data[1].source<0) data[1].source=size-1;
		data[1].dest=rank;
		pthread_create(&pth[1],NULL,&recvp,&data[1]);
//		pthread_detach(pth[1]);
		
	for(i=0;i<2;i++)
		pthread_join(pth[i],NULL);
//	printf("rank %d c'est fini\n",rank);fflush(stdout);
//	MPI_Barrier(MPI_COMM_WORLD);
//	printf("rank %d c'est fini apres barrier\n",rank);fflush(stdout);
	barrier_destroy(&barrier_test);
	MPI_Finalize();
	free(pth);
	free(data);
}

