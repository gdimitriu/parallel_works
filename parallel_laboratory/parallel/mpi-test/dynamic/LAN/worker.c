#include<mpi.h>
#include<stdio.h>
int main(int argc,char **argv)
{
	int size;
	MPI_Comm parent;
	int tid;
	MPI_Init(&argc,&argv);
	MPI_Comm_get_parent(&parent);
	if(parent==MPI_COMM_NULL) error("No parent!");
	MPI_Comm_remote_size(parent,&size);
	if(size!=1) error("Something's wrong with the parent");
	MPI_Comm_rank(MPI_COMM_WORLD,&tid);
	printf("world=%d ",tid);fflush(stdout);
	MPI_Comm_rank(parent,&tid);
	printf("total=%d\n",tid);fflush(stdout);
	if(tid==1) MPI_Send(&tid,1,MPI_INT,0,0,parent);
	MPI_Finalize();
	return(0);
}