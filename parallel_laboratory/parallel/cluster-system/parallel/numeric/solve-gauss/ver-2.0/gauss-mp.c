/* Programul CLIENT pentru gauss*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<zlib.h>
#include"definitii.h"
#include"fisier.h"
#include"calcul1.h"
#include"acceleration.h"
#include<parallel/solve-sth.h>
//#include<parallel/parallel-pthread.h>
//#include<parallel/parallel-omp.h>
#define P 10


//int main(int argc,char **argv)
int st_main()
{
int j,i,k;
struct timeval t1s,t2s,t1p,t2p;
FILE *sg,*pgrc;
int rank,size;
int nr,partition;
double **augumente,*paugumente,*px;
int *ciclique;
int derniere;
int temp;
	sg=(FILE *)fopen("sg.dat","w");
	pgrc=(FILE *)fopen("pgrc.dat","w");
	read_data();
	gettimeofday(&t1s,NULL);
	calculez1();
	gettimeofday(&t2s,NULL);
	for(j=0;j<variable;j++) 
	{
		fprintf(sg,"X%d=%f\n",j,tx[j]);
		fflush(sg);
	}
	fclose(sg);
	free(tx);
	gettimeofday(&t1p,NULL);
	tx=(double *)calloc(variable,sizeof(double));
	gauss_striped_fast(variable,P,tmat,tx,ty);
//	gauss_fast_omp(variable,P,tmat,tx,ty);
/*	for(i=0;i<variable;i++)
	{
		for(j=0;j<variable;j++)
		{
			fprintf(pgrc,"%e ",tmat[i][j]);
			fflush(stdout);
		}
		fprintf(pgrc,"%e \n",ty[i]);fflush(stdout);
	} */
	gettimeofday(&t2p,NULL);
	acceleration(t1s,t2s,t1p,t2p,P);
	for(j=0;j<variable;j++)
	{
		fprintf(pgrc, "X%d=%f\n",j,tx[j]);
		fflush(pgrc);
	}
	fclose(pgrc);
	free(*tmat);
	free(tmat);
	free(ty);
	free(tx);
	return 0;
}
