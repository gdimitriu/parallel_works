/* Programul CLIENT pentru gauss*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<math.h>
#include"definitii_client.h"
#include"fisier.h"
#include"comunicatii_client.h"

int main()
{
int error;
	error=configurare_retea();
	if(error==0)
	{
		read: read_data();
		send_data();
		recv_data();
		if(inchid_sistem()==2) goto read;
	}
	else
	{
		for(i=0;i<hosts;i++) close(client_socket[i]);
	}
	return 0;
}
