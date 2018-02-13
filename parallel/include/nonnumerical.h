/*
	Header for nonnumerical operations
*/
/* Definition for find extreme in a vector of double */

typedef struct 
{
	int type;			/* type of extreme */
	/*
		1 for maximum;
		0 for minumum;
	*/
	double *vector;	/* pointer to data vector */
	int N;				/* number of elements */
	int P;				/* number of threads */
	int who;				/* nr of thread which it is */
	double *extreme;	/* value of extreme */
	int *position;		/* position of extreme */
} sextreme;

int extreme(int dim,int thread,double *vector,int type,double *extremeout,int *position);
/*
	dim=dimension of vector
	thread=nr of thread
	vector=is pointer for vector of numbers
	type=	0 for minimum
			1 for maximum
	extremeout is the output data
	position is the position of extreme
*/
