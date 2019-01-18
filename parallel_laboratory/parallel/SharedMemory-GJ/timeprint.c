/*
	making the speedup for parallel calcul complete
*/
double timeprint(struct timeval t1,struct timeval t2,int T,long dim,FILE *fp,int thread)
{
double operand1,operand2,operand;
operand=0.0;
	operand1=(double)t1.tv_sec+((1e-6)*t1.tv_usec);
	operand2=(double)t2.tv_sec+((1e-6)*t2.tv_usec);
	operand=operand2-operand1;
	fprintf(fp,"%ld %lf %d %lf %d\n",dim,operand,thread,operand/T,T);
	fflush(fp);
	printf("%fs sum of %d executions with %ld equations and %d threads\n",operand,T,dim,thread);fflush(stdout);
	return operand;
}
