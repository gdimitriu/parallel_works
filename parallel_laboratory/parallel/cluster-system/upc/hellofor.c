#include <upc_relaxed.h>
#include <stdio.h>

int main()
{
int i;
  upc_forall(i=0;i<10;i++;i)
  printf("THREAD %d (of %d THREADS) performing iteration %d\n",MYTHREAD,THREADS,i);
}