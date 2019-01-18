/*
	header pentru functiile cu fisiere
*/
#include<math.h>
/* functia de citire a sistemului */
void read_data()
{
int j,k;
char nume[128];
double temp;
	for(j=0;j<128;j++) nume[j]=0;
	retray:	write(1,"Indroduceti numele fisierului ",30*sizeof(char));
	gets(nume);
	if((gzconfig=(gzFile *)gzopen(nume,"r"))==(gzFile *)NULL)
	{
		perror("error open file");
		j++;
		if(j<6) goto retray;
	}
	gzrewind(gzconfig);
	variable=0;
	while(!gzeof(gzconfig))
	{
		gzread(gzconfig,&temp,sizeof(double));
		variable++;
	}
	variable=(-1+sqrt(1+4*variable))/2;
	gzrewind(gzconfig);
	/* aloc memorie */
	ty=(double *)calloc(variable,sizeof(double));
	tmat=(double **)calloc(variable,sizeof(double *));
	ptmat=(double *)calloc(variable*variable,sizeof(double));
	for(j=0;j<variable;j++)
	{
		tmat[j]=ptmat;
		ptmat+=variable;
	}
	/* citesc datele si le distribui in structurile masinilor */
	for(k=0;k<variable;k++)
	{
		/* citesc matricea A */
		for(j=0;j<variable;j++)
			gzread(gzconfig,&tmat[k][j],sizeof(double));
		/* citesc b */
		gzread(gzconfig,&ty[k],sizeof(double));
	}
	gzclose(gzconfig);
}
