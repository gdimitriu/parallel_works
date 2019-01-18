/*
	la function pour faire elimation gaussiene dans le reseau ciclique avec
	distribution striped dans MPI clasique
*/
#include<string.h>
#include<math.h>
void jacobi_MPI(double **mat,double *ty,double *tx,int dim,double err)
{
double *xn,*xn_1;
int rank,size;
int i,j,k,m;
int nr,partition,part1;
double **matrix,*pmatrix;
int *vpart;
int firstrow;
MPI_Status status;
double q,sum,sum1;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	nr=dim%size;
	part1=partition=(dim-nr)/size;
	if(rank<nr) partition++;
	matrix=(double **)calloc(partition,sizeof(double *));
	pmatrix=(double *)calloc((dim+1)*partition,sizeof(double));
	for(i=0;i<partition;i++)
	{
		matrix[i]=pmatrix;
		pmatrix+=(dim+1);
	}
	if(rank!=0) xn=(double *)calloc(partition,sizeof(double));
	xn_1=(double *)calloc(dim,sizeof(double));
//make data distribution
	MPI_Bcast(&err,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	if(rank==0)
	{
		memcpy(&matrix[0][0],&mat[0][0],partition*dim*sizeof(double));
		vpart=(int *)calloc(size,sizeof(int));
		for(i=0;i<partition;i++) matrix[i][dim]=ty[i];
		vpart[0]=k=partition;
		for(i=1;i<size;i++)
		{
			if(i<nr) m=part1+1;
			else m=part1;
			vpart[i]=m;
			MPI_Send(&k,1,MPI_INT,i,0,MPI_COMM_WORLD);
			MPI_Send(&mat[k][0],m*dim,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			MPI_Send(&ty[k],m,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			k=k+m;
		}
	}
	else 
	{
		MPI_Recv(&firstrow,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&matrix[0][0],partition*dim,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(xn,partition,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		for(i=0;i<partition;i++) matrix[i][dim]=xn[i];
	}
	if(rank==0)
		xn=tx;
	else
		xn=(double *)calloc(partition,sizeof(double));
//JACOBI
	if(rank==0)
	{
		for(i=0;i<dim;i++) xn_1[i]=ty[i]/mat[i][i];
		//compute q
		q=0.0;
		for(i=1;i<dim;i++) q+=fabs(mat[0][i]/mat[0][0]);
		for(i=1;i<dim;i++)
		{
			sum=0.0;
			for(j=0;j<dim;j++) if(i!=j) sum+=fabs(mat[i][j]/mat[i][i]);
			if(q<sum) q=sum;
		}
		sum=fabs(ty[0]/mat[0][0]);
		for(i=1;i<dim;i++) if(sum<fabs(ty[i]/mat[i][i])) sum=fabs(ty[i]/mat[i][i]);
		sum=q*sum/(1-q);
		firstrow=0;
	}
	MPI_Bcast(&q,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(&sum,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(xn_1,dim,MPI_DOUBLE,0,MPI_COMM_WORLD);
	while(fabs(sum)>err)
	{	
		for(i=0;i<partition;i++)
		{
			k=firstrow+i;
			xn[i]=matrix[i][dim]/matrix[i][i];
			for(j=0;j<dim;j++) if(j!=k) xn[i]-=matrix[i][j]/matrix[i][k]*xn_1[j];
		}
		sum=fabs(xn[0]-xn_1[0]);
		for(i=1;i<partition;i++)
			if(sum<fabs(xn[i]-xn_1[i])) sum=fabs(xn[i]-xn_1[i]);
		MPI_Reduce(&sum,&sum1,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
		if(rank==0)
		{
			k=0;
			for(i=1;i<size;i++)
			{
				k=k+vpart[i-1];
				MPI_Recv(&xn[k],vpart[i],MPI_DOUBLE,i,1,MPI_COMM_WORLD,&status);
			}	
			memcpy(xn_1,xn,dim*sizeof(double));
		}
		else MPI_Send(xn,partition,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
		MPI_Bcast(&sum1,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
		sum=sum1*q/(1-q);
		MPI_Bcast(xn_1,dim,MPI_DOUBLE,0,MPI_COMM_WORLD);
	}
	if(rank!=0) free(xn);
	free(xn_1);
	free(*matrix);
	free(matrix);
	if(rank==0)	free(vpart);
}