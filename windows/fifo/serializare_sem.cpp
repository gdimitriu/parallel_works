// serializare_sem.cpp : implementation file
//

#include "stdafx.h"
#include <iostream.h>
#include "fifo.h"
#include "serializare_sem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// serializare_sem

IMPLEMENT_DYNCREATE(serializare_sem, CWinThread)

serializare_sem::serializare_sem()
{
}

serializare_sem::~serializare_sem()
{
}

serializare_sem::serializare_sem(int nr,CSemaphore *semaphore)
{
	threads=nr;
	sem=semaphore;
}
BOOL serializare_sem::InitInstance(int *indexare)
{
	// TODO:  perform and per-thread initialization here
	index=*indexare;
	return TRUE;
}

BOOL serializare_sem::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int serializare_sem::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}
int serializare_sem::Run()
{
	sem[index].Lock();
	cout<<index<<' ';
	cout.flush();
	if(index!=0/*(threads-1)*/) sem[index-1].Unlock();
	ExitThread(0);
	return 0;
}
BEGIN_MESSAGE_MAP(serializare_sem, CWinThread)
	//{{AFX_MSG_MAP(serializare_sem)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// serializare_sem message handlers
