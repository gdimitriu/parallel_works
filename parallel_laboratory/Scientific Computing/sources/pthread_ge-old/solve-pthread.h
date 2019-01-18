/*
	solve.h
	Header for solve systems of equations
*/
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include"barrier-pthread.h"
barrier_t barrier_system;
int gauss_striped_fast(long dim,int thread,double **mat,double *x,double *y);
/*
	dim is dimension of data matrix
	thread is the numbers of threads
	mat is data matrix
	x is uncknown vector
	y is free vector
*/

