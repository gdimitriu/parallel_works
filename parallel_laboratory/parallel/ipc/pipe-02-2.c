#include<stdio.h>
int main(int argc,char **argv)
{
	int fd,nr;
	char buffer[256];
	fd=atoi(argv[1]);
	switch(nr=read(fd,buffer,sizeof(buffer)))
	{
		case -1:
			perror("read");
			exit(-1);
		case 0:
			printf("Am atins sfirsitul fisierului!\n");
			exit(1);
		default:
			printf("Am citit %d octeti: %s\n",nr,buffer);
	}
}