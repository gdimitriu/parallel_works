void calculez1()
{
int i,k,j;
double **mat;
double *pmat;
FILE *pf;
//	pf=(FILE *)fopen("test.dat","w");
	mat=(double **)calloc(variable,sizeof(double *));
	pmat=(double *)calloc(variable*(variable+1),sizeof(double));
	for(i=0;i<variable;i++)
	{
		mat[i]=pmat;
		pmat+=variable+1;
	}
	for(i=0;i<variable;i++)
	for(j=0;j<variable;j++)
		mat[i][j]=tmat[i][j];
	for(i=0;i<variable;i++)
		mat[i][variable]=ty[i];
	/* begin gauss */
	for(j=1;j<(variable+1);j++)
		mat[0][j]=mat[0][j]/mat[0][0];
		mat[0][0]=1;
	for(i=1;i<variable;i++)
	{
		for(k=0;k<i;k++)
		{
			for(j=(k+1);j<(variable+1);j++)
				mat[i][j]=mat[i][j]-mat[i][k]*mat[k][j];
			mat[i][k]=0;
		}
	for(j=(i+1);j<(variable+1);j++)
		mat[i][j]=mat[i][j]/mat[i][i];
		mat[i][i]=1;
	}
	/* teste */
/*	for(i=0;i<variable;i++)
	{
		for(j=0;j<variable+1;j++)
		{
			fprintf(pf,"%e ",mat[i][j]);
			fflush(stdout);
		}
		fprintf(pf,"\n");fflush(stdout);
	}*/
	for(k=(variable-1);k>=0;k--)
	{
		tx[k]=mat[k][variable];
		for(i=(k-1);i>=0;i--)
			mat[i][variable]-=tx[k]*mat[i][k];
	}
	free(*mat);
	free(mat);
//	fclose(pf);
}
