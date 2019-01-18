	if(rank==0)
	{
		for(i=1;i<size;i++)
		{
			nr=variable%size;
			partition=(variable-nr)/size;
			if(i<=nr) partition++;
			MPI_Recv(&ligne[0],partition,MPI_DOUBLE,i,1,MPI_COMM_WORLD,&status);
			k=0;
			for(j=i;j<variable;j=j+size)
			{
				resoudre[j]=ligne[k];
				k++;
			}
		}
	}
	else MPI_Send(&resoudre[0],partition,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
