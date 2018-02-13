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
	JACOBI pthread with diagonal COLLUMN dominant
*/
#include<string.h>
#include<math.h>
#include<pthread.h>
#include"barrier-pthread.h"
struct jacobi_col_dp
{
	double *xn_1;
	double *yn;
	double *yn_1;
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
	double *max;
	double *count;
	barrier_t *barrier;
};
void *jacobi_col_pth(void *arg)
{
struct jacobi_col_dp *data=(struct jacobi_col_dp *)arg;
long i,j;
double temp;
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

	data->red[data->index]=fabs(data->mat[data->index][data->index]);
	for(i=data->index+data->thread;i<data->dim;i=i+data->thread)
		if(data->red[data->index]>fabs(data->mat[i][i]))
			data->red[data->index]=fabs(data->mat[i][i]);
	
	barrier_sync(data->barrier);

	if(data->index==0)
	{
		*(data->max)=data->red[0];
		for(i=1;i<data->thread;i++)
			if(*(data->max)>data->red[i]) *(data->max)=data->red[i];
		*(data->max)=*(data->q)/(*(data->max)*(1-*(data->q)));
		*(data->sum)=0.0;
	}

	barrier_sync(data->barrier);

	data->yn[data->index]=data->ty[data->index];
	data->red[data->index]=fabs(data->yn[data->index]);
	for(i=data->index+data->thread;i<data->dim;i=i+data->thread)
	{
		data->yn[i]=data->ty[i];
		data->red[data->index]+=fabs(data->yn[i]);
	}
	
	barrier_sync(data->barrier);
	
	if(data->index==0)
	{
		*(data->sum)=data->red[0];
		for(i=1;i<data->thread;i++)
			*(data->sum)+=data->red[i];
		*(data->count)=*(data->max)*(*(data->sum));
	}

	barrier_sync(data->barrier);

	while(fabs(*(data->count))>data->err)
	{
		for(i=data->index;i<data->dim;i=i+data->thread)
			data->yn_1[i]=data->yn[i];
		for(i=data->index;i<data->dim;i=i+data->thread)
		{
			data->yn[i]=data->ty[i];
			for(j=0;j<data->dim;j++) 
				if(i!=j) data->yn[i]-=data->mat[i][j]/data->mat[j][j]*data->yn_1[j];
			data->tx[i]=data->yn[i]/data->mat[i][i];
		}

		data->red[data->index]=fabs(data->yn[data->index]-data->yn_1[data->index]);
		for(i=data->index+data->thread;i<data->dim;i=i+data->thread)
			data->red[data->index]+=fabs(data->yn[i]-data->yn_1[i]);

		barrier_sync(data->barrier);

		if(data->index==0)
		{
			*(data->sum)=data->red[0];
			for(i=1;i<data->thread;i++)
				*(data->sum)+=data->red[i];
			*(data->count)=*(data->max)*(*(data->sum));
		}
		
		barrier_sync(data->barrier);
	}
	pthread_exit(NULL);
}
void jacobi_col_pthread(double **mat,double *ty,double *tx,long dim,double err,long thread)
{
long i;
double *xn_1;
double *red;
struct jacobi_col_dp *distrib;
double q;
double sum;
double max;
double count;
barrier_t barrier;
pthread_t *th;
double *yn,*yn_1;
	//compute q
	q=0.0;
	xn_1=(double *)calloc(dim,sizeof(double));
	yn=(double *)calloc(dim,sizeof(double));
	yn_1=(double *)calloc(dim,sizeof(double));
	red=(double *)calloc(thread,sizeof(double));
	distrib=(struct jacobi_col_dp *)calloc(thread,sizeof(struct jacobi_col_dp));
	th=(pthread_t *)calloc(thread,sizeof(pthread_t));
	barrier_init(&barrier,thread);
	for(i=0;i<thread;i++)
	{
		distrib[i].q=&q;
		distrib[i].max=&max;
		distrib[i].sum=&sum;
		distrib[i].count=&count;
		distrib[i].thread=thread;
		distrib[i].dim=dim;
		distrib[i].index=i;
		distrib[i].err=err;
		distrib[i].mat=mat;
		distrib[i].xn_1=xn_1;
		distrib[i].red=red;
		distrib[i].ty=ty;
		distrib[i].tx=tx;
		distrib[i].barrier=&barrier;
		distrib[i].yn=yn;
		distrib[i].yn_1=yn_1;
	}
	q=0.0;
	for(i=0;i<thread;i++)
		pthread_create(&th[i],NULL,jacobi_col_pth,(void *)&distrib[i]);
	for(i=0;i<thread;i++)
		pthread_join(th[i],NULL);
	barrier_destroy(&barrier);
	free(xn_1);
	free(distrib);
	free(red);
	free(yn);
	free(yn_1);
	free(th);
}