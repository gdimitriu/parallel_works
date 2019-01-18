// ser_GE.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ser_GE.h"
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include "ge_serial.c"
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
	double op1;
	QueryPerformanceFrequency(&freq);
	operand=(double)(t2.QuadPart-t1.QuadPart)/(double)freq.QuadPart;
//	op1=(t2.QuadPart-t1.QuadPart)%freq.QuadPart;
//	operand+=op1;
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
		long i,l,j;
		long dim;
		FILE *fp;
		LARGE_INTEGER time1,time2;
		double **mat,*x,*rez,*temp1,temp,*y,**mat1,*y1;
		dim=atol(argv[1]);
		x=(double *)calloc(dim,sizeof(double));
		y=(double *)calloc(dim,sizeof(double));
		y1=(double *)calloc(dim,sizeof(double));
		rez=(double *)calloc(dim,sizeof(double));
		for(i=0;i<dim;i++) rez[i]=(double)i;
		mat=(double **)calloc(dim,sizeof(double *));
		temp1=(double *)calloc(dim*dim,sizeof(double));
		for(i=0;i<dim;i++)
		{
			mat[i]=temp1;
			temp1+=dim;
			x[i]=0;
		}
		mat1=(double **)calloc(dim,sizeof(double *));
		temp1=(double *)calloc(dim*dim,sizeof(double));
		for(i=0;i<dim;i++)
		{
			mat1[i]=temp1;
			temp1+=dim;
		}
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
			for(j=0;j<dim;j++) y[i]+=mat[i][j]*rez[j];
		}
		QueryPerformanceCounter(&time1);
		for(l=0;l<numar;l++)
		{
			for(i=0;i<dim;i++) for(j=0;j<dim;j++) mat1[i][j]=mat[i][j];
			for(i=0;i<dim;i++) y1[i]=y[i];
			gauss_serial(mat1,y1,x,dim);
		}
		QueryPerformanceCounter(&time2);
		for(i=0;i<dim;i++) {if(abs(rez[i]-x[i])>1E-5) printf("%lf=%lf\n",rez[i],x[i]); fflush(stdout);}
		fp=fopen("time-ser.dat","a");
		timeprint(time1,time2,numar,dim,fp,1);
		fclose(fp);
		free(*mat);
		free(mat);
		free(*mat1);
		free(mat1);
		free(y1);
		free(x);
		free(rez);
		free(y);
	}
	return nRetCode;
}


