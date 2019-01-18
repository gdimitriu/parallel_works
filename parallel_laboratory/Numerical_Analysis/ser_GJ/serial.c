#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define numar 1
int gaussJ_serial(double **mat,double *x,double *y,long dim,long thread);
int main(int argc,char **argv)
{
	long i,l,j;
	long dim;
	long thread;
	double **mat,*x,*rez,*temp1,temp,*y,**mat1,*y1;
	scanf("%ld",&dim);
	x=(double *)calloc(dim,sizeof(double));
	y=(double *)calloc(dim,sizeof(double));
	y1=(double *)calloc(dim,sizeof(double));
	rez=(double *)calloc(dim,sizeof(double));
	for(i=0;i<dim;i++)
		rez[i]=(double)i;
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
		for(j=0;j<dim;j++) 
			mat[i][j]=(double)rand();
		temp=0.0;
		for(j=0;j<dim;j++) 
			if(j!=i) temp+=fabs(mat[i][j]);
		mat[i][i]+=temp;
	}
	for(i=0;i<dim;i++)
	{
		y[i]=0.0;
		for(j=0;j<dim;j++) 
			y[i]+=mat[i][j]*rez[j];
	}
	for(l=0;l<numar;l++)
	{
		for(i=0;i<dim;i++) 
			for(j=0;j<dim;j++) 
				mat1[i][j]=mat[i][j];
		for(i=0;i<dim;i++) 
			y1[i]=y[i];
		gaussJ_serial(mat1,x,y1,dim,thread);
	}
	for(i=0;i<dim;i++)
	{
		if(fabs(rez[i]-x[i])>1E-5) 
			printf("%lf=%lf\n",rez[i],x[i]); 
		fflush(stdout);
	}
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
