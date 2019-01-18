#include<mpi.h>
#include<stdio.h>
#include<st.h>
int st_main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	printf("Hello.\n");
	MPI_Finalize();
	return 0;
}