/* Programul pentru benchmark*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/unistd.h>
#include<zlib.h>
#include"CP_01_0.c"
#include"CP_01_1.c"
#include"CP_01_2.c"
int P;
double **matA,**matB,**matC1,**matC2;
double *ptmat;

int main(int argc,char **argv)
{
long variable;
struct timeval t1s,t2s,t1p,t2p;
int ciclu;
double temp;
long j,i,k;
FILE *perr;
int shmid;
	P=atoi(argv[1]);	
	variable=get_variable(argv[2]);
	matA=(double **)calloc(variable,sizeof(double *));
	ptmat=(double *)calloc(variable*variable,sizeof(double));
	for(j=0;j<variable;j++)
	{
		matA[j]=ptmat;
		ptmat+=variable;
	}
	matB=(double **)calloc(variable,sizeof(double *));
	ptmat=(double *)calloc(variable*variable,sizeof(double));
	for(j=0;j<variable;j++)
	{
		matB[j]=ptmat;
		ptmat+=variable;
	}
	read_data(argv[2],argv[3],matA,matB,variable);
	//allocation de memoire
	gettimeofday(&t1s,NULL);
	matC1=(double **)calloc(variable,sizeof(double *));
	ptmat=(double *)calloc(variable*variable,sizeof(double));
	for(j=0;j<variable;j++)
	{
		matC1[j]=ptmat;
		ptmat+=variable;
	}
	for(i=0;i<variable;i++)
		for(j=0;j<variable;j++)
			for(k=0;k<variable;k++)
				matC1[i][j]+=matA[i][k]*matB[k][j];
	gettimeofday(&t2s,NULL);
	printf("passe ser \n");fflush(stdout);
	//pour execution parallele
	//allocation de memoire
	gettimeofday(&t1p,NULL);	
	/* aici se introduce apelul fucntiei pentru calcul paralel */
	matC2=(double **)calloc(variable,sizeof(double *));
	shmid=mult_sq_mat_striped_ciclique_process(variable,P,matA,matB,matC2,1);
	gettimeofday(&t2p,NULL);
	perr=(FILE *)fopen("eroare.dat","a");
	for(i=0;i<variable;i++)
	for(j=0;j<variable;j++)
	{
		temp=matC1[i][j]-matC2[i][j];
		if((temp<=-1e-10)||(temp>1e-10))
		{
			fprintf(perr,"thread %d eroare la matser[%ld][%ld]=%f fata de matpar[%ld][%ld]=%f cu dim %ld\n",P,i,j,matC1[i][j],i,j,matC2[i][j],variable);
			fflush(perr);
		}
	}	
	fclose(perr);
	//pour teste la veridicite d'implementation
	free(*matC1);
	free(matC1);
	shmctl(shmid,IPC_RMID,0);
	printf("passe par\n");fflush(stdout);
	speedup(t1s,t2s,t1p,t2p,P,variable);
	return 0;
}
