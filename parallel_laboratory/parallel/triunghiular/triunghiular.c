/* Programul CLIENT pentru gauss*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include"definitii.h"
#include"fisier.h"
#include"calcul.h"
#include<parallel.h>
#define P 3


int main()
{
int j;
struct timeval t1,t2;
float TS,TP;
FILE *sg,*pgrc;
	sg=(FILE *)fopen("sg.dat","w");
	pgrc=(FILE *)fopen("pgrc.dat","w");
	read_data(tmat,tx,ty);
	gettimeofday(&t1,NULL);
	calculez();
	gettimeofday(&t2,NULL);
	TS=t2.tv_usec-t1.tv_usec;
	printf("Seqencial time %f\n",TS);
	for(j=0;j<variabila;j++) fprintf(sg,"X%d=%f\n",j,tx[j]);
	fflush(sg);
	gettimeofday(&t1,NULL);
	for(j=0;j<variabila;j++) tx[j]=0;
	triunghiular_UP_striped(variabila,P,tmat,tx,ty,0);
	gettimeofday(&t2,NULL);
	TP=t2.tv_usec-t1.tv_usec;
	printf("Parallel time rowwise ciclic %f\n",TP);
	printf("Acceleration is %f rowwise ciclic with %d thread\n",TS/TP,P);
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
