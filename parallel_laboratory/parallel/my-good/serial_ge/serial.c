#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include <math.h>
#include "ge_serial.c"
#include"timeprint.c"
#define numar 10
int main(int argc,char **argv)
{
	struct timeval t1,t2;
	long i,l,j;
	long dim;
	int thread;
	FILE *fp;
	double **mat,*x,*rez,*temp1,temp,*y,**mat1,*y1;
	dim=atol(argv[1]);
	x=(double *)calloc(dim,sizeof(double));
	y=(double *)calloc(dim,sizeof(double));
	y1=(double *)calloc(dim,sizeof(double));
	rez=(double *)calloc(dim,sizeof(double));
	for(i=0;i<dim;i++) rez[i]=(double)i;
	mat=(double **)calloc(dim,sizeof(double *));
	temp1=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		mat[i]=temp1;
		temp1+=dim;
		x[i]=0;
	}
	mat1=(double **)calloc(dim,sizeof(double *));
	temp1=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		mat1[i]=temp1;
		temp1+=dim;
	}
	for(i=0;i<dim;i++)
	{
		for(j=0;j<dim;j++) mat[i][j]=(double)rand();
		temp=0.0;
		for(j=0;j<dim;j++) if(j!=i) temp+=abs(mat[i][j]);
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
		gauss_serial(mat1,y1,x,dim);
	}
	gettimeofday(&t2,NULL);
	for(i=0;i<dim;i++) {if(abs(rez[i]-x[i])>1E-5) printf("%lf=%lf\n",rez[i],x[i]); fflush(stdout);}
	fp=fopen("time-ser.dat","a");
	timeprint(t1,t2,numar,dim,fp,1);
	fclose(fp);
	free(*mat);
	free(mat);
	free(*mat1);
	free(mat);
	free(y1);
	free(x);
	free(rez);
	free(y);
	return 0;
}
