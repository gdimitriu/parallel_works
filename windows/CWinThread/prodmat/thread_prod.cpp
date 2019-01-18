// thread_prod.cpp : implementation file
//

#include "stdafx.h"
#include "prodmat.h"
#include "thread_prod.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// thread_prod

IMPLEMENT_DYNCREATE(thread_prod, CWinThread)

thread_prod::thread_prod()
{
}

thread_prod::~thread_prod()
{
}

thread_prod::thread_prod(long dimensiune,int proc,double **mat0,double **mat1,double **mat2,int *indexare)
{
	dim=dimensiune;
	threads=proc;
	mata=mat0;
	matb=mat1;
	matc=mat2;
	index=*indexare;
}	

BOOL thread_prod::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int thread_prod::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(thread_prod, CWinThread)
	//{{AFX_MSG_MAP(thread_prod)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// thread_prod message handlers

int thread_prod::Run()
{
	long i,j,k;
	for(i=index;i<dim;i=i+threads)
		for(j=0;j<dim;j++)
		{
			matc[i][j]=0.0;
			for(k=0;k<dim;k++) matc[i][j]+=mata[i][k]*matb[k][j];
		}
	ExitThread(0);
	return 0;
}
