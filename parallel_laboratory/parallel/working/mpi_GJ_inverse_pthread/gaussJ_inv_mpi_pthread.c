/*
	The function who find the inverse for a square matrix with Gauss Jordan.
	Implementation in clasic MPI.
*/
#include<pthread.h>
#include<semaphore.h>

typedef struct
{
	int temp[2];
	int variable;
	int partition;
	double *row;
	double *row1;
	double **augumente;
	double **inv;
	int *ciclique;
	sem_t *sem1;
	sem_t *sem2;
	sem_t *sem3;
} gj_inv_mpi_pthread_data;
typedef struct
{
	double **inv;
	int size;
	int i;
	int variable;
} gj_inv_mpi_pth_gt_data;

void *gj_inv_mpi_pthread_gt_th(void *input)
{
gj_inv_mpi_pth_gt_data *data=(gj_inv_mpi_pth_gt_data *)input;
int i;
MPI_Status status;
	for(i=data->i;i<data->variable;i=i+data->size)
			MPI_Recv(&(data->inv[i][0]),data->variable,MPI_DOUBLE,data->i,data->i,MPI_COMM_WORLD,&status);
	pthread_exit(NULL);
}

void *gj_inv_mpi_pthread_th(void *input)
{
gj_inv_mpi_pthread_data *data=(gj_inv_mpi_pthread_data *)input;
MPI_Status status;
int j,k,i,m,counter,ete,rank,size;
double dtemp;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	ete=0;
	for(i=0;i<=data->partition;i++)
	{
		if(i!=data->partition) counter=data->ciclique[i];
		else counter=data->variable;
		for(k=ete;k<counter;k++)
		{
			sem_wait(data->sem1);
			//make operations for this machine
			for(m=0;m<data->partition;m++)
			{
				if(data->ciclique[m]!=data->temp[0])
				{
					dtemp=data->augumente[m][data->temp[0]]/data->row[data->temp[0]];
					for(j=data->temp[0]+1;j<data->variable;j++) 
						data->augumente[m][j]-=data->row[j]*dtemp;
					
						for(j=0;j<data->variable;j++)
							if(rank!=0)
								data->inv[m][j]-=data->row1[j]*dtemp;
							else //local
								data->inv[data->ciclique[m]][j]-=data->row1[j]*dtemp;
				}
			}
			sem_post(data->sem2);
		}
		if(i!=data->partition)
		{
			//make again the operations for this machine
			for(m=0;m<data->partition;m++)
			{
				if(data->ciclique[m]!=data->ciclique[i])
				{
					dtemp=data->augumente[m][data->ciclique[i]]/data->augumente[i][data->ciclique[i]];
					for(j=data->ciclique[i]+1;j<data->variable;j++)
						data->augumente[m][j]-=data->augumente[i][j]*dtemp;
					
					for(j=0;j<data->variable;j++)
						if(rank!=0)
							data->inv[m][j]-=data->inv[i][j]*dtemp;
						else //local
							data->inv[data->ciclique[m]][j]-=data->inv[data->ciclique[i]][j]*dtemp;
				}
				else
				{
					for(j=0;j<data->variable;j++)
					{
						if(rank!=0)
							data->inv[m][j]=data->inv[m][j]/data->augumente[m][data->ciclique[m]];
						else //local
							data->inv[data->ciclique[m]][j]=data->inv[data->ciclique[m]][j]/data->augumente[m][data->ciclique[m]];
						if(j!=data->ciclique[m])
							data->augumente[m][j]=data->augumente[m][j]/data->augumente[m][data->ciclique[m]];
					}
					data->augumente[m][data->ciclique[m]]=1.0; 
				}
			}
			ete=data->ciclique[i]+1;
		}
		sem_wait(data->sem3);
	}
	pthread_exit(NULL);
}

