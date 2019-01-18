// bench.cpp: implementation of the bench class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GE.h"
#include "bench.h"
#include "gauss.h"
#include <stdlib.h>
#include <sys/timeb.h>
#include <time.h>
#include <iostream.h>
#include <memory.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bench::bench()
{
	double *temp1,temp;
	LARGE_INTEGER time1,time2;
	long i,j;
	threaduri[0]=2;
	threaduri[1]=4;
	threaduri[2]=8;
	threaduri[3]=16;
	threaduri[4]=32;
	threaduri[5]=64;
	for(dim=8;dim<513;dim+=8)
	{
		//here we make the allocation for matrix
		mat=(double **)calloc(dim,sizeof(double *));
		temp1=(double *)calloc(dim*dim,sizeof(double));
		for(i=0;i<dim;i++)
		{
			mat[i]=temp1;
			temp1+=dim;
		}
		mat1=(double **)calloc(dim,sizeof(double *));
		temp1=(double *)calloc(dim*dim,sizeof(double));
		for(i=0;i<dim;i++)
		{
			mat1[i]=temp1;
			temp1+=dim;
		}
		x=(double *)calloc(dim,sizeof(double));
		y=(double *)calloc(dim,sizeof(double));
		y1=(double *)calloc(dim,sizeof(double));
		rez=(double *)calloc(dim,sizeof(double));
		for(i=0;i<dim;i++) rez[i]=(double)(i+1);
		for(i=0;i<dim;i++)
		{
			for(j=0;j<dim;j++) mat[i][j]=(double)rand();
			temp=0.0;
			for(j=0;j<dim;j++)	if(j!=i) temp+=abs(mat[i][j]);
			mat[i][i]=mat[i][i]+(double)temp;
		}
		for(i=0;i<dim;i++)
		{
			y[i]=0.0;
			x[i]=0.0;
			for(j=0;j<dim;j++) y[i]+=mat[i][j]*rez[j];
		}
		fp=new ofstream[6];
		fp[0].open("time-gauss-2.dat",ios::app);
		fp[1].open("time-gauss-4.dat",ios::app);
		fp[2].open("time-gauss-8.dat",ios::app);
		fp[3].open("time-gauss-16.dat",ios::app);
		fp[4].open("time-gauss-32.dat",ios::app);
		fp[5].open("time-gauss-64.dat",ios::app);
		for(threads=0;threads<6;threads++)
		{
			QueryPerformanceCounter(&time1);
			hthreads=new HANDLE[threaduri[threads]];
//			ordine=new long[threaduri[threads]];
			datacpy=new datac[threaduri[threads]];
			//running parallel computing
			for(j=0;j<10;j++)
			{	
				for(i=0;i<threaduri[threads];i++)
				{
					datacpy[i].dimensiune=dim;
					datacpy[i].freev=y;
					datacpy[i].freev1=y1;
					datacpy[i].matr=mat;
					datacpy[i].matr1=mat1;
					datacpy[i].pos=i;
					datacpy[i].thread=threaduri[threads];
					thread=AfxBeginThread(thread_cpy,(LPVOID)&datacpy[i]);
/*					ordine[i]=i;
					thread=new thread_cpy(dim,threaduri[threads],y,y1,mat,mat1,&ordine[i]);
					thread->CreateThread(0,0,NULL); //creez thread */
					hthreads[i]=thread->m_hThread;//capturez handler
				}
				//join
				::WaitForMultipleObjects(threaduri[threads],hthreads,TRUE,INFINITE);
			 	calcul=new gauss(dim,threaduri[threads],mat1,x,y1);
			}
			//delete [] ordine;
			delete [] datacpy;
			delete [] hthreads;
			QueryPerformanceCounter(&time2);
			timpp(time1,time2);
			for(i=0;i<dim;i++) if(abs(rez[i]-x[i])>1E-5){cout<<rez[i]<<'='<<x[i]<<endl; cout.flush();}
		}
		for(i=0;i<6;i++) fp[i].close();
		delete [] fp;
		cout<<"The parallel zone is end\n";cout.flush();
		fp=new ofstream;
		fp->open("serial-gauss.dat",ios::app);
		QueryPerformanceCounter(&time1);
		for(j=0;j<10;j++)
		{
			memcpy(&mat1[0][0],&mat[0][0],dim*dim*sizeof(double));
			memcpy(y1,y,dim*sizeof(double));
			gauss_ser(dim,mat1,x,y1);
		}
		QueryPerformanceCounter(&time2);
		timps(time1,time2);
		for(i=0;i<dim;i++) if(abs(rez[i]-x[i])>1E-5){cout<<rez[i]<<'='<<x[i]<<endl; cout.flush();}
		fp->close();
		delete fp;
		free(*mat1);
		free(mat1);
		free(*mat);
		free(mat);
		free(y);
		free(y1);
		free(x);
		free(rez);
	}
}

bench::~bench()
{

}

void bench::timpp(LARGE_INTEGER val1,LARGE_INTEGER val2)
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	if((val2.HighPart-val1.HighPart)==0)
		fp[threads]<<dim<<' '<<(double)(val2.LowPart-val1.LowPart)/(double)freq.LowPart<<' '<<threaduri[threads]<<endl;
	fp[threads].flush();
}

void bench::timps(LARGE_INTEGER val1,LARGE_INTEGER val2)
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	if((val2.HighPart-val1.HighPart)==0)
		*fp<<dim<<' '<<(double)(val2.LowPart-val1.LowPart)/(double)freq.LowPart<<' '<<1<<endl;
	fp->flush();
}

void bench::gauss_ser(long dim1,double **matr,double *sol,double *freev)
{
	long i,j,k;
	double temp;
	for(k=0;k<dim1-1;k++)
		for(i=k+1;i<dim1;i++)
		{
			temp=matr[i][k]/matr[k][k];
			for(j=k+1;j<dim1;j++) matr[i][j]-=temp*matr[k][j];
			freev[i]-=temp*freev[k];
		}
	for(i=dim1-1;i>=0;i--)
	{
		temp=0.0;
		sol[i]=0.0;
		for(j=i+1;j<dim1;j++) temp+=matr[i][j]*sol[j];
		sol[i]=(freev[i]-temp)/matr[i][i];
	}
}

UINT bench::thread_cpy(LPVOID pParam)
{
	datac *idata;
	idata=(datac *)pParam;
	long i;
	for(i=(long)idata->pos;i<idata->dimensiune;i=i+(long)idata->thread)
	{
		memcpy(idata->matr1[i],idata->matr[i],idata->dimensiune*sizeof(double));
		idata->freev1[i]=idata->freev[i];
	}
	return 0;
}
