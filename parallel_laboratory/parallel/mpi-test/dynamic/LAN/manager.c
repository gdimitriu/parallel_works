#include<mpi.h>
#include<stdio.h>
int main(int argc,char **argv)
{
	int world_size,universe_size,*universe_sizep,flag,tid;
	MPI_Comm everyone;	//intercomunicator
	char worker_program[100]="worker";
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&world_size);
	if(world_size!=1) error("Top heavy width management");
	MPI_Attr_get(MPI_COMM_WORLD,MPI_UNIVERSE_SIZE,&universe_size,&flag);
	printf("Flag=%d universe=%d \n",flag,universe_size);fflush(stdout);
	flag=0;
	if(!flag)
	{
		printf("This MPI does not support UNIVERSE_SIZE. How many\n processes total?");fflush(stdout);
		scanf("%d",&universe_size);
		fflush(stdin);
		universe_size++;
	}
	if(universe_size==1) error("No room to start workers");
	MPI_Comm_spawn(worker_program,MPI_ARGV_NULL,universe_size-1,MPI_INFO_NULL,0,MPI_COMM_SELF,&everyone,MPI_ERRCODES_IGNORE);
	//MPI_Comm_rank(MPI_COMM_WORLD,&tid);	
	//	MPI_Comm_rank(everyone,&tid);
	MPI_Recv(&tid,1,MPI_INT,1,0,everyone,&status);
	printf("procesul master=%d\n",tid);fflush(stdout);
	MPI_Finalize();
	return(0);
}