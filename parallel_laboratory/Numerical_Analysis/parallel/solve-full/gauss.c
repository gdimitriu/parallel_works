/* Programul CLIENT pentru gauss*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include"definitii.h"
#include"fisier.h"
#include"calcul1.h"
#include"acceleration.h"
#include<parallel.h>
#define P 20


int main()
{
int j;
struct timeval t1s,t2s,t1p,t2p;
FILE *sg,*pgrc;
	sg=(FILE *)fopen("sg.dat","w");
	pgrc=(FILE *)fopen("pgrc.dat","w");
	read_data(tmat,tx,ty);
	gettimeofday(&t1s,NULL);
	calculez1();
	gettimeofday(&t2s,NULL);
	for(j=0;j<variabila;j++) fprintf(sg,"X%d=%f\n",j,tx[j]);
	fflush(sg);
	gettimeofday(&t1p,NULL);
	for(j=0;j<variabila;j++) tx[j]=0;
	gauss_striped_fast(variabila,P,tmat,tx,ty,0);
	gettimeofday(&t2p,NULL);
	acceleration(t1s,t2s,t1p,t2p,P);
	for(j=0;j<variabila;j++) fprintf(pgrc, "X%d=%f\n",j,tx[j]);
	fflush(pgrc);
	fclose(sg);
	fclose(pgrc);
	free(*tmat);
	free(tmat);
	free(tx);
	free(ty);
	return 0;
}
