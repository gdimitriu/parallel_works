/*
	making the speedup for parallel calcul complete
*/
void speedup(struct timeval *t1ser,struct timeval *t2ser,struct timeval *t1par,struct timeval *t2par,int P,int T,int var)
{
FILE *fp,*fp1,*fp2;
unsigned long zecimal1,intreg1;
unsigned long zecimal2,intreg2;
unsigned long temp;
double *operand1,*operand2;
double operand1_tmp,operand2_tmp;
double speedup;
int i;
speedup=0.0;
operand1=(double *)calloc(T,sizeof(double));
operand2=(double *)calloc(T,sizeof(double));
for(i=0;i<T;i++)
{
	operand1[i]=operand2[i]=0.0;
	if(t1ser[i].tv_usec>t2ser[i].tv_usec)
	{
		temp=t2ser[i].tv_usec-t1ser[i].tv_usec;
		zecimal1=1000000+temp;
		intreg1=t2ser[i].tv_sec-t1ser[i].tv_sec-1;
	}
	else
	{
		zecimal1=t2ser[i].tv_usec-t1ser[i].tv_usec;
		intreg1=t2ser[i].tv_sec-t1ser[i].tv_sec;
	}
	if(t1par[i].tv_usec>t2par[i].tv_usec)
	{
		temp=t2par[i].tv_usec-t1par[i].tv_usec;
		zecimal2=1000000+temp;
		intreg2=t2par[i].tv_sec-t1par[i].tv_sec-1;
	}
	else
	{
		zecimal2=t2par[i].tv_usec-t1par[i].tv_usec;	
		intreg2=t2par[i].tv_sec-t1par[i].tv_sec;
	}
	operand1[i]=(1000000*intreg1+zecimal1);
	operand2[i]=(1000000*intreg2+zecimal2);
	speedup+=operand1[i]/operand2[i];
}
	speedup=speedup/T;
	operand1_tmp=operand2_tmp=0.0;
	for(i=0;i<T;i++)
	{
		operand1_tmp+=(double)operand1[i]/(double)1000000.0/T;
		operand2_tmp+=(double)operand2[i]/(double)1000000.0/T;
	}
	fp=(FILE *)fopen("speedup.dat","a");
	fp1=(FILE *)fopen("time-par.dat","a");
	fp2=(FILE *)fopen("time-ser.dat","a");
	fprintf(fp2,"%d %fs %d\n",P,operand1_tmp,var);fflush(fp2);
	fprintf(fp1,"%d %fs %d\n",P,operand2_tmp,var);fflush(fp1);
	fprintf(fp,"%d %f %d \n",P,speedup,var);fflush(fp);
	printf("Speedup=%f with %d thread nean of %d with %d ec\n",speedup,P,T,var);fflush(stdout);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	free(operand1);
	free(operand2);
}
