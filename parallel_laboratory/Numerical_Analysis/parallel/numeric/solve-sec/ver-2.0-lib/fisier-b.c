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
	variable=(-1+sqrt(1+4*variable))/2;
	gzclose(gzconfig);
	return variable;
}
void read_data(char *nume,double **tempMat,double *ty,long variable)
{
long j,k;
gzFile *gzconfig;
	if((gzconfig=(gzFile *)gzopen(nume,"r"))==(gzFile *)NULL)
	{
		perror("error open file");
		exit(-1);
	}
	gzrewind(gzconfig);
	/* citesc datele si le distribui in structurile masinilor */
	for(k=0;k<variable;k++)
	{
		/* citesc matricea A */
		for(j=0;j<variable;j++)
			gzread(gzconfig,&tempMat[k][j],sizeof(double));
		/* citesc b */
		gzread(gzconfig,&ty[k],sizeof(double));
	}
	gzclose(gzconfig);
}
