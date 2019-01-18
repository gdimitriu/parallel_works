/*
	header pentru functiile cu fisiere
*/
#include<zlib.h>
/* functia de citire a vectorului */
int read_data(char *nume)
{
double temp;
int k;
gzFile *fconfig;
	if((fconfig=(gzFile *)gzopen(nume,"r"))==(gzFile *)NULL)
	{
		perror("error open file");
		return(-1);
	}
	variabila=0;
	gzrewind(fconfig);
	while(!gzeof(fconfig))
	{
		gzread(fconfig,&temp,sizeof(double));
		variabila++;
	}
	variabila--;
	gzrewind(fconfig);
	/* aloc memorie */
	vector=(double *)calloc(variabila,sizeof(double));
	
	/* citesc datele si le distribui in structurile masinilor */
	for(k=0;k<variabila;k++)
		gzread(fconfig,&vector[k],sizeof(double));
	gzclose(fconfig);
	return(0);
}
