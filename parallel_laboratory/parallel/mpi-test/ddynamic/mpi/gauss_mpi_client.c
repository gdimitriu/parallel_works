/* 
	La function pour faire elimation gaussiene dans une reseau avec MPI dynamique et avec les 
	processeurs dans un reseau ciclique.
	C'est le client.
*/
#include<parallel/parallel-mpi_dyn.h>
	int gauss_mpi(int dimension,int numero,double **matrice,double *libre,double *variable,int type,int argc,char **argv)
	/*
		dimension c'est la dimension de reseau
		numero c'est le numero des equations
		matrice c'est la mtrice des equations
		libre c'est le termen libre des equations
		variable c'est le resoudre des equations
		type c'est le type de dynamique
	*/	
{
	int size;
	MPI_Comm toutes;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(size!=1)
	{
		perror("il a boucoup de processors");
		MPI_Finalize();
		return(-1);
	}
	MPI_Comm_spawn("../../ddynamic/bin/numeric",MPI_ARGV_NULL,dimension,MPI_INFO_NULL,0,MPI_COMM_SELF,&toutes,MPI_ERRCODES_IGNORE);
	printf("%d \n",argc);fflush(stdout);
	MPI_Finalize();
	return(0);
}