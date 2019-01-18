/*
	function who make calculation for extrem 
*/
void calculez(int nr,double *output,int *position,int type)
{
int i;
(*output)=vector[0];
(*position)=0;
for(i=1;i<nr;i++)
	if(vector[i]<(*output))
	{
		if(type==0)
		{
			(*position)=i;
			(*output)=vector[i];
		}
	}
	else
	{
		if(type==1)
		{
			(*position)=i;
			(*output)=vector[i];
		}
	}
}