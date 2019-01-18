#include<stdio.h>
#include<mpi.h>
int main(int argc, char **argv)
{
	MPI_Comm client;
	MPI_Status status;
	char port_name[MPI_MAX_NAME_LEN];
	char buf[10];
	int size,again;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);	
	if(size!=1) error("server toot big");
	MPI_Open_port(NULL,port_name);
	printf("server aivable at %s\n",port_name);fflush(stdout);
	while(1)
	{
		MPI_Comm_accept(port_name,NULL,0,MPI_COMM_WORLD,&client);
		again=1;
		while(again)
		{
			MPI_Recv(buf,10,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,client,&status);
			switch(status.MPI_TAG)
			{
				case 0:  MPI_Comm_free(&client);
							MPI_Finalize();
							return 0;
				case 1:	MPI_Comm_disconnect(&client);
							printf("recv=%s\n",buf);fflush(stdout);
							again=0;
							break;
				case 2:
							printf("recv=%s\n",buf);fflush(stdout);
							again=1;
							break;
				default:
							MPI_Abort(MPI_COMM_WORLD,"Unexpected message type");
			}
		}
	}
}