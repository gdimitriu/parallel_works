#include<stdio.h>
#include<stdlib.h>
#include<st.h>
#include"barrier-sthread.h"
int gauss_striped_fast(int dim,int threads,double **mat,double *x,double *y);
   /*
      dim c'est la dimensione de matrix
      thread c'est le numero de threads
      mat c'est le matrix de donne
      x c'est le resoudre vector
      y c'est el vector libre
   */
                                 