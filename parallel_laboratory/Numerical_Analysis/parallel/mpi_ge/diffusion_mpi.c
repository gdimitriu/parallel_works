/*
	le dossier pour diffusion de done dans le striped et ciclique patition dans
	implementation de MPI
*/
int diffusion_gauss_sc_mpi(int rank,int size,double **mat,double *ty,double **augumente,int *ciclique,int variable)
/*
	rank c'est le rank de processor
	size c'est le numero des processerus
	mat c'est la matrice complete dans le process principale
	ty c'est le vector libre dans le process principale
	augumente c'est la matrice partitione dans le client
	ciclique c'est le vector qui retien le numero de la ligne qui sont dans cette matrice
	variable c'est le numero de variable dans le system
*/
{
int nr,partition;
MPI_Status status;
int i,j,l,m;
int temp=0;
   nr=variable%size;
   partition=(variable-nr)/size;
if(rank==0)
{
		j=0;
		m=0;
		for(l=0;l<partition;l++)
		{
			memcpy(augumente[m],mat[j],sizeof(double)*variable);
			augumente[l][variable]=ty[j];
			ciclique[l]=j;
			j++;m++;
			for(i=1;i<size;i++)
   		{
   			MPI_Send(mat[j],variable,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
   			MPI_Send(&ty[j],1,MPI_DOUBLE,i,1,MPI_COMM_WORLD);
   			MPI_Send(&j,1,MPI_INT,i,2,MPI_COMM_WORLD);
		   	j++;
   		}
   	}
	 	m=partition;
   	if(nr>0)
   	{
   		memcpy(augumente[m],mat[j],sizeof(double)*variable);
			augumente[m][variable]=ty[j];
			ciclique[m]=j;
			j++;
		}
   	for(l=1;l<nr;l++)
   	{
			MPI_Send(mat[j],variable,MPI_DOUBLE,l,0,MPI_COMM_WORLD);
  			MPI_Send(&ty[j],1,MPI_DOUBLE,l,1,MPI_COMM_WORLD);
  			MPI_Send(&j,1,MPI_INT,l,2,MPI_COMM_WORLD);
   		j++;
	   }	   
	   if(nr==1)
	   {
	   	temp=0;
	   	for(i=1;i<size;i++)
	   		MPI_Send(&temp,1,MPI_INT,i,3,MPI_COMM_WORLD);
	   	return 1;
	   }
	   else if(nr==0)
	   		{
	   			temp=0;
	   			for(i=1;i<(size-1);i++)
	   				MPI_Send(&temp,1,MPI_INT,i,3,MPI_COMM_WORLD);
	   			temp=1;
	   			MPI_Send(&temp,1,MPI_INT,(size-1),3,MPI_COMM_WORLD);
	   			return 0;	
	   		}
	   else
	   for(i=1;i<size;i++)
	   {
	   	if(i==(nr-1)) 
	   	{
	   		temp=1;
	   		MPI_Send(&temp,1,MPI_INT,i,3,MPI_COMM_WORLD);
	   	}
	   	else
	   	{
	   		temp=0;
			   MPI_Send(&temp,1,MPI_INT,i,3,MPI_COMM_WORLD);
			}
		}
	   return temp;
}
else
	{
		if(rank<nr) partition++;
		for(i=0;i<partition;i++)
		{
			MPI_Recv(augumente[i],variable,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
			MPI_Recv(&augumente[i][variable],1,MPI_DOUBLE,0,1,MPI_COMM_WORLD,&status);
			MPI_Recv(&ciclique[i],1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
		}
		MPI_Recv(&temp,1,MPI_INT,0,3,MPI_COMM_WORLD,&status);
		return temp;
	}
}	