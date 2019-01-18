#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<string.h>
#include"timeprint.c"
#include "jacobi.c"
#define numar 10
int main(int argc,char **argv)
{
	struct timeval t1,t2;
	double **mat,*x,*rez,*temp1,temp,*y;
	double err;
	int i,l,j;
	int dim;
	FILE *fp;
	int thread;
	dim=atoi(argv[1]);
	y=(double *)calloc(dim,sizeof(double));
	rez=(double *)calloc(dim,sizeof(double));
	//here is the parallel zone
	mat=(double **)calloc(dim,sizeof(double *));
	temp1=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		mat[i]=temp1;
		temp1+=dim;
	}
	x=(double *)calloc(dim,sizeof(double));
	/* create the test matrix */
	for(i=0;i<dim;i++) rez[i]=(double)i+1;
	for(i=0;i<dim;i++)
		for(j=0;j<dim;j++)
		{
			if(i!=j)
			{
				mat[i][j]=20000*rand()/(double)RAND_MAX;
				if((rand()/(double)RAND_MAX)<0.5) mat[i][j]=-mat[i][j];
			}
		}
	for(i=0;i<dim;i++)
	{
		temp=0.0;
		for(j=0;j<dim;j++) if(j!=i) temp+=fabs(mat[i][j]);
		mat[i][i]=temp+(20000+temp)*rand()/(double)RAND_MAX+0.00001;
		if((rand()/(double)RAND_MAX)<0.5) mat[i][i]=-mat[i][i];
	}
	//generate the free term
	for(i=0;i<dim;i++)
	{
		y[i]=0.0;
		x[i]=0.0;
		for(j=0;j<dim;j++) y[i]+=mat[i][j]*rez[j];
	}
	err=atof(argv[2]);
	gettimeofday(&t1,NULL);
	for(l=0;l<numar;l++)
	{
		jacobi(mat,y,x,dim,err);
	}
	gettimeofday(&t2,NULL);
	for(i=0;i<dim;i++) {if(fabs(rez[i]-x[i])>1E-5) printf("%lf=%lf\n",rez[i],x[i]); fflush(stdout);}
	fp=fopen("time-ser.dat","a");
	timeprint(t1,t2,numar,dim,fp,1);
	fclose(fp);
	free(*mat);
	free(mat);
	free(rez);
	free(y);
	free(x);
	return 0;
}
