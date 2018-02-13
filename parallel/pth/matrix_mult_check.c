/*
	Matrix multiplication with output matrix in checkerboard partion
*/
#include<parallel/parallel-pth.h>
#include<math.h>
typedef struct
{
	int N;			/* nr of row/columns */
	int q;			/* partions width */
	int l;			/* first indics of thread */
	int m;			/* second indics of thread */
	double **a,**b;	/* matrix for computing */
	double **c;		/* matrix result */
} smult_sq_mat_check;

void *thread_mult_sq_mat_check(void *p)
{
	smult_sq_mat_check data=*(smult_sq_mat_check *)p;
	register int i,j,k;
	int s;
	s=data.N/data.q;
	for(i=s*data.l;i<s*(data.l+1);i++)
	for(j=s*data.m;j<s*(data.m+1);j++)
	{
		data.c[i][j]=0.0;
		for(k=0;k<data.N;k++)
			data.c[i][j]+=data.a[i][k]*data.b[k][j];
	}
	pth_exit((void *)NULL);
}

int mult_sq_mat_check(int mat,int thread,double **a,double **b,double **c)
{
/*
	mat is the size of square matrix
	thread is the number of threads which will be use for computantion
	a and b are the input matrix
	c is the output matrix
*/
	register int l,m;
	int q;
	smult_sq_mat_check **data;
	smult_sq_mat_check *pdata;
	pth_t **pt;
	pth_t *ppt;
	pth_attr_t attr;
	pth_init();
	q=sqrt(thread);
	/* Making alocation for structures */
	if((pt=(pth_t **)calloc(q,sizeof(pth_t *)))==(pth_t **)NULL)
	{
		perror("Error alocation data for pthread identifiers");
		return(-1);
	}
	if((ppt=(pth_t *)calloc(q*q,sizeof(pth_t)))==(pth_t *)NULL)
	{
		perror("Error alocation data for pthread identifiers");
		return(-1);
	}
	for(l=0;l<q;l++)
	{
		pt[l]=ppt;
		ppt+=q;
	}
	if((data=(smult_sq_mat_check **)calloc(q,sizeof(smult_sq_mat_check *)))==(smult_sq_mat_check **)NULL)
	{
		perror("Error alocation data for pthread identifiers");
		free(*pt);
		free(pt);
		return(-1);
	}
	if((pdata=(smult_sq_mat_check *)calloc(q*q,sizeof(smult_sq_mat_check)))==(smult_sq_mat_check *)NULL)
	{
		perror("Error alocation data for pthread identifiers");
		free(*pt);
		free(pt);
		return(-1);
	}
	for(l=0;l<q;l++)
	{
		data[l]=pdata;
		pdata+=q;
	}

	/*
		Create threads
	*/
	attr=pth_attr_new();
	pth_attr_set(attr,PTH_ATTR_JOINABLE,TRUE);
	for(l=0;l<q;l++)
	for(m=0;m<q;m++)
	{
		data[l][m].N=mat;
		data[l][m].q=q;
		data[l][m].l=l;
		data[l][m].m=m;
		data[l][m].a=a;
		data[l][m].b=b;
		data[l][m].c=c;
		if((pt[l][m]=pth_spawn(attr,thread_mult_sq_mat_check,&data[l][m]))==(pth_t )NULL)
		{
			perror("Can not create threads");
			free(*data);
			free(*pt);
			free(data);
			free(pt);
			return(-1);
		}
	}
	/*
		Waiting to finish the threads
	*/
	for(l=0;l<q;l++)
	for(m=0;m<q;m++)
		pth_join(pt[l][m],NULL);
	/*
		Dealocating the memory
	*/
	pth_attr_destroy(attr);
	free(*data);
	free(data);
	free(*pt);
	free(pt);
	pth_kill();
	return(0);
}
