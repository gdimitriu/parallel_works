/*
	matrix.h
	Header for matrix multiplications library
	by Gabriel Dimitriu
	v 0.1 2001.01
*/
/*
	Declarations for multiply with striped partitions (row and columns)
*/
typedef struct
{
	int N;				/* nr of rows/colummns */
	int P;				/* nr of threads */
	int who; 			/* thread nr */
	double **a,**b;	/* matrix for computing */
	double **c;			/* matrix result */
} smult_sq_mat_striped;
/*
    type=0 for columnwise partitioning
    type=1 for rowwise partitioning
*/
int mult_sq_mat_striped(int mat,int thread,double **a,double **b,double **c,int type);
/*
	Declarations for multiply with checkerboard partions
*/
typedef struct
{
	int N;			/* nr of row/columns */
	int q;			/* partions width */
	int l;			/* first indics of thread */
	int m;			/* second indics of thread */
	double **a,**b;	/* matrix for computing */
	double **c;		/* matrix result */
} smult_sq_mat_check;
int mult_sq_mat_check(int mat,int thread,double **a,double **b,double **c);

/*
	Declarations for matrix at power with striped partions (rowwise or columnwise)
*/
barrier_t barrier_pow_mat;
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
/*
    type=0 for columnwise partitioning
    type=1 for rowwise partitioning
*/
int pow_mat_striped(int mat,int thread,double **a,double **b,int pow,int type);	
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
int pow_mat_check(int mat,int thread,double **a,double **b,int pow);	
/*
	structure for matrix-vector multplication
*/
typedef struct
{
	int N;			/* nr of row/columns for matrix */
	int P;			/* nr of threads */
	int who;			/* indics */
	double **a;		/* input matrix */
	double *x;		/* vector input */
	double *y;		/* vector rezoult */
} smult_sq_mat_vect_striped;
int mult_sq_mat_vect_striped(int mat,int thread,double **a,double *x,double *y);
