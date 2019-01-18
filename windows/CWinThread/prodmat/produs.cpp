// produs.cpp: implementation of the produs class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "prodmat.h"
#include "produs.h"
#include "thread_prod.h"
#include <stdlib.h>
#include <sys/timeb.h>
#include <time.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

produs::produs()
{
	LARGE_INTEGER time1,time2;
	threaduri[0]=2;
	threaduri[1]=4;
	threaduri[2]=8;
	threaduri[3]=16;
	threaduri[4]=24;
	threaduri[5]=32;
	double *temp;
	long i,j;
	for(dim=8;dim<513;dim=dim+8)
	{
		mata=(double **)calloc(dim,sizeof(double *));
		temp=(double *)calloc(dim*dim,sizeof(double));
		for(i=0;i<dim;i++)
		{
			mata[i]=temp;
			temp+=dim;
		}
		matb=(double **)calloc(dim,sizeof(double *));
		temp=(double *)calloc(dim*dim,sizeof(double));
		for(i=0;i<dim;i++)
		{
			matb[i]=temp;
			temp+=dim;
		}	
		matc1=(double **)calloc(dim,sizeof(double *));
		temp=(double *)calloc(dim*dim,sizeof(double));
		for(i=0;i<dim;i++)
		{
			matc1[i]=temp;
			temp+=dim;
		}
		matc2=(double **)calloc(dim,sizeof(double *));
		temp=(double *)calloc(dim*dim,sizeof(double));
		for(i=0;i<dim;i++)
		{
			matc2[i]=temp;
			temp+=dim;
		}
		for(i=0;i<dim;i++)
			for(j=0;j<dim;j++)
			{
				mata[i][j]=rand();
				matb[i][j]=rand();
			}
		cout<<"serial "<<dim<<endl;cout.flush();
		fp=new ofstream;
		fp->open("serial-data.dat",ios::app);
		QueryPerformanceCounter(&time1);
		for(j=0;j<10;j++) calcul_ser(matc1);
		QueryPerformanceCounter(&time2);
		timps(time1,time2);
		fp->close();
		delete fp;
		fp=new ofstream[6];
		fp[0].open("mul-data-2.dat",ios::app);
		fp[1].open("mul-data-4.dat",ios::app);
		fp[2].open("mul-data-8.dat",ios::app);
		fp[3].open("mul-data-16.dat",ios::app);
		fp[4].open("mul-data-24.dat",ios::app);
		fp[5].open("mul-data-32.dat",ios::app);
		for(index=0;index<6;index++)
		{
			threads=threaduri[index];
			QueryPerformanceCounter(&time1);
			for(j=0;j<10;j++)	calcul_par(matc2);
			QueryPerformanceCounter(&time2);
			timpp(time1,time2);
			//verificare_calcul(matc1,matc2);
		}
		for(j=0;j<6;j++) fp[j].close();
		delete [] fp;
		cout<<"parallel "<<dim<<endl;cout.flush();
		free(*mata);
		free(mata);
		free(*matb);
		free(matb);
		free(*matc1);
		free(matc1);
		free(*matc2);
		free(matc2);
	}
}

produs::~produs()
{
	//for(int i=0;i<6;i++) fp[i].close();
}

void produs::calcul_par(double **matc)
{
	thread_prod *thread;
	int i;
	hthreads=new HANDLE[threads];
	ordine=new int[threads];
	for(i=0;i<threads;i++)
	{
		ordine[i]=i;
		thread=new thread_prod(dim,threads,mata,matb,matc,&ordine[i]);
		thread->CreateThread(0,0,NULL); //creez thread
		hthreads[i]=thread->m_hThread;//capturez handler
	}
	//join
	::WaitForMultipleObjects(threads,hthreads,TRUE,INFINITE);
	delete [] ordine;
	delete [] hthreads;
}

void produs::calcul_ser(double **matc)
{
	long i,j,k;
	for(i=0;i<dim;i++)
		for(j=0;j<dim;j++)
		{
			matc[i][j]=0.0;
			for(k=0;k<dim;k++) matc[i][j]+=mata[i][k]*matb[k][j];
		}
}

void produs::verificare_calcul(double **mat1, double **mat2)
{
	long i,j;
	for(i=0;i<dim;i++)
		for(j=0;j<dim;j++)
			if(abs(mat1[i][j]-mat2[i][j])>1E-5)
			{
				cout<<"Eroare la elementele "<<i<<' '<<j<<" avem "<<mat2[i][j]<<" in loc de "<<mat1[i][j]<<endl;
				cout.flush();
			}
}

void produs::timpp(LARGE_INTEGER val1,LARGE_INTEGER val2)
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	if((val2.HighPart-val1.HighPart)==0)
		fp[index]<<dim<<' '<<(double)(val2.LowPart-val1.LowPart)/(double)freq.LowPart<<' '<<threaduri[index]<<endl;
	fp[index].flush();
}

void produs::timps(LARGE_INTEGER val1,LARGE_INTEGER val2)
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	if((val2.HighPart-val1.HighPart)==0)
		*fp<<dim<<' '<<(double)(val2.LowPart-val1.LowPart)/(double)freq.LowPart<<' '<<1<<endl;
	fp->flush();
}
