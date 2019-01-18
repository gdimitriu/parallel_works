#include <upc_relaxed.h>
#include <stdio.h>
#define N 1000
shared int *vector;
shared int result;
upc_lock_t *lock;
int main()
{
  int i;
  int local;
  lock=upc_all_lock_alloc();
  vector=(shared int *)upc_all_alloc(N,sizeof(int));
  if(MYTHREAD==0)
  {
//    vector=upc_global_alloc(10,sizeof(int));
    for(i=0;i<N;i++)
      vector[i]=1;
     result=0;
  }
  upc_barrier;
  local=0;
  upc_forall(i=0;i<N;i++;i)
    local+=vector[i];
  upc_lock(lock);
  result+=local;
  upc_unlock(lock);
  upc_barrier;
  printf("Thread %d\n",MYTHREAD);
  fflush(stdout);
  if(MYTHREAD==0)
  {
    printf("Result is %d\n",result);
    fflush(stdout);
    upc_free(vector);
    upc_lock_free(lock);
  }
}