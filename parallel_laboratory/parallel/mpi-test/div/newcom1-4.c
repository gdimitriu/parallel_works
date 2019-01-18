/*
	program for testing new send,recive and bcast
*/
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<mpi.h>
typedef struct
{
	int *buf;
	int count;
	MPI_Datatype datatype;
	int dest;
	int source;
	int tag;
	MPI_Status *status;
} datas;

void *sendp(void *p)
{
	datas arg=*(datas *)p;
	printf("send %d %d\n",arg.buf[0],arg.buf[1]);fflush(stdout);
	MPI_Send(arg.buf,arg.count,arg.datatype,arg.dest,arg.tag,MPI_COMM_WORLD);
	pthread_exit(NULL);
}
void *recvp(void *p)
{
	datas arg=*(datas *)p;
	MPI_Recv(arg.buf,arg.count,arg.datatype,arg.source,arg.tag,MPI_COMM_WORLD,arg.status);
	printf("recv %d %d\n",arg.buf[0],arg.buf[1]);fflush(stdout);
	pthread_exit(NULL);
}
void *bcastp(void *p)
{
	datas arg=*(datas *)p;
	MPI_Bcast(arg.buf,arg.count,arg.datatype,arg.source,MPI_COMM_WORLD);
	printf("recv %d %d\n",arg.buf[0],arg.buf[1]);fflush(stdout);
	pthread_exit(NULL);
}


int main(int argc,char **argv)
{
int rank,size;
datas data;
MPI_Status statusdata;
pthread_t thread1,thread2;
	data.buf=(int *)calloc(2,sizeof(int));
	data.status=&statusdata;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(rank==0)
	{
		data.buf[0]=10;
		data.buf[1]=11;
	}
	data.count=2;
	data.source=0;
	data.dest=1;
	data.tag=0;
	data.datatype=MPI_INT;
	printf("passed %d %d\n",data.buf[0],data.buf[1]);fflush(stdout);
//	if(rank==0) pthread_create(&thread1,NULL,sendp,&data);
//	else pthread_create(&thread2,NULL,recvp,&data);
	pthread_create(&thread1,NULL,bcastp,&data);
//	if(rank==0)	pthread_join(thread1,NULL);
//	else pthread_join(thread2,NULL);
	pthread_join(thread1,NULL);
	MPI_Finalize();
	free(data.buf);
}

