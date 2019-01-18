/*
	making the speedup for parallel calcul complete
*/
void speedup(struct timeval t1ser,struct timeval t2ser,struct timeval t1par,struct timeval t2par,int P,int var)
{
long zecimal1,intreg1;
long zecimal2,intreg2;
float operand1,operand2;
float speedup;
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
	speedup=operand1/operand2;
	printf("time serial %d %fs %d\n",P,operand1/1000000.0,var);fflush(stdout);
	printf("time serial %d %fs %d\n",P,operand2/1000000.0,var);fflush(stdout);
	printf("Speedup=%f with %d thread  with %d ec\n",speedup,P,var);fflush(stdout);
}
