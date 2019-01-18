/*
	File for definitions of function who use MPI
*/
#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
/*
	function of finding the extreme
*/
int extreme_mpi_pthread(int dim,double *vector,double *output,int *position,int type);
/*
    dim c'est la dimension de vector
    vector c'est le name de date
    output c'est le extreme
    position c'est la position d'extreme
    il genere l'extreme et le resulte c'est dans le processeur nr 0.
*/
int mult_sq_mat_check_mpi_pthread(int dim,double **a,double **b,double **c);
int mult_sq_mat_check_mpi_cannon_pthread(int dim,double **a,double **b,double **c,MPI_Comm mesh2_comm,int q,int s);
/*
    dim c'est la dimension de vector
    a c'est la premier matrice
    b c'est la deuxieme matrice
    c c'est la resoudre matrice
*/
