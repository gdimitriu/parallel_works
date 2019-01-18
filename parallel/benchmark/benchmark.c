/*
	Program for banckmark for MPI
*/
#include<stdlib.h>
#include<stdio.h>
#include<mpi.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#define NR 10000
int data[NR];
FILE *flength;

long compute(struct timeval t1ser,struct timeval t2ser)
{
long zecimal1,intreg1;
long operand1;
	if(t1ser.tv_usec>t2ser.tv_usec)
	{
		zecimal1=1000000+t2ser.tv_usec-t1ser.tv_usec;
		intreg1=t2ser.tv_sec-t1ser.tv_sec-1;
	}
	else
	{
		zecimal1=t2ser.tv_usec-t1ser.tv_usec;
		intreg1=t2ser.tv_sec-t1ser.tv_sec;
	}
	operand1=1000000*intreg1+zecimal1;
	return(operand1);
}

int main(int argc,char **argv)
{
int rank,nrprocs;
struct timeval start;
struct timeval stop;
MPI_Datatype timestruct;
int blocklens[2];
MPI_Aint indices[2];
MPI_Datatype old_types[2];
MPI_Status status;
int length;
int i;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nrprocs);
	blocklens[0]=1; blocklens[1]=1;
	old_types[0]=MPI_LONG; old_types[1]=MPI_LONG;
	MPI_Address(&stop.tv_sec,&indices[0]);
	MPI_Address(&stop.tv_usec,&indices[1]);
	indices[1]-=indices[0];
	indices[0]=0;
	MPI_Type_struct(2,blocklens,indices,old_types,&timestruct);
	MPI_Type_commit(&timestruct);
	if(rank==0)
	{
		flength=(FILE *)fopen("length.dat","w");
		for(length=1;length<NR;length+=10)
		{
			for(i=0;i<length;i++) data[i]=i;
			gettimeofday(&start,NULL);
			MPI_Send(&data,length,MPI_INT,1,0,MPI_COMM_WORLD);
			MPI_Recv(&stop,1,timestruct,1,1,MPI_COMM_WORLD,&status);
			fprintf(flength,"%d %ld\n",length,compute(start,stop));
		}
		fclose(flength);
	}
	else
	{
		for(length=1;length<NR;length+=10)
		{
			for(i=0;i<length;i++) data[i]=i;
			MPI_Recv(&data,length,MPI_INT,0,0,MPI_COMM_WORLD,&status);
			gettimeofday(&stop,NULL);
			MPI_Send(&stop,1,timestruct,0,1,MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
}