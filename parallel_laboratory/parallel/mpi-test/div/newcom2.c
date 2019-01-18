/*
	program for testing new send,recive and bcast
	il est non-preemptive et il marche biensur
*/
#include<stdio.h>
#include<mpi.h>
#include<pth.h>
#include<stdlib.h>
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
pth_barrier_t barrier;
void *bcastp(void *p)
{
int rank,i;
int sync=10;
MPI_Request request;
MPI_Status status;
datas arg=*(datas *)p;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	pth_barrier_reach(&barrier);
	if(arg.source==rank)
		MPI_Bcast(&sync,1,MPI_INT,rank,MPI_COMM_WORLD);
	else
		MPI_Bcast(&sync,1,MPI_INT,arg.source,MPI_COMM_WORLD);
}


int main(int argc,char **argv)
{
int i;
int rank,size;
datas *data;
pth_t *pth;
int buff1[2],buff2[2];
pth_attr_t *attr;
	MPI_Init(&argc,&argv);
	pth_init();
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	pth_barrier_init(&barrier,size);
	pth=(pth_t *)calloc(size,sizeof(pth_t));
	data=(datas *)calloc(size,sizeof(datas));
	attr=(pth_attr_t *)calloc(size,sizeof(pth_attr_t));
	buff1[0]=rank;
	buff1[1]=rank+1;
	for(i=0;i<size;i++)
	{
		data[i].count=2;
		data[i].datatype=MPI_INT;
		data[i].input=buff1;
		data[i].output=buff2;
		data[i].source=i;
		data[i].size=size;
		attr[i]=pth_attr_new();
		pth_attr_set(attr[i],PTH_ATTR_NAME,"bcastp");
		pth_attr_set(attr[i],PTH_ATTR_STACK_SIZE,64*1024);
		pth_attr_set(attr[i],PTH_ATTR_JOINABLE,TRUE);
		pth[i]=pth_spawn(attr[i],bcastp,&data[i]);
	}
	for(i=0;i<size;i++)
		pth_join(pth[i],NULL);
	printf("rank %d c'est fini\n",rank);fflush(stdout);
	MPI_Finalize();
	free(pth);
	free(data);
	free(attr);
}

