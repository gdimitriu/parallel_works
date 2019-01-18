/*
	header pentru functiile cu fisiere
*/
/* functia de citire a vectorului */
int read_data(char *nume)
{
double temp;
int k;
FILE *fconfig;
	if((fconfig=(FILE *)fopen(nume,"r"))==(FILE *)NULL)
	{
		perror("error open file");
		return(-1);
	}
	variabila=0;
	rewind(fconfig);
	while(!feof(fconfig))
	{
		fscanf(fconfig,"%lf",&temp);
		variabila++;
	}
	variabila--;
	rewind(fconfig);
	/* aloc memorie */
	vector=(double *)calloc(variabila,sizeof(double));
	
	/* citesc datele si le distribui in structurile masinilor */
	for(k=0;k<variabila;k++)
		fscanf(fconfig,"%lf",&vector[k]);
	fclose(fconfig);
	return(0);
}
