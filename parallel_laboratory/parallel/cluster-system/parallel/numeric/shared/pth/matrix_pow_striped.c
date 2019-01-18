/*
	Matrix multiplications with output matrix in rowwise or colmnwise stiping partion
*/
#include<parallel/parallel-pth.h>
#include<math.h>
typedef struct
{
	int N;					/* nr of rows/colummns */
	int P;					/* nr of threads */
	int who; 				/* thread nr */
	double **a;				/* matrix for computing */
	double **b;				/* matrix result */
	double **temp;			/* temporary matrix */
	int pow;					/* power of the matrix */
} spow_mat_striped;

void *thread_pow_mat_row(void *p)
{
	spow_mat_striped data=*(spow_mat_striped *)p;
	register int i,j,k,l;
	int s,ord;
	double **operand,**output,**temp;
	s=data.N/data.P;
	ord=data.pow%2;
	if(ord==1)
	{
		operand=data.temp;
		output=data.temp;
	}
	else
	{
		operand=data.b;
		output=data.b;
	}
	for(i=s*data.who;i<s*(data.who+1);i++)
		for(j=0;j<data.N;j++)
		{
			output[i][j]=0.0;
			for(k=0;k<data.N;k++)
				output[i][j]+=data.a[i][k]*data.a[k][j];
		}
	pth_barrier_reach(&barrier_pow_mat);
	if(ord==1)
		output=data.b;
	else
		output=data.temp;
	for(l=1;l<(data.pow-1);l++)
	{
		for(i=s*data.who;i<s*(data.who+1);i++)
			for(j=0;j<data.N;j++)
			{
				output[i][j]=0.0;
				for(k=0;k<data.N;k++)
					output[i][j]+=operand[i][k]*data.a[k][j];
			}
		pth_barrier_reach(&barrier_pow_mat);
		temp=operand;
		operand=output;
		output=temp;
	}	
	pth_exit((void *)NULL);
}

void *thread_pow_mat_col(void *p)
{
	spow_mat_striped data=*(spow_mat_striped *)p;
	register int i,j,k,l;
	int s,ord;
	double **operand,**output,**temp;
	s=data.N/data.P;
	ord=data.pow%2;
	if(ord==1)
	{
		operand=data.temp;
		output=data.temp;
	}
	else
	{
		operand=data.b;
		output=data.b;
	}
	for(i=s*data.who;i<s*(data.who+1);i++)
		for(j=0;j<data.N;j++)
		{
			output[i][j]=0.0;
			for(k=0;k<data.N;k++)
				output[i][j]+=data.a[i][k]*data.a[k][j];
		}
	pth_barrier_reach(&barrier_pow_mat);
	if(ord==1)
		output=data.b;
	else
		output=data.temp;
	
	for(l=1;l<(data.pow-1);l++)
	{
		for(j=s*data.who;j<s*(data.who+1);j++)
			for(i=0;i<data.N;i++)
			{
				output[i][j]=0.0;
				for(k=0;k<data.N;k++)
					output[i][j]+=operand[i][k]*data.a[k][j];
			}
		pth_barrier_reach(&barrier_pow_mat);
		temp=operand;
		operand=output;
		output=temp;
	}	
	pth_exit((void *)NULL);
}



int pow_mat_striped(int mat,int thread,double **a,double **b,int pow,int type)
{
/*
	mat is the size of square matrix
	thread is the nr of thread which will be use for computation
	a is the input matrix
	b is the output matrix
	pow is the argument of power
	type is 0 for colummnwise or 1 for rowwise partioning
*/
	int i,j;
	pth_t *pt;
	spow_mat_striped *data;
	double **mattemp;
	double *pmattemp;
	pth_attr_t attr;
	/*  Treat diferent exception */
	if(pow==0) 
	{
		for(i=0;i<mat;i++)
			for(j=0;j<mat;j++)
				b[i][j]=1.0f;
		return(0);
	}
	else if(pow==1)
	{
		for(i=0;i<mat;i++)
			for(j=0;j<mat;j++)
				b[i][j]=a[i][j];
		return(0);
	}
				
	/* Making alocations for structures */
	if((pt=(pth_t *)calloc(thread,sizeof(pth_t)))==(pth_t *)NULL) 
	{
		perror("Error alocation data for pthread identifier");
		return(-1);
	}
	if((data=(spow_mat_striped *)calloc(thread,sizeof(spow_mat_striped)))==(spow_mat_striped *)NULL) 
	{
		perror("Error alocation data for pthread identifier");
		free(pt);
		return(-1);
	}
	/* 
		Alocation for temporary matrix 
	*/
	if((mattemp=(double **)calloc(mat,sizeof(double **)))==(double **)NULL)
	{
		perror("Error alocation temporary matrix");
		free(pt);
		free(data);
		return(-1);
	}
	if((pmattemp=(double *)calloc(mat*mat,sizeof(double)))==(double *)NULL)
	{
		perror("Error alocation pointer for temporary matrix");
		return(-1);
	}
	for(i=0;i<mat;i++)
	{
		mattemp[i]=pmattemp;
		pmattemp+=mat;
	}
	/*
		Create threads
	*/
	pth_barrier_init(&barrier_pow_mat,thread);
	attr=pth_attr_new();
	pth_attr_set(attr,PTH_ATTR_JOINABLE,TRUE);
	for(i=0;i<thread;i++)
	{
		data[i].P=thread;
		data[i].N=mat;
		data[i].who=i;
		data[i].a=a;
		data[i].b=b;
		data[i].temp=mattemp;
		data[i].pow=pow;
		switch(type)
		{
			case 0:
			{
				if((pt[i]=pth_spawn(attr,thread_pow_mat_col,&data[i]))==(pth_t)NULL) 
				{
				perror("Can not create threads");
				free(data);
				free(pt);
				free(*mattemp);
				free(mattemp);
				return(-1);
				}
				break;
			}
			case 1:
			{
				if((pt[i]=pth_spawn(attr,thread_pow_mat_row,&data[i]))==(pth_t )NULL)
				{
				perror("Can not create threads");
				free(data);
				free(pt);
				free(*mattemp);
				free(mattemp);
				return(-1);
				}
				break;
			}
			default:
			{
				if((pt[i]=pth_spawn(attr,thread_pow_mat_row,&data[i]))==(pth_t)NULL) 
				{
				perror("Can not create threads");
				free(data);
				free(pt);
				free(*mattemp);
				free(mattemp);
				return(-1);
				}
			}
		}
	}
	/*
		Waiting to finish the threads
	*/
	for(i=0;i<thread;i++)
		pth_join(pt[i],NULL);
	/* dealocating the memory */
	pth_attr_destroy(attr);
	free(pt);
	free(data);
	free(*mattemp);
	free(mattemp);
	pth_kill();
	return(0);
}
