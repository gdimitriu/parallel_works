/*
	JACOBI SERIAL with diagonal COLLUMN dominant
*/
#include<string.h>
#include<math.h>

void jacobi_col(double **mat,double *ty,double *tx,int dim,double err)
{
	double *xn_1;
	double *yn,*yn_1;
	double max,sum,q;
	long i,j;
	double count;
	xn_1=(double *)calloc(dim,sizeof(double));
	yn=(double *)calloc(dim,sizeof(double));
	yn_1=(double *)calloc(dim,sizeof(double));
	//compute q
	q=0.0;
	for(i=1;i<dim;i++) q+=fabs(mat[i][0]/mat[i][i]);
	for(i=1;i<dim;i++)
	{
		sum=0.0;
		for(j=0;j<dim;j++) if(i!=j) sum+=fabs(mat[j][i]/mat[j][j]);
		if(q<sum) q=sum;
	}
	max=fabs(mat[0][0]);
	for(i=1;i<dim;i++) if(max>fabs(mat[i][i])) max=fabs(mat[i][i]);
	count=q/(max*(1-q));
	memcpy(yn,ty,dim*sizeof(double));
	sum=0.0;
	for(i=0;i<dim;i++) sum+=fabs(yn[i]);
	count=count*sum;
	while(fabs(count)>err)
	{
		memcpy(yn_1,yn,dim*sizeof(double));
		for(i=0;i<dim;i++)
		{
			yn[i]=ty[i];
			for(j=0;j<dim;j++) if(i!=j) yn[i]-=mat[i][j]/mat[j][j]*yn_1[j];
			tx[i]=yn[i]/mat[i][i];
		}
		sum=0.0;
		for(i=0;i<dim;i++) sum+=fabs(yn[i]-yn_1[i]);
		count=q*sum/(max*(1-q));
	}
	free(xn_1);
	free(yn);
	free(yn_1);
}
