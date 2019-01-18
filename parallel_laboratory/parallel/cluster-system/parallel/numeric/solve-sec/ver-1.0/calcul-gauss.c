void calculez_gauss()
{
int i,k,j;
double **mat;
double *pmat;
//FILE *out;
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
//	out=(FILE *)fopen("outs","w");
	/* begin gauss */
	for(j=1;j<(variabila+1);j++)
		mat[0][j]=mat[0][j]/mat[0][0];
		mat[0][0]=1;
	for(i=1;i<variabila;i++)
	{
		
		for(k=0;k<i;k++)
		{
			for(j=(k+1);j<(variabila+1);j++)
				mat[i][j]=mat[i][j]-mat[i][k]*mat[k][j];
			mat[i][k]=0;
		}
		if(mat[i][i]==0){ printf("Impartire prin zero %d \n",i);fflush(stdout); }
		for(j=(i+1);j<(variabila+1);j++)
			mat[i][j]=mat[i][j]/mat[i][i];
		mat[i][i]=1;
	}
/*	
	for(i=0;i<variabila;i++)
	{
	for(j=0;j<(variabila+1);j++)
	    fprintf(out,"%lf ",mat[i][j]);
	fprintf(out,"\n");
	fflush(out);
	} 
*/
	for(k=(variabila-1);k>=0;k--)
	{
		tx[k]=mat[k][variabila];
		for(i=(k-1);i>=0;i--)
			mat[i][variabila]-=tx[k]*mat[i][k];
	}
	free(*mat);
	free(mat);
//	fclose(out);
}
