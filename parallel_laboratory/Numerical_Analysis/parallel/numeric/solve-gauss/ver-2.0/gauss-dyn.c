/* Programul CLIENT pentru gauss*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<zlib.h>
#include<mpi.h>
#include"definitii.h"
#include"fisier.h"
#include"calcul1.h"
#include"acceleration.h"
#include<parallel/parallel-mpi.h>
#include<parallel/parallel-mpi_dyn.h>

int main(int argc,char **argv)
{
int j,i,k;
struct timeval t1s,t2s,t1p,t2p;
FILE *sg,*pgrc;
int rank,size;
int temp;
	sg=(FILE *)fopen("sg.dat","w");
	pgrc=(FILE *)fopen("pgrc.dat","w");
	read_data();
/*	for(i=0;i<variable;i++)
	{
		for(j=0;j<variable;j++)
		{
			fprintf(sg,"%e ",tmat[i][j]);
			fflush(stdout);
		}
		fprintf(sg,"%e \n",ty[i]);
		fflush(stdout);
	}	*/
	gettimeofday(&t1s,NULL);
	calculez1();
	gettimeofday(&t2s,NULL);
	for(j=0;j<variable;j++) 
	{
		fprintf(sg,"X%d=%f\n",j,tx[j]);
		fflush(sg);
	}
	fclose(sg);
	for(j=0;j<variable;j++) tx[j]=0.0;
	gettimeofday(&t1p,NULL);
	gauss_mpi(4,variable,tmat,ty,tx,1,argc,argv);
	gettimeofday(&t2p,NULL);
	acceleration(t1s,t2s,t1p,t2p,4);
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
