#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int mult_sq_mat_striped_omp(int mat,double **a,double **b,double **c);
/*
   mat c'est la dimension de matrice
   a et b sount la matrice de entre
   c c'est la matrice resoudre
*/
