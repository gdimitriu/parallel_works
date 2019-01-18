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
	data=new long[thread];
	counters=new int[dim];
	barrier.barrier_init(thread);
	for(i=0;i<dim;i++) counters[i]=0;
	for(i=0;i<thread;i++)
	{
		data[i]=i;
		threads=new solveGE(mat,y,dim,thread,crit,counters,&barrier,data[i]);
		threads->CreateThread(0,0);
		hthreads[i]=threads->m_hThread;
	}
	::WaitForMultipleObjects(thread,hthreads,true,INFINITE);
	memcpy(x,y,dim*sizeof(double));
}