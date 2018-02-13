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
	JACOBI SERIAL with diagonal ROW dominant
*/
#include<string.h>
#include<math.h>
void jacobi(double **mat,double *ty,double *tx,int dim,double err)
{
double *xn_1;
int i,j,k,m;
double q,sum;
	xn_1=(double *)calloc(dim,sizeof(double));
//JACOBI
	for(i=0;i<dim;i++) tx[i]=ty[i]/mat[i][i];
	//compute q
	q=0.0;
	for(i=1;i<dim;i++) q+=fabs(mat[0][i]/mat[0][0]);
	for(i=1;i<dim;i++)
	{
		sum=0.0;
		for(j=0;j<dim;j++) if(i!=j) sum+=fabs(mat[i][j]/mat[i][i]);
		if(q<sum) q=sum;
	}
	sum=fabs(ty[0]/mat[0][0]);
	for(i=1;i<dim;i++) if(sum<fabs(ty[i]/mat[i][i])) sum=fabs(ty[i]/mat[i][i]);
	sum=q*sum/(1-q);
	while(fabs(sum)>err)
	{	
		memcpy(xn_1,tx,dim*sizeof(double));
		for(i=0;i<dim;i++)
		{
			tx[i]=ty[i]/mat[i][i];
			for(j=0;j<dim;j++) if(j!=i) tx[i]-=mat[i][j]/mat[i][i]*xn_1[j];
		}
		sum=fabs(tx[0]-xn_1[0]);
		for(i=0;i<dim;i++) if(sum<fabs(tx[i]-xn_1[i])) sum=fabs(tx[i]-xn_1[i]);
		sum=sum*q/(1-q);
	}
	free(xn_1);
}