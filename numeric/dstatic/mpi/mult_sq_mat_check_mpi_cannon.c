/* 
	dossier pour la multiplication de matrices dans checkerboard partition 
	dans MPI implementation avec Cannon alghoritm
*/
#include<parallel/parallel-mpi.h>
int mult_sq_mat_check_mpi_cannon(int dim,double **a,double **b,double **c,MPI_Comm mesh2_comm,int q,int s)
{
/*
	dim c'est la dimension de matrice
	a c'est la premier matrice
	b c'est la deuxieme matrice
	c c'est la matrice de resoudere
	mesh2_comm c'est le communicator
	q c'est le dimension de mesh
	s c'est le numero de done qui a chacun processeur
*/
int i,j,k,l,m,r;	/* counters */
/* pour le reseau */
int rank;			/* rank de thread in normal communicator */
int coordonne[2];	/* rank de thread in cartesian communicator */
int size;			/* le numero de processes */
int next[2],prev[2]; /* le voicines */
int source;
MPI_Status status;
double *temp;
	temp=(double *)calloc(s,sizeof(double));
	/* cree la reseau */
	MPI_Comm_rank(mesh2_comm,&rank);
	MPI_Cart_coords(mesh2_comm,rank,2,coordonne);
	MPI_Cart_shift(mesh2_comm,1,1,&source,&next[0]);
	MPI_Cart_shift(mesh2_comm,1,-1,&source,&prev[0]);
	MPI_Cart_shift(mesh2_comm,0,1,&source,&next[1]);
	MPI_Cart_shift(mesh2_comm,0,-1,&source,&prev[1]);
	for(r=0;r<q;r++)
	{
		for(i=0;i<s;i++)
			for(j=0;j<s;j++)
				for(k=0;k<s;k++)
					c[i][j]+=a[i][k]*b[k][j];
		//la function circulare
		if(coordonne[1]==0)
		{
			for(i=0;i<s;i++)
			{
				MPI_Send(a[i],s,MPI_DOUBLE,next[0],0,mesh2_comm);
				MPI_Recv(a[i],s,MPI_DOUBLE,prev[0],0,mesh2_comm,&status);
			}
		}
		else
		{
			for(i=0;i<s;i++)
			{
				MPI_Recv(temp,s,MPI_DOUBLE,prev[0],0,mesh2_comm,&status);
				MPI_Send(a[i],s,MPI_DOUBLE,next[0],0,mesh2_comm);
				for(j=0;j<s;j++) a[i][j]=temp[j];
			}
		}
		if(coordonne[0]==0)
		{
			for(i=0;i<s;i++)
			{
				MPI_Send(b[i],s,MPI_DOUBLE,next[1],0,mesh2_comm);
				MPI_Recv(b[i],s,MPI_DOUBLE,prev[1],0,mesh2_comm,&status);
			}
		}
		else
		{
			for(i=0;i<s;i++)
			{
				MPI_Recv(temp,s,MPI_DOUBLE,prev[1],0,mesh2_comm,&status);
				MPI_Send(b[i],s,MPI_DOUBLE,next[1],0,mesh2_comm);
				for(j=0;j<s;j++) b[i][j]=temp[j];
			}
		}
	} 
	free(temp);
	MPI_Barrier(MPI_COMM_WORLD);
}
