/* 
	file for function of matrix multiplication with chekerboard partition 
	in MPI implementation
*/
#include<stdio.h>
#include<math.h>
#include<parallel/parallel-mpi-pth.h>
#include<pth.h>
/* structure de comunication pour Bcastp */
typedef struct
{
	int source;
	int dim;
	double **c;
	MPI_Comm com;
}datas_MPI_Bcastp_check_pth;
void *MPI_Bcastp_check_pth(void *p)
{
int i,j,l;
int coordonne[2];
datas_MPI_Bcastp_check_pth data=*(datas_MPI_Bcastp_check_pth *)p;
	MPI_Cart_coords(data.com,data.source,2,coordonne);
	j=data.dim*coordonne[1];
	for(l=0;l<data.dim;l++)
	{
		i=data.dim*coordonne[0]+l;
		MPI_Bcast(&data.c[i][j],data.dim,MPI_DOUBLE,data.source,MPI_COMM_WORLD);
	}
}
int mult_sq_mat_check_mpi_pth(int dim,double **a,double **b,double **c)
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
pth_t *pth;
pth_attr_t attr;
datas_MPI_Bcastp_check_pth *datacom;	/* structure de comunication pour Bcastp */
	pth_init();
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
	if((pth=(pth_t *)calloc(size,sizeof(pth_t)))==(pth_t *)NULL)
	{
		perror("Erreur de alocation pour pth");
		return(-1);
	}
	if((datacom=(datas_MPI_Bcastp_check_pth *)calloc(size,sizeof(datas_MPI_Bcastp_check_pth)))==(datas_MPI_Bcastp_check_pth *)NULL)
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
	attr=pth_attr_new();
	pth_attr_set(attr,PTH_ATTR_JOINABLE,TRUE);
	for(i=0;i<size;i++)
	{
	 	datacom[i].com=mesh2_comm;
		datacom[i].source=i;
		datacom[i].c=c;
		datacom[i].dim=s;
		pth[i]=pth_spawn(attr,MPI_Bcastp_check_pth,&datacom[i]);
	}
	for(i=0;i<size;i++)
		pth_join(pth[i],NULL);
	/* elibere la memoire */
	MPI_Comm_free(&mesh2_comm);
	free(pth);
	free(datacom);
	MPI_Barrier(MPI_COMM_WORLD);
	pth_kill();
}