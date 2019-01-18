#include<stdio.h>
#include<mpi.h>
int main(int argc,char **argv)
{
	MPI_Comm server;
	char buf[10];
	char port_name[MPI_MAX_NAME_LEN];
	int done;
	MPI_Init(&argc,&argv);
	strcpy(port_name,argv[1]);
	MPI_Comm_connect(port_name,NULL,0,MPI_COMM_WORLD,&server);
	done=0;
	while(done!=1)
	{
		gets(buf);
		MPI_Send(buf,10,MPI_CHAR,0,2,server);
		scanf("%d",&done);fflush(stdin);
	}
	strcpy(buf,"FIN");
	MPI_Send(buf,10,MPI_CHAR,0,1,server);
	MPI_Comm_disconnect(&server);
	MPI_Finalize();
}