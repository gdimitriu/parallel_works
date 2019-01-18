/*
	making the speedup for parallel calcul complete
*/
double temps1(struct timeval t1,struct timeval t2)
{
long zecimal1,intreg1;
double operand1;
operand1=0.0;
	if(t1.tv_usec>t2.tv_usec)
	{
		zecimal1=1000000+t2.tv_usec-t1.tv_usec;
		intreg1=t2.tv_sec-t1.tv_sec-1;
	}
	else
	{
		zecimal1=t2.tv_usec-t1.tv_usec;
		intreg1=t2.tv_sec-t1.tv_sec;
	}
	operand1=1000000*intreg1+zecimal1;
//	printf("time petrecut %fs \n",operand1/1000000.0);fflush(stdout);
	return operand1;
}
