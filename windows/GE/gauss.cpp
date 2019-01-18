// gauss.cpp: implementation of the gauss class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GE.h"
#include "gauss.h"
#include "bariera.h"
#include "solveGE.h"
#include <memory.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

gauss::gauss()
{

}

gauss::~gauss()
{
	delete [] data;
	delete [] counters;
	delete [] hthreads;
	barrier.barrier_destroy();
	delete [] crit;
}
gauss::gauss(long dim,int thread,double **mat,double *x,double *y)
{
	/*
	dim is dimension of data matrix
	thread is the numbers of threads
	mat is data matrix
	x is unknown vector
	y is free vector
	*/
	long i;
	hthreads=new HANDLE[thread];
	crit=new CCriticalSection[dim];
	datag *datathg;
//	data=new long[thread];
	datathg=new datag[thread];
	counters=new int[dim];
	barrier.barrier_init(thread);
	CWinThread *threads;
	for(i=0;i<dim;i++) counters[i]=0;
	for(i=0;i<thread;i++)
	{
		datathg[i].who=i;
		datathg[i].barrier=&barrier;
		datathg[i].counters=counters;
		datathg[i].crit=crit;
		datathg[i].mat=mat;
		datathg[i].y=y;
		datathg[i].N=dim;
		datathg[i].P=thread;
		threads=AfxBeginThread(calcul_tgauss,(LPVOID)&(datathg[i]));
/*		data[i]=i;
		threads=new solveGE(mat,y,dim,thread,crit,counters,&barrier,data[i]);
		threads->CreateThread(0,0); */
		hthreads[i]=threads->m_hThread;

	}
	::WaitForMultipleObjects(thread,hthreads,true,INFINITE);
	memcpy(x,y,dim*sizeof(double));
}

UINT gauss::calcul_tgauss(LPVOID pParam)
{
	long s;				//Nr of row in partition
	long nr;			//average nr of row in each partition
	long lastrow;		//if this thread have the last line
	long proccount;		//variable for eliminate the row=0 and last row
	long proccount1;	//variable who represent the processor who is working
	long replay;		//variable for continue the elimination where it was stop
	long i,j,k,l;		//counters
	datag *idata;
	idata=(datag *)pParam;
	//computing
	nr=idata->N%idata->P;
	s=(idata->N-nr)/idata->P;
	//for the common row
	if((idata->who+1)<nr) s++;
	else //for the last row
		if((idata->who+1)==nr)
		{
			s++;
			lastrow=1;
		}
	if((nr==0) && (idata->who+1)==idata->P) lastrow=1;
	//making allocation for vector of cicle variable
	i=0;
	//for first row on first processor
	if(idata->who==0)
	{
		//make division step
		for(j=1;j<idata->N;j++) idata->mat[0][j]=idata->mat[0][j]/idata->mat[0][0];
		idata->y[0]=idata->y[0]/idata->mat[0][0];
		idata->mat[0][0]=1;
		idata->counters[0]=1;
		proccount=idata->P;
		for(i=idata->P;i<idata->N;i+=idata->P) idata->crit[i].Lock();
	}
	else
	{
		proccount=idata->who;
		for(i=proccount;i<idata->N;i+=idata->P) idata->crit[i].Lock();
	}
	//until all row are elimitate
	//for all the row who are in this thread
	replay=0;
	idata->barrier->barrier_sync();
	//i is the line who is working
	for(i=proccount;i<idata->N;i+=idata->P)
	{
		//make elimination step
		for(k=replay;k<i;k++)
		{
			if(idata->counters[k]==0)
			{
				idata->crit[k].Lock();
				idata->crit[k].Unlock();
			}
			for(j=(k+1);j<idata->N;j++) idata->mat[i][j]-=idata->mat[i][k]*idata->mat[k][j];
			idata->y[i]-=idata->y[k]*idata->mat[i][k];
			idata->mat[i][k]=0.0;
		}
		//make division step
		for(j=i+1;j<idata->N;j++) idata->mat[i][j]=idata->mat[i][j]/idata->mat[i][i];
		idata->y[i]=idata->y[i]/idata->mat[i][i];
		idata->mat[i][i]=1.0;
		//make settings for next row
		idata->counters[i]=1;
		idata->crit[i].Unlock();
		proccount1=i+idata->P;
		for(k=replay;k<i;k++)
		{
			for(l=proccount1;l<idata->N;l+=idata->P)
			{
				for(j=(k+1);j<idata->N;j++) idata->mat[l][j]-=idata->mat[l][k]*idata->mat[k][j];
				idata->y[l]-=idata->y[k]*idata->mat[l][k];
				idata->mat[l][k]=0;
			}
		}
		replay=i;
	}
	//will be sincronize before back-substitution step
	idata->barrier->barrier_sync();
	//back substitution
	for(i=idata->who;i<idata->N;i+=idata->P) idata->counters[i]=0;
	if(lastrow==1)
	{
		proccount=idata->N-1-idata->P;
		//modification vector is reset
		idata->counters[idata->N-1]=1;
		for(i=idata->who;i<idata->N-1;i+=idata->P) idata->crit[i].Lock();
	}
	else
	{
		proccount=idata->who+idata->P*(s-1);
		for(i=idata->who;i<idata->N;i+=idata->P) idata->crit[i].Lock();
	}
	idata->barrier->barrier_sync();
	replay=idata->N-1;
	for(i=proccount;i>=idata->who;i-=idata->P)
	{
		//for all rows before current row
		for(k=replay;k>i;k--)
		{
			if(idata->counters[k]==0)
			{
				idata->crit[k].Lock();
				idata->crit[k].Unlock();
			}
			idata->y[i]-=idata->y[k]*idata->mat[i][k];
		}
		idata->counters[i]=1;
		idata->crit[i].Unlock();
		proccount1=i-idata->P;
		for(k=replay;k>i;k--)
		{
			for(l=proccount1;l>=idata->who;l-=idata->P)
			{
				//make back-substitution
				idata->y[l]-=idata->y[k]*idata->mat[l][k];
			}
		}
		replay=i;
	}
	return 0;
}
