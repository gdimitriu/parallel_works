/*
	Il fait elimination gaussiene sur matrix patratique avec des preocessors en vector
*/
#include<parallel/solve-sth.h>
barrier_t barrier_gauss;
int thread_gauss_striped_1(int P,long N,long who,double **mat,double *y,st_mutex_t *mutex,st_cond_t *cond,int *counters,st_join_counter_t *joinc)
{
	/*
		P c'est le numero de threads
		N c'est la dimension de matrice
		who c'est le numero courent de thread
		mat c'est la matrice
		y c'est le termen libre
		cond et mutex c'est la condition pour le ligne
	*/
	long s;	// le numero de ligne dans la partition
	long nr;	// la moyene de nr de ligne pour chacun partition
	int last=0;	//c'est 0 si le thread a le dernierre ligne
	long i,j,k,l;	//contors
	long proccount,proccount1;
	int replay;
	/* 
		fait les l'initilization pour les partitions
	*/
	nr=N%P;
	s=(N-nr)/P;
	/* pour les common ligne */
	if((who+1)<nr) s++;
	else /* pour le dernierre ligne */
	if((who+1)==nr)
	{
		s++;
		last=1;
	}
	if((nr==0) &&(who+1)==P) last=1;
	printf("%ld %ld\n",who,s);fflush(stdout);
	i=0;
	ST_POLLING();
	// pour le premier processor
	if(who==0)
	{
		// fait la division part
		st_mutex_lock(&mutex[0]);
		for(j=1;j<N;j++) mat[0][j]=mat[0][j]/mat[0][0];
		y[0]=y[0]/mat[0][0];
		mat[0][0]=1;
		counters[0]=1;
		st_cond_broadcast(&cond[0]);
		st_mutex_unlock(&mutex[0]);
		proccount=proccount1=who+P;
	}
	else proccount=proccount1=who;
	ST_POLLING();
	barrier_sync(&barrier_gauss);
	if(who==0) barrier_init(&barrier_gauss,P);
	replay=0;
	for(i=proccount;i<N;i+=P)
	{
		// fait la elimination step
		for(k=replay;k<i;k++)
		{
			ST_POLLING();
			st_mutex_lock(&mutex[k]);
			if(counters[k]==0) st_cond_wait(&cond[k],&mutex[k]);
			if(counters[k]==1)
			{
				for(l=proccount1;l<N;l+=P)
				{
					for(j=(k+1);j<N;j++) mat[l][j]-=mat[l][k]*mat[k][j];
					y[l]-=mat[l][k]*y[k];
					mat[l][k]=0;
				}
			}
			st_mutex_unlock(&mutex[k]);
			ST_POLLING();
		}
		// fait la division
		if(counters[i-1]==1)
		{
			st_mutex_lock(&mutex[i]);
			for(j=i+1;j<N;j++) mat[i][j]=mat[i][j]/mat[i][i];
			y[i]=y[i]/mat[i][i];
			mat[i][i]=1;
			replay=i;
			proccount1=proccount1+P;
			counters[i]=1;
			st_cond_broadcast(&cond[i]);
			st_mutex_unlock(&mutex[i]);
		}		
	}
	barrier_sync(&barrier_gauss);
	// dans ce moment c'est la substitution ranverse
	for(i=who;i<(N-1);i+=P) counters[i]=0;
	if(last==1)
	{
		proccount=proccount1=N-1-P;
		counters[N-1]=1;
	}
	else proccount=proccount1=who+P*(s-1);
	barrier_sync(&barrier_gauss);
	replay=N-1;
	for(i=proccount;i>=who;i-=P)
	{
		// pour toutes les ligne apres courent ligne
		for(k=replay;k>i;k--)
		{			
			if(counters[k]==0)
			{	
				st_mutex_lock(&mutex[k]);
				st_cond_wait(&cond[k],&mutex[k]);
				st_mutex_unlock(&mutex[k]);
			}
			if(counters[k]==1)
			{
				for(l=proccount1;l>=who;l-=P)
					y[l]-=y[k]*mat[l][k];
			}

		}
		if(counters[i+1]==1)
		{
			st_mutex_lock(&mutex[i]);
			counters[i]=1;
			replay=i;
			proccount1-=P;
			st_cond_broadcast(&cond[i]);
			st_mutex_unlock(&mutex[i]);
		}
	}
	st_join_counter_finish(joinc);
	return 0;
}
int gauss_striped_1(long dim,int threads,double **tmat,double *tx,double *ty)
{
	/* 
		dim c'est la dimensione de matrix
		thread c'est le numero de threads
		mat c'est le matrix de donne
		x c'est le resoudre vector
		y c'est el vector libre
	*/
	st_join_counter_t *join_c;
	long i;
//	FILE *fp;
	st_mutex_t *mutex;
	st_cond_t *cond;
	int *counters;
	// faites l'allocation de donne
	if((join_c=(st_join_counter_t *)calloc(threads,sizeof(st_join_counter_t)))==(st_join_counter_t *)NULL)
	{
		perror("Erreur d'allocation pour join counter");
		return(-1);
	}
	if((mutex=(st_mutex_t *)calloc(dim,sizeof(st_mutex_t)))==(st_mutex_t *)NULL)
	{
		perror("Erreur d'allocation pour mutex");
		free(join_c);
		return(-2);
	}
	if((cond=(st_cond_t *)calloc(dim,sizeof(st_cond_t)))==(st_cond_t *)NULL)
	{
		perror("Erreur d'allocation pour condition");
		free(join_c);
		free(mutex);
		return(-2);
	}
	if((counters=(int *)calloc(dim,sizeof(int)))==(int *)NULL)
	{
		perror("Error d'alocation pour le vector des counters");
		free(join_c);
		free(mutex);
		free(cond);
		return(-4);
	}
	st_join_counter_init(join_c,threads);	
	barrier_init(&barrier_gauss,threads);
	for(i=0;i<dim;i++)
	{
		st_mutex_init(&mutex[i]);
		st_cond_init(&cond[i]);
	}
	for(i=0;i<threads;i++)
	{
		ST_THREAD_CREATE(thread_gauss_striped_1(threads,dim,i,tmat,ty,mutex,cond,counters,join_c));
		ST_POLLING();
	}
	st_join_counter_wait(join_c);
	for(i=0;i<dim;i++) tx[i]=ty[i];
/*	fp=(FILE *)fopen("testp.dat","w");
	for(i=0;i<dim;i++)
	{
		for(j=0;j<(dim+1);j++)
		{
			fprintf(fp,"%e ",workmat[i][j]);fflush(fp);
		}
		fprintf(fp,"\n");fflush(fp);
	}
	fclose(fp); */
	//	elibere la memoire	
	free(join_c);
	for(i=0;i<dim;i++)
	{
		st_mutex_destroy(&mutex[i]);
		st_cond_destroy(&cond[i]);
	}
	free(mutex);
	free(cond);
	free(counters);
	return(0);
}