/* 
	file for function of extrem in MPI implementation
*/
#include<stdio.h>
#include<parallel/parallel-mpi-pth.h>

int extreme_link_mpi(int dim,double *vector,double *output,int *position,int type)
{
int thread;		/* le numeros des processors */
int numero;		/* le numero de processor */
int start;		/* le point de partir	 */
int stop;		/* le point de arrete */
int nr;			/* le numero des elements dans le thread */
int i;			/* le contor */
struct
{
	int pos;
	double val;
} data,temp;
MPI_Datatype datastruct1;	/* la nouvele structure */
int blocklens[2];				/* la longeurs de dones */
MPI_Aint indices[2];			/* l'adresse des dones */
MPI_Datatype type_old[2];			
	MPI_Comm_size(MPI_COMM_WORLD,&thread);
	MPI_Comm_rank(MPI_COMM_WORLD,&numero);
	numero++;
	thread++;
	/* initialize la structure de comunication */
	blocklens[0]=1;blocklens[1]=1;
	type_old[0]=MPI_INT;type_old[1]=MPI_DOUBLE;
	/*location des elements */
	MPI_Address(&data.pos,&indices[0]);
	MPI_Address(&data.val,&indices[1]);
	/* fait relatives */
	indices[1]-=indices[0];
	indices[0]=0;
	MPI_Type_struct(2,blocklens,indices,type_old,&datastruct1);
	MPI_Type_commit(&datastruct1);
   /* calcule le constants de partagement */
	nr=dim%thread;
	if((numero!=(thread-1))&&(numero!=0))
	{
		start=numero*nr;
		stop=start+nr;
	}
	if(numero==0)
	{
		start=0;
		stop=start+nr;
	}
	if(numero==(thread-1))
	{
		start=numero*nr;
		stop=dim;
		nr=stop-start+1;
	}
	*output=vector[start];
	*position=start;
	/* zone pour le extrem (zone parallele) */
	for(i=start;i<stop;i++)
	{
		if(vector[i]<*output)
		{
			if(type==0)
			{
				*output=vector[i];
				*position=i;
			}
		}
		else
		{
			if(type==1)
			{
				*output=vector[i];
				*position=i;
			}
		}
	}
	data.val=*output;
	data.pos=*position;
	/* zone pour le extrem (zone seriale) */
	for(i=1;i<size;i++)
	{
		temp=data;
		MPI_Bcast(&temp,1,datastruct1,i,MPI_COMM_WORLD);
		if(temp.val<data.val)
		{
			if(type==0)
			{
				data.val=temp.val;
				data.pos=temp.pos;
			}
		}
		else
		{
			if(type==1)
			{
				data.val=temp.val;
				data.pos=temp.pos;
			}
		}
	}
	*output=data.val;
	*position=data.pos;
	MPI_Type_free(&datastruct1);
	return(0);
}