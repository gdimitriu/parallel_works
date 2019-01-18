/*
	solve-openmp.h
	Header for solve systems of equations
*/
#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
int mul_striped_fast(long dim,int thread,double **mat,double **mat2);

