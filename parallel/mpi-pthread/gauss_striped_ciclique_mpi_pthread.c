/*
	la function pour faire elimation gaussiene dans le reseau ciclique avec
	distribution striped dans MPI avec pthread
*/
#include<parallel/parallel-mpi-pthread.h>
pthread_mutex_t gauss_striped_ciclique_mutex;
pthread_cond_t gauss_striped_ciclique_cond0=PTHREAD_COND_INITIALIZER;
typedef struct
{
	double *ligne;
	int *temp;
	int variable;
	int rank;
	int size;
} gauss_striped_ciclique_mpi_pthread_data;
void *gauss_striped_ciclique_mpi_pthread_th(void *input)
{
gauss_striped_ciclique_mpi_pthread_data data=*((gauss_striped_ciclique_mpi_pthread_data *)input);
MPI_Status status;
int l;
	l=data.rank-1;
	if(l<0) l=data.size-1;
	MPI_Recv(data.temp,2,MPI_INT,l,0,MPI_COMM_WORLD,&status);
	MPI_Recv(&data.ligne[data.temp[0]],(data.variable-data.temp[0]+1),MPI_DOUBLE,l,1,MPI_COMM_WORLD,&status);
//	MPI_Recv(data.ligne,data.variable+1,MPI_DOUBLE,l,1,MPI_COMM_WORLD,&status);
	pthread_mutex_lock(&gauss_striped_ciclique_mutex);
	pthread_cond_signal(&gauss_striped_ciclique_cond0);
	pthread_mutex_unlock(&gauss_striped_ciclique_mutex);
	l=data.rank+1;
	if(l==data.size) l=0;
	if((l!=data.temp[1])&&(!((data.temp[0]+1+data.rank-data.temp[1])>=data.variable)))
	{
		MPI_Send(data.temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
		MPI_Send(&data.ligne[data.temp[0]],(data.variable-data.temp[0]+1),MPI_DOUBLE,l,1,MPI_COMM_WORLD);
//		MPI_Send(data.ligne,data.variable+1,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
	}
	pthread_exit(NULL);
}
void *gauss_striped_ciclique_mpi_pthread_th1(void *input)
{
gauss_striped_ciclique_mpi_pthread_data data=*((gauss_striped_ciclique_mpi_pthread_data *)input);
MPI_Status status;
int l;
	l=data.rank+1;
	if(l==data.size) l=0;
	MPI_Send(data.temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
	MPI_Send(&data.ligne[data.temp[0]],(data.variable-data.temp[0]+1),MPI_DOUBLE,l,1,MPI_COMM_WORLD);
//	MPI_Send(data.ligne,(data.variable+1),MPI_DOUBLE,l,1,MPI_COMM_WORLD);
	pthread_exit(NULL);
}
void gauss_striped_ciclique_mpi_pthread(int rank,int size,int partition,int *ciclique,int derniere,double **augumente,double *resoudre,int variable)
/*
	rank c'est le numero de processeur
	size c'est le numero des processeurs
	partition c'est le numero de ligne dans cette machine
	ciclique c'est le vector avec les numeros des lignes
	derniere c'est le flag qui sais si cette machine est la derniere machine de calcul
	augumente c'est la matrice augumente
	resoudre c'est le vector de resoudre
*/
{
double *ligne;
int num,ete;
int l;
int i,j,k,m;
int temp[2];
MPI_Status status;
gauss_striped_ciclique_mpi_pthread_data data;
pthread_t pth;
	ligne=(double *)calloc(variable+1,sizeof(double));
	ete=0;
	data.ligne=ligne;
	data.variable=variable;
	data.temp=temp;
	data.rank=rank;
	data.size=size;
	pthread_mutex_init(&gauss_striped_ciclique_mutex,NULL);
	pthread_cond_init(&gauss_striped_ciclique_cond0,NULL);
	for(i=0;i<partition;i++)
	{
		for(k=ete;k<ciclique[i];k++)
		{
			//il a receptione une ligne
			pthread_mutex_lock(&gauss_striped_ciclique_mutex);
			pthread_create(&pth,NULL,gauss_striped_ciclique_mpi_pthread_th,&data);
			pthread_cond_wait(&gauss_striped_ciclique_cond0,&gauss_striped_ciclique_mutex);
			pthread_mutex_unlock(&gauss_striped_ciclique_mutex);
			//fait elimination pour les lignes qui sont dans le meme machine
			for(m=i;m<partition;m++)
			{
				for(j=temp[0]+1;j<variable+1;j++)
					augumente[m][j]-=augumente[m][temp[0]]*ligne[j];
				augumente[m][temp[0]]=0.0;
			}
			pthread_join(pth,NULL);
		}
		//faite la division
		for(j=ciclique[i]+1;j<variable+1;j++)
			augumente[i][j]=augumente[i][j]/augumente[i][ciclique[i]];
		augumente[i][ciclique[i]]=1.0;
if(rank==0)	resoudre[ciclique[i]]=augumente[i][variable];
else	resoudre[i]=augumente[i][variable];
		if(!((i==(partition-1))&&(derniere==1)))
		{
			memcpy(ligne,augumente[i],(variable+1)*sizeof(double));
			temp[0]=ciclique[i];
			temp[1]=rank;
			pthread_create(&pth,NULL,gauss_striped_ciclique_mpi_pthread_th1,&data);
			//fait elimination pour les lignes qui sont dans le meme machine
			for(k=i+1;k<partition;k++)
			{
				for(j=ciclique[i]+1;j<variable+1;j++)
					augumente[k][j]-=augumente[k][ciclique[i]]*ligne[j];
				augumente[k][ciclique[i]]=0.0;
			}
			ete=ciclique[i]+1;
			pthread_join(pth,NULL);
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(derniere==1)
		ete=ciclique[partition-1]-1;
	else ete=variable-1;
	for(i=partition-1;i>=0;i--)
	{
		for(k=0;k<(ete-ciclique[i]);k++)
		{
			l=rank+1;
			if(l==size) l=0;
			MPI_Recv(temp,2,MPI_INT,l,0,MPI_COMM_WORLD,&status);
			MPI_Recv(&ligne[0],1,MPI_DOUBLE,l,1,MPI_COMM_WORLD,&status);
			l=rank-1;
			if(l<0) l=size-1;
			if((l!=temp[1])&&!((rank==0)&&(i==0)))
			{
				MPI_Send(temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
				MPI_Send(&ligne[0],1,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
			}
			if(rank==0) resoudre[temp[0]]=ligne[0];
			for(m=0;m<i+1;m++)
			if(rank==0)
				resoudre[ciclique[m]]=resoudre[ciclique[m]]-augumente[m][temp[0]]*ligne[0];
			else
				resoudre[m]=resoudre[m]-augumente[m][temp[0]]*ligne[0];
		}
		if(!((i==0)&&(rank==0)))
		{
			l=rank-1;
			if(l<0) l=size-1;
			if(rank==0) ligne[0]=resoudre[ciclique[i]];
			else ligne[0]=resoudre[i];
			temp[0]=ciclique[i];
			temp[1]=rank;
			MPI_Send(temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
			MPI_Send(&ligne[0],1,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
			for(m=0;m<i;m++)
			if(rank==0)
				resoudre[ciclique[m]]=resoudre[ciclique[m]]-augumente[m][temp[0]]*ligne[0];
			else
				resoudre[m]=resoudre[m]-augumente[m][temp[0]]*ligne[0];
			ete=ciclique[i]-1;
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);	
	pthread_mutex_destroy(&gauss_striped_ciclique_mutex);
	pthread_cond_destroy(&gauss_striped_ciclique_cond0);
	free(ligne);
}