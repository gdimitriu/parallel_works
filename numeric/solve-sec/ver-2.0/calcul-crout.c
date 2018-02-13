//l'algorithm c'est dans "SPICE Simularea si analiza circuitelor electronice"
int calculez_crout_normal()
{
long i,k,j,p;
double **U,**L;
double *pmat;
//FILE *outL,*outU;
	U=(double **)calloc(variable,sizeof(double *));
	pmat=(double *)calloc(variable*variable,sizeof(double));
	for(i=0;i<variable;i++)
	{
		U[i]=pmat;
		pmat+=variable;
	}
	L=(double **)calloc(variable,sizeof(double *));
	pmat=(double *)calloc(variable*variable,sizeof(double));
	for(i=0;i<variable;i++)
	{
		L[i]=pmat;
		pmat+=variable;
	}
	
//	outL=(FILE *)fopen("outL_1","w");
//	outU=(FILE *)fopen("outU_1","w");
	//le crout start
	//fait la decomposition
	for(k=0;k<variable;k++)
	{
		for(i=k;i<variable;i++)
		{
			L[i][k]=tmat[i][k];
			U[k][i]=tmat[k][i];
			for(p=0;p<k;p++) 
			{
				L[i][k]-=L[i][p]*U[p][k];
				U[k][i]-=L[k][p]*U[p][i];
			}
			if(L[k][k]==0.0)
			{
				printf("Impartire prin zero %d\n",k);
				fflush(stdout);
			}
			U[k][i]=U[k][i]/L[k][k];
		}
	}
/*
	for(i=0;i<variable;i++)
	{
		for(j=0;j<variable;j++)
		{
			fprintf(outL,"%g ",L[i][j]);fflush(outL);
			fprintf(outU,"%g ",U[i][j]);fflush(outU);
		}
		fprintf(outL,"\n");
		fprintf(outU,"\n");
		fflush(outL);
		fflush(outU);
	} 
*/
	//je fait la substituition
  	for(i=0;i<variable;i++)
   {
   	tx[i]=ty[i];
   	for(j=0;j<i;j++)  tx[i]-=L[i][j]*tx[j];
   	tx[i]=tx[i]/L[i][i];
   }
   for(i=variable-1;i>=0;i--)
   {
   	for(j=variable-1;j>=i+1;j--) tx[i]-=U[i][j]*tx[j];
   	tx[i]=tx[i]/U[i][i];
   }
/*
   for(i=0;i<variable;i++)
   {
   	printf("X[%d]=%f\n",i,tx[i]);fflush(stdout);
   }
*/
	free(*U);
	free(*L);
	free(U);
	free(L);
//	fclose(outL);
//	fclose(outU);
	return(0);
}


int calculez_crout_modificat()
{
long i,k,j,p;
double **mat,**U,**L;
double *pmat;
//FILE *outL,*outU;
	mat=(double **)calloc(variable,sizeof(double *));
	pmat=(double *)calloc(variable*variable,sizeof(double));
	for(i=0;i<variable;i++)
	{
		mat[i]=pmat;
		pmat+=variable;
	}
	U=(double **)calloc(variable,sizeof(double *));
	pmat=(double *)calloc(variable*variable,sizeof(double));
	for(i=0;i<variable;i++)
	{
		U[i]=pmat;
		pmat+=variable;
	}
	L=(double **)calloc(variable,sizeof(double *));
	pmat=(double *)calloc(variable*variable,sizeof(double));
	for(i=0;i<variable;i++)
	{
		L[i]=pmat;
		pmat+=variable;
	}
	
	for(i=0;i<variable;i++)
	for(j=0;j<variable;j++)
		mat[i][j]=tmat[i][j];
//	outL=(FILE *)fopen("outL","w");
//	outU=(FILE *)fopen("outU","w");
	//le crout start
	//fait la decomposition
	for(k=0;k<variable;k++)
	{
		for(i=k;i<variable;i++)
		{
			L[i][k]=mat[i][k];
			if(L[k][k]==0.0)
			{
				printf("Imparitre prin zero\n");
				fflush(stdout);
			}
			U[k][i]=mat[k][i]/L[k][k];	
		}
		for(i=k+1;i<variable;i++)
			for(j=k+1;j<variable;j++) mat[i][j]=mat[i][j]-L[i][k]*U[k][j];		
	}
/*	
	for(i=0;i<variable;i++)
	{
		for(j=0;j<variable;j++)
		{
			fprintf(outL,"%g ",L[i][j]);fflush(outL);
			fprintf(outU,"%g ",U[i][j]);fflush(outU);
		}
		fprintf(outL,"\n");
		fprintf(outU,"\n");
		fflush(outL);
		fflush(outU);
	} 
*/
	//je fait la substituition
  	for(i=0;i<variable;i++)
   {
   	tx[i]=ty[i];
   	for(j=0;j<i;j++)  tx[i]-=L[i][j]*tx[j];
   	tx[i]=tx[i]/L[i][i];
   }
   for(i=variable-1;i>=0;i--)
   {
   	for(j=variable-1;j>=i+1;j--) tx[i]-=U[i][j]*tx[j];
   	tx[i]=tx[i]/U[i][i];
   }

/*   for(i=0;i<variable;i++)
   {
   	printf("X[%d]=%f\n",i,tx[i]);fflush(stdout);
   }
*/
	free(*mat);
	free(mat);
	free(*U);
	free(*L);
	free(U);
	free(L);
//	fclose(outL);
//	fclose(outU);
	return(0);
}

