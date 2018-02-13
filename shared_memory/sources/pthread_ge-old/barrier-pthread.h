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
#include<pthread.h>
#define BARRIER_ER_THREADS_WAITING -100
typedef struct
{
	pthread_mutex_t lock;
	pthread_cond_t cond;
	volatile int count;		/* semaphore value */
	volatile int waiters;	/* nr of threads in waiting */
} sema_t;
typedef struct
{
	int nWaiters;		/* nr of threads in waiting */
	int bCount;			/* threads counter */
	pthread_mutex_t bMutex;
	pthread_cond_t bCond;
}	barrier_t;
typedef struct
{
	int nWaiters;
	int bCount;
	pthread_mutex_t bMutex;
	sema_t barrierEntrySema;
	sema_t barrierSema;
} BARRIER;

/* semaphores */
int sema_init(sema_t *sem,int val);
void sema_destroy(sema_t *sem);
void sema_wait(sema_t *sem);
void sema_signal(sema_t *sem,int release);

/* Barrier implement with 1 cond var an 1 mutex */
int barrier_init(barrier_t *pBarrier,int bCount);
int barrier_reset(barrier_t *pBarrier,int bCount);
int barrier_sync(barrier_t *pBarrier);
void barrier_destroy(barrier_t *pBarrier);

/* Barrier implement with1 mutex and 2 semaphores */
int BarrierInit(BARRIER *pBarrier,int bCount);
int BarrierSync(BARRIER *pBarrier);
