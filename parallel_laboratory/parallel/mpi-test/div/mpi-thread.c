/*
	le test pour verifie la MPI thread
	c'est pour utilize dans le project CNN
*/
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
int send_data[100];
int recv_data[100];
int send_rank,recv_rank;
int rank;
int procs;
void *send_th(void *input)
{
MPI_Status status;
int temp;
	MPI_Send(&send_rank,1,MPI_INT,*(int *)input,0,MPI_COMM_WORLD);
	printf("proces %d send to %d\n",rank,(*(int *)input));fflush(stdout);
	MPI_Send(send_data,100,MPI_INT,*(int *)input,0,MPI_COMM_WORLD);
	MPI_Recv(&temp,1,MPI_INT,*(int *)input,0,MPI_COMM_WORLD,&status);
}

void *recv_th(void *input)
{
MPI_Status status;
	MPI_Recv(&recv_rank,1,MPI_INT,*(int *)input,0,MPI_COMM_WORLD,&status);
	printf("proces %d receive from %d\n",rank,recv_rank);fflush(stdout);
	MPI_Recv(recv_data,100,MPI_INT,*(int *)input,0,MPI_COMM_WORLD,&status);
	MPI_Send(&rank,1,MPI_INT,*(int *)input,0,MPI_COMM_WORLD);
}

int main(int argc,char **argv)
{
int i;
int *data;
int provide;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&procs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	data=(int *)calloc(procs,sizeof(int));
	printf("%d\n",provide);fflush(stdout);
	for(i=0;i<100;i++)
		send_data[i]=i;
if(rank==0)
{
	for(i=1;i<procs;i++)
	{
		data[i]=i;
//		pthread_create(&pt[i],NULL,send_th,&data[i]);
	}
}
else
{
	data[0]=0;
//	pthread_create(pth,NULL,recv_th,&data[0]);
}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
}