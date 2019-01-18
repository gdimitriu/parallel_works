/* Programul pentru benchmark*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<zlib.h>
#include"definitii.h"
#include"fisier-b.c"
//#include<st.h>
#include<omp.h>
#include<pthread.h>
#include"speedup-b.c"
//#include"calcul-crout.c"
//#include"crout-omp.c"
//#include"crout-sth.c"
#include"calcul-gauss.c"
//#include<parallel/solve-sth.h>
#include<parallel/parallel-omp.h>
//#include<parallel/parallel-pthread.h>
#define T 10
int P;

//int st_main(int argc,char **argv)
int main(int argc,char **argv)
{
int j,i,k;
struct timeval t1s[T],t2s[T],t1p[T],t2p[T];
int ciclu;
double temp,*tx1;
FILE *perr;
	P=atoi(argv[1]);
	read_data(argv[2]);

for(ciclu=0;ciclu<T;ciclu++)
{
	tx=(double *)calloc(variable,sizeof(double));
	gettimeofday(&t1s[ciclu],NULL);
//	calculez_crout_normal();
	calculez_gauss();	
	gettimeofday(&t2s[ciclu],NULL);
	tx1=(double *)calloc(variable,sizeof(double));
	gettimeofday(&t1p[ciclu],NULL);
//	calculez_crout_normal_omp(variable,2,tmat,tx,ty);
	gauss_fast_omp(variable,2,tmat,tx1,ty);
	gettimeofday(&t2p[ciclu],NULL);
	perr=(FILE *)fopen("eroare.dat","a");
	for(i=0;i<variable;i++)
	{
		temp=tx[i]-tx1[i];
		if((temp<=-1e-10)||(temp>1e-10))
		{
			fprintf(perr,"eroare la ec %d la iter %d la thread-ul %d din blocul %d tx=%f fata de %f\n",i,ciclu,P,variable,tx[i],tx1[i]);
			fflush(perr);
		}
	}
	fclose(perr);
	free(tx);
	free(tx1);
	printf("passe %d\n",ciclu);fflush(stdout);
}
	speedup(t1s,t2s,t1p,t2p,P,T,variable);
	free(*tmat);
	free(tmat);
	free(ty);
	return 0;
}
