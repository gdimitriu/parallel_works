/* 
	La function pour faire la combination d'une matrice striped ciclique
	dans l'implementation de MPI clasique
*/
#include<parallel/parallel-mpi.h>
void combine_sq_striped_ciclique_mpi(int rank,int size,int variable,int *ciclique,double **matrix,double **tmatrix)
/*
	rank c'est le rank de la machine
	size c'est le numero de lignes dans la matrice originale
	partition c'est le numero de ligne qui sont dans ce machine
*/
{
int nr,temp;
int i,j,k; //counters
int partition;
MPI_Status status;
	nr=variable%size;
	partition=(variable-nr)/size;
	if(rank!=0)
	{	
		if(rank<nr) partition++;
		for(i=0;i<partition;i++)
		{
			MPI_Send(&ciclique[i],1,MPI_INT,0,0,MPI_COMM_WORLD);
			MPI_Send(matrix[i],variable,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
		}
	}
	else
	{
		if(nr>0) temp=partition+1;
		else temp=partition;
		for(i=0;i<temp;i++)
		{
			memcpy(tmatrix[ciclique[i]],matrix[i],variable*sizeof(double));
		} 
		for(i=1;i<size;i++)
		{
			if(i<nr) temp=partition+1;
			else temp=partition;
			for(k=0;k<temp;k++)
			{
				MPI_Recv(&j,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);	
				MPI_Recv(tmatrix[j],variable,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&status);
			}
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
}