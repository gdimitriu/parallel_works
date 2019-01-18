void verificare(void)
{
int i,j,k;
double temp;
	for(i=0;i<variabila;i++)
	{
		temp=0;
		for(j=0;j<variabila;j++) temp+=tmat[i][j]*tx[j];
		temp-=ty[i];
		if((temp<(-1e-15))||(temp>1e-15))
		{ 
			printf("Eroare la ecuatia %d err=%g\n",i,temp);
			fflush(stdout);
		}
	}	
}