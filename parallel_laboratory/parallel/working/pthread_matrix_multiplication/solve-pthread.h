/*
	solve.h
	Header for solve systems of equations
*/
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include"barrier-pthread.h"
barrier_t barrier_system;
int mul_striped_fast(long dim,int thread,double **mat,double **mat2);

