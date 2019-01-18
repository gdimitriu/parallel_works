/*
	la function pour difusion de done dans un reseau checheboard
*/
#include<parallel/parallel-mpi.h>
void diffusion_sq_check_mpi(MPI_Comm mesh2_comm,int size,int rank,double **matin,double **matout,int partition)
{
/*
	size c'est le numero de processeurs
	rank c'est le rank de processeur dans le MPI_COMM_WORLD
	matin c'est la matrice complete
	matout c'est la matrice mesh
	partition c'est la doone qui a chacun processeur
*/
int coordonne[2];
int i,j,l,k;
MPI_Status status;
if(rank==0)
{
	for(i=0;i<partition;i++)
		for(j=0;j<partition;j++)
			matout[i][j]=matin[i][j];
	for(k=1;k<size;k++)
	{
		MPI_Cart_coords(mesh2_comm,k,2,coordonne);
		j=partition*coordonne[1];
		for(l=0;l<partition;l++)
		{
			i=partition*coordonne[0]+l;
			MPI_Send(&matin[i][j],partition,MPI_DOUBLE,k,0,MPI_COMM_WORLD);
		}
	}
}
else
    {
	for(l=0;l<partition;l++)
		MPI_Recv(&matout[l][0],partition,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
    }
}