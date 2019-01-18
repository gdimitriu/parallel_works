/* 
	La function pour faire elimation gaussiene dans une reseau avec MPI dynamique et avec les 
	processeurs dans un reseau ciclique.
	C'est le server.
*/
#include<parallel/parallel-mpi.h>
int main(int argc,char **argv)
{
	int size,rank;
	MPI_Comm parent;
	MPI_Init(&argc,&argv);
	char port_name[MPI_MAX_NAME_LEN];
if(argc==1)
{
	MPI_Comm_get_parent(&parent);
	MPI_Comm_remote_size(parent,&size);
	if((parent==MPI_COMM_NULL) || (size!=1))
	{
		perror("il n'a pas de pere ou il se passe quelque chose avec le pere");
		MPI_Finalize();
		return(-1);
	}
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
}
else
{
	
}	
	printf("rank=%d ",rank);fflush(stdout);
	MPI_Finalize();
	return(0);
}