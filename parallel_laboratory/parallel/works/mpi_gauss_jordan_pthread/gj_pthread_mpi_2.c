/*

*/
#include<pthread.h>
#include<semaphore.h>

typedef struct
{
	int temp[2];
	int variable;
	int partition;
	double *ligne;
	double **augumente;
	int *ciclique;
	sem_t *sem1;
	sem_t *sem2;
	sem_t *sem3;
} gj_mpi_pthread_data;
typedef struct
{
	double *resoudre;
	int size;
	int i;
	int variable;
} gj_mpi_pth_gt_data;

void *gj_mpi_pthread_gt_th(void *input)
{
gj_mpi_pth_gt_data *data=(gj_mpi_pth_gt_data *)input;
double *ligne;
long nr,partition;
int k,j;
MPI_Status status;
	nr=data->variable%data->size;
	partition=(data->variable-nr)/data->size;
	if(data->i<nr) partition++;
	ligne=(double *)calloc(partition,sizeof(double));
	MPI_Recv(&ligne[0],partition,MPI_DOUBLE,data->i,1,MPI_COMM_WORLD,&status);
	
	k=0;
	for(j=data->i;j<data->variable;j=j+data->size)
	{
		data->resoudre[j]=ligne[k];
		k++;
	}
	
	free(ligne);
	pthread_exit(NULL);
}
void *gj_mpi_pthread_th(void *input)
{
gj_mpi_pthread_data *data=(gj_mpi_pthread_data *)input;
MPI_Status status;
int j,k,i,m,counter,ete,rank,size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
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
					for(j=data->temp[0]+1;j<data->variable+1;j++)
						data->augumente[m][j]-=data->augumente[m][data->temp[0]]*data->ligne[j]/data->ligne[data->temp[0]];
			}
			sem_post(data->sem2);
		}
		if(i!=data->partition)
		{
			//make again the operations for this machine
			for(m=0;m<data->partition;m++)
			{
				if(data->ciclique[m]!=data->ciclique[i])
					for(j=data->ciclique[i]+1;j<data->variable+1;j++)
						data->augumente[m][j]-=data->augumente[m][data->ciclique[i]]*data->augumente[i][j]/data->augumente[i][data->ciclique[i]];
			}
			ete=data->ciclique[i]+1;
		}
		sem_wait(data->sem3);
	}
	pthread_exit(NULL);
}

