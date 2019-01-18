#include<math.h>
#include<stdio.h>
#include<mpi.h>
	int gauss_mpi(int dimension,int numero,double **matrice,double *libre,double *variable,int type,int argc,char **argv);
	/*
		dimension c'est la dimension de reseau
		numero c'est le numero des equations
		matrice c'est la mtrice des equations
		libre c'est le termen libre des equations
		variable c'est le resoudre des equations
		type c'est le type de dinamique
	*/
	
