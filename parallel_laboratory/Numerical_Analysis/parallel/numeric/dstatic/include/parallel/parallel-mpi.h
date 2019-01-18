/*
	File for definitions of function who use MPI
*/
#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
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

void diffusion_sq_check_mpi(MPI_Comm mesh2_comm,int size,int rank,double **matin,double **matout,int partition);
/*
	size c'est le numero de processeurs
	rank c'est le rank de processeur dans le MPI_COMM_WORLD
	matin c'est la matrice complete
	matout c'est la matrice mesh
	partition c'est la doone qui a chacun processeur
*/
int mult_sq_mat_check_mpi(int dim,double **a,double **b,double **c);
int mult_sq_mat_check_mpi_cannon(int dim,double **a,double **b,double **c,MPI_Comm mesh2_comm,int q,int s);
/*
    dim c'est la dimension de vector
    a c'est la premier matrice
    b c'est la deuxieme matrice
    c c'est la resoudre matrice
    mesh2_comm c'est le comunnicator
    q c'est la dimension de mesh
    s c'est le numero de done qui a chacun processeurs
*/
int diffusion_gauss_striped_ciclique_mpi(int rank,int size,double **mat,double *ty,double **augumente,int *ciclique,int variable);
/*
	rank c'est le rank de processor
	size c'est le numero des processerus
	mat c'est la matrice complete dans le processe principale
	ty c'est le vector libre dans le process principale
	augumente c'est la matrice partitione dans le client
	ciclique c'est le vector qui retien le numero de la ligne qui sont dans cette matrice
	variable c'est le numero de variable dans le system
*/
void gauss_striped_ciclique_mpi(int rank,int size,int partition,int *ciclique,int derniere,double **augumente,double *resoudre,int variable);
/*
	rank c'est le numero de processeur
	size c'est le numero des processeurs
	partition c'est le numero de ligne dans cette machine
	ciclique c'est le vector avec les numeros des lignes
	derniere c'est le flag qui sais si cette machine est la derniere machine de calcul
	augumente c'est la matrice augumente
	resoudre c'est le vector de resoudre
*/

void diffusion_sq_striped_ciclique_mpi(int rank,int size,double **mat,double **augumente,int *ciclique,int variable,int type);
/*
	rank c'est le rank de processor
	size c'est le numero des processerus
	mat c'est la matrice complete dans le processe principale
	augumente c'est la matrice partitione dans le client
	ciclique c'est le vector qui retien le numero de la ligne qui sont dans cette matrice
	variable c'est le numero de variable dans le system
	type c'est si la matice c'est a(0) ou b(1) (normale ou ranverse)
*/
void combine_sq_striped_ciclique_mpi(int rank,int size,int variable,int *ciclique,double **matrix,double **tmatrix);
/*
	rank c'est le rank de processor
	size c'est le numero des processerus
	tmatrix c'est la matrice complete dans le processe principale
	matrix c'est la matrice partitione dans le client
	ciclique c'est le vector qui retien le numero de la ligne qui sont dans cette matrice
	variable c'est le numero de variable dans le system
*/
void mult_sq_mat_striped_ciclique_mpi(int rank,int size,int partition,double **mata,double **matb,double **matc,int variable);
/*
  rank c'est le numero de processeur
  size c'est le numero des processeurs
  partition c'est le numero de ligne dans cette machine
  ciclique c'est le vector avec les numeros des lignes
  mata c'est la premiere matrice
  matb c'est la deuxieme matrice
  matc c'est la troisieme matrice
  variable c'est le numeros de variable qui sont dans la matrice
*/
