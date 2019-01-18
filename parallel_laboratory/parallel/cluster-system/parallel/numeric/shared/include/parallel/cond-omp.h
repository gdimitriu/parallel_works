#include<omp.h>
#include<stdio.h>
typedef struct
{
	int cond;
	int counter;
	int broadcast;
} omp_cond_t;

void omp_init_cond(omp_cond_t *cond,omp_lock_t *mutex);
void omp_signal_cond(omp_cond_t *cond,omp_lock_t *mutex);
void omp_bcast_cond(omp_cond_t *cond,omp_lock_t *mutex,int nr);
void omp_wait_cond(omp_cond_t *cond,omp_lock_t *mutex);
