/*
	making the acceleration for parallel calcul
*/
void timesec(struct timeval t1ser,struct timeval t2ser)
{
long zecimal1,intreg1;
float operand1;
	if(t1ser.tv_usec>t2ser.tv_usec)
	{
		zecimal1=1000000+t2ser.tv_usec-t1ser.tv_usec;
		intreg1=t2ser.tv_sec-t1ser.tv_sec-1;
	}
	else
	{
		zecimal1=t2ser.tv_usec-t1ser.tv_usec;
		intreg1=t2ser.tv_sec-t1ser.tv_sec;
	}
	operand1=1000000*intreg1+zecimal1;
	printf("serial time is %fus %fs\n",operand1,operand1/1000000.0);
}
