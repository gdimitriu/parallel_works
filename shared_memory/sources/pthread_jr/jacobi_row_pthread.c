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
/*
	JACOBI pthread with diagonal ROW dominant
*/
#include<string.h>
#include<math.h>
#include<pthread.h>
#include"barrier-pthread.h"

struct jacobi_row_dp
{
	double *xn_1;
	double *red;
	double *q;
	long dim;
	long thread;
	double **mat;
	double *ty;
	double *tx;
	long index;
	double err;
	double *sum;
	barrier_t *barrier;
};
void *jacobi_row_th(void *arg)
{
struct jacobi_row_dp *data=(struct jacobi_row_dp *)arg;
long i,j;
double temp;
	for(i=data->index;i<data->dim;i=i+data->thread)
		data->tx[i]=data->ty[i]/data->mat[i][i];
	data->red[data->index]=0.0;
	for(j=0;j<data->dim;j++)
		if(j!=data->index)
			data->red[data->index]+=fabs(data->mat[data->index][j]/data->mat[data->index][data->index]);
	for(i=data->index+data->thread;i<data->dim;i=i+data->thread)
	{
		temp=0.0;
		for(j=0;j<data->dim;j++)
			if(i!=j) temp+=fabs(data->mat[i][j]/data->mat[i][i]);
		if(data->red[data->index]<temp) data->red[data->index]=temp;
	}

	barrier_sync(data->barrier);

	if(data->index==0)
	{
		*(data->q)=data->red[0];
		for(i=1;i<data->thread;i++)
			if(*(data->q)<data->red[i])
				*(data->q)=data->red[i];
	}
	
	barrier_sync(data->barrier);
	
	data->red[data->index]=fabs(data->ty[data->index]/data->mat[data->index][data->index]);
	for(i=data->index+data->thread;i<data->dim;i=i+data->thread)
	{
		if(data->red[data->index]<fabs(data->ty[i]/data->mat[i][i]))
				data->red[data->index]=fabs(data->ty[i]/data->mat[i][i]);
	}
	
	barrier_sync(data->barrier);

	if(data->index==0)
	{
		*(data->sum)=data->red[0];
		for(i=1;i<data->thread;i++)
			if(*(data->sum)<data->red[i]) *(data->sum)=data->red[i];
		*(data->sum)=*(data->sum)*(*(data->q))/(1-(*(data->q)));
	}
	
	barrier_sync(data->barrier);
	
	while(fabs(*(data->sum))>data->err)
	{	
		for(i=data->index;i<data->dim;i=i+data->thread)
		{
			data->xn_1[i]=data->tx[i];
			data->tx[i]=data->ty[i]/data->mat[i][i];
		}
		for(i=data->index;i<data->dim;i=i+data->thread)
		{
			for(j=0;j<data->dim;j++)
				if(j!=i) data->tx[i]-=data->mat[i][j]/data->mat[i][i]*data->xn_1[j];
		}
		data->red[data->index]=fabs(data->tx[data->index]-data->xn_1[data->index]);
		for(i=data->index+data->thread;i<data->dim;i=i+data->thread)
			if(data->red[data->index]<fabs(data->tx[i]-data->xn_1[i]))
				data->red[data->index]=fabs(data->tx[i]-data->xn_1[i]);
		
		barrier_sync(data->barrier);
		
		if(data->index==0)
		{
			*(data->sum)=data->red[0];
			for(i=1;i<data->thread;i++)
				if(*(data->sum)<data->red[i]) *(data->sum)=data->red[i];
			*(data->sum)=*(data->sum)*(*(data->q))/(1-(*data->q));
		}
		barrier_sync(data->barrier);
	}
	return NULL;
}

void jacobi_row_pthread(double **mat,double *ty,double *tx,int dim,double err,long thread)
{
long i;
double *xn_1;
double *red;
struct jacobi_row_dp *distrib;
double q;
double sum;
barrier_t barrier;
pthread_t *th;
	xn_1=(double *)calloc(dim,sizeof(double));
	red=(double *)calloc(thread,sizeof(double));
	distrib=(struct jacobi_row_dp *)calloc(thread,sizeof(struct jacobi_row_dp));
	th=(pthread_t *)calloc(thread,sizeof(pthread_t));
	barrier_init(&barrier,thread);
	for(i=0;i<thread;i++)
	{
		distrib[i].xn_1=xn_1;
		distrib[i].red=red;
		distrib[i].q=&q;
		distrib[i].thread=thread;
		distrib[i].dim=dim;
		distrib[i].mat=mat;
		distrib[i].ty=ty;
		distrib[i].tx=tx;
		distrib[i].index=i;
		distrib[i].err=err;
		distrib[i].sum=&sum;
		distrib[i].barrier=&barrier;
	}
	q=0.0;
	for(i=0;i<thread;i++)
		pthread_create(&th[i],NULL,jacobi_row_th,(void *)&distrib[i]);
	for(i=0;i<thread;i++)
		pthread_join(th[i],NULL);
	free(xn_1);
	free(distrib);
	free(red);
	barrier_destroy(&barrier);
	free(th);
}
