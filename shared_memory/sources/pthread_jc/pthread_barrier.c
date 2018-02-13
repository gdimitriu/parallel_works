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
/* Implementation with conditioner variable and mutex */
#include<barrier-pthread.h>
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

