#include<stdio.h>
int main()
{
	int pdescr[2];
	char buffer[256];
	int nr;
	char *mesaj="sir de caractere";
	if(pipe(pdescr)==-1)
	{
		perror("pipe");
		exit(-1);
	}
	strcpy(buffer,mesaj);
	if(write(pdescr[1],buffer,strlen(buffer)+1)==-1)
	{
		perror("write");
		exit(-1);
	}
	switch(nr=read(pdescr[0],buffer,sizeof(buffer)))
	{
		case -1:
			perror("read");
			exit(-1);
		case 0:
			printf("Eroare: EOF!\n");
			exit(-1);
		default:
			printf("Am citit %d octeti: %s\n",nr,buffer);
	}
}