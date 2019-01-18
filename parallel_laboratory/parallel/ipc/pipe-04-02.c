#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>

#define MAXOPEN 7
#define MAXTRIES 3
#define NAPTIME 5
#define FALSE 0
#define TRUE 1

static char *fifoname(long key)//creaza un fis temp
{
	static char fifo[20];
	sprintf(fifo,"/tmp/fifi%ld",key);
	return fifo;
}
static int mkfifos(char *path) //creaza fifo
{
	return mknod(path,S_IFIFO|0666,0);
}
static int openfifo(long key,int flags)
{
	static struct
	{
		long key;
		int fd;
		int time;
	} fifos[MAXOPEN];
	static int clock;
	int i,avail,oldest,fd,tries;
	char *fifo;
	extern int errno;
	clock++;
	avail=-1;	//cauta un loc liber
	for(i=0;i<MAXOPEN;i++)
	{
		if(fifos[i].key==key)
		{
			fifos[i].time=clock;
			return fifos[i].fd;
		}
		if(fifos[i].key==0 && avail==-1) avail=i;
	}
	if(avail==-1)  //daca nu il foloseste pe cel vechi
	{
		oldest=-1;
		for(i=0;i<MAXOPEN;i++)
			if(oldest==-1 || fifos[i].time<oldest)
			{
				oldest=fifos[i].time;
				avail=i;
			}
		if(close(fifos[avail].fd)==-1) return -1;
	}
	fifo=fifoname(key);
	if(mkfifos(fifo)==-1 && errno !=EEXIST) return -1;
	for(tries=1;tries<=MAXTRIES;tries++)
	{
		if((fd=open(fifo,flags|O_NDELAY))!=-1) break;
		if(errno!=ENXIO) return -1;
		sleep(NAPTIME);
	}
	if(fd==-1)
	{
		errno=ENXIO;
		return -1;
	}
	if(fcntl(fd,F_SETFL,flags)==-1) return -1;
	fifos[avail].key=key;
	fifos[avail].fd=fd;
	fifos[avail].time=clock;
	return fd;
}
int send(long dstkey,char *buf,int nbytes)//trimite un mesaj
{
	int fd;
	if((fd=openfifo(dstkey,O_WRONLY))==-1) return FALSE;
	return write(fd,buf,nbytes)!=-1;
}
int receive(long srckey,char *buf,int nbytes)//receptioneaza un mesaj
{
	int fd,nread;
	if((fd=openfifo(srckey,O_RDONLY))==-1) return FALSE;
	while((nread=read(fd,buf,nbytes))==0) sleep(NAPTIME);
	return nread!=-1;
}
void rmqueue(long key)
{
	int errno;
	if((unlink(fifoname(key))==-1) && errno!=ENOENT) perror("unlink");
}
typedef struct
{
	long unused;
	int pid;
	int number;
}MESSAGE;
int main()
{
	MESSAGE m;
	m.pid=getpid();
	for(m.number=1;m.number<=4;m.number++)
	{
		sleep(1);
		if(!send(1000L,&m,sizeof(m))) perror("send");
	}
	exit(0);
}