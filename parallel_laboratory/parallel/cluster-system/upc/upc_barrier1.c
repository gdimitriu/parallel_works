#include <upc_relaxed.h>
#include <stdio.h>
shared int a=0;
int b;
int computation(int temp)
{
  return temp+5;
}
int main()
{
  int result=0,i=0;
  do
  {
    if(MYTHREAD==0)
    {
      result=computation(a);
      a=result*THREADS;
    }
    
    upc_barrier 1;
    b=a;
    upc_barrier 1;
    
    if(MYTHREAD==1) b++;
    printf("THREAD %d: b=%d at iteration %d\n",MYTHREAD,b,i);
    fflush(stdout);
    i++;
  }
  while(i<4);
  return 0;
}