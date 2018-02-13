/*
	Matrix power with output matrix in checkerboard partion
*/
#include<parallel/parallel-pthread.h>
#include<math.h>
typedef struct
{
	int N;			/* nr of row/columns */
	int q;			/* partions width */
	int l;			/* first indics of thread */
	int m;			/* second indics of thread */
	int pow;			/* power of matrix */
	double **a;		/* matrix for computing */
	double **b;			/* matrix result */
	double **temp;		/* temporary matrix */
} spow_mat_check;

void *thread_pow_mat_check(void *p)
{
	spow_mat_check data=*(spow_mat_check *)p;
	register int i,j,k,l;
	double **operand,**output,**temp;
	int ord,s;
	ord=data.pow%2;
	s=data.N/data.q;
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
	for(i=s*data.l;i<s*(data.l+1);i++)
		for(j=s*data.m;j<s*(data.m+1);j++)
		{
			output[i][j]=0.0;
			for(k=0;k<data.N;k++)
				output[i][j]+=data.a[i][k]*data.a[k][j];
		}
	barrier_sync(&barrier_pow_mat);

	if(ord==1)
		output=data.b;
	else
		output=data.temp;
	for(l=1;l<(data.pow-1);l++)
	{
		for(i=s*data.l;i<s*(data.l+1);i++)
		for(j=s*data.m;j<s*(data.m+1);j++)
		{
			output[i][j]=0.0;
			for(k=0;k<data.N;k++)
				output[i][j]+=operand[i][k]*data.a[k][j];
		}
		barrier_sync(&barrier_pow_mat);
		temp=operand;
		operand=output;
		output=temp;
	}
	pthread_exit(NULL);
}

int pow_mat_check(int mat,int thread,double **a,double **b,int pow)
{
/*
	mat is the size of square matrix
	thread is the number of threads which will be use for computantion
	a  is the input matrix
	b is the output matrix
	pow is the power of the matrix
*/
	register int l,m;
	int q;
	spow_mat_check **data;
	spow_mat_check *pdata;
	double **mattemp;
	double *pmattemp;
	pthread_t **pt;
	pthread_t *ppt;
	q=sqrt(thread);
	/* Threat diferent exception */
	if(pow==0)
	{
		for(l=0;l<mat;l++)
			for(m=0;m<mat;m++)
				b[l][m]=1.0f;
		return(0);
	}
	else if(pow==1)
	{
		for(l=0;l<mat;l++)
			for(m=0;m<mat;m++)
				b[l][m]=a[l][m];
		return(0);
	}
		
	/* Making alocation for structures */
	if((pt=(pthread_t **)calloc(q,sizeof(pthread_t *)))==(pthread_t **)NULL)
	{
		perror("Error alocation data for pthread identifiers");
		return(-1);
	}
	if((ppt=(pthread_t *)calloc(q*q,sizeof(pthread_t)))==(pthread_t *)NULL)
	{
		perror("Error alocation data for pthread identifiers");
		return(-1);
	}
	for(l=0;l<q;l++)
	{
		pt[l]=ppt;
		ppt+=q;
	}
	if((data=(spow_mat_check **)calloc(q,sizeof(spow_mat_check *)))==(spow_mat_check **)NULL)
	{
		perror("Error alocation data for pthread identifiers");
		free(*pt);
		free(pt);
		return(-1);
	}
	if((pdata=(spow_mat_check *)calloc(q*q,sizeof(spow_mat_check)))==(spow_mat_check *)NULL)
	{
		perror("Error alocation data for pthread identifiers");
		return(-1);
	}
	for(l=0;l<q;l++)
	{
		data[l]=pdata;
		pdata+=q;
	}
	if((mattemp=(double **)calloc(mat,sizeof(double **)))==(double **)NULL)
	{
		perror("Error alocation temporary matrix");
		free(*pt);
		free(pt);
		free(*data);
		free(data);
		return(-1);
	}
	if((pmattemp=(double *)calloc(mat*mat,sizeof(double)))==(double *)NULL)
	{
		perror("Error alocation temporary matrix");
		free(*pt);
		free(pt);
		free(*data);
		free(data);
		return(-1);
	}
	for(l=0;l<mat;l++)
	{
		mattemp[l]=pmattemp;
		pmattemp+=mat;
	}
	/*
		Create threads
	*/
	barrier_init(&barrier_pow_mat,thread);
	for(l=0;l<q;l++)
	for(m=0;m<q;m++)
	{
		data[l][m].N=mat;
		data[l][m].q=q;
		data[l][m].l=l;
		data[l][m].m=m;
		data[l][m].a=a;
		data[l][m].b=b;
		data[l][m].temp=mattemp;
		data[l][m].pow=pow;
		if(pthread_create(&pt[l][m],NULL,thread_pow_mat_check,&data[l][m]))
		{
			perror("Can not create threads");
			free(*pt);
			free(pt);
			free(*data);
			free(data);
			free(*mattemp);
			free(mattemp);
			return(-1);
		}
	}
	/*
		Waiting to finish the threads
	*/
	for(l=0;l<q;l++)
	for(m=0;m<q;m++)
		pthread_join(pt[l][m],NULL);
	/*
		Dealocating the memory
	*/
	barrier_destroy(&barrier_pow_mat);
	free(*mattemp);
	free(mattemp);
	free(*data);
	free(data);
	free(*pt);
	free(pt);
	return(0);
}
