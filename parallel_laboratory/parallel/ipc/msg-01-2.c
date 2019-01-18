#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define CHEIE 0100 //identificatorul cozii
struct mesaj
{
	long mtip;
	char mtext[256];
};
int id_coada;
int cleanup(void)
{
	msgctl(id_coada,IPC_RMID,0);
	exit(1);
}
int main()
{
	struct mesaj mes;
	int i,*pint;
	for(i=0;i<20;i++) signal(i,cleanup);
	id_coada=msgget(CHEIE,0666|IPC_CREAT);
	while(1)
	{
		msgrcv(id_coada,&mes,256,1,0);
		pint=(int *)mes.mtext;
		printf("Server: mesaj de la clientul: %d\n",*pint);
		mes.mtip=*pint;
		*pint=getpid();
		msgsnd(id_coada,&mes,sizeof(int),0);
	}
}