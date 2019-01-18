/*
	This is the function which made the solving  of system of liner equations
	with gauss siedel in MPI implementation with last computing the static zone.
*/
void gs_MPI(double **mat,double *ty,double *tx,long variable,double err)
{
double *line;
double *x;
int rank,size;
long num,last;
long l,i,j,k,m;
struct { double x;long pos; int rank;} send;
double temp;
MPI_Datatype ssend;
int blocklens[3];
MPI_Aint indices[3];
MPI_Datatype old_types[3];
long nr,partition;
double **matrix,*pmat,*px;
long *ciclique;
double *x_1;
double stop;
double q,max;
long lastrow;
double *qi;
MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	nr=variable%size;
	partition=(variable-nr)/size;
	if(rank<nr) partition++;
	matrix=(double **)calloc(partition,sizeof(double *));
	pmat=(double *)calloc((variable+1)*partition,sizeof(double));
	for(i=0;i<partition;i++)
	{
		matrix[i]=pmat;
		pmat+=(variable+1);
	}
	ciclique=(long *)calloc(partition,sizeof(long));
	line=(double *)calloc(variable+1,sizeof(double));
	x_1=(double *)calloc(variable,sizeof(double));
//make data distribution
if(rank==0)
{
	j=0;
	if(rank<nr) lastrow=partition-1;
	else lastrow=partition;
	for(l=0;l<lastrow;l++)
	{
		memcpy(matrix[l],mat[j],sizeof(double)*variable);
		matrix[l][variable]=ty[j];
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
  		memcpy(matrix[lastrow],mat[j],sizeof(double)*variable);
		matrix[lastrow][variable]=ty[j];
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
	x=tx;
}
else
{
	px=(double *)calloc(variable,sizeof(double));
	for(i=0;i<partition;i++)
	{
		MPI_Recv(matrix[i],variable,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&matrix[i][variable],1,MPI_DOUBLE,0,1,MPI_COMM_WORLD,&status);
		MPI_Recv(&ciclique[i],1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
	}
	x=px;
}
	free(line);
	//create the send structure
	blocklens[0]=1;
	blocklens[1]=1;
	blocklens[2]=1;
	old_types[0]=MPI_DOUBLE;
	old_types[1]=MPI_LONG;
	old_types[2]=MPI_INT;
	MPI_Address(&send.x,&indices[0]);
	MPI_Address(&send.pos,&indices[1]);
	MPI_Address(&send.rank,&indices[2]);
	indices[2]-=indices[0];
	indices[1]-=indices[0];
	indices[0]=0;
	MPI_Type_struct(3,blocklens,indices,old_types,&ssend);
	MPI_Type_commit(&ssend);
	for(i=0;i<variable;i++)
	{
		x_1[i]=0.0;
		x[i]=0.0;
	}
	//make gauss siedel
	if(rank==0)
	{
		qi=(double *)calloc(variable,sizeof(double));
		for(i=0;i<variable;i++) qi[i]=0.0;
		for(i=0;i<variable;i++)
		{
			temp=0.0;
			for(j=i+1;j<variable;j++)
				temp+=fabs(mat[i][j]/mat[i][i]);
			for(j=0;j<i;j++)
				temp+=fabs(mat[i][j]/mat[i][i])*qi[j];
			qi[i]=temp;
		}
		q=qi[0];
		for(i=1;i<variable;i++) if(q<qi[i]) q=qi[i];
		free(qi);
		if(q>1)
		{
			printf("We can not solve this system\n");
			return;
		}
	}
	MPI_Bcast(&q,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	stop=1.0;
	while(fabs(stop)>err)
	{
		//first we compute the static zone
		last=0;
		for(i=0;i<partition;i++)
			x[ciclique[i]]=matrix[i][variable];		
		for(i=ciclique[0];i<variable;i++)
			x[ciclique[0]]+=matrix[0][i]*x_1[i];
		//we deal with rest of the rows
		for(j=0;j<partition;j++)
		{
			for(i=last;i<ciclique[j];i++)
			{
				nr=rank-1;
				if(nr<0) nr=size-1;
				MPI_Recv(&send,1,ssend,nr,0,MPI_COMM_WORLD,&status);
				x[send.pos]=send.x;
				nr=rank+1;
				if(nr>size-1) nr=0;
				if(nr!=send.rank)
					MPI_Send(&send,1,ssend,nr,0,MPI_COMM_WORLD);
				for(k=j;k<partition;k++)
					x[ciclique[k]]+=matrix[k][send.pos]*send.x;
			}
			//compute the static zone
			for(i=ciclique[j];i<variable;i++)
				x[ciclique[j]]+=matrix[j][i]*x_1[i];
			send.x=x[ciclique[j]];
			send.pos=ciclique[j];
			send.rank=rank;
			nr=rank+1;
			if(nr>size-1) nr=0;
			MPI_Send(&send,1,ssend,nr,0,MPI_COMM_WORLD);	
			for(k=j+1;k<partition;k++)
				x[ciclique[k]]+=matrix[k][send.pos]*send.x;
			last=ciclique[j]+1;
			if(last>variable-1) last=variable-1;
		}
		if(ciclique[partition-1]!=variable-1)
		{
			for(i=ciclique[partition-1]+1;i<variable;i++)
			{
				nr=rank-1;
				if(nr<0) nr=size-1;
				MPI_Recv(&send,1,ssend,nr,0,MPI_COMM_WORLD,&status);
				nr=rank+1;
				if(nr>size-1) nr=0;
				if(nr!=send.rank)
					MPI_Send(&send,1,ssend,nr,0,MPI_COMM_WORLD);				
				x[send.pos]=send.x;
			}
		}
		max=fabs(x[ciclique[0]]-x_1[ciclique[0]]);
		for(i=1;i<partition;i++)
		{
			temp=fabs(x[ciclique[i]]-x_1[ciclique[i]]);
			if(max<temp) max=temp;
		}
		temp=max;
		MPI_Reduce(&temp,&max,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
		MPI_Bcast(&max,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
		memcpy(x_1,x,variable*sizeof(double));
		stop=q*max/(1-q);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank!=0)	free(px);
	MPI_Type_free(&ssend);
	free(*matrix);
	free(matrix);
	free(x_1);
	free(ciclique);
}