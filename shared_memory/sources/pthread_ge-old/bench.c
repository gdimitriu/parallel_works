/*
	Copyright 2006 Gabriel Dimitriu

	This file is part of scientific_computing.

    scientific_computing is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    scientific_computing is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with scientific_computing; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/time.h>
#include"solve-pthread.h"
#include"timeprint.c"
#define numar 10

struct datas
{
	int id;	//nr of thread
	int threads; //nr of threads
	double **mat,**mat1,*y,*y1;
	long dim;
};
void *thread_distribution(void *data1)
{
	struct datas *data=(struct datas *)data1;
	long i,j;
	long s,nr;
	nr=data->dim/data->threads;
	s=(data->dim-nr)/data->threads;
	if((data->id+1)<=nr) s++;
	for(i=data->id;i<data->dim;i+=data->threads)
	{
		for(j=0;j<data->dim;j++) data->mat1[i][j]=data->mat[i][j];
		data->y1[i]=data->y[i];
	}
	pthread_exit(NULL);
}
int main(int argc,char **argv)
{
	struct timeval t1,t2;
	double **mat,*x,*rez,*temp1,temp,*y,**mat1,*y1;
	pthread_t *th;
	long i,l,j;
	long dim;
	int thread;
	FILE *fp;
	pthread_attr_t attr;
	struct datas *data;
	dim=atol(argv[1]);
	thread=atoi(argv[2]);
	x=(double *)calloc(dim,sizeof(double));
	y=(double *)calloc(dim,sizeof(double));
	y1=(double *)calloc(dim,sizeof(double));
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
		x[i]=0.0;
		for(j=0;j<dim;j++) y[i]+=mat[i][j]*rez[j];
	}
	gettimeofday(&t1,NULL);
	th=(pthread_t *)calloc(thread,sizeof(pthread_t));
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);
	data=(struct datas *)calloc(thread,sizeof(struct datas));
	for(i=0;i<thread;i++)
	{
		data[i].id=i;
		data[i].dim=dim;
		data[i].threads=thread;
		data[i].mat1=mat1;
		data[i].mat=mat;
		data[i].y=y;
		data[i].y1=y1;
	}
	for(l=0;l<numar;l++)
	{	
		for(i=0;i<thread;i++) pthread_create(&th[i],&attr,thread_distribution,(void *)&data[i]);
		for(i=0;i<thread;i++) pthread_join(th[i],NULL);
		gauss_striped_fast(dim,thread,mat1,x,y1);
//		for(i=0;i<dim;i++) {if(abs(rez[i]-x[i])>1E-5) printf("%lf=%lf\n",rez[i],x[i]); fflush(stdout);}
	}
	gettimeofday(&t2,NULL);
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
	free(th);
	free(data);
	return 0;
}
