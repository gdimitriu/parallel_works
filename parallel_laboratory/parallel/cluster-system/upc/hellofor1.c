#include <upc_relaxed.h>
#include <stdio.h>
#include <stdlib.h>
#define N 10
shared [2] int arr[N];

int main()
{

  int i=0;
  upc_forall(i=0;i<N;i++;&arr[i])
    printf("THREAD %d (of %d THREADS) performing iteration %d \n",MYTHREAD,THREADS,i);
  return 0;
}
