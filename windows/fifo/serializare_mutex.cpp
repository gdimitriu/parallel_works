// serializare_mutex.cpp : implementation file
//

#include "stdafx.h"
#include <iostream.h>
#include "fifo.h"
#include "serializare_mutex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// serializare_mutex

IMPLEMENT_DYNCREATE(serializare_mutex, CWinThread)

serializare_mutex::serializare_mutex()
{
}

serializare_mutex::~serializare_mutex()
{
}

BOOL serializare_mutex::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

serializare_mutex::serializare_mutex(int nr,CMutex *mtx,int *indexare)
{
	threads=nr;
	mutexuri=mtx;
	index=*indexare;
}

int serializare_mutex::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

int serializare_mutex::Run()
{
	mutexuri[index].Lock();
	cout<<index<<' ';
	cout.flush();
	mutexuri[index].Unlock();
	ExitThread(0);
	return 0;
}
BEGIN_MESSAGE_MAP(serializare_mutex, CWinThread)
	//{{AFX_MSG_MAP(serializare_mutex)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// serializare_mutex message handlers