void gj_inv_MPI(double **mat,double **tinv,int variable)
{
double *row,*row1;
double **inv;
int rank,size;
int num,ete;
int i,j,k,m,l;
int nr,partition,counter;
double **augumente,*paugumente;
int *ciclique;
int lastrow;
sem_t sem1;
sem_t sem2;
sem_t sem3;
MPI_Status status;
gj_inv_mpi_pthread_data data;
pthread_t pth;
gj_inv_mpi_pth_gt_data *pdata;
pthread_t *dpth;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	nr=variable%size;
	partition=(variable-nr)/size;
	if(rank<nr) partition++;
	augumente=(double **)calloc(partition,sizeof(double *));
	paugumente=(double *)calloc(variable*partition,sizeof(double));
	for(i=0;i<partition;i++)
	{
		augumente[i]=paugumente;
		paugumente+=variable;
	}
	ciclique=(int *)calloc(partition,sizeof(int));
	row=(double *)calloc(variable,sizeof(double));
	row1=(double *)calloc(variable,sizeof(double));
//make data distribution
if(rank==0)
{
	j=0;
	if(rank<nr) lastrow=partition-1;
	else lastrow=partition;
	for(l=0;l<lastrow;l++)
	{
		memcpy(augumente[l],mat[j],sizeof(double)*variable);
		ciclique[l]=j;
		j++;
		for(i=1;i<size;i++)
  		{
  			MPI_Send(mat[j],variable,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
  			MPI_Send(&j,1,MPI_INT,i,2,MPI_COMM_WORLD);
	   	j++;
  		}
  	}
  	if(nr>0)
  	{
  		memcpy(augumente[lastrow],mat[j],sizeof(double)*variable);
		ciclique[lastrow]=j;
		j++;
	}
  	for(l=1;l<nr;l++)
  	{
		MPI_Send(mat[j],variable,MPI_DOUBLE,l,0,MPI_COMM_WORLD);
		MPI_Send(&j,1,MPI_INT,l,2,MPI_COMM_WORLD);
 		j++;
   }
   inv=tinv;
}
else
{
	inv=(double **)calloc(partition,sizeof(double *));
	paugumente=(double *)calloc(variable*partition,sizeof(double));
	for(i=0;i<partition;i++)
	{
		inv[i]=paugumente;
		paugumente+=variable;
	}
	for(i=0;i<partition;i++)
	{
		MPI_Recv(augumente[i],variable,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&ciclique[i],1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
	}
}
	
	//make inverse with Gauss Jordan
	data.inv=inv;
	data.augumente=augumente;
	data.partition=partition;
	data.variable=variable;
	data.row1=row1;
	data.row=row;
	data.ciclique=ciclique;
	data.sem1=&sem1;
	data.sem2=&sem2;
	data.sem3=&sem3;
	sem_init(&sem1,0,0);
	sem_init(&sem2,0,0);
	sem_init(&sem3,0,0);
	for(i=0;i<partition;i++)
	{
		if(rank!=0) inv[i][ciclique[i]]=1.0;
		else inv[ciclique[i]][ciclique[i]]=1.0;
	}
	ete=0;
	pthread_create(&pth,NULL,gj_inv_mpi_pthread_th,(void *)&data);
	for(i=0;i<=partition;i++)
	{
		if(i!=partition) counter=ciclique[i];
		else counter=variable;
		for(k=ete;k<counter;k++)
		{
			l=rank-1;
			if(l<0) l=size-1;
			MPI_Recv(data.temp,2,MPI_INT,l,0,MPI_COMM_WORLD,&status);
			MPI_Recv(&row[0],variable,MPI_DOUBLE,l,1,MPI_COMM_WORLD,&status);
			MPI_Recv(&row1[0],variable,MPI_DOUBLE,l,2,MPI_COMM_WORLD,&status);
			sem_post(&sem1);
			l=rank+1;
			if(l==size) l=0;
			if(l!=data.temp[1])
			{
				MPI_Send(data.temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
				MPI_Send(&row[0],variable,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
				MPI_Send(&row1[0],variable,MPI_DOUBLE,l,2,MPI_COMM_WORLD);
			}
			sem_wait(&sem2);
		}
		if(i!=partition)
		{
			//send the line
			l=rank+1;
			if(l==size) l=0;
			memcpy(row,&augumente[i][0],variable*sizeof(double));
			if(rank!=0)
				memcpy(row1,&inv[i][0],variable*sizeof(double));
			else
				memcpy(row1,&inv[ciclique[i]][0],variable*sizeof(double));
			data.temp[0]=ciclique[i];
			data.temp[1]=rank;
			MPI_Send(data.temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
			MPI_Send(&row[0],variable,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
			MPI_Send(&row1[0],variable,MPI_DOUBLE,l,2,MPI_COMM_WORLD);
			ete=ciclique[i]+1;
		}
		sem_post(&sem3);
	}
	pthread_join(pth,NULL);
	MPI_Barrier(MPI_COMM_WORLD);
	//make inverse at 0 machine
	if(rank==0)
	{
		j=0;
		for(i=0;i<variable;i++)
		{
			if(j!=0)
				MPI_Recv(&inv[i][0],variable,MPI_DOUBLE,j,j,MPI_COMM_WORLD,&status);
			j=(j+1)%size;
		}
	}
	else
	{
		for(i=0;i<partition;i++)
			MPI_Send(&inv[i][0],variable,MPI_DOUBLE,0,rank,MPI_COMM_WORLD);
	}
/*
	//make inverse at 0 machine
	if(rank==0)
	{
		pdata=(gj_inv_mpi_pth_gt_data *)calloc(size,sizeof(gj_inv_mpi_pth_gt_data));
		dpth=(pthread_t *)calloc(size,sizeof(pthread_t));
		for(i=1;i<size;i++)
		{
			pdata[i].variable=variable;
			pdata[i].i=i;
			pdata[i].inv=inv;
			pdata[i].size=size;
			pthread_create(&dpth[i],NULL,gj_inv_mpi_pthread_gt_th,(void *)&pdata[i]);
		}
		for(i=1;i<size;i++)
			pthread_join(dpth[i],NULL);
	}
	else
	{
		for(i=0;i<partition;i++)
			MPI_Send(&inv[i][0],variable,MPI_DOUBLE,0,rank,MPI_COMM_WORLD);
	}
	if(rank!=0)
	{
		free(*inv);
		free(inv);
	}
	else
	{
		free(pdata);
		free(dpth);
	}*/
	free(*augumente);
	free(augumente);
	free(ciclique);
	free(row);
	free(row1);
	sem_destroy(&sem1);
	sem_destroy(&sem2);
	sem_destroy(&sem3);
}