#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include <math.h>
#include"timeprint.c"
#define numar 10
int main(int argc,char **argv)
{
	struct timeval t1,t2;
	long i,l,j,k;
	long dim;
	int thread;
	FILE *fp;
	double **mat,*x,*rez,*mtemp1,temp,*y,**mat1,*y1;
	dim=atol(argv[1]);
	x=(double *)calloc(dim,sizeof(double));
	y=(double *)calloc(dim,sizeof(double));
	y1=(double *)calloc(dim,sizeof(double));
	rez=(double *)calloc(dim,sizeof(double));
	for(i=0;i<dim;i++) rez[i]=(double)i;
	mat=(double **)calloc(dim,sizeof(double *));
	mtemp1=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		mat[i]=mtemp1;
		mtemp1+=dim;
		x[i]=0;
	}
	mat1=(double **)calloc(dim,sizeof(double *));
	mtemp1=(double *)calloc(dim*dim,sizeof(double));
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
		for(j=0;j<dim;j++) y[i]+=mat[i][j]*rez[j];
	}
	gettimeofday(&t1,NULL);
	for(l=0;l<numar;l++)
	{
		for(i=0;i<dim;i++) for(j=0;j<dim;j++) mat1[i][j]=mat[i][j];
		for(i=0;i<dim;i++) y1[i]=y[i];
		for(k=0;k<dim;k++)
		{
			for(i=0;i<dim;i++)
				if(i!=k)
				{
					temp=mat1[i][k]/mat1[k][k];
					for(j=k+1;j<dim;j++) mat1[i][j]-=temp*mat1[k][j];
					y1[i]-=temp*y1[k];
				}
/*
			for(i=k+1;i<dim;i++)
			{
				temp=mat1[i][k]/mat1[k][k];
				for(j=k+1;j<dim;j++) mat1[i][j]-=temp*mat1[k][j];
				y1[i]-=temp*y1[k];
			}
			for(i=0;i<k;i++)
			{
				temp=mat1[i][k]/mat1[k][k];
				for(j=k+1;j<dim;j++) mat1[i][j]-=temp*mat1[k][j];
				y1[i]-=temp*y1[k];
			}
*/
		}
		for(i=0;i<dim;i++) x[i]=y1[i]/mat1[i][i];
	}
	gettimeofday(&t2,NULL);
	for(i=0;i<dim;i++) {if(fabs(rez[i]-x[i])>1E-5) printf("%lf=%lf\n",rez[i],x[i]); fflush(stdout);}
	fp=fopen("time-gcc.dat","a");
	timeprint(t1,t2,numar,dim,fp,1);
	fclose(fp);
	free(*mat);
	free(mat);
	free(*mat1);
	free(mat1);
	free(y1);
	free(x);
	free(rez);
	free(y);
	return 0;
}
