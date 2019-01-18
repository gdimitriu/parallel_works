/*
	making the speedup for parallel calcul complete
*/
void speedup(struct timeval *t1ser,struct timeval *t2ser,struct timeval *t1par,struct timeval *t2par,int P,int T,int var)
{
FILE *fp,*fp1,*fp2;
long zecimal1,intreg1;
long zecimal2,intreg2;
float operand1,operand2;
float speedup;
int i;
operand1=operand2=0.0;

for(i=0;i<T;i++)
{
	if(t1ser[i].tv_usec>t2ser[i].tv_usec)
	{
		zecimal1=1000000+t2ser[i].tv_usec-t1ser[i].tv_usec;
		intreg1=t2ser[i].tv_sec-t1ser[i].tv_sec-1;
	}
	else
	{
		zecimal1=t2ser[i].tv_usec-t1ser[i].tv_usec;
		intreg1=t2ser[i].tv_sec-t1ser[i].tv_sec;
	}
	if(t1par[i].tv_usec>t2par[i].tv_usec)
	{
		zecimal2=1000000+t2par[i].tv_usec-t1par[i].tv_usec;
		intreg2=t2par[i].tv_sec-t1par[i].tv_sec-1;
	}
	else
	{
		zecimal2=t2par[i].tv_usec-t1par[i].tv_usec;	
		intreg2=t2par[i].tv_sec-t1par[i].tv_sec;
	}
	operand1=operand1+1000000*intreg1+zecimal1;
	operand2=operand2+1000000*intreg2+zecimal2;
}
	speedup=operand1/operand2;
	fp=(FILE *)fopen("speedup.dat","a");
	fp1=(FILE *)fopen("time-par.dat","a");
	fp2=(FILE *)fopen("time-ser.dat","a");
	fprintf(fp2,"%d %fs %d\n",P,operand1/1000000.0/T,var);fflush(fp2);
	fprintf(fp1,"%d %fs %d\n",P,operand2/1000000.0/T,var);fflush(fp1);
	fprintf(fp,"%d %f %d \n",P,speedup,var);fflush(fp);
	printf("Speedup=%f with %d thread nean of %d with %d ec\n",speedup,P,T,var);fflush(stdout);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
}
