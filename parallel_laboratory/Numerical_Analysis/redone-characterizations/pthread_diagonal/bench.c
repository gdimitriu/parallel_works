#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include"pthread_diag.c"
#include"timeprint.c"
#define numar 10

int main(int argc,char **argv)
{
	struct timeval t1,t2;
	double **mat,*x,*rez,*temp1,temp,*y;
	long i,l,j;
	long dim;
	int thread;
	FILE *fp;
	dim=atol(argv[1]);
	thread=atoi(argv[2]);
	x=(double *)calloc(dim,sizeof(double));
	y=(double *)calloc(dim,sizeof(double));
	rez=(double *)calloc(dim,sizeof(double));
	for(i=0;i<dim;i++) rez[i]=(double)i;
	//here is the parallel zone
	mat=(double **)calloc(dim,sizeof(double *));
	temp1=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		mat[i]=temp1;
		temp1+=dim;
	}
	for(i=0;i<dim;i++)
	{
		for(j=0;j<=i;j++) mat[i][j]=(double)rand();
		for(j=i+1;j<dim;j++) mat[i][j]=0.0;
		temp=0.0;
		for(j=0;j<i;j++) temp+=abs(mat[i][j]);
		mat[i][i]+=temp;
	}
	for(i=0;i<dim;i++)
	{
		y[i]=0.0;
		x[i]=0.0;
		for(j=0;j<=i;j++) y[i]+=mat[i][j]*rez[j];
	}
	gettimeofday(&t1,NULL);
	for(l=0;l<numar;l++)
		pthread_diag(dim,thread,mat,x,y);
	gettimeofday(&t2,NULL);
	fp=fopen("time-par.dat","a");
	timeprint(t1,t2,numar,dim,fp,thread);
	fclose(fp);
	free(*mat);
	free(mat);		
	free(x);
	free(rez);
	free(y);
	return 0;
}
