/*
	header pentru functiile cu fisiere
*/
#include<math.h>
/* functia de citire a sistemului */
long get_variable(char *nume)
{
long variable;
double temp;
gzFile *gzconfig;
	if((gzconfig=(gzFile *)gzopen(nume,"r"))==(gzFile *)NULL)
	{
		perror("error open file");
		exit(-1);
	}
	gzrewind(gzconfig);
	variable=0;
	while(!gzeof(gzconfig))
	{
		gzread(gzconfig,&temp,sizeof(double));
		variable++;
	}
	variable=sqrt(variable);
	gzclose(gzconfig);
	return variable;
}
void read_data(char *nume1,char *nume2,double **matA,double **matB,double **matBd,long variable)
{
long j,k;
gzFile *gzconfig1,*gzconfig2;
	if((gzconfig1=(gzFile *)gzopen(nume1,"r"))==(gzFile *)NULL)
	{
		perror("error open file");
		exit(-1);
	}
	if((gzconfig2=(gzFile *)gzopen(nume2,"r"))==(gzFile *)NULL)
	{
		perror("error open file");
		exit(-1);
	}
	gzrewind(gzconfig1);
	gzrewind(gzconfig2);
	/* citesc datele si le distribui in structurile masinilor */
	for(k=0;k<variable;k++)
	{
		/* citesc matricea A si B*/
		for(j=0;j<variable;j++)
		{
			gzread(gzconfig1,&matA[k][j],sizeof(double));
			gzread(gzconfig2,&matB[k][j],sizeof(double));
			matBd[j][k]=matB[k][j];
		}
	}
	gzclose(gzconfig1);
	gzclose(gzconfig2);
}
