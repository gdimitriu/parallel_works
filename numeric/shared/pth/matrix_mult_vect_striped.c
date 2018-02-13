#include<parallel/parallel-pth.h>
typedef struct
{
	int N;			/* nr of row/columns for matrix */
	int P;			/* nr of threads */
	int who;			/* indics */
	double **a;		/* input matrix */
	double *x;		/* vector input */
	double *y;		/* vector rezoult */
} smult_sq_mat_vect_striped;

void *thread_mult_sq_mat_vect_row(void *p)
{
	smult_sq_mat_vect_striped data=*(smult_sq_mat_vect_striped *)p;
	int s,i,j;
	s=data.N/data.P;
	for(i=s*data.who;i<(data.who+1)*s;i++)
	{
		data.y[i]=0.0f;
		for(j=0;j<data.N;j++)
			data.y[i]+=data.a[i][j]*data.x[j];
	}
	pth_exit((void *)NULL);
}

int mult_sq_mat_vect_striped(int mat,int thread,double **a,double *x,double *y)
{
/*
	N nr of row/column for matrix
	thread nr of threads for computing
	a input matrix
	x input vector
	y input vector
*/
	smult_sq_mat_vect_striped *data;
	int i;
	pth_t *pt;
	pth_attr_t attr;
	/*	Make alocation */
	if((data=(smult_sq_mat_vect_striped *)calloc(thread,sizeof(smult_sq_mat_vect_striped)))==(smult_sq_mat_vect_striped *)NULL)
	{
		perror("Error alocation identification");
		return(-1);
	}
	if((pt=(pth_t *)calloc(thread,sizeof(pth_t)))==(pth_t *)NULL)
	{
		perror("Error alocation indics vector");
		return(-1);
	}
	/* Create threads */
	attr=pth_attr_new();
	pth_attr_set(attr,PTH_ATTR_JOINABLE,TRUE);
	for(i=0;i<thread;i++)
	{
		data[i].a=a;
		data[i].x=x;
		data[i].y=y;
		data[i].who=i;
		data[i].N=mat;
		data[i].P=thread;
		if((pt[i]=pth_spawn(attr,thread_mult_sq_mat_vect_row,&data[i]))==(pth_t)NULL)
		{
			perror("Can not create threads");
			free(pt);
			free(data);
			return(-1);
		}
	}
	/* Wait all thread to finish */
	for(i=0;i<thread;i++)
		pth_join(pt[i],NULL);
	/* dealocating memory */
	pth_attr_destroy(attr);
	free(pt);
	free(data);
	pth_kill();
	return(0);
}
