/* Program for testing sorting and extrem*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include"definitii.h"
#include"fisier.h"
#include"calcul.h"
#include"acceleration.h"
#include<parallel/parallel-pthread.h>
#define P 1


int main(int argc,char **argv)
{
double output;
int position,i;
struct timeval t1s,t2s,t1p,t2p;
double temp;
		for(i=0;i<128;i++) nume[i]=0;
		write(1,"Introduceti numele fisierului:",30*sizeof(char));
		gets(nume);
		read_data(nume);
		gettimeofday(&t1s,NULL);
		calculez(P,variabila,&output,&position,1);
		gettimeofday(&t2s,NULL);
		printf("Serial %e at %d\n",output,position);
		output=0.0;
		position=0;
		gettimeofday(&t1p,NULL);
		extreme(variabila,P,vector,1,&output,&position);
		gettimeofday(&t2p,NULL);
		acceleration(t1s,t2s,t1p,t2p,P);
		printf("Parallel %e at %d\n",output,position);
	free(vector);
	return 0;
}
