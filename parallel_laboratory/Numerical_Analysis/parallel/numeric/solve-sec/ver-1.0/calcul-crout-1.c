//l'algorithm c'est dans la metode numerique de Ioan Rusu
int calculez_crout_alg()
{
long i,k,j;
double **U,**L,*D;
double *pmat;
//FILE *out;
double suma;	// pour la somme iterative
	if((tmat[0][0]==1)||(tmat[0][0]==0))
	{
		printf("Sistem nerezolvabil");
		return(-1);
	}
	U=(double **)calloc(variabila,sizeof(double *));
	pmat=(double *)calloc(variabila*variabila,sizeof(double));
	for(i=0;i<variabila;i++)
	{
		U[i]=pmat;
		pmat+=variabila;
	}
	L=(double **)calloc(variabila,sizeof(double *));
	pmat=(double *)calloc(variabila*variabila,sizeof(double));
	for(i=0;i<variabila;i++)
	{
		L[i]=pmat;
		pmat+=variabila;
	}
//	D=(double *)calloc(variabila,sizeof(double));
	
//	out=(FILE *)fopen("outs","w");
	//le crout start
	//fait la decomposition
//   D[0]=ty[0]/tmat[0][0]; 
   for(i=0;i<variabila;i++)
   for(j=0;j<variabila;j++) 
   {
   	L[i][0]=tmat[i][0];
   	U[0][j]=tmat[0][j]/tmat[0][0];
   }
   
   for(i=0;i<variabila;i++)
   {
   	for(j=0;j<variabila;j++)
   	{
   		if(i==j) U[i][j]=1.0;
   		if(i>=j)
   		{
   			L[i][j]=tmat[i][j];
   			for(k=0;k<j;k++) L[i][j]-=L[i][k]*U[k][j];
/*   			U[i][j]=tmat[i][j];
 	 			for(k=0;k<i;k++) U[i][j]-=L[i][k]*U[k][j];
   			if(L[i][i]==0.0)
   			{
   				printf("Impartire prin zero 1 %d \n",i);
   				fflush(stdout);
   			}
   			U[i][j]=U[i][j]/L[i][i]; */
/*   			D[i]=ty[i];
   			for(k=0;k<i;k++) D[i]-=L[i][k]*ty[k];
   			D[i]=D[i]/L[i][i]; */
   		}
   		if(i<j)
   		{
   			U[i][j]=tmat[i][j];
   			for(k=0;k<i;k++) U[i][j]-=L[i][k]*U[k][j];
   			if(L[i][i]==0.0)
   			{
   				printf("Impartire prin zero 1 %d \n",i);
   				fflush(stdout);
   			}
   			U[i][j]=U[i][j]/L[i][i];
   		}
   	}
   }
   //obtien le resoult
    for(i=0;i<variabila;i++)
    {
      tx[i]=ty[i];
      for(j=0;j<i;j++)  tx[i]-=L[i][j]*tx[j];
      tx[i]=tx[i]/L[i][i];
    }
    for(i=variabila-1;i>=0;i--)
    {
      for(j=variabila-1;j>=i+1;j--) tx[i]-=U[i][j]*tx[j];
	   tx[i]=tx[i]/U[i][i];
    }
/*                                                 
   tx[variabila-1]=D[variabila-1];
   for(i=variabila-2;i>=0;i--)
   {
   	suma=0;
   	for(j=i+1;j<variabila;j++) suma+=tmat[i][j]*tx[j];
   	tx[i]=(D[i]-suma)/U[i][i];
   } */
   for(i=0;i<variabila;i++)
   {
   	printf("X[%d]=%f\n",i,tx[i]);fflush(stdout);
   }
	free(*U);
	free(*L);
	free(U);
	free(L);
//	free(D);
//	fclose(out);
	return(0);
}


