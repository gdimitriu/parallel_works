/*
	making the speedup for parallel calcul complete
*/
void temps(struct timeval *t1,struct timeval *t2,int T)
{
long zecimal1,intreg1;
float operand1;
float speedup;
int i;
operand1=0.0;
for(i=0;i<T;i++)
{
	if(t1[i].tv_usec>t2[i].tv_usec)
	{
		zecimal1=1000000+t2[i].tv_usec-t1[i].tv_usec;
		intreg1=t2[i].tv_sec-t1[i].tv_sec-1;
	}
	else
	{
		zecimal1=t2[i].tv_usec-t1[i].tv_usec;
		intreg1=t2[i].tv_sec-t1[i].tv_sec;
	}
	operand1=operand1+1000000*intreg1+zecimal1;
}
	printf("time petrecut %fs \n",operand1/1000000.0/T);fflush(stdout);
}
