/* 
	file for function of matrix multiplication with chekerboard partition 
	in MPI implementation
*/
#include<stdio.h>
#include<math.h>
#include<parallel/parallel-mpi-pthread.h>
/* structure de comunication pour Bcastp */
typedef struct
{
	int source;
	int dim;
	double **c;
	MPI_Comm com;
}datas_MPI_Bcastp_check;
void *MPI_Bcastp_check(void *p)
{
int i,j,l,m,k;
int coordonne[2];
datas_MPI_Bcastp_check data=*(datas_MPI_Bcastp_check *)p;
double *temp;
	temp=(double *)calloc(data.dim,sizeof(double));
	MPI_Cart_coords(data.com,data.source,2,coordonne);
	for(l=0;l<data.dim;l++)
	{
		i=data.dim*coordonne[0]+l;
		for(m=0;m<data.dim;m++)
		{
			k=data.dim*coordonne[1]+m;
			temp[m]=data.c[i][k];
		}
		MPI_Bcast(temp,data.dim,MPI_DOUBLE,data.source,MPI_COMM_WORLD);
		for(m=0;m<data.dim;m++)
		{
			k=data.dim*coordonne[1]+m;
			data.c[i][k]=temp[m];
		}
	}
	free(temp);
	pthread_exit(NULL);
}
int mult_sq_mat_check_mpi_pthread(int dim,double **a,double **b,double **c)
/*
	dim c'est la dimension de matrice
	thread c'est le numero de threads
	a c'est la premier matrice
	b c'est la deuxieme matrice
	c c'est la matrice de resoudre
*/
{
int q;	/* c'est la dimension de partition */
int s;	/* c'est le numero de done qui a chacun proces */
int i,j,k,l,m;	/* counteurs */
/* pour le reseau */
MPI_Comm	mesh2_comm;			/* reseau pour faire calcule */
int rank;						/* rank de thread in normal communicator */
int coordonne[2];				/* rank de thread in cartesian coordonne */
int size;						/* numero de threads */
int dims[2];					/* les dimensions */
int periodicite[2];			/* la periodicite de reseau */
pthread_t *pth;
datas_MPI_Bcastp_check *datacom;	/* structure de comunication pour Bcastp */
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	/* cree le reseau de processors */
	q=sqrt(size);	/* numero de processerus s*s */
	dims[0]=dims[1]=q;
	periodicite[0]=periodicite[1]=0;
	MPI_Cart_create(MPI_COMM_WORLD,2,dims,periodicite,1,&mesh2_comm);
	MPI_Comm_rank(mesh2_comm,&rank);
	MPI_Cart_coords(mesh2_comm,rank,2,coordonne);
	/* forme le done de transmetre */
	s=dim/q;		/* la dimension de done dans un processeur */
	if((pth=(pthread_t *)calloc(size,sizeof(pthread_t)))==(pthread_t *)NULL)
	{
		perror("Erreur de alocation pour pth");
		return(-1);
	}
	if((datacom=(datas_MPI_Bcastp_check *)calloc(size,sizeof(datas_MPI_Bcastp_check)))==(datas_MPI_Bcastp_check *)NULL)
	{
		perror("Erreur de alocation pour datacom");
		free(pth);
		return(-1);
	}
	/* faites les calcules */
	for(l=0;l<s;l++)
	{
		i=s*coordonne[0]+l;
		for(m=0;m<s;m++)
		{
			j=s*coordonne[1]+m;
			c[i][j]=0.0;
			for(k=0;k<dim;k++)
				c[i][j]+=a[i][k]*b[k][j];
		}
	}
	/* faire la comunication et done le resoudre */
	for(i=0;i<size;i++)
	{
	 	datacom[i].com=mesh2_comm;
		datacom[i].source=i;
		datacom[i].c=c;
		datacom[i].dim=s;
		pthread_create(&pth[i],NULL,MPI_Bcastp_check,(void *)&datacom[i]);
	}	
	for(i=0;i<size;i++)
		pthread_join(pth[i],NULL);
	/* elibere la memoire */
	MPI_Comm_free(&mesh2_comm);
	free(pth);
	free(datacom);
	MPI_Barrier(MPI_COMM_WORLD);
}