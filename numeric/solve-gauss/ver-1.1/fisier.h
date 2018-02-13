/*
	header pentru functiile cu fisiere
*/
/* functia de citire a sistemului */
void read_data()
{
int j,k;
char nume[128];
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
	gzread(gzconfig,&variabila,sizeof(int));
	/* aloc memorie */
	ty=(double *)calloc(variabila,sizeof(double));
	tx=(double *)calloc(variabila,sizeof(double));
	tmat=(double **)calloc(variabila,sizeof(double *));
	ptmat=(double *)calloc(variabila*variabila,sizeof(double));
	for(j=0;j<variabila;j++)
	{
		tmat[j]=ptmat;
		ptmat+=variabila;
	}
	/* citesc datele si le distribui in structurile masinilor */
	for(k=0;k<variabila;k++)
	{
		/* citesc matricea A */
		for(j=0;j<variabila;j++)
			gzread(gzconfig,&tmat[k][j],sizeof(double));
		/* citesc b */
		gzread(gzconfig,&ty[k],sizeof(double));
	}
	gzclose(gzconfig);
}
