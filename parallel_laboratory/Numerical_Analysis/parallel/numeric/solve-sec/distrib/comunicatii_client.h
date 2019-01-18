/*
	hederul pentru comunicatii client
*/

//functia de realizare a conexiunii in inel intre masinile implicate
int configurare_retea()
{
int temp,i;
	server_addr=(struct sockaddr_in *)calloc(RETEA,sizeof(struct sockaddr_in));
	for(i=0;i<RETEA;i++)	
	{
		memset((char *)&server_addr[i],0,sizeof(server_addr[i]));
		ready[i]=0;
	}
	hosts=readconfig();
	//se executa pentru toate serverele din configurare
	for(i=0;i<hosts;i++)
	{
		server_addr[i].sin_family=AF_INET;
		server_addr[i].sin_port=htons(adresa[i].port);
		server_addr[i].sin_addr.s_addr=inet_addr(adresa[i].IP);
		//creare socket
		if((client_socket[i]=socket(AF_INET,SOCK_STREAM,0))<0)
		{
			perror("error socket");
			printf(" for server %d",i);
			exit(2);
		}
		//conectare
		if(connect(client_socket[i],(struct sockaddr *)&server_addr[i],sizeof(server_addr[i]))<0)
		{
			perror("error connect");
			printf(" for server %d",i);
			close(client_socket[i]);
			exit(3);
		}
	}
	time_2_wait.tv_sec=2;
	FD_ZERO(&read_fd);
	FD_ZERO(&test1_fd);
	for(i=0;i<hosts;i++) FD_SET(client_socket[i],&read_fd);
		
	flag_iesire=0;
	while(flag_iesire==0)
	{
		memcpy(&test1_fd,&read_fd,sizeof(read_fd));
 		if(select(client_socket[hosts],&test1_fd,(fd_set *)NULL,(fd_set *)NULL,&time_2_wait)<0)
			perror("server socket select");
		for(i=0;i<hosts;i++)
		{
			if(FD_ISSET(client_socket[i],&test1_fd))
			{
				if(i==0)
				{
					//primesc cerere
					memset((char *)&buffer,0,sizeof(buffer));
					recv(client_socket[i],buffer,SIZE,0);
					//trimit cine este prima masina
					send(client_socket[i],zero,strlen(zero),0);
					//trimit porturile
					porturi[0]=PORT_circular;
					porturi[1]=PORT_circular-1;
					send(client_socket[i],porturi,sizeof(porturi),0);
					//trimit adresa IP urmatoare
					write(client_socket[i],adresa[i+1].IP,15*sizeof(char));
					memset((char *)&buffer,0,sizeof(buffer));
					recv(client_socket[i],buffer,SIZE,0);
					ready[i]=1;
					//scot din coada
					FD_CLR(client_socket[i],&read_fd);
				}else
				if(i==(hosts-1))
				{
					//receptionez cerere
					memset((char *)&buffer,0,sizeof(buffer));
					recv(client_socket[i],buffer,SIZE,0);
					//trimit cine este prima masina
					send(client_socket[i],&unu,strlen(unu),0);
					//trimit porturile
					porturi[0]=PORT_circular-1;
					porturi[1]=PORT_circular+i-1;
					send(client_socket[i],porturi,sizeof(porturi),0);
					//trimit adresa IP urmatoare
					write(client_socket[i],adresa[0].IP,15*sizeof(char));
					memset((char *)&buffer,0,sizeof(buffer));
					recv(client_socket[i],buffer,SIZE,0);
					ready[i]=1;
					//scot din coada
					FD_CLR(client_socket[i],&read_fd);
				}
				else
				{
					//receptionez cerere
					memset((char *)&buffer,0,sizeof(buffer));
					recv(client_socket[i],buffer,SIZE,0);
					//trimit cine este prima masina
					send(client_socket[i],unu,strlen(unu),0);
					//trimit porturile
					porturi[0]=PORT_circular+i;
					porturi[1]=PORT_circular+i-1;
					send(client_socket[i],porturi,sizeof(porturi),0);
					//trimit adresa IP urmatoare
					write(client_socket[i],adresa[i+1].IP,15*sizeof(char));
					memset((char *)&buffer,0,sizeof(buffer));
					recv(client_socket[i],buffer,SIZE,0);
					ready[i]=1;
					//scot din coada
					FD_CLR(client_socket[i],&read_fd);
				}
			}
		}
		temp=0;
		for(i=0;i<hosts;i++) temp=temp+ready[i];
		if(temp==hosts) flag_iesire=1;
	}
	//pornesc crearea inelului
	flag_iesire=0;
	for(i=1;i<hosts;i++)
	{
		//trimit initializarea serverului
		if(flag_iesire==0) 
		{
			send(client_socket[i],"START",5,0);	
			//receptionez pornirea serverului
			memset((char *)&buffer,0,sizeof(buffer));
			recv(client_socket[i],buffer,SIZE,0); 
			if(strcmp(buffer,mesaj_ack)<0)
			{
				write(1,buffer,sizeof(buffer));
				flag_iesire++;
			}
		}
		else send(client_socket[i],mesaj_exit,strlen(mesaj_exit),0);
	}
	if(flag_iesire==0) for(i=1;i<hosts;i++) send(client_socket[i],"START",5,0);
	if(flag_iesire!=0) for(i=0;i<hosts;i++) 
	{
		send(client_socket[i],mesaj_exit,strlen(mesaj_exit),0);
		return 1;
	}
	//trimit initalizare conexiunilor
	send(client_socket[0],"START",5,0);
	memset((char *)&buffer,0,sizeof(buffer));
	recv(client_socket[0],buffer,SIZE,0);
	if(strcmp(buffer,mesaj_ack)<0) return 1;
	return 0;
}

