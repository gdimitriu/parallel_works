/* Programul CLIENT pentru gauss*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include"definitii.h"
#include"fisier.c"
#include"calcul-gauss.c"
#include"calcul-doolittle.c"
#include"calcul-crout.c"
#include"calcul-crout-1.c"
#include"acceleration.c"
#include"verificare.c"
#include<parallel/parallel-pthread.h>
#define P 1


int main()
{
int j;
struct timeval t1s,t2s,t1p,t2p;
FILE *sg,*pgrc;
	sg=(FILE *)fopen("sg.dat","w");
	pgrc=(FILE *)fopen("pgrc.dat","w");
	read_data(tmat,tx,ty);
	gettimeofday(&t1s,NULL);
	calculez_gauss();
	gettimeofday(&t2s,NULL);
	for(j=0;j<variabila;j++) fprintf(sg,"X%d=%g\n",j,tx[j]);
	fflush(sg);
//	verificare();
	for(j=0;j<variabila;j++) tx[j]=0;
	gettimeofday(&t1p,NULL);
	calculez_crout_normal();
	gettimeofday(&t2p,NULL);
	acceleration(t1s,t2s,t1p,t2p,P);
	for(j=0;j<variabila;j++) fprintf(pgrc,"X%d=%g\n",j,tx[j]);
	fflush(pgrc);
//	verificare();
	fclose(sg);
	fclose(pgrc);
	free(*tmat);
	free(tmat);
	free(tx);
	free(ty);
	return 0;
}
