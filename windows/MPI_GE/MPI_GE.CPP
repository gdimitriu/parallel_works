// mpi_ge.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <mpi.h>
#include "mpi_ge.h"
#include "stdafx.h"
#include "gauss_sc_mpi.c"
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#define numar 10

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

double timeprint(LARGE_INTEGER t1,LARGE_INTEGER t2,int T,long dim,FILE *fp,int thread)
{
	LARGE_INTEGER freq;
	double operand;
	QueryPerformanceFrequency(&freq);
	operand=(double)(t2.QuadPart-t1.QuadPart)/(double)freq.QuadPart;
	fprintf(fp,"%ld %lf %d %lf %d\n",dim,operand,thread,operand/T,T);
	fflush(fp);
	printf("%fs sum of %d executions with %ld equations and %d threads\n",operand,T,dim,thread);fflush(stdout);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		double **mat,*x,*rez,*temp1,temp,*y;
		LARGE_INTEGER time1,time2;
		int i,l,j;
		int dim;
		int rank;
		FILE *fp;
		int thread;
		MPI_Init(&argc,&argv);
		MPI_Comm_size(MPI_COMM_WORLD,&thread);
		MPI_Comm_rank(MPI_COMM_WORLD,&rank);
		dim=atoi(argv[1]);
		MPI_Bcast(&dim,1,MPI_INT,0,MPI_COMM_WORLD);
		if(rank==0)
		{
			y=(double *)calloc(dim,sizeof(double));
			rez=(double *)calloc(dim,sizeof(double));
			for(i=0;i<dim;i++) rez[i]=(double)i;
			//here is the parallel zone
			mat=(double **)calloc(dim,sizeof(double *));
			temp1=(double *)calloc(dim*dim,sizeof(double));
			for(i=0;i<dim;i++)
			{
				mat[i]=temp1;
				temp1+=dim;
			}
		}
		x=(double *)calloc(dim,sizeof(double));
		if(rank==0)
		{
			for(i=0;i<dim;i++)
			{
				for(j=0;j<dim;j++) mat[i][j]=(double)rand();
				temp=0.0;
				for(j=0;j<dim;j++) if(j!=i) temp+=abs(mat[i][j]);
				mat[i][i]+=temp;
			}
			for(i=0;i<dim;i++)
			{
				y[i]=0.0;
				x[i]=0.0;
				for(j=0;j<dim;j++) y[i]+=mat[i][j]*rez[j];
			}
			QueryPerformanceCounter(&time1);
		}
		for(l=0;l<numar;l++)
		{
			gauss_MPI(mat,y,x,dim);
		}
		if(rank==0) 
		{
			QueryPerformanceCounter(&time2);
			if(rank==0) for(i=0;i<dim;i++) {if(fabs(rez[i]-x[i])>1E-5) printf("%lf=%lf\n",rez[i],x[i]); fflush(stdout);}
			fp=fopen("time-par.dat","a");
			timeprint(time1,time2,numar,dim,fp,thread);
			fclose(fp);
			free(*mat);
			free(mat);
			free(rez);
			free(y);
		}		
		free(x);
		MPI_Finalize();
	}
	return nRetCode;
}


