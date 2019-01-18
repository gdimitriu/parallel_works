// solveGE.cpp : implementation file
//

#include "stdafx.h"
#include "GE.h"
#include "solveGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// solveGE

IMPLEMENT_DYNCREATE(solveGE, CWinThread)

solveGE::solveGE()
{
}

solveGE::~solveGE()
{
}

BOOL solveGE::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}
solveGE::solveGE(double **matrix,double *yterm,long dim,int dim_threads,CCriticalSection *critical,int *count,bariera *bar,int whoindex)
{
	mat=matrix;
	y=yterm;
	counters=count;
	N=dim;
	P=dim_threads;
	crit=critical;
	barrier=bar;
	who=whoindex;
}
int solveGE::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(solveGE, CWinThread)
	//{{AFX_MSG_MAP(solveGE)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// solveGE message handlers

int solveGE::Run(void)
{
	nr=N%P;
	s=(N-nr)/P;
	//for the common row
	if((who+1)<nr) s++;
	else //for the last row
		if((who+1)==nr)
		{
			s++;
			lastrow=1;
		}
	if((nr==0) && (who+1)==P) lastrow=1;
	//making allocation for vector of cicle variable
	i=0;
	//for first row on first processor
	if(who==0)
	{
		//make division step
		for(j=1;j<N;j++) mat[0][j]=mat[0][j]/mat[0][0];
		y[0]=y[0]/mat[0][0];
		mat[0][0]=1;
		counters[0]=1;
		proccount=P;
		for(i=P;i<N;i+=P) crit[i].Lock();
	}
	else
	{
		proccount=who;
		for(i=proccount;i<N;i+=P) crit[i].Lock();
	}
	//until all row are elimitate
	//for all the row who are in this thread
	replay=0;
	barrier->barrier_sync();
	//i is the line who is working
	for(i=proccount;i<N;i+=P)
	{
		//make elimination step
		for(k=replay;k<i;k++)
		{
			if(counters[k]==0)
			{
				crit[k].Lock();
				crit[k].Unlock();
			}
			for(j=(k+1);j<N;j++) mat[i][j]-=mat[i][k]*mat[k][j];
			y[i]-=y[k]*mat[i][k];
			mat[i][k]=0.0;
		}
		//make division step
		for(j=i+1;j<N;j++) mat[i][j]=mat[i][j]/mat[i][i];
		y[i]=y[i]/mat[i][i];
		mat[i][i]=1.0;
		//make settings for next row
		counters[i]=1;
		crit[i].Unlock();
		proccount1=i+P;
		for(k=replay;k<i;k++)
		{
			for(l=proccount1;l<N;l+=P)
			{
				for(j=(k+1);j<N;j++) mat[l][j]-=mat[l][k]*mat[k][j];
				y[l]-=y[k]*mat[l][k];
				mat[l][k]=0;
			}
		}
		replay=i;
	}
	//will be sincronize before back-substitution step
	barrier->barrier_sync();
	//back substitution
	for(i=who;i<N;i+=P) counters[i]=0;
	if(lastrow==1)
	{
		proccount=N-1-P;
		//modification vector is reset
		counters[N-1]=1;
		for(i=who;i<N-1;i+=P) crit[i].Lock();
	}
	else
	{
		proccount=who+P*(s-1);
		for(i=who;i<N;i+=P) crit[i].Lock();
	}
	barrier->barrier_sync();
	replay=N-1;
	for(i=proccount;i>=who;i-=P)
	{
		//for all rows before current row
		for(k=replay;k>i;k--)
		{
			if(counters[k]==0)
			{
				crit[k].Lock();
				crit[k].Unlock();
			}
			y[i]-=y[k]*mat[i][k];
		}
		counters[i]=1;
		crit[i].Unlock();
		proccount1=i-P;
		for(k=replay;k>i;k--)
		{
			for(l=proccount1;l>=who;l-=P)
			{
				//make back-substitution
				y[l]-=y[k]*mat[l][k];
			}
		}
		replay=i;
	}
	m_bAutoDelete=true;
	ExitInstance();
	return 0;
}
