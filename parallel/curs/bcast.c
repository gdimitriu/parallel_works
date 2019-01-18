/*
		Program who make the broadcast thru process
*/

#include<stdio.h>
#include<mpi.h>

int main(int argc,char **argv)
{
	int nrprocs; /* nr de procesoare */
	int myid;	 /* id-ul procesorului */
	int number=0;	 /* numarul de trimis*/
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Comm_size(MPI_COMM_WORLD,&nrprocs);
	do
	{
		if(myid==0)
		{
			printf("Introduceti numarul de transmis.\n");
			fflush(stdout);
			scanf("%d",&number);
			fflush(stdin);
		}
		MPI_Bcast(&number,1,MPI_INT,0,MPI_COMM_WORLD);
		printf("id-ul meu este %d si am primit numbarul is %d\n",myid,number);
		fflush(stdout);
	}while(number>=0);
	MPI_Finalize();
	return 0;
}