//functia de transmitere a liniilor
void send_data()
{
int temp,i,j;
	time_2_wait.tv_sec=1;
	//prima faza a transferului cind se trimit nr de variabile si de linii alocate
	FD_ZERO(&read_fd);
	FD_ZERO(&test1_fd);
	for(i=0;i<hosts;i++) FD_SET(client_socket[i],&read_fd);
	flag_iesire=0;
	for(i=0;i<hosts;i++) ready[i]=0;
	while(flag_iesire==0)
	{
		memcpy(&test1_fd,&read_fd,sizeof(read_fd));
 		if(select(client_socket[hosts],&test1_fd,(fd_set *)NULL,(fd_set *)NULL,&time_2_wait)<0)
			perror("server socket select");
		for(i=0;i<hosts;i++)
			if(FD_ISSET(client_socket[i],&test1_fd))
			{
				recv(client_socket[i],buffer,SIZE,0);
				//trimit numarul de necunoscute
				send(client_socket[i],&variabila,1,0);
				//trimit numarul de linii alocate masinii respective
				send(client_socket[i],&data[i].contor,1,0);
				//trimit care este ultima masina (ca ecuatii)
				send(client_socket[i],&data[i].ultim,1,0);
				//trimit indexul masinii
				send(client_socket[i],&i,1,0);
				//trimit numarul de hosturi
				send(client_socket[i],&hosts,1,0);
				FD_CLR(client_socket[i],&read_fd);
				ready[i]=1;
			}
		temp=0;
		for(i=0;i<hosts;i++) temp+=ready[i];
		if(temp==hosts) flag_iesire=1;
	}
	//a doua faza a transferului cind se transmit datele matricii
	FD_ZERO(&read_fd);
	FD_ZERO(&test1_fd);
	for(i=0;i<hosts;i++) FD_SET(client_socket[i],&read_fd);
	flag_iesire=0;
	for(i=0;i<hosts;i++) ready[i]=0;
	while(flag_iesire==0)
	{
		memcpy(&test1_fd,&read_fd,sizeof(read_fd));
 		if(select(client_socket[hosts],&test1_fd,(fd_set *)NULL,(fd_set *)NULL,&time_2_wait)<0)
			perror("server socket select");
		for(i=0;i<hosts;i++)
			if(FD_ISSET(client_socket[i],&test1_fd))
			{
				recv(client_socket[i],buffer,SIZE,0);
				//trimit liniile alocate masinii
				for(j=0;j<data[i].contor;j++)
					write(client_socket[i],data[i].mat[j],(variabila+1)*sizeof(float));
				write(client_socket[i],data[i].vector,(data[i].contor)*sizeof(int));
				FD_CLR(client_socket[i],&read_fd);
				ready[i]=1;
			}
		temp=0;
		for(i=0;i<hosts;i++) temp+=ready[i];
		if(temp==hosts) flag_iesire=1;
	}
}

