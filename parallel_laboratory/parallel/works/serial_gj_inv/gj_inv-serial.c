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
