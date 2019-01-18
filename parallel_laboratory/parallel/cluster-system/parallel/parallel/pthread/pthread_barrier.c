/*
	Bariers for pthread library
*/
/* Implementation with conditioner variable and mutex */
#include<parallel/barrier-pthread.h>
int barrier_init(barrier_t *pBarrier,int bCount)
{
	pBarrier->nWaiters=0;
	pBarrier->bCount=bCount;
	if(pthread_mutex_init(&pBarrier->bMutex,NULL)) return 1;
	if(pthread_cond_init(&pBarrier->bCond,NULL)) return 2;
	return 0;
}
int barrier_reset(barrier_t *pBarrier,int bCount)
{
	pthread_mutex_lock(&pBarrier->bMutex);
	if(pBarrier->nWaiters>0)
	{
		pthread_mutex_unlock(&pBarrier->bMutex);
		return(BARRIER_ER_THREADS_WAITING);
	}
	pBarrier->bCount=bCount;
	pthread_mutex_unlock(&pBarrier->bMutex);
	return 0;
}
int barrier_sync(barrier_t *pBarrier)
{
	pthread_mutex_lock(&pBarrier->bMutex);
	++pBarrier->nWaiters;
	if(pBarrier->nWaiters==pBarrier->bCount)
	{
		/* last thread tuch the barrier */
		pBarrier->nWaiters=0;
		pthread_mutex_unlock(&pBarrier->bMutex);
		pthread_cond_broadcast(&pBarrier->bCond);
		return 0;
	}
	else
		pthread_cond_wait(&pBarrier->bCond,&pBarrier->bMutex);
	pthread_mutex_unlock(&pBarrier->bMutex);
	return 0;
}
void barrier_destroy(barrier_t *pBarrier)
{
	pthread_mutex_destroy(&pBarrier->bMutex);
	pthread_cond_destroy(&pBarrier->bCond);
}

