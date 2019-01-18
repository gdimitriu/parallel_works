/*
	making the acceleration for parallel calcul
*/
void acceleration(struct timeval t1ser,struct timeval t2ser,struct timeval t1par,struct timeval t2par,int P)
{
long zecimal1,intreg1;
long zecimal2,intreg2;
float operand1,operand2;
float acceleration;
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
	if(t1par.tv_usec>t2par.tv_usec)
	{
		zecimal2=1000000+t2par.tv_usec-t1par.tv_usec;
		intreg2=t2par.tv_sec-t1par.tv_sec-1;
	}
	else
	{
		zecimal2=t2par.tv_usec-t1par.tv_usec;	
		intreg2=t2par.tv_sec-t1par.tv_sec;
	}
	operand1=1000000*intreg1+zecimal1;
	operand2=1000000*intreg2+zecimal2;
	acceleration=operand1/operand2;
	printf("serial time is %fus %fs\n",operand1,operand1/1000000.0);
	printf("parallel time with %d thread is %fus %fs\n",P,operand2,operand2/1000000.0);
	printf("Acceleration=%f with %d thread\n",acceleration,P);
}

void timecalc(struct timeval t1ser,struct timeval t2ser)
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
