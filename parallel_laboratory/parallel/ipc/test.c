/*

*/
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
int main(void)
{
	pid_t pid,w;
	int i,status;
	char value[3];	//index pt siruri
	for(i=0;i<3;i++)	//genereaza trei fii
	{
		if((pid=fork())==0)
		{
			printf("Fork fiu %d\n",i);fflush(stdout);
		}
		else
		{
		 printf("Fork fiu %d %d\n",pid,i);
		 fflush(stdout);
		 break;
		}
	}
		/* Asteapta terminarea proceselor fii */
		while((w=wait(&status)) &&w!=-1)
		{
			if(w!=-1)
			{
				printf("Astepatarea pentru PID:%d a returnat starea: %04X\n",w,status);
				fflush(stdout);
			}
		}
		exit(0);
}