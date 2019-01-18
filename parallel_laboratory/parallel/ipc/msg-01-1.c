#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>
#define CHEIE 0100 //identificatorul cozii
struct mesaj
{
	long mtip;
	char mtext[256];
};
int main()
{
	struct mesaj mes;
	int id_coada,id_proces,*pint;
	//descidere coada
	id_coada=msgget(CHEIE,0666);
	//construirea mesajului
	mes.mtip=1;
	id_proces=getpid();
	pint=(int *)mes.mtext;
	*pint=id_proces;
	msgsnd(id_coada,&mes,sizeof(int),0);
	msgrcv(id_coada,&mes,256,id_proces,0);
	printf("client: serverul este: %d\n",*(int *)mes.mtext);
}