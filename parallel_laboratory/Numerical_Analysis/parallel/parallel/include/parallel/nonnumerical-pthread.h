/*
	Header for nonnumerical operations
*/
/* Definition for find extreme in a vector of double */
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
