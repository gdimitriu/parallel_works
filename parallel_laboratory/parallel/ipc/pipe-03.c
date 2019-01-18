#include<stdio.h>
int main(int argc,char **argv)
{
	int pdescr[2];
	int status;
	if(argc!=3)
	{
		printf("Utilizare: prog cmd1 cmd2\n");
		exit(-1);
	}
	if(pipe(pdescr)==-1)
	{	
		perror("pipe");
		exit(-2);
	}
	switch(fork())
	{
		case -1:
			perror("fork");
			exit(-2);
		case 0:
			//fiul unu
			close(pdescr[0]);
			close(1);
			if(dup(pdescr[1])!=1)
			{
				perror("dup");
				exit(-3);
			}
			close(pdescr[1]);
			execlp(argv[1],argv[1],NULL);
			perror("execlp");
			exit(-3);
	}
	switch(fork())
	{
		case -1:
			perror("fork 2");
			exit(-2);
		case 0:
			//fiul doi
			close(pdescr[1]);
			close(0);
			if(dup(pdescr[0])!=0)
			{
				perror("dup2");
				exit(-4);
			}
			close(pdescr[0]);
			execlp(argv[2],argv[2],NULL);
			perror("execlp2");
			exit(-4);
	}
	//parinte
	close(pdescr[0]);
	close(pdescr[1]);
	wait(&status);
	wait(&status);
}