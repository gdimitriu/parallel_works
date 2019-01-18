#include<sys/unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/wait.h>

int mult_sq_mat_striped_ciclique_process(long mat,int thread,double **a,double **b,double **c,int type);