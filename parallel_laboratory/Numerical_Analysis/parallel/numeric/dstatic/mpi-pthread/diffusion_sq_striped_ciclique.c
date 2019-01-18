/*
	le dossier pour diffusion de done dans le striped et ciclique patition dans
	implementation de MPI avec STHREAD
*/
#include<parallel/parallel-mpi-pthread.h>
typedef struct
{
	int i;
	int j;
} diffusion_sq_data;
double **diffusion_sq_mat;
int diffusion_sq_variable;
void *diffusion_sq_striped_ciclique_mpi_sth_th1(void *input)
{
diffusion_sq_data data=*(diffusion_sq_data *)input;
   MPI_Send(diffusion_sq_mat[data.j],diffusion_sq_variable,MPI_DOUBLE,data.i,0,MPI_COMM_WORLD);
   MPI_Send(&data.j,1,MPI_INT,data.i,2,MPI_COMM_WORLD);
   pthread_exit(NULL);
}
void diffusion_sq_striped_ciclique_mpi_pthread(int rank,int size,double **mat,double **augumente,int *ciclique,int variable,int type)
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
pthread_t *pth;
diffusion_sq_data *data;
   nr=variable%size;
   partition=(variable-nr)/size;
if(type==0)
	{
if(rank==0)
		{
			j=0;
			m=0;
			diffusion_sq_mat=mat;
			diffusion_sq_variable=variable;
			pth=(pthread_t *)calloc(size,sizeof(pthread_t));
			data=(diffusion_sq_data *)calloc(size,sizeof(diffusion_sq_data));
			for(l=0;l<partition;l++)
			{
				memcpy(augumente[m],mat[j],sizeof(double)*variable);
				ciclique[l]=j;
				j++;m++;
				for(i=1;i<size;i++)
   			{
   				data[i].i=i;
   				data[i].j=j;
   				pthread_create(&pth[i],NULL,diffusion_sq_striped_ciclique_mpi_sth_th1,&data[i]);
			   	j++;
   			}
   			for(i=1;i<size;i++)
   				pthread_join(pth[i],NULL);
   		}
   		free(pth);
   		free(data);
		 	m=partition;
   		if(nr>0)
   		{
	   		memcpy(augumente[m],mat[j],sizeof(double)*variable);
				ciclique[m]=j;
				j++;
			}
			if(nr>1)
			{
				pth=(pthread_t *)calloc(nr,sizeof(pthread_t));
				data=(diffusion_sq_data *)calloc(nr,sizeof(diffusion_sq_data));
   			for(l=1;l<nr;l++)
		   	{
		   		data[l].i=l;
   				data[l].j=j;
   				pthread_create(&pth[l],NULL,diffusion_sq_striped_ciclique_mpi_sth_th1,&data[l]);
   				j++;
			   }
			   for(l=0;l<nr;l++)
			   	pthread_join(pth[l],NULL);
			  	free(pth);
			  	free(data);
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