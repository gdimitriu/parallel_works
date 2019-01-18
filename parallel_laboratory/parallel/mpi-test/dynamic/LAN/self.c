#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc,char **argv)
{
	MPI_Comm everyone;	//intercomunicator
	MPI_Comm	new_world;	//coresponding intecomunicator
	MPI_Comm parent;		//parent
	int size,nslots,flag,tid;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Attr_get(MPI_COMM_WORLD,MPI_UNIVERSE_SIZE,&nslots,&flag);
	nslots=10;
	if(size==1)	//I'm the parent
	{
		MPI_Comm_spawn("self",MPI_ARGV_NULL,nslots-1,MPI_INFO_NULL,0,MPI_COMM_SELF,&everyone,NULL);
		MPI_Intercomm_merge(everyone,1,&new_world);
	}
	else
	{
		MPI_Comm_get_parent(&parent);
		MPI_Intercomm_merge(parent,0,&new_world);
	}
	MPI_Comm_rank(new_world,&tid);
	printf("world=%d size=%d\n",tid,size);fflush(stdout);
	MPI_Finalize();
}