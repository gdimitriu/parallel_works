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
int gj_inv_serial(double **mat,double **inv,long dim)
{
	long k,i,j;
	double temp;
	for(i=0;i<dim;i++) inv[i][i]=1.0;
	for(k=0;k<dim;k++)
	{
		for(i=0;i<dim;i++)
		{
			if(k!=i)
			{
				temp=mat[i][k]/mat[k][k];
				for(j=k+1;j<dim;j++) mat[i][j]-=temp*mat[k][j];
				for(j=0;j<dim;j++) inv[i][j]-=temp*inv[k][j];
			}
			else
			{
				for(j=0;j<dim;j++)
				{
					inv[i][j]=inv[i][j]/mat[i][i];
					if(j!=i)
						mat[i][j]=mat[i][j]/mat[i][i];
				}
				mat[i][i]=1.0;
			}
		}
	}
	return 0;
}
