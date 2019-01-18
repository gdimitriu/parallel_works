/*
	Bariers for pthread library
*/
/* Implementation with mutex and two semaphores */
#include <pthread_barrier.h>

int BarrierInit(BARRIER *pBarrier,int bCount)
{
	pBarrier->nWaiters=0;
	pBarrier->bCount=bCount;
	pthread_mutex_init(&pBarrier->bMutex,NULL);
	sema_init(&pBarrier->barrierEntrySema,bCount);
	sema_init(&pBarrier->barrierSema,0);
	return(0);
}
int BarrierSync(BARRIER *pBarrier)
{
	int nWaiters=0;
	/* Each thread decrement the entry semaphore */
	sema_wait(&pBarrier->barrierEntrySema);
	pthread_mutex_lock(&pBarrier->bMutex);
	++pBarrier->nWaiters;
	/* Last thread touch the barrier */
	if(pBarrier->nWaiters==pBarrier->bCount)
	{
		--pBarrier->nWaiters;
		pthread_mutex_unlock(&pBarrier->bMutex);
		sema_signal(&pBarrier->barrierSema,pBarrier->bCount-1);
		return 0;
	}
	else
	{
		pthread_mutex_unlock(&pBarrier->bMutex);
		sema_wait(&pBarrier->barrierSema);
		pthread_mutex_lock(&pBarrier->bMutex);
		--pBarrier->nWaiters;
		nWaiters=pBarrier->nWaiters;
		pthread_mutex_unlock(&pBarrier->bMutex);
		
		/* Last thread */
		if(nWaiters==0)
			sema_signal(&pBarrier->barrierEntrySema,pBarrier->bCount);
	}
	return(0);
}