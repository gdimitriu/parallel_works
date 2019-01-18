#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include"timeprint.c"
#include "leahu.c"
#define numar 10
int main(int argc,char **argv)
{
	struct timeval t1,t2;
	int i,l,j;
	int dim;
	int thread;
	FILE *fp;
	float **mat,*x,*rez,*temp1,temp,*y,**mat1,*y1;
	dim=atoi(argv[1]);
	thread=atoi(argv[2]);
	x=(float *)calloc(dim,sizeof(float));
	y=(float *)calloc(dim,sizeof(float));
	y1=(float *)calloc(dim,sizeof(float));
	rez=(float *)calloc(dim,sizeof(float));
	for(i=0;i<dim;i++) rez[i]=(float)i;
	//here is the parallel zone
	mat=(float **)calloc(dim,sizeof(float *));
	temp1=(float *)calloc(dim*dim,sizeof(float));
	for(i=0;i<dim;i++)
	{
		mat[i]=temp1;
		temp1+=dim;
	}
	mat1=(float **)calloc(dim,sizeof(float *));
	temp1=(float *)calloc(dim*dim,sizeof(float));
	for(i=0;i<dim;i++)
	{
		mat1[i]=temp1;
		temp1+=dim;
	}
	for(i=0;i<dim;i++)
	{
		for(j=0;j<dim;j++) mat[i][j]=(float)rand();
		temp=0.0;
		for(j=0;j<dim;j++) if(j!=i) temp+=abs(mat[i][j]);
		mat[i][i]+=temp;
	}
	for(i=0;i<dim;i++)
	{
		y[i]=0.0;
		x[i]=0.0;
		for(j=0;j<dim;j++) y[i]+=mat[i][j]*rez[j];
	}
	gettimeofday(&t1,NULL);
	for(l=0;l<numar;l++)
	{	
		#pragma omp parallel for private(j)
		for(i=0;i<dim;i++)
		{
			for(j=0;j<dim;j++) mat1[i][j]=mat[i][j];
		}
		#pragma omp parallel for
		for(i=0;i<dim;i++) y1[i]=y[i];
		gauss(dim,mat1,x,y1,thread);
	}
	gettimeofday(&t2,NULL);
	for(i=0;i<dim;i++) {if(abs(rez[i]-x[i])>1E-5) printf("%f=%f\n",rez[i],x[i]); fflush(stdout);}
	fp=fopen("time-par.dat","a");
	timeprint(t1,t2,numar,dim,fp,thread);
	fclose(fp);
	free(*mat);
	free(mat);		
	free(*mat1);
	free(mat1);
	free(x);
	free(rez);
	free(y);
	free(y1);
	return 0;
}
