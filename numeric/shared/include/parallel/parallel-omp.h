#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
void mult_sq_mat_striped_omp(int mat,int thread,double **a,double **b,double **c,int type);
/*
   mat c'est la dimension de matrice
   thread c'est le numero de threads
   a et b sount la matrice de entre
   c c'est la matrice resoudre
   type c;est 1 pour ligne et 0 pour le colonnes
*/
void mult_sq_mat_striped_omp_1(int mat,int thread,double **a,double **b,double **c,int type);
/*
   mat c'est la dimension de matrice
   thread c'est le numero de threads
   a et b sount la matrice de entre
   c c'est la matrice resoudre
   type c;est 1 pour ligne et 0 pour le colonnes
*/
void mult_sq_mat_check_omp(int mat,int thread,double **a,double **b,double **c,int type);
/*
   mat c'est la dimension de matrice
   thread c'est le numero de threads
   a et b sount la matrice de entre
   c c'est la matrice resoudre
   type c;est 1 pour ligne et 0 pour le colonnes
*/
int gauss_fast_omp(int dim,int thread,double **mat,double *x,double *libre);
/*
    dim c'est la dimension de matrice
    thread c'est le numere de threads
    mat c'est la matrice de system
    libre c'est le termen libre
    x c'est le resoudre
*/