void gaussJ_MPI(double **mat,double *ty,double *tx,int variable)
{
double *ligne;
double *resoudre;
int rank,size;
int num,ete;
int i,j,k,m,l;
int nr,partition,counter;
double **augumente,*paugumente,*px;
int *ciclique;
int lastrow;
MPI_Status status;
gj_mpi_pthread_data data;
sem_t sem1,sem2,sem3;
pthread_t pth;
gj_mpi_pth_gt_data *dsend;
pthread_t *psend;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	nr=variable%size;
	partition=(variable-nr)/size;
	if(rank<nr) partition++;
	augumente=(double **)calloc(partition,sizeof(double *));
	paugumente=(double *)calloc((variable+1)*partition,sizeof(double));
	for(i=0;i<partition;i++)
	{
		augumente[i]=paugumente;
		paugumente+=(variable+1);
	}
	ciclique=(int *)calloc(partition,sizeof(int));
	ligne=(double *)calloc(variable+1,sizeof(double));
//make data distribution
if(rank==0)
{
	j=0;
	if(rank<nr) lastrow=partition-1;
	else lastrow=partition;
	for(l=0;l<lastrow;l++)
	{
		memcpy(augumente[l],mat[j],sizeof(double)*variable);
		augumente[l][variable]=ty[j];
		ciclique[l]=j;
		j++;
		for(i=1;i<size;i++)
  		{
  			MPI_Send(mat[j],variable,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
  			MPI_Send(&ty[j],1,MPI_DOUBLE,i,1,MPI_COMM_WORLD);
  			MPI_Send(&j,1,MPI_INT,i,2,MPI_COMM_WORLD);
	   	j++;
  		}
  	}
  	if(nr>0)
  	{
  		memcpy(augumente[lastrow],mat[j],sizeof(double)*variable);
		augumente[lastrow][variable]=ty[j];
		ciclique[lastrow]=j;
		j++;
	}
  	for(l=1;l<nr;l++)
  	{
		MPI_Send(mat[j],variable,MPI_DOUBLE,l,0,MPI_COMM_WORLD);
		MPI_Send(&ty[j],1,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
		MPI_Send(&j,1,MPI_INT,l,2,MPI_COMM_WORLD);
 		j++;
   }
   //send the last flag (which processor have the last equation)	   
   if(nr==1)
   {
   	lastrow=0;
   	for(i=1;i<size;i++) MPI_Send(&lastrow,1,MPI_INT,i,3,MPI_COMM_WORLD);
   	lastrow=1;
   }
   else
   {
   	if(nr==0)
   	{
   		lastrow=1;
   		MPI_Send(&lastrow,1,MPI_INT,(size-1),3,MPI_COMM_WORLD);
   		lastrow=0;
   		for(i=1;i<(size-1);i++) MPI_Send(&lastrow,1,MPI_INT,i,3,MPI_COMM_WORLD);
   	}
   	else
   	{
   		for(i=1;i<size;i++)
   		{
   			if(i==(nr-1)) 
   			{
   				lastrow=1;
   				MPI_Send(&lastrow,1,MPI_INT,i,3,MPI_COMM_WORLD);
   			}
   			else
   			{
   				lastrow=0;
		   		MPI_Send(&lastrow,1,MPI_INT,i,3,MPI_COMM_WORLD);
				}
			}
			lastrow=0;	
		}
	}
	resoudre=tx;
}
else
{
	px=(double *)calloc(partition,sizeof(double));
	for(i=0;i<partition;i++)
	{
		MPI_Recv(augumente[i],variable,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&augumente[i][variable],1,MPI_DOUBLE,0,1,MPI_COMM_WORLD,&status);
		MPI_Recv(&ciclique[i],1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
	}
	MPI_Recv(&lastrow,1,MPI_INT,0,3,MPI_COMM_WORLD,&status);
	resoudre=px;
}	
	//make Gauss Jordan
	sem_init(&sem1,0,0);
	sem_init(&sem2,0,0);
	sem_init(&sem3,0,0);
	data.sem1=&sem1;
	data.sem2=&sem2;
	data.sem3=&sem3;
	data.variable=variable;
	data.partition=partition;
	data.ligne=ligne;
	data.augumente=augumente;
	data.ciclique=ciclique;
	ete=0;
	pthread_create(&pth,NULL,gj_mpi_pthread_th,(void *)&data);
	for(i=0;i<=partition;i++)
	{
		if(i!=partition) counter=ciclique[i];
		else counter=variable;
		for(k=ete;k<counter;k++)
		{
			l=rank-1;
			if(l<0) l=size-1;
			MPI_Recv(data.temp,2,MPI_INT,l,0,MPI_COMM_WORLD,&status);
			MPI_Recv(ligne,variable+1,MPI_DOUBLE,l,1,MPI_COMM_WORLD,&status);
			sem_post(&sem1);
			l=rank+1;
			if(l==size) l=0;
			if(l!=data.temp[1])
			{
				MPI_Send(data.temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
				MPI_Send(ligne,variable+1,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
			}
			sem_wait(&sem2);
		}
		if(i!=partition)
		{
			//send the line
			l=rank+1;
			if(l==size) l=0;
			memcpy(ligne,&augumente[i][0],(variable+1)*sizeof(double));
			data.temp[0]=ciclique[i];
			data.temp[1]=rank;
			MPI_Send(data.temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
			MPI_Send(ligne,variable+1,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
			ete=ciclique[i]+1;
		}
		sem_post(&sem3);
	}
	pthread_join(pth,NULL);
	for(i=0;i<partition;i++)
	{
		if(rank!=0)
			resoudre[i]=augumente[i][variable]/augumente[i][ciclique[i]];
		else
			resoudre[ciclique[i]]=augumente[i][variable]/augumente[i][ciclique[i]];
	}
	
	if(rank==0)
	{
		dsend=(gj_mpi_pth_gt_data *)calloc(size,sizeof(gj_mpi_pth_gt_data));
		psend=(pthread_t *)calloc(size,sizeof(pthread_t));
		for(i=1;i<size;i++)
		{
			dsend[i].resoudre=resoudre;
			dsend[i].size=size;
			dsend[i].variable=variable;
			dsend[i].i=i;
			pthread_create(&psend[i],NULL,gj_mpi_pthread_gt_th,(void *)&dsend[i]);
		}
		for(i=1;i<size;i++)
			pthread_join(psend[i],NULL);
	}
	else MPI_Send(&resoudre[0],partition,MPI_DOUBLE,0,1,MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	if(rank!=0)	free(px);
	else
	{
		free(dsend);
		free(psend);
	}
	sem_destroy(&sem1);
	sem_destroy(&sem2);
	sem_destroy(&sem3);
	free(*augumente);
	free(augumente);
	free(ciclique);
	free(ligne);
}