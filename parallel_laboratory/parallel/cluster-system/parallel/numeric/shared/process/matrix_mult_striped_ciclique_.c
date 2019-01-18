/*
	C'est le fichier pour efectue la multiplication matriciale avec process
*/
#include<parallel/process.h>
int mult_sq_mat_striped_ciclique_process(long mat,int thread,double **a,double **b,double **c,int type)
{
pid_t *pids;
long i,j,k;
double *temp;
int shmid;
long pos;
pid_t pid;
pid_t pidtemp;
	pid=getpid();
	/* j'affecte la memoire pour la serie de pid */
	if((pids=(pid_t *)calloc(thread,sizeof(pid_t)))==(pid_t *)NULL)
	{
		perror("erreur d'affecte la memoire pour pid");
		return(-1);
	}
	if((shmid=shmget((key_t)pid,(mat*mat*sizeof(double)),IPC_CREAT|0666))==-1)
	{
		perror("erreur d'affecte la memoire partage");
		free(pids);
		return(-1);
	}
	for(pos=0;pos<thread;pos++)
	{
		pidtemp=fork();
		if(pidtemp==0)
		{
			//je suis dans le proc fils
			break;
		}
		else
		{
			pids[pos]=pidtemp;
		}
	}
	if((temp=(double *)shmat(shmid,0,0))==(double *)-1)
	{
		perror("error a atache");
		shmctl(shmid,IPC_RMID,0);
		exit(-1);
	}
	for(j=0;j<mat;j++)
	{
		c[j]=temp;
		temp+=mat;
	}
	if(pidtemp!=0)
	{
		for(i=0;i<thread;i++)
			waitpid(pids[i],NULL,0);
		return(shmid);
	}
	else
	{
		if(type==1)
		{
			for(i=pos;i<mat;i+=thread)
			for(j=0;j<mat;j++)
			{
				c[i][j]=0.0;
				for(k=0;k<mat;k++) c[i][j]+=a[i][k]*b[k][j];
			}
		}
		else
		{
			for(j=pos;j<mat;j+=thread)
			for(i=0;i<mat;i++)
			{
				c[i][j]=0.0;
				for(k=0;k<mat;k++) c[i][j]+=a[i][k]*b[k][j];
			}
		}
		exit(1);
	}
}