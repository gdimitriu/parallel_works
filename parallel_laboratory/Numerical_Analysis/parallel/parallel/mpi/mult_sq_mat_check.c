/* 
	file for function of extrem in MPI implementation
*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<parallel_mpi.h>

int mult_sq_mat_check_mpi(int dim,double **a,double **b,double **c)
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
/* pour  le done de transmetre */
double *pdone;
double **resoudre;
MPI_Datatype smatrice;
int blocklens[1];				/* la longeur de done */
MPI_Aint	indices[1];			/* l'addresse de done */
MPI_Datatype orig_type[1];	/* le type de done */
/* pour le reseau */
MPI_Comm	mesh2_comm;			/* reseau pour faire calcule */
int rank;						/* rank de thread in normal communicator */
int coordonne[2];				/* rank de thread in cartesian coordonne */
int size;						/* numero de threads */
int dims[2];					/* les dimensions */
int periodicite[2];			/* la periodicite de reseau */
MPI_Status	status;			/* le status pour MPI_Recv */
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	/* cree le reseau de processors */
	q=sqrt(size);	/* numero de processerus s*s */
	dims[0]=dims[1]=q;
	periodicite[0]=periodicite[1]=0;
	MPI_Cart_create(MPI_COMM_WORLD,2,dims,periodicite,1,&mesh2_comm);
	MPI_Comm_rank(mesh2_comm,&rank);
	MPI_Cart_coords(mesh2_comm,rank,2,coordonne);
	/* forme le done de transmetre */
	s=dim/q;		/* la dimension de done dans un processeur */
	if((resoudre=(double **)calloc(s,sizeof(double *)))==(double **)NULL)
	{
		perror("Erreur de alocation pour sdone");
		return(-1);
	}
	if((pdone=(double *)calloc(s*s,sizeof(double)))==(double *)NULL)
	{
		perror("Erreur de alocation pour pdone");
		return(-1);
	}
	for(i=0;i<s;i++)
	{
		resoudre[i]=pdone;
		pdone+=s;
	}
	blocklens[0]=s*s;
	orig_type[0]=MPI_DOUBLE;
	// l'addresse des elementes 
	MPI_Address(*resoudre,&indices[0]);
	// fait relatives 
	indices[0]=0;
	/* cree la structure de communication */
	MPI_Type_struct(1,blocklens,indices,orig_type,&smatrice);
	MPI_Type_commit(&smatrice);
	/* faites les calcules */
	for(l=0;l<s;l++)
	{
		i=s*coordonne[0]+l;
		for(m=0;m<s;m++)
		{
			j=s*coordonne[1]+m;
			resoudre[l][m]=0.0;
			for(k=0;k<dim;k++)
			{
				resoudre[l][m]+=a[i][k]*b[k][j];
				c[i][j]=resoudre[l][m];
			}
		}
	}
	/* elibere la memoire */
	MPI_Comm_free(&mesh2_comm);
	MPI_Type_free(&smatrice);
	free(*resoudre);
	free(resoudre);
}