/*
	This is the function which made the solving  of system of liner equations
	with gauss siedel in MPI implementation with last computing the static zone.
*/
void gs_MPI(double **mat,double *ty,double *tx,long variable,double err)
{
double *line;
double *x;
int rank,size;
long num,last;
long l,i,j,k,m;
struct { double x;long pos; int rank;} send;
double temp;
MPI_Datatype ssend;
int blocklens[3];
MPI_Aint indices[3];
MPI_Datatype old_types[3];
long nr,partition;
double **matrix,*pmat,*px;
long *ciclique;
double *x_1;
double stop;
double q,max;
long lastrow;
double *qi;
MPI_Status status;
MPI_Request req;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	nr=variable%size;
	partition=(variable-nr)/size;
	if(rank<nr) partition++;
	matrix=(double **)calloc(partition,sizeof(double *));
	pmat=(double *)calloc((variable+1)*partition,sizeof(double));
	for(i=0;i<partition;i++)
	{
		matrix[i]=pmat;
		pmat+=(variable+1);
	}
	ciclique=(long *)calloc(partition,sizeof(long));
	line=(double *)calloc(variable+1,sizeof(double));
	x_1=(double *)calloc(variable,sizeof(double));
//make data distribution
if(rank==0)
{
	j=0;
	if(rank<nr) lastrow=partition-1;
	else lastrow=partition;
	for(l=0;l<lastrow;l++)
	{
		memcpy(matrix[l],mat[j],sizeof(double)*variable);
		matrix[l][variable]=ty[j];
		ciclique[l]=j;
		j++;
		for(i=1;i<size;i++)
  		{
  			MPI_Isend(mat[j],variable,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&req);
  			MPI_Isend(&ty[j],1,MPI_DOUBLE,i,1,MPI_COMM_WORLD,&req);
  			MPI_Isend(&j,1,MPI_INT,i,2,MPI_COMM_WORLD,&req);
	   	j++;
  		}
  	}
  	if(nr>0)
  	{
  		memcpy(matrix[lastrow],mat[j],sizeof(double)*variable);
		matrix[lastrow][variable]=ty[j];
		ciclique[lastrow]=j;
		j++;
	}
  	for(l=1;l<nr;l++)
  	{
		MPI_Isend(mat[j],variable,MPI_DOUBLE,l,0,MPI_COMM_WORLD,&req);
		MPI_Isend(&ty[j],1,MPI_DOUBLE,l,1,MPI_COMM_WORLD,&req);
		MPI_Isend(&j,1,MPI_INT,l,2,MPI_COMM_WORLD,&req);
 		j++;
   }
	x=tx;
}
else
{
	px=(double *)calloc(variable,sizeof(double));
	for(i=0;i<partition;i++)
	{
		MPI_Recv(matrix[i],variable,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&matrix[i][variable],1,MPI_DOUBLE,0,1,MPI_COMM_WORLD,&status);
		MPI_Recv(&ciclique[i],1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
	}
	x=px;
}
	free(line);
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank!=0)	free(px);
	free(*matrix);
	free(matrix);
	free(x_1);
	free(ciclique);
}