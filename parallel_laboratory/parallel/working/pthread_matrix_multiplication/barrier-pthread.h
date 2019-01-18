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
