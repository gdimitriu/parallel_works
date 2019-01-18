/*
	le test pour verifie la MPI thread
	c'est pour utilize dans le project CNN
*/
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<curses.h>
int send_data[100];
int recv_data[100];
int send_rank,recv_rank;
int rank;
int procs;

int main(int argc,char **argv)
{
int i;
int *data;
int provide;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&procs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank==0)
	{
		initscr();cbreak();noecho();
		beep();
		endwin();
	}
	else
	sleep(2);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
}