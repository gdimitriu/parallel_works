/*
	This is the function which solves a triangular system of linear equations
*/
void gauss_MPI(double **mat,double *ty,double *tx,int variable)
{
double *ligne;
double *resoudre;
int rank,size;
int num,ete;
int l;
int i,j,k,m;
int temp[2];
int nr,partition;
double **augumente,*paugumente,*px;
int *ciclique;
int lastrow;
MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	nr=variable%size;
	partition=(variable-nr)/size;
	if(rank<nr) partition++;
	augumente=(double **)calloc(partition,sizeof(double *));
	paugumente=(double *)calloc((variable+1)*partition,sizeof(double));
	for(i=0;i<partition;i++)
	{
		augumente[i]=paugumente;
		paugumente+=(variable+1);
	}
	ciclique=(int *)calloc(partition,sizeof(int));
	ligne=(double *)calloc(variable+1,sizeof(double));
//make data distribution
if(rank==0)
{
	j=0;
	if(rank<nr) lastrow=partition-1;
	else lastrow=partition;
	for(l=0;l<lastrow;l++)
	{
		memcpy(augumente[l],mat[j],sizeof(double)*variable);
		augumente[l][variable]=ty[j];
		ciclique[l]=j;
		j++;
		for(i=1;i<size;i++)
  		{
  			MPI_Send(mat[j],variable,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
  			MPI_Send(&ty[j],1,MPI_DOUBLE,i,1,MPI_COMM_WORLD);
  			MPI_Send(&j,1,MPI_INT,i,2,MPI_COMM_WORLD);
	   	j++;
  		}
  	}
  	if(nr>0)
  	{
  		memcpy(augumente[lastrow],mat[j],sizeof(double)*variable);
		augumente[lastrow][variable]=ty[j];
		ciclique[lastrow]=j;
		j++;
	}
  	for(l=1;l<nr;l++)
  	{
		MPI_Send(mat[j],variable,MPI_DOUBLE,l,0,MPI_COMM_WORLD);
		MPI_Send(&ty[j],1,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
		MPI_Send(&j,1,MPI_INT,l,2,MPI_COMM_WORLD);
 		j++;
   }
   //send the last flag (which processor have the last equation)	   
   if(nr==1)
   {
   	lastrow=0;
   	for(i=1;i<size;i++) MPI_Send(&lastrow,1,MPI_INT,i,3,MPI_COMM_WORLD);
   	lastrow=1;
   }
   else
   {
   	if(nr==0)
   	{
   		lastrow=1;
   		MPI_Send(&lastrow,1,MPI_INT,(size-1),3,MPI_COMM_WORLD);
   		lastrow=0;
   		for(i=1;i<(size-1);i++) MPI_Send(&lastrow,1,MPI_INT,i,3,MPI_COMM_WORLD);
   	}
   	else
   	{
   		for(i=1;i<size;i++)
   		{
   			if(i==(nr-1)) 
   			{
   				lastrow=1;
   				MPI_Send(&lastrow,1,MPI_INT,i,3,MPI_COMM_WORLD);
   			}
   			else
   			{
   				lastrow=0;
		   		MPI_Send(&lastrow,1,MPI_INT,i,3,MPI_COMM_WORLD);
				}
			}
			lastrow=0;	
		}
	}
	resoudre=tx;
}
else
{
	px=(double *)calloc(partition,sizeof(double));
	for(i=0;i<partition;i++)
	{
		MPI_Recv(augumente[i],variable,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&augumente[i][variable],1,MPI_DOUBLE,0,1,MPI_COMM_WORLD,&status);
		MPI_Recv(&ciclique[i],1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
	}
	MPI_Recv(&lastrow,1,MPI_INT,0,3,MPI_COMM_WORLD,&status);
	resoudre=px;
}	
	//solve triangular system
	if(lastrow==1) ete=ciclique[partition-1]-1;
	else ete=variable-1;
	for(i=partition-1;i>=0;i--)
	{
		for(k=0;k<(ete-ciclique[i]);k++)
		{
			l=rank+1;
			if(l==size) l=0;
			MPI_Recv(temp,2,MPI_INT,l,0,MPI_COMM_WORLD,&status);
			MPI_Recv(&ligne[0],1,MPI_DOUBLE,l,1,MPI_COMM_WORLD,&status);
			l=rank-1;
			if(l<0) l=size-1;
			if((l!=temp[1])&&!((rank==0)&&(i==0)))
			{
				MPI_Send(temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
				MPI_Send(&ligne[0],1,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
			}
			if(rank==0) resoudre[temp[0]]=ligne[0];
			for(m=0;m<i+1;m++)
				if(rank==0) resoudre[ciclique[m]]=resoudre[ciclique[m]]-augumente[m][temp[0]]*ligne[0];
				else resoudre[m]=resoudre[m]-augumente[m][temp[0]]*ligne[0];
		}
		if(!((i==0)&&(rank==0)))
		{
			l=rank-1;
			if(l<0) l=size-1;
			if(rank==0) ligne[0]=resoudre[ciclique[i]];
			else ligne[0]=resoudre[i];
			temp[0]=ciclique[i];
			temp[1]=rank;
			MPI_Send(temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
			MPI_Send(&ligne[0],1,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
			for(m=0;m<i;m++)
				if(rank==0) resoudre[ciclique[m]]=resoudre[ciclique[m]]-augumente[m][temp[0]]*ligne[0];
				else resoudre[m]=resoudre[m]-augumente[m][temp[0]]*ligne[0];
			ete=ciclique[i]-1;
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank!=0)	free(px);
	free(*augumente);
	free(augumente);
	free(ciclique);
	free(ligne);
}