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
#include<omp.h>
int omp_gj(double **mat,double *x,double *y,long dim,long threads)
{
	long k,i,j;
	double temp;
	omp_set_num_threads(threads);
	for(k=0;k<dim;k++)
	{
#pragma omp parallel for private(j,temp)
		for(i=0;i<dim;i++)
		{
			if(i!=k)
			{
				temp=mat[i][k]/mat[k][k];
				for(j=k+1;j<dim;j++) mat[i][j]-=temp*mat[k][j];
				y[i]-=temp*y[k];
			}
		}
	}
#pragma omp parallel for
	for(i=0;i<dim;i++) x[i]=y[i]/mat[i][i];
	return 0;
}
