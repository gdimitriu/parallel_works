/* Programul SERVER pentru gauss */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include"definitii_server.h"
#include"comunicatii_server.h"
#include"calcule_server.h"

int main(int argc, char **argv)
{
int i;
	if(argc!=2)
	{
		write(1,"gaussd [port]\n",14*sizeof(char));
		write(1,"Now it is at port 50000\n",24*sizeof(char));
		PORT=Port;
	}
	else	PORT=atoi(argv[1]);
	//creare socket
	if((listen_socket=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("listen error");
		exit(1);
	}
	memset((char *)&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	//legare (bind)
	if(bind(listen_socket,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)
	{
		perror("bind error");
		close(listen_socket);
		exit(2);
	}
	if(listen(listen_socket,1)<0)
	{
		perror("listen socket error");
		close(listen_socket);
		exit(3);
	}
	client_len=sizeof(client_addr);
	while(1)
	{
		//acceptare conexiune
		if((server_socket=accept(listen_socket,(struct sockaddr *)&client_addr,&client_len))<0)
		{
			perror("accept error server");
			close(listen_socket);
			exit(4);
		}
		//creez un nou fiu
		if((client1=fork())<0)
		{
			perror("fork error server");
			close(server_socket);
			close(listen_socket);
			exit(5);
		}
		if(client1==0)	//proces client
		{	
			for(i=0;i<15;i++)
				adresa[i]=0;
			//initializez comunicatia
			send(server_socket,"OK",2,0);
			//receptionez cine este prima masina
			memset((char *)&buffer,0,sizeof(buffer));
			recv(server_socket,buffer,1,0);
			primul=buffer[0];
			//receptionez porturile pentru inel
			memset((int *)&porturi,0,sizeof(porturi));
			read(server_socket,&porturi,sizeof(porturi));
			//receptionez adresa masinii urmatoare
			memset((char *)&adresa,0,sizeof(adresa));
			read(server_socket,adresa,15*sizeof(float));
			send(server_socket,"READY",5,0);
			if(primul=='0')
			{
				//pentru prima masina (0) care initializeaza conexiunile
				//receptionez inceputul
				memset((char *)&buffer,0,sizeof(buffer));
				recv(server_socket,buffer,SIZE,0); 
				if(strcmp(buffer,"START")<0) 
				{
				//serverele celelalte nu au pornit
					close(server_socket);
					return 1;
				}
				//conectez masina urmatoare
				error=conect_sv_up();
				send(server_socket,peroare,strlen(peroare),0);
				if(error==1) 
				{
					close(server_socket);
					return 1;
				}
				//pornesc serverul pentru masina precedenta
				error=conect_sv_dw();
				if(error==1) 
				{
					close(client_socket_up);
					close(server_socket);
					return 1;
				}
			}
			else
			{
				//pentru masinile diferite de 0
				//receptionez inceputul
				memset((char *)&buffer,0,sizeof(buffer));
				recv(server_socket,buffer,SIZE,0); 
				if(strcmp(buffer,"START")<0) 
				{
					recv(server_socket,buffer,SIZE,0);
					close(server_socket);
					return 1;
				}
				error=conect_sv_dw();	//pornesc serverul pentru masina precedenta
				if(error==2) return 2;
				else if(error==1) 
				{
					send(server_socket,peroare,strlen(peroare),0);
					recv(server_socket,buffer,SIZE,0);
					close(server_socket);
					return 1;
				}
				//conectez masina urmatoare
				error=conect_sv_up();	
				if(error!=0)
				{
					close(server_socket_dw);
					close(listen_socket_dw);
					close(server_socket);
					return 1;
				}
			}
initiere:
			//initiez noua faza a transferului
			send(server_socket,"READY",5,0);
			//receptionez numarul de variabile
			recv(server_socket,&variabila,1,0);
			//receptionez numarul de linii pe care se face calculul
			recv(server_socket,&nr_linii,1,0);
			//receptionez care este ultima masina care a primit o ecuatie
			recv(server_socket,&ec_ultim,1,0);
			//receptionez indexul hostului
			recv(server_socket,&host,1,0);
			//receptionez numarul de hosturi
			recv(server_socket,&host_nr,1,0);
			host_nr--;
			//aloc matricea pentru date
			data=(float **)calloc(nr_linii,sizeof(float *));
			pdata=(float *)calloc(nr_linii*(variabila+1),sizeof(float));
			for(i=0;i<nr_linii;i++)
			{
				data[i]=pdata;
				pdata+=variabila+1;
			}
			//aloc pentru vectorul de informatii
			vector=(int *)calloc(nr_linii,sizeof(int));
			//aloc data pentru rezultate
			rezultate=(float *)calloc(variabila,sizeof(float));
			//aloc data pentru liniile temporare
			temporar=(float *)calloc(variabila+1,sizeof(float));
			//initiez noua faza a transferului
			send(server_socket,"READY",5,0);
			//primesc liniile din matrice
			for(i=0;i<nr_linii;i++)
			{
				read(server_socket,data[i],(variabila+1)*sizeof(float));
			}	
			read(server_socket,vector,(nr_linii)*sizeof(int));
			/*
				fac eliminarea gaussiana
			*/
			gaussian_elimination();
			/*
				rezolv matricea triunghiulara ramasa
			*/
			for(i=0;i<variabila;i++) rezultate[i]=0;
			back_substitution();
			if(primul=='0')
			{
				send(server_socket,"0",1,0);
				write(server_socket,rezultate,variabila*sizeof(float));
			}
			send(server_socket,"0",1,0);
			memset((char *)&buffer,0,sizeof(buffer));
			recv(server_socket,buffer,SIZE,0);
			if(strcmp(buffer,"READY")<0);
			else
			{
				free(*data);
				free(data);
				free(vector);
				free(rezultate);
				free(temporar);
				goto initiere;
			}
			send(server_socket,mesaj_exit,strlen(mesaj_exit),0);
			close(server_socket);
			//eliberez memorie
			free(*data);
			free(data);
			free(vector);
			free(rezultate);
			free(temporar);
			//inchid socketurile
			if(primul=='0')
			{
				deconectez_up();
				deconectez_dw();
			}
			else
			{
				deconectez_dw();
				deconectez_up();
			}
			exit(0);
		}
		if((client1=waitpid(client1,&status,0))<0) 
		{
			perror("error waitpid client1");
			close(server_socket);
			close(listen_socket);
			exit(1);
		}
		close(server_socket);
	}
}