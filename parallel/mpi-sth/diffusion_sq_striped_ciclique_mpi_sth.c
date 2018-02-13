/*
	le dossier pour diffusion de done dans le striped et ciclique patition dans
	implementation de MPI avec STHREAD
*/
#include<parallel/parallel-mpi-sth.h>
int diffusion_sq_striped_ciclique_mpi_sth_th1(double **mat,int j,int i, int variable)
{
   MPI_Send(mat[j],variable,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
   MPI_Send(&j,1,MPI_INT,i,2,MPI_COMM_WORLD);
   st_join_counter_finish(diffusion_sth);
}
void diffusion_sq_striped_ciclique_mpi_sth(int rank,int size,double **mat,double **augumente,int *ciclique,int variable,int type)
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
			diffusion_sth=(st_join_counter_t *)calloc(size-1,sizeof(st_join_counter_t));
			for(l=0;l<partition;l++)
			{
				memcpy(augumente[m],mat[j],sizeof(double)*variable);
				ciclique[l]=j;
				j++;m++;
				st_join_counter_init(diffusion_sth,size-1);
				for(i=1;i<size;i++)
   			{
   				ST_THREAD_CREATE(diffusion_sq_striped_ciclique_mpi_sth_th1(mat,j,i,variable));
			   	j++;
   			}
   			st_join_counter_wait(diffusion_sth);
   		}
   		free(diffusion_sth);
		 	m=partition;
   		if(nr>0)
   		{
	   		memcpy(augumente[m],mat[j],sizeof(double)*variable);
				ciclique[m]=j;
				j++;
			}
			if(nr>1)
			{
				diffusion_sth=(st_join_counter_t *)calloc(nr-1,sizeof(st_join_counter_t));
				st_join_counter_init(diffusion_sth,nr-1);
   			for(l=1;l<nr;l++)
		   	{
		   		ST_THREAD_CREATE(diffusion_sq_striped_ciclique_mpi_sth_th1(mat,j,i,variable));
   				j++;
			   }
			   st_join_counter_wait(diffusion_sth);
			   free(diffusion_sth);
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
}	