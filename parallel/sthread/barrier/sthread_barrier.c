/*
	Bariers for pthread library
*/
/* Implementation with conditioner variable and mutex */
#include<parallel/barrier-sthread.h>
int barrier_init(barrier_t *pBarrier,int bCount)
{
	pBarrier->nWaiters=0;
	pBarrier->bCount=bCount;
	if(st_mutex_init(pBarrier->bMutex)) return 1;
	if(st_cond_init(pBarrier->bCond)) return 2;
	return 0;
}
int barrier_reset(barrier_t *pBarrier,int bCount)
{
	st_mutex_lock(pBarrier->bMutex);
	if(pBarrier->nWaiters>0)
	{
		st_mutex_unlock(pBarrier->bMutex);
		return(BARRIER_ER_THREADS_WAITING);
	}
	pBarrier->bCount=bCount;
	st_mutex_unlock(pBarrier->bMutex);
	return 0;
}
int barrier_sync(barrier_t *pBarrier)
{
	st_mutex_lock(pBarrier->bMutex);
	++pBarrier->nWaiters;
	if(pBarrier->nWaiters==pBarrier->bCount)
	{
		/* last thread tuch the barrier */
		pBarrier->nWaiters=0;
		st_mutex_unlock(pBarrier->bMutex);
		st_cond_broadcast(pBarrier->bCond);
		return 0;
	}
	else
		st_cond_wait(pBarrier->bCond,pBarrier->bMutex);
	st_mutex_unlock(pBarrier->bMutex);
	return 0;
}
void barrier_destroy(barrier_t *pBarrier)
{
	st_mutex_destroy(pBarrier->bMutex);
	st_cond_destroy(pBarrier->bCond);
}

