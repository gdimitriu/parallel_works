/*
	la function pour faire elimation gaussiene dans le reseau ciclique avec
	distribution striped dans MPI avec STHREAD
*/
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<st.h>
#include"diffusion_mpi_sth.c"
st_join_counter_t joinc[1];
st_mutex_t gauss_striped_ciclique_mutex[1];
st_cond_t gauss_striped_ciclique_cond[1];

void gauss_striped_ciclique_mpi_sth_th(double *ligne,int *temp, int variable, int rank,int size,int lastrow)
{
MPI_Status status;
int l;
	l=rank-1;
	if(l<0) l=size-1;
	MPI_Recv(temp,2,MPI_INT,l,0,MPI_COMM_WORLD,&status);
	MPI_Recv(&ligne[temp[0]],(variable-temp[0]+1),MPI_DOUBLE,l,1,MPI_COMM_WORLD,&status);
	st_mutex_lock(gauss_striped_ciclique_mutex);
	st_cond_signal(gauss_striped_ciclique_cond);
	st_mutex_unlock(gauss_striped_ciclique_mutex);
	ST_POLLING();
	st_yield();
	l=rank+1;
	if(l==size) l=0;
	if((variable-temp[0])<size)
	{
		if(lastrow==0)
		{
//			printf("send1 %d at %d  from %d row %d\n",rank,l,temp[1],temp[0]);fflush(stdout);
			MPI_Send(temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
			MPI_Send(&ligne[temp[0]],(variable-temp[0]+1),MPI_DOUBLE,l,1,MPI_COMM_WORLD);
		}
	}
	else	
	if(l!=temp[1])
	{
//		printf("send1 %d at %d  from %d row %d\n",rank,l,temp[1],temp[0]);fflush(stdout);
		MPI_Send(temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
		MPI_Send(&ligne[temp[0]],(variable-temp[0]+1),MPI_DOUBLE,l,1,MPI_COMM_WORLD);
	}
	st_join_counter_finish(joinc);
}
void gauss_striped_ciclique_mpi_sth_th1(double *ligne,int *temp, int variable, int rank,int size)
{
MPI_Status status;
int l;
	l=rank+1;
	if(l==size) l=0;
//	printf("send2 %d at %d  from %d row %d\n",rank,l,temp[1],temp[0]);fflush(stdout);
	MPI_Send(temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
	MPI_Send(&ligne[temp[0]],(variable-temp[0]+1),MPI_DOUBLE,l,1,MPI_COMM_WORLD);
	st_join_counter_finish(joinc);
}

void st_gauss_MPI(double **mat,double *y,double *x,long variable)
{
double *ligne;
int num,ete;
int l;
int i,j,k,m;
int temp[2];
MPI_Status status;
int flag;
int rank,size;
long nr;
double *tx,*ty,*px;
double *resoudre;
double **augumente,*paugumente;
int *ciclique,partition,derniere;
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
if(rank!=0)	px=(double *)calloc(partition,sizeof(double));
	derniere=diffusion_gauss_sc_mpi_sth(rank,size,mat,y,augumente,ciclique,variable);
if(rank==0) resoudre=x;
else resoudre=px;

	//make gaussian elimination	
	ete=0;
	st_mutex_init(gauss_striped_ciclique_mutex);
	st_cond_init(gauss_striped_ciclique_cond);
	for(i=0;i<partition;i++)
	{
		for(k=ete;k<ciclique[i];k++)
		{
			//il a receptione une ligne
			st_mutex_lock(gauss_striped_ciclique_mutex);
			st_join_counter_init(joinc,1);
			ST_THREAD_CREATE(gauss_striped_ciclique_mpi_sth_th(ligne,temp,variable,rank,size,derniere));
			st_cond_wait(gauss_striped_ciclique_cond,gauss_striped_ciclique_mutex);
			st_mutex_unlock(gauss_striped_ciclique_mutex);
			ST_POLLING();
			//fait elimination pour les lignes qui sont dans le meme machine
			for(m=i;m<partition;m++)
			{
				for(j=temp[0]+1;j<variable+1;j++) augumente[m][j]-=augumente[m][temp[0]]*ligne[j];
				augumente[m][temp[0]]=0.0;
			}
			st_join_counter_wait(joinc);
		}
		//faite la division
		for(j=ciclique[i]+1;j<variable+1;j++) augumente[i][j]=augumente[i][j]/augumente[i][ciclique[i]];
		augumente[i][ciclique[i]]=1.0;
if(rank==0)	resoudre[ciclique[i]]=augumente[i][variable];
else	resoudre[i]=augumente[i][variable];
		if(!((i==(partition-1))&&(derniere==1)))
		{
			memcpy(ligne,augumente[i],(variable+1)*sizeof(double));
			temp[0]=ciclique[i];
			temp[1]=rank;
			st_join_counter_init(joinc,1);
			ST_THREAD_CREATE(gauss_striped_ciclique_mpi_sth_th1(ligne,temp,variable,rank,size));
			ST_POLLING();
			//fait elimination pour les lignes qui sont dans le meme machine
			for(k=i+1;k<partition;k++)
			{
				for(j=ciclique[i]+1;j<variable+1;j++) augumente[k][j]-=augumente[k][ciclique[i]]*ligne[j];
				augumente[k][ciclique[i]]=0.0;
			}
			ete=ciclique[i]+1;
			st_join_counter_wait(joinc);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	//make backsubstitution
	if(derniere==1) ete=ciclique[partition-1]-1;
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
			if(rank==0) resoudre[ciclique[m]]=resoudre[ciclique[m]]-augumente[m][temp[0]]*ligne[0];
			else resoudre[m]=resoudre[m]-augumente[m][temp[0]]*ligne[0];
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
			if(rank==0) resoudre[ciclique[m]]=resoudre[ciclique[m]]-augumente[m][temp[0]]*ligne[0];
			else resoudre[m]=resoudre[m]-augumente[m][temp[0]]*ligne[0];
			ete=ciclique[i]-1;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);	
	
	st_mutex_destroy(gauss_striped_ciclique_mutex);
	st_cond_destroy(gauss_striped_ciclique_cond);
	st_join_counter_destroy(joinc);
	free(ligne);
	if(rank!=0)	free(px);
	free(*augumente);
	free(augumente);
	free(ciclique);
}