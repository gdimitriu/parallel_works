/*
	solve.h
	Header for solve systems of equations
*/
barrier_t barrier_system;
int gauss_striped_fast(long dim,int thread,double **mat,double *x,double *y);
/*
	dim is dimension of data matrix
	thread is the numbers of threads
	mat is data matrix
	x is uncknown vector
	y is free vector
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
