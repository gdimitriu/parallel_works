/*
	program for testing new send,recive and bcast
	il est preemptive et il ne marche pas
*/
#include<stdio.h>
#include<st.h>
#include<stdlib.h>
#include<mpi.h>
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
st_join_counter_t *joinc;
void bcastp(datas arg)
{
int rank,i;
int sync=10;
MPI_Request request;
MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(arg.source==rank)
	{
		for(i=0;i<arg.size;i++)
		{
			printf("rank %d source %d to %d\n",rank,arg.source,i);fflush(stdout);
			sync=rank;
			if(i!=rank)
			{
				printf("send rank %d source %d to %d with tag %d have %d\n",rank,arg.source,i,rank,sync);fflush(stdout);
				MPI_Send(&sync,1,MPI_INT,i,arg.source,MPI_COMM_WORLD);
			}
		}
	}
	if(arg.source!=rank)
	{
		printf("before rank %d source %d from %d with tag %d\n",rank,arg.source,arg.source,arg.source);fflush(stdout);
		MPI_Recv(&sync,1,MPI_INT,arg.source,arg.source,MPI_COMM_WORLD,&status);
		printf("recv rank %d source %d have %d\n",rank,arg.source,sync);fflush(stdout);
	}
	st_join_counter_finish(joinc);
}


int st_main(int argc,char **argv)
{
int i;
int rank,size;
datas *data;
int buff1[2],buff2[2];
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	data=(datas *)calloc(size,sizeof(datas));
	joinc=(st_join_counter_t *)calloc(size,sizeof(st_join_counter_t));
	buff1[0]=rank;
	buff1[1]=rank+1;
	st_join_counter_init(joinc,size);
	for(i=0;i<size;i++)
	{
		data[i].count=2;
		data[i].datatype=MPI_INT;
		data[i].input=buff1;
		data[i].output=buff2;
		data[i].source=i;
		data[i].size=size;
		ST_THREAD_CREATE(bcastp(data[i]));
	}
	printf("rank %d c'est fini\n",rank);fflush(stdout);
	st_join_counter_wait(joinc);
	MPI_Barrier(MPI_COMM_WORLD);
	printf("rank %d c'est fini apres barrier\n",rank);fflush(stdout);
	MPI_Finalize();
	free(data);
}

