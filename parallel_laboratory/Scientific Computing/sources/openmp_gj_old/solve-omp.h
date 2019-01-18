#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
int gaussJ_fast_omp(long dim,int thread,double **mat,double *x,double *y);
/*
    dim is dimension of matrix
    thread is the number of threads
    mat is the matrix 
    libre is the free termen
    x is solve
*/