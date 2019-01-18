/* Programul pentru benchmark*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<zlib.h>
#include<st.h>
//#include<omp.h>
#include<serial.h>
#include<parallel/solve-sth.h>
//#include<parallel/parallel-omp.h>
#include"fisier-b.c"
#include"speedup-b.c"
#define T 10
int P;
double **tmat,*tx,*ty,*tx1;
double *ptmat;

int st_main(int argc,char **argv)
//int main(int argc,char **argv)
{
long variable;
struct timeval t1s[T],t2s[T],t1p[T],t2p[T];
int ciclu;
double temp;
long j,i;
FILE *perr;
//FILE *fser,*fpar;
	P=atoi(argv[1]);	
	variable=get_variable(argv[2]);
for(ciclu=0;ciclu<T;ciclu++)
{
	//allocation de memoire
	tx=(double *)calloc(variable,sizeof(double));
	ty=(double *)calloc(variable,sizeof(double));
	tmat=(double **)calloc(variable,sizeof(double *));
	ptmat=(double *)calloc(variable*variable,sizeof(double));
	for(j=0;j<variable;j++)
	{
		tmat[j]=ptmat;
		ptmat+=variable;
	}
	read_data(argv[2],tmat,ty,variable);
	gettimeofday(&t1s[ciclu],NULL);
	gauss(tmat,tx,ty,variable);
	gettimeofday(&t2s[ciclu],NULL);
	free(*tmat);
	free(tmat);
	free(ty);
	
	//pour execution parallele
	//allocation de memoire
	tx1=(double *)calloc(variable,sizeof(double));
	ty=(double *)calloc(variable,sizeof(double));
	tmat=(double **)calloc(variable,sizeof(double *));
	ptmat=(double *)calloc(variable*variable,sizeof(double));
	for(j=0;j<variable;j++)
	{
		tmat[j]=ptmat;
		ptmat+=variable;
	}
	read_data(argv[2],tmat,ty,variable);
	gettimeofday(&t1p[ciclu],NULL);
	gauss_striped_1(variable,P,tmat,tx1,ty);
	gettimeofday(&t2p[ciclu],NULL);
	free(*tmat);
	free(tmat);
	free(ty);
	perr=(FILE *)fopen("eroare.dat","a");
	for(i=0;i<variable;i++)
	{
		temp=tx[i]-tx1[i];
		if((temp<=-1e-10)||(temp>1e-10))
		{
			fprintf(perr,"eroare la ec %ld la cilcul %d la ec %ld tx=%f fata de %f\n",i,ciclu,variable,tx[i],tx1[i]);
			fflush(perr);
		}
	}	
	fclose(perr);
	//pour teste la veridicite d'implementation
/*
	if(ciclu==0)
	{
		fser=(FILE *)fopen("serial.dat","w");
		fpar=(FILE *)fopen("parallel.dat","w");
		for(i=0;i<variable;i++)
		{
			fprintf(fser,"X%ld=%f\n",i,tx[i]);fflush(fser);
			fprintf(fpar,"X%ld=%f\n",i,tx[i]);fflush(fpar);
		}
		fclose(fser);
		fclose(fpar);
	} */
	free(tx);
	free(tx1);
	printf("passe %d\n",ciclu);fflush(stdout);
}
	speedup(t1s,t2s,t1p,t2p,P,T,variable);
	return 0;
}
