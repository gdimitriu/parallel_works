#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <upc_relaxed.h>
#include "timeprint.c"
#define numar 10
typedef shared double *shared tmat;
shared tmat *shared mat1;
shared tmat *shared mat;
shared double *shared mtemp1;
shared double *x;
shared double *y;
shared double *ytemp;
shared long dim;
int main(int argc,char **argv)
{
	struct timeval t1,t2;
	long i,j;
	long k;
	long l;
	int thread;
	FILE *fp;
	double *rez,temp;
	if(MYTHREAD==0)
	{
		dim=atol(argv[1]);
		thread=atoi(argv[2]);
	}
	x=(shared double *)upc_all_alloc(dim,sizeof(double));
	y=(shared double *)upc_all_alloc(dim,sizeof(double));
	ytemp=(shared double *)upc_all_alloc(dim,sizeof(double));
	mat=(shared tmat *shared)upc_all_alloc(dim,sizeof(shared double *shared));
	mat1=(shared tmat *shared)upc_all_alloc(dim,sizeof(shared double *shared));	
	if(MYTHREAD==0)
	{
		rez=(double *)calloc(dim,sizeof(double));
		for(i=0;i<dim;i++) rez[i]=(double)i;
		//here is the parallel zone
		mtemp1=(shared double *shared )upc_global_alloc(dim*dim,sizeof(shared double));
		for(i=0;i<dim;i++)
		{
			mat[i]=mtemp1;
			mtemp1+=dim;
		}
		mtemp1=(shared double *shared)upc_global_alloc(dim*dim,sizeof(shared double));
		for(i=0;i<dim;i++)
		{
			mat1[i]=mtemp1;
			mtemp1+=dim;
		}
		for(i=0;i<dim;i++)
		{
			for(j=0;j<dim;j++) mat[i][j]=(double)rand();
			temp=0.0;
			for(j=0;j<dim;j++) if(j!=i) temp+=fabs(mat[i][j]);
			mat[i][i]+=temp;
		}
		for(i=0;i<dim;i++)
		{
			y[i]=0.0;
			x[i]=0.0;
			for(j=0;j<dim;j++) y[i]+=mat[i][j]*rez[j];
		}
		gettimeofday(&t1,NULL);
	}
	for(l=0;l<numar;l++)
	{	
		upc_barrier;
		upc_forall(i=0;i<dim;i++;i)
		{
			for(j=0;j<dim;j++) mat1[i][j]=mat[i][j];
			ytemp[i]=y[i];
		}
		for(k=0;k<dim;k++)
		{
			upc_barrier;
			upc_forall(i=0;i<dim;i++;i)
			{
				if(i!=k)
				{
					temp=mat1[i][k]/mat1[k][k];
					for(j=k+1;j<dim;j++) mat1[i][j]-=temp*mat1[k][j];
					ytemp[i]-=temp*ytemp[k];
				}
			}
		}
//		upc_barrier;
		upc_forall(i=0;i<dim;i++;i) x[i]=ytemp[i]/mat1[i][i];
	}
	upc_barrier;
	if(MYTHREAD==0)
	{
		gettimeofday(&t2,NULL);
		for(i=0;i<dim;i++) {if(fabs(rez[i]-x[i])>1E-5) printf("%lf=%lf\n",rez[i],x[i]); fflush(stdout);}
		fp=fopen("time-upc.dat","a");
		timeprint(t1,t2,numar,dim,fp,thread);
		fclose(fp);
		upc_free(*mat);
		upc_free(mat);		
		upc_free(*mat1);
		upc_free(mat1);
		upc_free(x);
		free(rez);
		upc_free(y);
		upc_free(ytemp);
	}
	return 0;
}
