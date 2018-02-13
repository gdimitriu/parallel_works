/* Programul CLIENT pentru gauss*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<zlib.h>
#include<mpi.h>
#include"definitii.h"
#include"fisier.h"
#include"calcul1.h"
#include"acceleration.h"
#include<parallel/parallel-mpi.h>
//#include<parallel/parallel-mpi-pthread.h>

int main(int argc,char **argv)
{
int j,i,k;
struct timeval t1s,t2s,t1p,t2p;
FILE *sg,*pgrc;
int rank,size;
int nr,partition;
double **augumente,*paugumente,*px;
int *ciclique;
int derniere;
int temp;
MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
if(rank==0)
{
	sg=(FILE *)fopen("sg.dat","w");
	pgrc=(FILE *)fopen("pgrc.dat","w");
	read_data();
/*	for(i=0;i<variable;i++)
	{
		for(j=0;j<variable;j++)
		{
			fprintf(sg,"%e ",tmat[i][j]);
			fflush(stdout);
		}
		fprintf(sg,"%e \n",ty[i]);
		fflush(stdout);
	}	*/
	gettimeofday(&t1s,NULL);
	calculez1();
	gettimeofday(&t2s,NULL);
	for(j=0;j<variable;j++) 
	{
		fprintf(sg,"X%d=%f\n",j,tx[j]);
		fflush(sg);
	}
	fclose(sg);
	free(tx);
}
/* MPI implementation */
	MPI_Barrier(MPI_COMM_WORLD);
if(rank==0)	gettimeofday(&t1p,NULL);
	MPI_Bcast(&variable,1,MPI_INT,0,MPI_COMM_WORLD);
if(rank==0) tx=(double *)calloc(variable,sizeof(double));
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
if(rank!=0)	px=(double *)calloc(partition,sizeof(double));
	derniere=diffusion_gauss_striped_ciclique_mpi(rank,size,tmat,ty,augumente,ciclique,variable);
if(rank==0) gauss_striped_ciclique_mpi(rank,size,partition,ciclique,derniere,augumente,tx,variable);
else 	gauss_striped_ciclique_mpi(rank,size,partition,ciclique,derniere,augumente,px,variable);		
/*
if(rank!=0)
{	
	for(i=0;i<partition;i++)
	{
		MPI_Send(&ciclique[i],1,MPI_INT,0,0,MPI_COMM_WORLD);
		MPI_Send(augumente[i],variable,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
		MPI_Send(&augumente[i][variable],1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
//		MPI_Send(&px[i],1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
	}
}
else
{
//	for(i=0;i<variable;i++)
//	for(j=0;j<variable;j++)
//		tmat[i][j]=0.0;
//	for(i=0;i<variable;i++) ty[i]=10.0;
	nr=variable%size;
	partition=(variable-nr)/size;
	if(nr>0) temp=partition+1;
	else temp=partition;
	for(i=0;i<temp;i++)
//		tx[ciclique[i]]=px[i];
	{
		memcpy(tmat[ciclique[i]],augumente[i],variable*sizeof(double));
		ty[ciclique[i]]=augumente[i][variable];
	} 
	for(i=1;i<size;i++)
	{
		if(i<nr) temp=partition+1;
		else temp=partition;
		for(k=0;k<temp;k++)
		{
			MPI_Recv(&j,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);	
			MPI_Recv(tmat[j],variable,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&status);
			MPI_Recv(&ty[j],1,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&status);
//			MPI_Recv(&tx[j],1,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&status);
		}
	}
}*/
if(rank==0)
{
/*	for(i=0;i<variable;i++)
	{
		for(j=0;j<variable;j++)
		{
			fprintf(pgrc,"%e ",tmat[i][j]);
			fflush(stdout);
		}
		fprintf(pgrc,"%e \n",ty[i]);fflush(stdout);
	} */
	gettimeofday(&t2p,NULL);
	acceleration(t1s,t2s,t1p,t2p,size);
	for(j=0;j<variable;j++)
	{
		fprintf(pgrc, "X%d=%f\n",j,tx[j]);
		fflush(pgrc);
	}
	fclose(pgrc);
	free(*tmat);
	free(tmat);
	free(ty);
	free(tx);
}
else 	free(px);
	free(*augumente);
	free(augumente);
	free(ciclique);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	
	return 0;
}
