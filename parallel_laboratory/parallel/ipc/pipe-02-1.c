#include<stdio.h>
int main()
{
	int pdescr[2];
	char buffer[256];
	if(pipe(pdescr)==-1)
	{
		perror("pipe");
		exit(1);
	}
	switch(fork())
	{
		case -1:
			perror("fork");
			exit(-1);
		case 0:
			//fiu
			if(close(pdescr[1])==-1)
			{
				perror("close");
				exit(-1);
			}
			sprintf(buffer,"%d ",pdescr[0]);
			execlp("./aldoilea","aldoilea",buffer,NULL);
			perror("execlp");
			exit(-1);
	}
	if(close(pdescr[0])==-1)
	{
		perror("close2");
		exit(-1);
	}
	if(write(pdescr[1],"salut",6)==-1)
	{
		perror("write");
	}
}