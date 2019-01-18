/*
	The function who find the inverse for a square matrix with Gauss Jordan.
	Implementation in clasic MPI.
*/
void gj_inv_MPI(double **mat,double **tinv,int variable)
{
double *row,*row1;
double **inv;
int rank,size;
int num,ete;
int i,j,k,m,l;
int temp[2];
int nr,partition,counter;
double **augumente,*paugumente;
double dtemp;
int *ciclique;
int lastrow;
MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	nr=variable%size;
	partition=(variable-nr)/size;
	if(rank<nr) partition++;
	augumente=(double **)calloc(partition,sizeof(double *));
	paugumente=(double *)calloc(variable*partition,sizeof(double));
	for(i=0;i<partition;i++)
	{
		augumente[i]=paugumente;
		paugumente+=variable;
	}

	ciclique=(int *)calloc(partition,sizeof(int));
	row=(double *)calloc(variable,sizeof(double));
	row1=(double *)calloc(variable,sizeof(double));
//make data distribution
if(rank==0)
{
	j=0;
	if(rank<nr) lastrow=partition-1;
	else lastrow=partition;
	for(l=0;l<lastrow;l++)
	{
		memcpy(augumente[l],mat[j],sizeof(double)*variable);
		ciclique[l]=j;
		j++;
		for(i=1;i<size;i++)
  		{
  			MPI_Send(mat[j],variable,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
  			MPI_Send(&j,1,MPI_INT,i,2,MPI_COMM_WORLD);
	   	j++;
  		}
  	}
  	if(nr>0)
  	{
  		memcpy(augumente[lastrow],mat[j],sizeof(double)*variable);
		ciclique[lastrow]=j;
		j++;
	}
  	for(l=1;l<nr;l++)
  	{
		MPI_Send(mat[j],variable,MPI_DOUBLE,l,0,MPI_COMM_WORLD);
		MPI_Send(&j,1,MPI_INT,l,2,MPI_COMM_WORLD);
 		j++;
   }
   inv=tinv;
}
else
{
	inv=(double **)calloc(partition,sizeof(double *));
	paugumente=(double *)calloc(variable*partition,sizeof(double));
	for(i=0;i<partition;i++)
	{
		inv[i]=paugumente;
		paugumente+=variable;
	}
	for(i=0;i<partition;i++)
	{
		MPI_Recv(augumente[i],variable,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&ciclique[i],1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
	}
}
	
	//make inverse with Gauss Jordan
	for(i=0;i<partition;i++)
	{
		if(rank!=0) inv[i][ciclique[i]]=1.0;
		else inv[ciclique[i]][ciclique[i]]=1.0;
	}
	ete=0;
	for(i=0;i<=partition;i++)
	{
		if(i!=partition) counter=ciclique[i];
		else counter=variable;
		for(k=ete;k<counter;k++)
		{
			l=rank-1;
			if(l<0) l=size-1;
			MPI_Recv(temp,2,MPI_INT,l,0,MPI_COMM_WORLD,&status);
			MPI_Recv(&row[0],variable,MPI_DOUBLE,l,1,MPI_COMM_WORLD,&status);
			MPI_Recv(&row1[0],variable,MPI_DOUBLE,l,2,MPI_COMM_WORLD,&status);
			l=rank+1;
			if(l==size) l=0;
			if(l!=temp[1])
			{
				
				MPI_Send(temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
				MPI_Send(&row[0],variable,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
				MPI_Send(&row1[0],variable,MPI_DOUBLE,l,2,MPI_COMM_WORLD);
			}
			//make operations for this machine
			for(m=0;m<partition;m++)
			{
				if(ciclique[m]!=temp[0])
				{
					dtemp=augumente[m][temp[0]]/row[temp[0]];
					for(j=temp[0]+1;j<variable;j++) 
						augumente[m][j]-=row[j]*dtemp;
					
						for(j=0;j<variable;j++)
							if(rank!=0)
								inv[m][j]-=row1[j]*dtemp;
							else //local
								inv[ciclique[m]][j]-=row1[j]*dtemp;
				}
			}
		}
		if(i!=partition)
		{
			//send the line
			l=rank+1;
			if(l==size) l=0;
			memcpy(row,&augumente[i][0],variable*sizeof(double));
			if(rank!=0)
				memcpy(row1,&inv[i][0],variable*sizeof(double));
			else
				memcpy(row1,&inv[ciclique[i]][0],variable*sizeof(double));
			temp[0]=ciclique[i];
			temp[1]=rank;
			MPI_Send(temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
			MPI_Send(&row[0],variable,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
			MPI_Send(&row1[0],variable,MPI_DOUBLE,l,2,MPI_COMM_WORLD);
			//make again the operations for this machine
			for(m=0;m<partition;m++)
			{
				if(ciclique[m]!=temp[0])
				{
					dtemp=augumente[m][temp[0]]/row[temp[0]];
					for(j=temp[0]+1;j<variable;j++)
						augumente[m][j]-=row[j]*dtemp;
					
					for(j=0;j<variable;j++)
						if(rank!=0)
							inv[m][j]-=row1[j]*dtemp;
						else //local
							inv[ciclique[m]][j]-=row1[j]*dtemp;
				}
				else
				{
					for(j=0;j<variable;j++)
					{
						if(rank!=0)
							inv[m][j]=inv[m][j]/augumente[m][ciclique[m]];
						else //local
							inv[ciclique[m]][j]=inv[ciclique[m]][j]/augumente[m][ciclique[m]];
						if(j!=ciclique[m])
							augumente[m][j]=augumente[m][j]/augumente[m][ciclique[m]];
					}
					augumente[m][ciclique[m]]=1.0;
				}
			}
			ete=ciclique[i]+1;
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	//make inverse at 0 machine
	if(rank==0)
	{
		j=0;
		for(i=0;i<variable;i++)
		{
			if(j!=0)
				MPI_Recv(&inv[i][0],variable,MPI_DOUBLE,j,j,MPI_COMM_WORLD,&status);
			j=(j+1)%size;
		}
	}
	else
	{
		for(i=0;i<partition;i++)
			MPI_Send(&inv[i][0],variable,MPI_DOUBLE,0,rank,MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank!=0)
	{
		free(*inv);
		free(inv);
	}
	free(*augumente);
	free(augumente);
	free(ciclique);
	free(row);
	free(row1);
}