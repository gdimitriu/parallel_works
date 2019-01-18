void thread_crout(int k1,int i1,int nr,double **Lmat,double **Umat,st_join_counter_t *c,int num)
{
int p;
	for(p=nr;p<k1;p=p+num) 
	{
		Lmat[i1][k1]-=Lmat[i1][p]*Umat[p][k1];
		Umat[k1][i1]-=Lmat[k1][p]*Umat[p][i1];
	}
	st_join_counter_finish(c);
	ST_POLLING();
}
//l'algorithm c'est dans "SPICE Simularea si analiza circuitelor electronice" parallelise avec sth
int calculez_crout_normal_sth(int dim,int thread,double **mat,double *x,double *libre)
/*
	dim c'est la dimension de matrice
	thread c'est le numero de thread qui travaile
	mat c'est la matrice de system
	libre c'est le termen libre
	x c'est le vector de resoudre
*/
{
long i,k,j,r;
double **U,**L;
double *pmat;
st_join_counter_t *c;
//FILE *outL,*outU;
	c=(st_join_counter_t *)calloc(thread,sizeof(st_join_counter_t));
	U=(double **)calloc(dim,sizeof(double *));
	pmat=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		U[i]=pmat;
		pmat+=dim;
	}
	L=(double **)calloc(dim,sizeof(double *));
	pmat=(double *)calloc(dim*dim,sizeof(double));
	for(i=0;i<dim;i++)
	{
		L[i]=pmat;
		pmat+=dim;
	}
	
//	outL=(FILE *)fopen("outL_1","w");
//	outU=(FILE *)fopen("outU_1","w");
	//le crout start
	//fait la decomposition
	for(k=0;k<dim;k++)
	{
		for(i=k;i<dim;i++)
		{
			L[i][k]=mat[i][k];
			U[k][i]=mat[k][i];
			st_join_counter_init(c,thread);
			for(r=0;r<thread;r++)
			ST_THREAD_CREATE(thread_crout(k,i,r,L,U,c,thread));
			st_join_counter_wait(c);
			if(L[k][k]==0.0)
			{
				printf("Impartire prin zero %d\n",k);
				fflush(stdout);
			}
			U[k][i]=U[k][i]/L[k][k];
		}
	}
/*
	for(i=0;i<dim;i++)
	{
		for(j=0;j<dim;j++)
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
  	for(i=0;i<dim;i++)
   {
   	x[i]=libre[i];
   	for(j=0;j<i;j++)  x[i]-=L[i][j]*x[j];
   	x[i]=x[i]/L[i][i];
   }
   for(i=dim-1;i>=0;i--)
   {
   	for(j=dim-1;j>=i+1;j--) x[i]-=U[i][j]*x[j];
   	x[i]=x[i]/U[i][i];
   }
/*
   for(i=0;i<dim;i++)
   {
   	printf("X[%d]=%f\n",i,x[i]);fflush(stdout);
   }
*/
	free(*U);
	free(*L);
	free(U);
	free(L);
	free(c);
//	fclose(outL);
//	fclose(outU);
	return(0);
}


