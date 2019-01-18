/*
	C'est le fichier pour efectue la multiplication matriciale avec process
*/
#include<parallel/process.h>
double **mult_sq_mat_striped_process(long mat,int thread,double **a,double **b,int *pshmid,int type)
{
pid_t *pids;
long i,j,k,s;
double **c;
double *temp;
long pos;
pid_t pid;
pid_t pidtemp;
int start,stop;
	pid=getpid();
	/* j'affecte la memoire pour la serie de pid */
	if((pids=(pid_t *)calloc(thread,sizeof(pid_t)))==(pid_t *)NULL)
	{
		perror("erreur d'affecte la memoire pour pid");
		return(NULL);
	}
	if((c=(double **)calloc(mat,sizeof(double *)))==NULL)
	{
		perror("erreur d'affecte la memoire pour *c");
		free(pids);
		return(NULL);
	}
	if((*pshmid=shmget((key_t)pid,(mat*mat*sizeof(double)),IPC_CREAT|0666))==-1)
	{
		perror("erreur d'affecte la memoire partage");
		free(pids);
		free(*c);
		return(NULL);
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
	if((temp=(double *)shmat(*pshmid,0,0))==(double *)-1)
	{
		perror("error a atache");
		shmctl(*pshmid,IPC_RMID,0);
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
		return(c);
	}
	else
	{
/*		i=mat%thread;
		s=mat/thread;
		if(pos==0)
		{
			start=0;
			stop=s+1;
		}
		else
		{
			if(i!=0)
			{
				if(pos<i)
				{
					s=s+1;
					start=s*pos;
					stop=s*(1+pos);
				}
				else
				{
					start=stop=(s+1)*i;
					start+=(pos-i)*s;
					stop+=((pos+1)-i)*s;
				}
			}
			else
			{
				start=s*pos;
				stop=s*(1+pos);
			}
		}*/
		if(type==1)
		{
/*			for(j=0;j<mat;j++)
			for(i=start;i<stop;i++)
			{
				c[i][j]=0.0;
				for(k=0;k<mat;k++) c[i][j]+=a[i][k]*b[k][j];
			} */
			for(i=pos;i<mat;i+=thread)
			for(j=0;j<mat;j++)
			{
				c[i][j]=0.0;
				for(k=0;k<mat;k++) c[i][j]+=a[i][k]*b[k][j];
			}
		}
		else
		{
/*			for(j=start;j<stop;j++)
			for(i=0;i<mat;i++)
			{
				c[i][j]=0.0;
				for(k=0;k<mat;k++) c[i][j]+=a[i][k]*b[k][j];
			} */
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