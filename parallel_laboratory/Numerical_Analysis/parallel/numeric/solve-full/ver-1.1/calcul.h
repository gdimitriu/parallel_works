void calculez()
{
int i,k,j;
double **mat;
double *pmat;
	mat=(double **)calloc(variabila,sizeof(double *));
	pmat=(double *)calloc(variabila*(variabila+1),sizeof(double));
	for(i=0;i<variabila;i++)
	{
		mat[i]=pmat;
		pmat+=variabila+1;
	}
	for(i=0;i<variabila;i++)
	for(j=0;j<variabila;j++)
		mat[i][j]=tmat[i][j];
	for(i=0;i<variabila;i++)
		mat[i][variabila]=ty[i];
	for(k=0;k<variabila;k++)
	{
		for(j=(k+1);j<variabila;j++)
			mat[k][j]=mat[k][j]/mat[k][k];
		mat[k][variabila]=mat[k][variabila]/mat[k][k];
		mat[k][k]=1;
		for(i=(k+1);i<variabila;i++)
		{
			for(j=(k+1);j<variabila;j++)
				mat[i][j]=mat[i][j]-mat[i][k]*mat[k][j];
			mat[i][variabila]-=mat[i][k]*mat[k][variabila];
			mat[i][k]=0;
		}
	}
	
	for(i=0;i<variabila;i++)
	{
	for(j=0;j<(variabila+1);j++)
	    printf(" %lf ",mat[i][j]);
	printf("\n");
	fflush(stdout);
	} 
	printf("\n");

	for(k=(variabila-1);k>=0;k--)
	{
		tx[k]=mat[k][variabila];
		for(i=(k-1);i>=0;i--)
			mat[i][variabila]-=tx[k]*mat[i][k];
	}
	free(*mat);
	free(mat);
}
