#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
int gauss_fast_omp(long dim,int thread,double **mat,double *x,double *libre);
int gaussJ_fast_omp(long dim,int thread,double **mat,double *x,double *libre);
/*
    dim c'est la dimension de matrice
    thread c'est le numere de threads
    mat c'est la matrice de system
    libre c'est le termen libre
    x c'est le resoudre
*/