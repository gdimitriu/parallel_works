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
void gauss_striped_ciclique_mpi_sth(int rank,int size,int partition,int *ciclique,int derniere,double **augumente,double *resoudre,int variable);
/*
	rank c'est le numero de processeur
	size c'est le numero des processeurs
	partition c'est le numero de ligne dans cette machine
	ciclique c'est le vector avec les numeros des lignes
	derniere c'est le flag qui sais si cette machine est la derniere machine de calcul
	augumente c'est la matrice augumente
	resoudre c'est le vector de resoudre
*/
int diffusion_gauss_striped_ciclique_mpi_sth(int rank,int size,double **mat,double *ty,double **augumente,int *ciclique,int variable);
/*
	rank c'est le rank de processor
	size c'est le numero des processerus
	mat c'est la matrice complete dans le processe principale
	ty c'est le vector libre dans le process principale
	augumente c'est la matrice partitione dans le client
	ciclique c'est le vector qui retien le numero de la ligne qui sont dans cette matrice
	variable c'est le numero de variable dans le system
*/
void diffusion_sq_striped_ciclique_mpi_sth(int rank,int size,double **mat,double **augumente,int *ciclique,int variable,int type);
/*
   rank c'est le rank de processor
   size c'est le numero des processerus
   mat c'est la matrice complete dans le process principale
   augumente c'est la matrice partitione dans le client
   ciclique c'est le vector qui retien le numero de la ligne qui sont dans cett
   variable c'est le numero de variable dans le system
   type c'est si la mtrice c'est normale ou ranverse (0 ou 1) pour a ou b
*/
void combine_sq_striped_ciclique_mpi_sth(int rank,int size,int variable,int *ciclique,double **matrix,double **tmatrix);
/*
   rank c'est le rank de la machine
   size c'est le numero de lignes dans la matrice originale
   partition c'est le numero de ligne qui sont dans ce machine
*/
                              
st_join_counter_t *diffusion_sth,*combine_sth;