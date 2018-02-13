/*
	matrix.h
	Header for matrix multiplications library
	by Gabriel Dimitriu
	v 0.1 2001.01
*/
int mult_sq_mat_striped(int mat,int thread,double **a,double **b,double **c,int type);
/*
    type=0 for columnwise partitioning
    type=1 for rowwise partitioning
*/
int mult_sq_mat_check(int mat,int thread,double **a,double **b,double **c);
pth_barrier_t barrier_pow_mat;
int pow_mat_striped(int mat,int thread,double **a,double **b,int pow,int type);	
/*
    type=0 for columnwise partitioning
    type=1 for rowwise partitioning
*/
int pow_mat_check(int mat,int thread,double **a,double **b,int pow);	
int mult_sq_mat_vect_striped(int mat,int thread,double **a,double *x,double *y);
