/*
	le dossier pour diffusion de done dans le striped et ciclique patition dans
	implementation de MPI
*/
#include<parallel/parallel-mpi.h>
void diffusion_sq_striped_ciclique_mpi(int rank,int size,double **mat,double **augumente,int *ciclique,int variable,int type)
/*
	rank c'est le rank de processor
	size c'est le numero des processerus
	mat c'est la matrice complete dans le process principale
	augumente c'est la matrice partitione dans le client
	ciclique c'est le vector qui retien le numero de la ligne qui sont dans cette matrice
	variable c'est le numero de variable dans le system
	type c'est si la mtrice c'est normale ou ranverse (0 ou 1) pour a ou b
*/
{
int nr,partition;
MPI_Status status;
int i,j,l,m;
int temp=0;
   nr=variable%size;
   partition=(variable-nr)/size;
if(type==0)
	{
if(rank==0)
		{
			j=0;
			m=0;
			for(l=0;l<partition;l++)
			{
				memcpy(augumente[m],mat[j],sizeof(double)*variable);
				ciclique[l]=j;
				j++;m++;
				for(i=1;i<size;i++)
   			{
   				MPI_Send(mat[j],variable,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
   				MPI_Send(&j,1,MPI_INT,i,2,MPI_COMM_WORLD);
			   	j++;
   			}
   		}
		 	m=partition;
   		if(nr>0)
   		{
	   		memcpy(augumente[m],mat[j],sizeof(double)*variable);
				ciclique[m]=j;
				j++;
			}
   		for(l=1;l<nr;l++)
	   	{
				MPI_Send(mat[j],variable,MPI_DOUBLE,l,0,MPI_COMM_WORLD);
  				MPI_Send(&j,1,MPI_INT,l,2,MPI_COMM_WORLD);
   			j++;
		   }	   
		}
else
		{
			if(rank<nr) partition++;
			for(i=0;i<partition;i++)
			{
				MPI_Recv(augumente[i],variable,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
				MPI_Recv(&ciclique[i],1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
			}
		}
	}
else
	{

	}
}	