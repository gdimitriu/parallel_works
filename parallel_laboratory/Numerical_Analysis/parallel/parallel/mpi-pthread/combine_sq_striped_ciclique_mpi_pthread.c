/* 
	La function pour faire la combination d'une matrice striped ciclique
	dans l'implementation de MPI clasique
*/
#include<parallel/parallel-mpi-pthread.h>
double **combine_sq_mat;
typedef struct
{
	int i;
	int variable;
	int partition;
	int nr;
} combine_sq_data;
void *combine_sq_striped_ciclique_mpi_sth_th1(void *input)
{
MPI_Status status;
int temp;
int k,j;
combine_sq_data data=*(combine_sq_data *)input;
	if(data.i<data.nr) temp=data.partition+1;
	else temp=data.partition;
		for(k=0;k<temp;k++)
		{
			MPI_Recv(&j,1,MPI_INT,data.i,0,MPI_COMM_WORLD,&status);	
			MPI_Recv(combine_sq_mat[j],data.variable,MPI_DOUBLE,data.i,0,MPI_COMM_WORLD,&status);
		}
		pthread_exit(NULL);
}

void combine_sq_striped_ciclique_mpi_pthread(int rank,int size,int variable,int *ciclique,double **matrix,double **tmatrix)
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
pthread_t *pth;
combine_sq_data *data;
	nr=variable%size;
	partition=(variable-nr)/size;
	combine_sq_mat=tmatrix;
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
		pth=(pthread_t *)calloc(size,sizeof(pthread_t));
		data=(combine_sq_data *)calloc(size,sizeof(combine_sq_data));
		for(i=1;i<size;i++)
		{
			data[i].i=i;
			data[i].variable=variable;
			data[i].partition=partition;
			data[i].nr=nr;
			pthread_create(&pth[i],NULL,combine_sq_striped_ciclique_mpi_sth_th1,&data[i]);
		}
		for(i=1;i<size;i++)
			pthread_join(pth[i],NULL);
		free(pth);
		free(data);
	}
	MPI_Barrier(MPI_COMM_WORLD);
}