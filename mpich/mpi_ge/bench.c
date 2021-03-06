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
#include<sys/time.h>
#include<string.h>
#include"timeprint.c"
#include <mpi.h>
#include"gauss_sc_mpi.c"
#include <math.h>
#define numar 10

int main(int argc,char **argv)
{
	struct timeval t1,t2;
	double **mat,*x,*rez,*temp1,temp,*y;
	int i,l,j;
	int dim;
	int rank;
	FILE *fp;
	int thread;
/*	#if defined(GLOBUS_CALLBACK_GLOBAL_SPACE)
	    globus_module_set_args(&argc, &argv);
	#endif

	    globus_module_activate(GLOBUS_DUROC_RUNTIME_MODULE);
	    globus_duroc_runtime_barrier();
	    globus_module_deactivate(GLOBUS_DUROC_RUNTIME_MODULE);*/
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&thread);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	dim=atoi(argv[1]);
	MPI_Bcast(&dim,1,MPI_INT,0,MPI_COMM_WORLD);
	if(rank==0)
	{
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
	}
	x=(double *)calloc(dim,sizeof(double));
	if(rank==0)
	{
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
	}
	for(l=0;l<numar;l++)
	{
		gauss_MPI(mat,y,x,dim);
	}
	if(rank==0) 
	{
		gettimeofday(&t2,NULL);
		if(rank==0) for(i=0;i<dim;i++) {if(fabs(rez[i]-x[i])>1E-5) printf("%lf=%lf\n",rez[i],x[i]); fflush(stdout);}
		fp=fopen("time-par.dat","a");
		timeprint(t1,t2,numar,dim,fp,thread);
		fclose(fp);
		free(*mat);
		free(mat);
		free(rez);
		free(y);
	}		
	free(x);
	MPI_Finalize();
	return 0;
}
