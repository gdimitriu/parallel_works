/*
	matrix.h
	Header for matrix multiplications library
	by Gabriel Dimitriu
	v 0.1 2001.01
*/
int mult_sq_mat_striped(long mat,int thread,double **a,double **b,double **c,int type);
/*
    type=0 for columnwise partitioning
    type=1 for rowwise partitioning
*/
int mult_sq_mat_check(long mat,int thread,double **a,double **b,double **c);
barrier_t barrier_pow_mat;

                      