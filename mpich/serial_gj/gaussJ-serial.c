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
int gaussJ_serial(double **mat,double *x,double *y,long dim)
{
	long k,i,j;
	double temp;
	
	for(k=0;k<dim;k++)
	{
		for(i=k+1;i<dim;i++)
		{
			temp=mat[i][k]/mat[k][k];
			for(j=k+1;j<dim;j++) mat[i][j]-=temp*mat[k][j];
			y[i]-=temp*y[k];
		}
		for(i=0;i<k;i++)
		{
			temp=mat[i][k]/mat[k][k];
			for(j=k+1;j<dim;j++) mat[i][j]-=temp*mat[k][j];
			y[i]-=temp*y[k];
		}
	}
	for(i=0;i<dim;i++) x[i]=y[i]/mat[i][i];
	return 0;
}
