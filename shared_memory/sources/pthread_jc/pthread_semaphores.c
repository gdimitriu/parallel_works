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
/* semaphores by condition variables */
#include<barrier-pthread.h>
/* initialize the semaphore */
int sema_init(sema_t *sem, int val)
{
	if(pthread_mutex_init(&sem->lock,NULL)) return 1;
	if(pthread_cond_init(&sem->cond,NULL)) return 2;
	sem->count=val;
	sem->waiters=0;
	return 0;
}
/* destroy de semaphore */
void sema_destroy(sema_t *sem)
{
	pthread_mutex_destroy(&sem->lock);
	pthread_cond_destroy(&sem->cond);
}
/* semaphore wait */
void sema_wait(sema_t *sem)
{
	pthread_mutex_lock(&sem->lock);
	while(sem->count==0)
	{
		++sem->waiters;
		pthread_cond_wait(&sem->cond,&sem->lock);
		--sem->waiters;
	}
	--sem->count;
	pthread_mutex_unlock(&sem->lock);
}
/* signal for semaphore */
void sema_signal(sema_t *sem,int release)
{
	int waiters=0;
	pthread_mutex_lock(&sem->lock);
	sem->count+=release;
	waiters=sem->waiters;
	pthread_mutex_unlock(&sem->lock);
	if(waiters>0)
		pthread_cond_signal(&sem->cond);
}
