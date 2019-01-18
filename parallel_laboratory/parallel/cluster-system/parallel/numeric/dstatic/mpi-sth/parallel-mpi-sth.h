/*
	File for definitions of function who use MPI
*/
#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include<st.h>
int mult_sq_mat_check_mpi_sth(int dim,double **a,double **b,double **c);
/*
    dim c'est la dimension de vector
    a c'est la premier matrice
    b c'est la deuxieme matrice
    c c'est la resoudre matrice
*/
