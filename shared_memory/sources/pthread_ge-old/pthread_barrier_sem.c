/*
	Copyright 2006 Gabriel Dimitriu

	This file is part of scientific_computing.

    scientific_computing is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    scientific_computing is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with scientific_computing; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
*/
/*
	Bariers for pthread library
*/
/* Implementation with mutex and two semaphores */
#include <barrier-pthread.h>

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
