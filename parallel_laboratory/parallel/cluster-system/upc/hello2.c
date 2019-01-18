#include <upc_relaxed.h>
#include <stdio.h>
int main()
{
  if(MYTHREAD==0)
    printf("Rcv'd: 'Starting Execution' from THREAD %d\n",MYTHREAD);
  else
    printf("Hello World from THREAD %d (of %d THREADS)\n",MYTHREAD,THREADS);
}