/* 
	dossier pour la multiplication de matrices dans checkerboard partition 
	dans MPI implementation avec Cannon alghoritm et avec pthread
*/
#include<pthread.h>
#include<parallel/parallel-mpi.h>
typedef struct
{
	double **a;
	double **b;
	MPI_Comm com;
	int s;
	int source;
} datas_mult_sq_mat_check_mpi_cannon_pthread;
pthread_mutex_t mutex_mult_sq_mat_check_mpi_cannon_pthread;
pthread_cond_t cond_mult_sq_mat_check_mpi_cannon_pthread;
int fin_mult_sq_mat_check_mpi_cannon_pthread=0;
/*
	le thread pour la premier matrice
*/
void *a_mult_sq_mat_check_mpi_cannon_pthread(void *input)
{
int i;
int next,prev; /* le voicines */
int source;
datas_mult_sq_mat_check_mpi_cannon_pthread *data=(datas_mult_sq_mat_check_mpi_cannon_pthread *)input;
MPI_Status status;
	MPI_Cart_shift(data->com,1,1,&source,&next);
	MPI_Cart_shift(data->com,1,-1,&source,&prev);
	while(1)
	{
		pthread_mutex_lock(&mutex_mult_sq_mat_check_mpi_cannon_pthread);
		pthread_cond_wait(&cond_mult_sq_mat_check_mpi_cannon_pthread,&mutex_mult_sq_mat_check_mpi_cannon_pthread);
		pthread_mutex_unlock(&mutex_mult_sq_mat_check_mpi_cannon_pthread);
/*		if(data->source==0)
		{
			for(i=0;i<data->s;i++)
			{
				MPI_Send(data->a[i],data->s,MPI_DOUBLE,next,0,data->com);
				MPI_Recv(data->a[i],data->s,MPI_DOUBLE,prev,0,data->com,&status);
			}
		}
		else
		{
			for(i=0;i<data->s;i++)
			{
				MPI_Recv(data->a[i],data->s,MPI_DOUBLE,prev,0,data->com,&status);
				MPI_Send(data->a[i],data->s,MPI_DOUBLE,next,0,data->com);
			}
		} */
		if(fin_mult_sq_mat_check_mpi_cannon_pthread==1) pthread_exit(NULL);
	}
}
/*
	le thread pour la deuxime matrice
*/
void *b_mult_sq_mat_check_mpi_cannon_pthread(void *input)
{
int i;
int next,prev; /* le voicines */
int source;
datas_mult_sq_mat_check_mpi_cannon_pthread *data=(datas_mult_sq_mat_check_mpi_cannon_pthread *)input;
	MPI_Cart_shift(data->com,0,1,&source,&next);
	MPI_Cart_shift(data->com,0,-1,&source,&prev);
	while(1)
	{
		pthread_mutex_lock(&mutex_mult_sq_mat_check_mpi_cannon_pthread);
		pthread_cond_wait(&cond_mult_sq_mat_check_mpi_cannon_pthread,&mutex_mult_sq_mat_check_mpi_cannon_pthread);
		pthread_mutex_unlock(&mutex_mult_sq_mat_check_mpi_cannon_pthread);
/*		if(data->source==0)
		{
			for(i=0;i<data->s;i++)
			{
				MPI_Send(data->b[i],data->s,MPI_DOUBLE,next,0,data->com);
				MPI_Recv(data->b[i],data->s,MPI_DOUBLE,prev,0,data->com,&status);
			}
		}
		else
		{
			for(i=0;i<data->s;i++)
			{
				MPI_Recv(data->b[i],data->s,MPI_DOUBLE,prev,0,data->com,&status);
				MPI_Send(data->b[i],data->s,MPI_DOUBLE,next,0,data->com);
			}
		}	*/
		if(fin_mult_sq_mat_check_mpi_cannon_pthread==1) pthread_exit(NULL);
	}
}


int mult_sq_mat_check_mpi_cannon_pthread(int dim,double **a,double **b,double **c,MPI_Comm mesh2_comm,int q,int s)
{
/*
	dim c'est la dimension de matrice
	a c'est la premier matrice
	b c'est la deuxieme matrice
	c c'est la matrice de resoudere
	q c'est le diemnsion de mesh
	s c'est le numero de done qui a chacun processeur
*/
int i,j,k,l,m,r;	/* counters */
/* pour le reseau */
int rank;			/* rank de trhread dans normal communicator */
int coordonne[2];	/* rank de thread dans cartesian communicator */
pthread_t pth[2];
datas_mult_sq_mat_check_mpi_cannon_pthread data[2];
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Cart_coords(mesh2_comm,rank,2,coordonne);
	/* cree la reseau */
	data[0].s=data[1].s=s;
	data[0].a=data[1].a=a;
	data[0].b=data[1].b=b;
	data[0].source=coordonne[1];
	data[1].source=coordonne[0];
	data[0].com=data[1].com=mesh2_comm;
	pthread_cond_init(&cond_mult_sq_mat_check_mpi_cannon_pthread,NULL);
	pthread_mutex_init(&mutex_mult_sq_mat_check_mpi_cannon_pthread,NULL);
	pthread_create(&pth[0],NULL,a_mult_sq_mat_check_mpi_cannon_pthread,&data[0]);
	pthread_create(&pth[1],NULL,b_mult_sq_mat_check_mpi_cannon_pthread,&data[1]);
	for(r=0;r<q;r++)
	{
		for(i=0;i<s;i++)
			for(j=0;j<s;j++)
				for(k=0;k<s;k++)
					c[i][j]+=a[i][k]*b[k][j];
		pthread_mutex_lock(&mutex_mult_sq_mat_check_mpi_cannon_pthread);
		pthread_cond_broadcast(&cond_mult_sq_mat_check_mpi_cannon_pthread);
		pthread_mutex_unlock(&mutex_mult_sq_mat_check_mpi_cannon_pthread);
	}
	fin_mult_sq_mat_check_mpi_cannon_pthread=1;
	pthread_mutex_lock(&mutex_mult_sq_mat_check_mpi_cannon_pthread);
	pthread_cond_broadcast(&cond_mult_sq_mat_check_mpi_cannon_pthread);
	pthread_mutex_unlock(&mutex_mult_sq_mat_check_mpi_cannon_pthread);
	pthread_join(pth[0],NULL);
	pthread_join(pth[1],NULL);
	pthread_cond_destroy(&cond_mult_sq_mat_check_mpi_cannon_pthread);
	pthread_mutex_destroy(&mutex_mult_sq_mat_check_mpi_cannon_pthread);
	MPI_Barrier(MPI_COMM_WORLD);
}
