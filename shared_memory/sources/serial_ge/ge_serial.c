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
int gauss_serial(double **mat,double *y,double *x,long dim)
{
	int k,i,j;
	double temp;

	for(k=0;k<dim-1;k++)
	for(i=k+1;i<dim;i++)
	{
		temp=mat[i][k]/mat[k][k];
		for(j=k+1;j<dim;j++) mat[i][j]-=temp*mat[k][j];
		y[i]-=temp*y[k];
	}
	
	for(i=dim-1;i>=0;i--)
	{
		temp=0.0;
		x[i]=0.0;
		for(j=i+1;j<dim;j++) temp+=mat[i][j]*x[j];
		x[i]=(y[i]-temp)/mat[i][i];
	} 
	
	return 0;
}
