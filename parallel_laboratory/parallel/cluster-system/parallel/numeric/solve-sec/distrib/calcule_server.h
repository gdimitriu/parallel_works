/*
	Hederul pentru calcule in server
*/
/*
	Functia de testare host
*/
int test_host()
{
int test1_index;
	if(host==host_nr) test1_index=0;
	else test1_index=host+1;
	if(masina==test1_index) return 0;
	else return 1;
}
/*
	Functia de schimbare a unei linii
*/
int schimb_linie_intern(int r,int schimb)
{
float temporar1;
int i,j;
	if(r<(nr_linii-1))
	for(i=(r+1);i<nr_linii;i++)
	{
		if((data[i][schimb]>1e-10)||(data[i][schimb]<(-1e-10)))
		{
			for(j=0;j<(variabila+1);j++)
			{
				temporar1=data[r][j];
				data[r][j]=data[i][j];
				data[i][j]=temporar1;
			}
			return 1;
		}
	}
	return 0;
}
/* 
	Functia care realizeaza eliminarea gaussiana 
*/
void gaussian_elimination()
{
int i,j,k=0;
int temp,test1;
   time_2_wait.tv_sec=1;
   FD_ZERO(&read_fd);
   FD_ZERO(&test1_fd);
   FD_SET(server_socket_dw,&read_fd);
   k=0;
   if((primul=='0')&&(k==0))
   {
   	for(j=vector[k]+1;j<(variabila+1);j++) data[k][j]=data[k][j]/data[k][vector[k]];
   	data[k][vector[k]]=1;
   	//trimit care linie este
   	send(client_socket_up,&vector[k],1,0);
   	//trimit ce masina a trimis
   	send(client_socket_up,&host,1,0);
   	write(client_socket_up,data[k],(variabila+1)*sizeof(float));
   	if(nr_linii>1)
	   for(i=(k+1);i<nr_linii;i++)
		{
			for(j=(vector[k]+1);j<(variabila+1);j++) 
				data[i][j]=data[i][j]-data[i][vector[k]]*data[k][j];
			data[i][vector[k]]=0;
		}
	}
	last=0;
   while(k<nr_linii)
   {
   	if((primul!='0')||(k!=0)) 
	   {
   		flag=0;
	   	temp=0;
	   	while(flag==0)
			{
	   		memcpy(&test1_fd,&read_fd,sizeof(read_fd));
	   		select(server_socket_dw+1,&test1_fd,(fd_set *)NULL,(fd_set *)NULL,&time_2_wait);
			   if(FD_ISSET(server_socket_dw,&test1_fd))
   			{
   				//receptionez ce linie este
   				linie=0;
			   	recv(server_socket_dw,&linie,1,0);
			   	//receptionez masina care a trimis
			   	recv(server_socket_dw,&masina,1,0);
			   	//receptionez linia
			   	for(j=0;j<(variabila+1);j++) temporar[j]=0;
	   			read(server_socket_dw,temporar,(variabila+1)*sizeof(float));
		   		//trimit mai departe daca nu este ultima ecuatie
					if(test_host()==1)
					{
	   				if((ec_ultim==1)&&(k==(nr_linii-1))) ;
	   				else
		   			{
		   				//trimit linia
					   	send(client_socket_up,&linie,1,0);
					   	//trimit indexul masinii
					   	send(client_socket_up,&masina,1,0);
				   		write(client_socket_up,temporar,(variabila+1)*sizeof(float));
	   				}
	   			}
	   			//facem elimiarile
		   		for(i=k;i<nr_linii;i++)
		   		{
			   		for(j=(linie+1);j<(variabila+1);j++) 
		   				data[i][j]=data[i][j]-data[i][linie]*temporar[j];
		   			data[i][linie]=0;
		   		}
		   		temp++;
		   		test1=vector[k]-last;
		   		if(test1>host_nr) test1=host_nr;
		   		if(temp==test1) flag=1;
			   }	
			}
			//facem diviziunile
			if((data[k][vector[k]]<1e-10)&&(data[k][vector[k]]>(-1e-10))) 
				schimb_linie_intern(k,vector[k]);
	  		for(j=(vector[k]+1);j<(variabila+1);j++) data[k][j]=data[k][j]/data[k][vector[k]];
   		data[k][vector[k]]=1;
   		//trimit linia daca nu este ultima ecuatie
			if((ec_ultim==1)&&(k==(nr_linii-1)));
			else
		   {
  				//trimit care linie este
   			send(client_socket_up,&vector[k],1,0);
   			//trimit indexul masinii
	   		send(client_socket_up,&host,1,0);
			  	write(client_socket_up,data[k],(variabila+1)*sizeof(float));
			}
   		if(nr_linii>1)
	   	for(i=(k+1);i<nr_linii;i++)
			{
				for(j=(vector[k]+1);j<(variabila+1);j++) 
					data[i][j]=data[i][j]-data[i][vector[k]]*data[k][j];
				data[i][vector[k]]=0;
			}
		}
		last=vector[k];
		k++;
	}
	k=0;
}

/* 
	Functia care realizeaza back-substitution 
*/
int back_substitution()
{
int i,j;
	i=nr_linii-1;
	if((ec_ultim==1)&&(i==(nr_linii-1)))
	{
		//incepe substitutia, primul pas
		rezultate[variabila-1]=data[i][variabila];
		send(server_socket_dw,&vector[i],1,0);
		write(server_socket_dw,&rezultate[variabila-1],sizeof(float));
		i--;
	}
	for(j=(vector[i]+1);j<variabila;j++)
	{
		linie=0;
		recv(client_socket_up,&linie,1,0);
		read(client_socket_up,&rezultate[j],sizeof(float));
	}
  	rezultate[vector[i]]=data[i][variabila];
  	for(j=(variabila-1);j>vector[i];j--)
  		rezultate[vector[i]]-=data[i][j]*rezultate[j];
   if((primul=='0')&&(i==0)) return 0;
   else
   {
		for(j=vector[i];j<variabila;j++)
		{
 			send(server_socket_dw,&j,1,0);
 			write(server_socket_dw,&rezultate[j],sizeof(float));
		}
		i--;
	}
	while(i>=0)
	{
		for(j=vector[i+1];j>vector[i];j--)
		{
			recv(client_socket_up,&linie,1,0);
			read(client_socket_up,&rezultate[linie],sizeof(float));
		}
     	rezultate[vector[i]]=data[i][variabila];
	  	for(j=(variabila-1);j>vector[i];j--)
	   	rezultate[vector[i]]-=data[i][j]*rezultate[j];
	   if((primul=='0')&&(i==0)) break;
	   else
	   {
			for(j=vector[i];j<(vector[i+1]+1);j++)
			{
	 			send(server_socket_dw,&j,1,0);
	 			write(server_socket_dw,&rezultate[j],sizeof(float));
			}
			i--;
		}
	}
	return 0;
}
