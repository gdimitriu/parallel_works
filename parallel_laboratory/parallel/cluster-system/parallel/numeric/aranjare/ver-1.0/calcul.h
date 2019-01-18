/*
	function who make calculation for extrem 
*/
void calculez(int P,int N,double *output,int *positionout,int type)
{
int *start;			/*	point of start */
int *stop;			/* point of stop */
int *nr;				/* nr of elements per thread */
double *extreme;
int *position;
int i,j;				/* counters */
start=(int *)calloc(P,sizeof(int));
stop=(int *)calloc(P,sizeof(int));
nr=(int *)calloc(P,sizeof(int));
extreme=(double *)calloc(P,sizeof(double));
position=(int *)calloc(P,sizeof(int));
  	/*
  		Make setings for partitioning
  	*/
for(i=0;i<P;i++)
{
	nr[i]=N%P;
	if((i!=(P-1))&&(i!=0))
	{
		start[i]=i*nr[i];
		stop[i]=start[i]+nr[i];
	}
	if(i==0)
	{	
		start[i]=0;
		stop[i]=start[i]+nr[i];
	}
	if(i==(P-1))
	{
		start[i]=i*nr[i];
		stop[i]=N;
		nr[i]=stop[i]-start[i]+1;
	}
	extreme[i]=vector[start[i]];
	position[i]=start[i];
	for(j=start[i];j<stop[i];j++)
	{
		if(vector[j]<extreme[i])
		{
			if(type==0)
			{
				extreme[i]=vector[j];
				position[i]=j;
			}
		}
		else
		{
			if(type==1)
			{
				extreme[i]=vector[j];
				position[i]=j;
			}
		}
	}
}
	*output=extreme[0];
	*positionout=position[0];
	for(i=0;i<P;i++)
	{
		if(extreme[i]<(*output))
		{
			if(type==0)
			{
				*output=extreme[i];
				*positionout=position[i];
			}
		}
		else
		{
			if(type==1)
			{
				*output=extreme[i];
				*positionout=position[i];
			}
		}
	}
	free(nr);
	free(start);
	free(stop);
	free(extreme);
	free(position);
}