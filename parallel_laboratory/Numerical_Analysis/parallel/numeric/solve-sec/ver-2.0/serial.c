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
//#include<mpi.h>

//int main(int argc,char **argv)
int st_main(int argc,char **argv)
{
int j,i,k;
struct timeval t1s,t2s;
FILE *sg;
int rank,size;
int nr,partition;
double **augumente,*paugumente,*px;
int *ciclique;
int derniere;
int temp;
//	MPI_Init(&argc,&argv);
//	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
//	MPI_Comm_size(MPI_COMM_WORLD,&size); 
//if(rank==0)
//{
	sg=(FILE *)fopen("sg.dat","w");
	read_data();
	tx=(double *)calloc(variable,sizeof(double));
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
	timecalc(t1s,t2s);
//}
//	MPI_Finalize();
	return 0;
}
