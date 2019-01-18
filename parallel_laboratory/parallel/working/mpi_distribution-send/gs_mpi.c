/*
	This is the function which made the solving  of system of liner equations
	with gauss siedel in MPI implementation with last computing the static zone.
*/
void gs_MPI(double **mat,double *ty,double *tx,long variable,double err)
{
double *line;
double *x;
int rank,size;
long l,i,j,k,m;
double temp;
long nr,partition;
double **matrix,*pmat,*px;
long *ciclique;
long lastrow;
long *distrib;
MPI_Status status;
long start;
double *y;
long 
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	if(rank!=0)
	{
		nr=variable%size;
		start=0;
		for(i=0;i<=rank;i++)
		{
			partition=(variable-nr)/size;
			if(i<nr) partition++;
			if(i!=rank) start=start+partition;
		}
		matrix=(double **)calloc(partition,sizeof(double *));
		pmat=(double *)calloc(variable*partition,sizeof(double));
		for(i=0;i<partition;i++)
		{
			matrix[i]=pmat;
			pmat+=variable;
		}
		y=(double *)calloc(variable,sizeof(double));
		ciclique=(long *)calloc(partition,sizeof(long));
		for(i=0;i<partition;i++)
			ciclique[i]=i+start;
		if(rank==size-1) lastrow=0;
		else lastrow=1;
		MPI_Recv(&matrix[0][0],variable*partition,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&y[0],partition,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
	}
	else
	{
		nr=variable%size;
		start=0;
		distrib=(long *)calloc(size,sizeof(long));
		for(i=0;i<size;i++)
		{
			partition=(variable-nr)/size;
			if(i<nr) partition++;
			distrib[i]=partition;
		}
		matrix=(double **)calloc(partition,sizeof(double *));
		pmat=(double *)calloc(variable*partition,sizeof(double));
		for(i=0;i<partition;i++)
		{
			matrix[i]=pmat;
			pmat+=variable;
		}
		y=(double *)calloc(variable,sizeof(double));
		ciclique=(long *)calloc(partition,sizeof(long));
		for(i=0;i<partition;i++)
			ciclique[i]=i+start;
		memcpy(&matrix[0][0],&mat[start][0],variable*partition*sizeof(double));
		memcpy(&y[0],&ty[start],partition*sizeof(double));
		start=start+partition;
		lastrow=0;
		for(i=1;i<size;i++)
		{
			MPI_Send(&tmat[start][0],distribution[i]*variable,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			MPI_Send(&ty[start],distribution[i],MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			start=start+distribution[i];
		}
		free(distribution);
	}
	free(*matrix);
	free(matrix);
	free(y);
	free(ciclique);
}