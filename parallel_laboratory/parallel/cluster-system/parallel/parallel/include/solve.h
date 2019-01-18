/*
	solve.h
	Header for solve systems of equations
*/
/*
	data for making ring of process
*/
typedef struct
{
	int *counters;					/* counters for number of wait */
	pthread_mutex_t *mutex;		/* variable for condition signal */
	pthread_cond_t *cond;		/* condition for serialization of acces */
} spipe_fast;
/*
	data for solve in rowwise or columnwise partitioning
*/
barrier_t barrier_system;
typedef struct
{
	double **mat;		/* adjounct matrix */
	int who;				/* nr of courent thread*/
	int N;				/* dimensions of matrix */
	int P;				/* nr of threads */
	spipe_fast *pipe;	/* pointer at communications vector */
} ssystem_striped_fast;
int gauss_striped_fast(int dim,int thread,double **mat,double *x,double *y,int type);
/*
	dim is dimension of data matrix
	thread is the numbers of threads
	mat is data matrix
	x is uncknown vector
	y is free vector
	type is 0 for rowwise ciclic or 1 for rowwise block
	NOTE: the clumnwise partitioning doesn't exist
*/
int triungiular_UP_striped(int dim,int thread,double **mat,double *x,double *y,int type);
/*
	dim is dimension of data matrix
	thread is the numbers of threads
	mat is data matrix
	x is uncknown vector
	y is free vector
	type is 0 for rowwise ciclic or 1 for rowwise block
	NOTE: the clumnwise partitioning doesn't exist
*/
