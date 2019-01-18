#include<stdio.h>
#include<serial.h>
int gauss(double **mat,double *x,double *y,long variable)
{
long i,k,j;
	/* begin gauss */
	for(j=1;j<(variable);j++)
		mat[0][j]=mat[0][j]/mat[0][0];
	y[0]=y[0]/mat[0][0];
	mat[0][0]=1;
	for(i=1;i<variable;i++)
	{
		for(k=0;k<i;k++)
		{
			for(j=(k+1);j<variable;j++)
				mat[i][j]=mat[i][j]-mat[i][k]*mat[k][j];
			y[i]=y[i]-mat[i][k]*y[k];
			mat[i][k]=0;
		}
		if(mat[i][i]==0){ printf("Impartire prin zero %ld \n",i);fflush(stdout); return(-1);}
		for(j=(i+1);j<variable;j++)
			mat[i][j]=mat[i][j]/mat[i][i];
		y[i]=y[i]/mat[i][i];
		mat[i][i]=1;
	}
	for(k=(variable-1);k>=0;k--)
	{
		x[k]=y[k];
		for(i=(k-1);i>=0;i--)
			y[i]-=x[k]*mat[i][k];
	}

	return(0);
}
