#include<stdlib.h>
#include<omp.h>
void ge_omp(long dim,double **mat,double *x,double *y,long thread)
{
	double *