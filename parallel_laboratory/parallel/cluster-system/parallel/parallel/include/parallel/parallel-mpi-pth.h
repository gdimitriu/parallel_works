/*
	File for definitions of function who use MPI
*/
#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include<pth.h>
/*
	function of finding the extreme
*/
int extreme_mpi(int dim,double *vector,double *output,int *position,int type);
/*
    dim c'est la dimension de vector
    vector c'est le name de date
    output c'est le extreme
    position c'est la position d'extreme
    il genere l'extreme et le resulte c'est dans le processeur nr 0.
*/
int extreme_link_mpi(int dim,double *vector,double *output,int *position,int type);
/*
    dim c'est la dimension de vector
    vector c'est le name de date
    output c'est le extreme
    position c'est la position d'extreme
    il genere l'extreme et le resulte c'est dans touts les processeurs.
*/

int extreme_link1_mpi(int dim,double *vector,double *output,int *position,int type);
/*
    dim c'est la dimension de vector
    vector c'est le name de date
    output c'est le extreme
    position c'est la position d'extreme
    il genere l'extreme et le resulte c'est dans touts les processeurs.
*/

int mult_sq_mat_check_mpi(int dim,double **a,double **b,double **c);
int mult_sq_mat_check_mpi_pth(int dim,double **a,double **b,double **c);
/*
    dim c'est la dimension de vector
    a c'est la premier matrice
    b c'est la deuxieme matrice
    c c'est la resoudre matrice
*/
/*
	definitions for MPI
*/
int rank,size,i;
MPI_Status status;