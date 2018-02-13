/*
	Copyright 2006 Gabriel Dimitriu

	This file is part of scientific_computing.

    scientific_computing is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    scientific_computing is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with scientific_computing; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
*/
/*
	la function pour faire elimation gaussiene dans le reseau ciclique avec
	distribution striped dans MPI clasique
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
	
	//make gaussian elimination
	ete=0;
	for(i=0;i<partition;i++)
	{
		for(k=ete;k<ciclique[i];k++)
		{
			l=rank-1;
			if(l<0) l=size-1;
			MPI_Recv(temp,2,MPI_INT,l,0,MPI_COMM_WORLD,&status);
			MPI_Recv(&ligne[temp[0]],(variable-temp[0]+1),MPI_DOUBLE,l,1,MPI_COMM_WORLD,&status);
			l=rank+1;
			if(l==size) l=0;
			if((variable-temp[0])<size)
			{
				if(lastrow==0)
				{
//					printf("send1 %d  at %d from %d row %d\n",rank,l,temp[1],temp[0]);fflush(stdout);
					MPI_Send(temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
					MPI_Send(&ligne[temp[0]],(variable-temp[0]+1),MPI_DOUBLE,l,1,MPI_COMM_WORLD);
				}
			}
			else if(l!=temp[1])
			{
				
//				printf("send1 %d  at %d from %d row %d\n",rank,l,temp[1],temp[0]);fflush(stdout);
				MPI_Send(temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
				MPI_Send(&ligne[temp[0]],(variable-temp[0]+1),MPI_DOUBLE,l,1,MPI_COMM_WORLD);
			}
			//fait elimination pour les lignes qui sont dans le meme machine
			for(m=i;m<partition;m++)
			{
				for(j=temp[0]+1;j<variable+1;j++) augumente[m][j]-=augumente[m][temp[0]]*ligne[j];
				augumente[m][temp[0]]=0.0;
			}
		}
		//faite la division
		for(j=ciclique[i]+1;j<variable+1;j++) augumente[i][j]=augumente[i][j]/augumente[i][ciclique[i]];
		augumente[i][ciclique[i]]=1.0;
		if(rank==0)	resoudre[ciclique[i]]=augumente[i][variable];
		else	resoudre[i]=augumente[i][variable];
		if(!((i==(partition-1))&&(lastrow==1)))
		{
			l=rank+1;
			if(l==size) l=0;
			memcpy(ligne,augumente[i],(variable+1)*sizeof(double));
			temp[0]=ciclique[i];
			temp[1]=rank;
//			printf("send2 %d  at %d from %d row %d\n",rank,l,temp[1],temp[0]);fflush(stdout);
			MPI_Send(temp,2,MPI_INT,l,0,MPI_COMM_WORLD);
			MPI_Send(&ligne[temp[0]],(variable-temp[0]+1),MPI_DOUBLE,l,1,MPI_COMM_WORLD);
			//fait elimination pour les lignes qui sont dans le meme machine
			for(k=i+1;k<partition;k++)
			{
				for(j=ciclique[i]+1;j<variable+1;j++) augumente[k][j]-=augumente[k][ciclique[i]]*ligne[j];
				augumente[k][ciclique[i]]=0.0;
			}
			ete=ciclique[i]+1;
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	//make backsubstitution
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