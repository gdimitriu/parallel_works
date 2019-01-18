#include<stdio.h>
#include<stdlib.h>
int gauss_serial(double **mat,double *x,long dim);
int gaussJ_serial(double **mat,double *x,int dim);
/*
    dim c'est la dimension de matrice
    thread c'est le numere de threads
    mat c'est la matrice de system
    libre c'est le termen libre
    x c'est le resoudre
*/