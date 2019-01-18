/* Programul CLIENT pentru rezolv-omp*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<zlib.h>
#include"definitii.h"
#include"fisier.c"
#include<st.h>
#include"acceleration.c"
//#include"calcul-crout.c"
//#include"crout-sth.c"
#include"calcul-gauss.c"
#include<parallel/solve-sth.h>
#define P 20


int st_main()
{
int j,i,k;
struct timeval t1s,t2s,t1p,t2p;
FILE *sg,*pgrc;
	sg=(FILE *)fopen("sg.dat","w");
	pgrc=(FILE *)fopen("pgrc.dat","w");
	read_data();
	tx=(double *)calloc(variable,sizeof(double));
	gettimeofday(&t1s,NULL);
//	calculez_crout_normal();
	calculez_gauss();
	gettimeofday(&t2s,NULL);
	for(j=0;j<variable;j++) 
	{
		fprintf(sg,"X%d=%f\n",j,tx[j]);
		fflush(sg);
	}
	fclose(sg);
	free(tx);
	tx=(double *)calloc(variable,sizeof(double));
	gettimeofday(&t1p,NULL);
//	calculez_crout_normal_sth(variable,2,tmat,tx,ty);
	gauss_striped_fast_lock(variable,2,tmat,tx,ty);
	gettimeofday(&t2p,NULL);
	for(i=0;i<variable;i++)
	{
		for(j=0;j<variable;j++)
		{
			fprintf(pgrc,"%e ",tmat[i][j]);
			fflush(stdout);
		}
		fprintf(pgrc,"%e \n",ty[i]);fflush(stdout);
	} 
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