//receptionez rezultatele calculelor
void recv_data()
{
char nume[128];
	rezultate=(float *)calloc(variabila,sizeof(float));
	recv(client_socket[0],buffer,SIZE,0);
	//receptionez rezultatele sistemului
	read(client_socket[0],rezultate,variabila*sizeof(float));
	retray1:	printf("Doriti printare in fisier dD/nN ");
	gets(nume);
	if((nume[0]=='n')||(nume[0]=='N'))
	for(i=0;i<variabila;i++) printf("X%d=%f\n",i,rezultate[i]);
	else if((nume[0]=='d')||(nume[0]=='D'))
	{
		i=0;
		retray:	write(1,"Indroduceti numele fisierului ",30*sizeof(char));
		gets(nume);
		if((fconfig=(FILE *)fopen(nume,"w"))==(FILE *)NULL)
		{
			perror("error open file");
		   i++;
		   if(i<6) goto retray;
		}
		for(i=0;i<variabila;i++) fprintf(fconfig,"X%d=%f\n",i,rezultate[i]);
		fclose(fconfig);
	}
	else goto retray1;
}
	
//protocol de inchidere
int inchid_sistem()
{
int temp;
char test[10];
	retry1: for(i=0;i<10;i++) test[i]=0;
	write(1,"Doriti reintroducere ecuatii dD/nN",34*sizeof(char));
	gets(test);
	if((test[0]=='d')||(test[0]=='D')||(test[0]=='N')||(test[0]=='n')) ;
	else goto retry1;
	time_2_wait.tv_sec=1;
	FD_ZERO(&read_fd);
	FD_ZERO(&test1_fd);
	for(i=0;i<hosts;i++) FD_SET(client_socket[i],&read_fd);
	flag_iesire=0;
	for(i=0;i<hosts;i++) ready[i]=0;
	while(flag_iesire==0)
	{
		memcpy(&test1_fd,&read_fd,sizeof(read_fd));
		if(select(client_socket[hosts],&test1_fd,(fd_set *)NULL,(fd_set *)NULL,&time_2_wait)<0)
			perror("server socket select");
		for(i=0;i<hosts;i++)
			if(FD_ISSET(client_socket[i],&test1_fd))
			{
				recv(client_socket[i],buffer,SIZE,0);
				if((test[0]=='d')||(test[0]=='D'))
				{
					send(client_socket[i],mesaj_ready,sizeof(mesaj_ready),0);
					//eliberez memorie
					free(*(data[i].mat));
					free(data[i].mat);
					free(data[i].vector);
					FD_CLR(client_socket[i],&read_fd);
				}
				else
				{
					send(client_socket[i],mesaj_exit,strlen(mesaj_exit),0);
					recv(client_socket[i],buffer,strlen(mesaj_exit),0);
					//eliberez memorie
					free(*(data[i].mat));
					free(data[i].mat);
					free(data[i].vector);
					FD_CLR(client_socket[i],&read_fd);
					close(client_socket[i]);
				}
				ready[i]=1;
			}
		temp=0;
		for(i=0;i<hosts;i++) temp+=ready[i];
		if(temp==hosts) flag_iesire=1;
	}
	//eliberez memorie
	free(rezultate);
	free(data);
	if((test[0]=='d')||(test[0]=='D')) 
		return 2;
	else 
	{
		free(adresa);
		free(server_addr);
		return 1;
	}
}