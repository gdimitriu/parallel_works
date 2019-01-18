#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int gauss_siedel(double **mat,double *va,double *xn,long N,double err)
{
	double sum1,sum2,*qi,q,max,count;
	double *xn_1;
	long i,j;
	xn_1=(double *)calloc(N,sizeof(double));
	qi=(double *)calloc(N,sizeof(double));
	for(i=0;i<N;i++) qi[i]=0.0;
	for(i=0;i<N;i++)
	{
		sum1=0.0;
		for(j=i+1;j<N;j++)
		sum1+=fabs(mat[i][j]/mat[i][i]);
		for(j=0;j<i;j++)
			sum1+=fabs(mat[i][j]/mat[i][i])*qi[j];
		qi[i]=sum1;
	}
	q=qi[0];
	for(i=1;i<N;i++) if(q<qi[i]) q=qi[i];
	if(q>=1)
	{
		printf("Sistemul nu poate fi rezolvat cu metodat Gauss-Siedel deoarece q=%lf >=1\n",q);
		fflush(stdout);
		return -1;
	}
	for(i=0;i<N;i++) xn_1[i]=0.0;
	count=1.0;
	while(fabs(count)>err)
	{
		for(i=0;i<N;i++) xn_1[i]=xn[i];
		for(i=0;i<N;i++)
		{
			xn[i]=va[i]/mat[i][i];
			for(j=i+1;j<N;j++) xn[i]-=mat[i][j]/mat[i][i]*xn_1[j];
			for(j=0;j<i;j++) xn[i]-=mat[i][j]/mat[i][i]*xn[j];
		}
		max=fabs(xn[0]-xn_1[0]);
		for(i=1;i<N;i++)
			if(max<fabs(xn[i]-xn_1[i])) max=fabs(xn[i]-xn_1[i]);
		count=q*max/(1-q);
	}
	free(xn_1);
	free(qi);
	return 0;
}
