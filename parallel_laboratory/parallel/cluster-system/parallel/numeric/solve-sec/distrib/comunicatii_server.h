/*
	hederul pentru comunicatii server (patial)
*/
//pornire server pentru conexiunea din jos
int conect_sv_dw()
{
	//creare socket
	if((listen_socket_dw=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket");
		peroare=strerror(listen_socket_dw);
		return 1;
	}
	memset((char *)&server_addr_dw,0,sizeof(server_addr_dw));
	server_addr_dw.sin_family=AF_INET;
	server_addr_dw.sin_port=htons(porturi[1]);
	server_addr_dw.sin_addr.s_addr=htonl(INADDR_ANY);
	//legare (bind)
	if((error=bind(listen_socket_dw,(struct sockaddr *)&server_addr_dw,sizeof(server_addr_dw)))<0)
	{
		perror("bind");
		peroare=strerror(error);
		close(listen_socket_dw);
		return 1;
	}
	if((error=listen(listen_socket_dw,5))<0)
	{
		perror("listen");
		peroare=strerror(error);
		close(listen_socket_dw);
		return 1;
	}
	client_len_dw=sizeof(client_addr_dw);
	if(primul!='0')
	{
		peroare=mesaj_ack;
		send(server_socket,peroare,strlen(peroare),0);
		//astept confirmare pornire toate servere
		memset((char *)&buffer,0,sizeof(buffer));
		recv(server_socket,buffer,SIZE,0);
		if(strcmp(buffer,"START")<0)
		{
			close(listen_socket_dw);
			return 2;
		}
	}
	//acceptare conexiune
	if((server_socket_dw=accept(listen_socket_dw,(struct sockaddr *)&client_addr_dw,&client_len_dw))<0)
	{
		perror("accept");
		peroare=strerror(server_socket_dw);
		close(listen_socket_dw);
		if(primul=='0') return 1;
		return 2;
	}
	return 0;
}
//conectare in sus
int conect_sv_up()
{
int i;
	i=0;
	while(i<5)
	{
		server_addr_up.sin_family=AF_INET;
		server_addr_up.sin_port=htons(porturi[0]);
		server_addr_up.sin_addr.s_addr=inet_addr(adresa);
		//creare socket
		if((client_socket_up=socket(AF_INET,SOCK_STREAM,0))<0)
		{
			perror("socket");
			peroare=strerror(client_socket_up);
			return 1;
		}
		//conectare
		if((error=connect(client_socket_up,(struct sockaddr *)&server_addr_up,sizeof(server_addr_up)))<0)
		{
			perror("connect");
			peroare=strerror(error);
			close(client_socket_up);
			i++;
		}
		else break;
		sleep(1);
	}
		if(i==5)
		{

			return 1;
		}
	peroare=mesaj_ack;
	return 0;
}
/*
	Functia pentru deconectare in sus
*/
void deconectez_up()
{
	send(client_socket_up,mesaj_exit,strlen(mesaj_exit),0);
	recv(client_socket_up,buffer,strlen(mesaj_exit),0);
	close(client_socket_up);
}
/*
	Functia pentru deconectare in jos
*/
void deconectez_dw()
{
	recv(server_socket_dw,buffer,strlen(mesaj_exit),0);
	send(server_socket_dw,mesaj_exit,strlen(mesaj_exit),0);
	close(server_socket_dw);
	close(listen_socket_dw);
}