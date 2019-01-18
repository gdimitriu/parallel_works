/* 
	la function pour faire la multiplication dans le reseau ciclique avec 
	distribution striped dans MPI clasique
*/
#include<parallel/parallel-mpi.h>
void mult_sq_mat_striped_ciclique_mpi(int rank,int size,int partition,double **mata,double **matb,double **matc,int variable)
{
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
int i,j,k,l,m;	//count
	for(i=0;i<partition;i++)
	{
			for(j=0;j<variable;j++)
			{
				matc[i][j]=0.0;
				for(k=0;k<variable;k++)
					matc[i][j]+=mata[i][k]*matb[k][j];
			}
	}
}
