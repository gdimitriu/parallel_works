/* semaphores by condition variables */
#include<parallel/barrier-pthread.h>
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