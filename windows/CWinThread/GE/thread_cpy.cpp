// thread_cpy.cpp : implementation file
//

#include "stdafx.h"
#include "GE.h"
#include "thread_cpy.h"
#include <memory.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// thread_cpy

IMPLEMENT_DYNCREATE(thread_cpy, CWinThread)

thread_cpy::thread_cpy()
{
}

thread_cpy::thread_cpy(long dim,int threads,double *y,double *y1,double **mat,double **mat1,long *index)
{
	dimensiune=dim;
	thread=threads;
	freev=y;
	freev1=y1;
	matr1=mat1;
	matr=mat;
	pos=*index;
}
thread_cpy::~thread_cpy()
{
}

BOOL thread_cpy::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int thread_cpy::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(thread_cpy, CWinThread)
	//{{AFX_MSG_MAP(thread_cpy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// thread_cpy message handlers

int thread_cpy::Run()
{
	long i;
	for(i=(long)pos;i<dimensiune;i=i+(long)thread)
	{
		memcpy(matr1[i],matr[i],dimensiune*sizeof(double));
		freev1[i]=freev[i];
	}
	m_bAutoDelete=true;
	ExitInstance();
	return 0;
}
