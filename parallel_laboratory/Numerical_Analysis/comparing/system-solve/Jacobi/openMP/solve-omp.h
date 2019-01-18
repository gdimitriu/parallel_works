#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
int jacobi_serial(double **mat,double *x,double *y,long dim,double eroare);
int jacobi_fast_omp(long dim,int thread,double **mat,double *x,double *y,double eroare);
/*
    dim in dimension of matrix
    thread is the number of threads
    mat is the adjounct matrix
    y is the free termen
    x is the solve
*/