/*
		Program who make the broadcast thru process
*/

#include<stdio.h>
#include<mpi.h>
#include<st.h>

int st_main(int argc,char **argv)
{
	int nrprocs; /* number of processors */
	int myid;	 /* id number (rank) */
	int number=0;	 /* buffer (number) to transmite */
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Comm_size(MPI_COMM_WORLD,&nrprocs);
	do
	{
		if(myid==0)
		{
			printf("Input the number to transmite.\n");
			fflush(stdout);
			scanf("%d",&number);
			fflush(stdin);
		}
		MPI_Bcast(&number,1,MPI_INT,0,MPI_COMM_WORLD);
		printf("my id is %d and number is %d\n",myid,number);
	}while(number==0);
	MPI_Finalize();
	return 0;
}