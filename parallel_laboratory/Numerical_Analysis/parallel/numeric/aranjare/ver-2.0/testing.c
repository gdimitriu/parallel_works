/* Program for testing sorting and extrem*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include"definitii.h"
#include"fisier.h"
#include"calcul.h"
#include"acceleration.h"
#include<parallel/parallel-pthread.h>
#include<parallel/parallel-mpi.h>
//#include<parallel/parallel-pth.h>
#define P 2


int main(int argc,char **argv)
{
double output;
int position,i;
struct timeval t1s,t2s,t1p,t2p;
double temp;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(rank==0)
	{
		for(i=0;i<128;i++) nume[i]=0;
		write(1,"Introduceti numele fisierului:",30*sizeof(char));
		gets(nume);
		read_data(nume);
		gettimeofday(&t1s,NULL);
		calculez(P,variabila,&output,&position,1);
		gettimeofday(&t2s,NULL);
		printf("Serial %e at %d\n",output,position);fflush(stdout);
		output=0.0;
		position=0;
		gettimeofday(&t1p,NULL);
		extreme(variabila,P,vector,1,&output,&position);
		gettimeofday(&t2p,NULL);
		acceleration(t1s,t2s,t1p,t2p,P);
		printf("Parallel with thread %e at %d\n",output,position);fflush(stdout);
		/* 	
			MPI implementation
		*/
		write(1,"MPI:\n",5*sizeof(char));
	}
	output=0.0;
	position=0;
	MPI_Bcast(nume,128,MPI_CHAR,0,MPI_COMM_WORLD);
	if(rank!=0)
	{
		read_data(nume);
	}
	gettimeofday(&t1p,NULL);
	extreme_mpi(variabila,vector,&output,&position,1);
	gettimeofday(&t2p,NULL);
	if(rank==0)
	{
		acceleration(t1s,t2s,t1p,t2p,size);
		printf("Parallel %e at %d\n",output,position);fflush(stdout);
		write(1,"MPI link:\n",10*sizeof(char));
	}
	output=0.0;
	position=0;
	MPI_Bcast(nume,128,MPI_CHAR,0,MPI_COMM_WORLD);
	if(rank!=0)
	{
		read_data(nume);
	}
	gettimeofday(&t1p,NULL);
	extreme_link_mpi(variabila,vector,&output,&position,1);
	gettimeofday(&t2p,NULL);
	if(rank==0)
	{
		acceleration(t1s,t2s,t1p,t2p,size);
		printf("Parallel link %e at %d\n",output,position);fflush(stdout);
	}
	output=0.0;
	position=0;
	MPI_Bcast(nume,128,MPI_CHAR,0,MPI_COMM_WORLD);
	if(rank!=0)
	{
		read_data(nume);
	}
	gettimeofday(&t1p,NULL);
	extreme_link1_mpi(variabila,vector,&output,&position,1);
	gettimeofday(&t2p,NULL);
	if(rank==0)
	{
		acceleration(t1s,t2s,t1p,t2p,size);
		printf("Parallel link1 %e at %d\n",output,position);fflush(stdout);
	}
	free(vector);
	MPI_Finalize();
	return 0;
}
