// serializare_bariera.cpp : implementation file
//

#include "stdafx.h"
#include "fifo.h"
#include "serializare_bariera.h"
#include <iostream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// serializare_bariera

IMPLEMENT_DYNCREATE(serializare_bariera, CWinThread)

serializare_bariera::serializare_bariera()
{
}

serializare_bariera::~serializare_bariera()
{
}

serializare_bariera::serializare_bariera(int nr,CSemaphore *mtx,int *indexare,bariera *bar)
{
	threads=nr;
	sem=mtx;
	index=*indexare;
	barrier=bar;
}

BOOL serializare_bariera::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int serializare_bariera::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(serializare_bariera, CWinThread)
	//{{AFX_MSG_MAP(serializare_bariera)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// serializare_bariera message handlers
int serializare_bariera::Run()
{
	sem[index].Lock();
	cout<<index<<' ';
	cout.flush();
	if(index!=0/*(threads-1)*/) sem[index-1].Unlock();
	barrier->barrier_sync();
	cout<<"A trecut de bariera "<<index<<endl;
	cout.flush();
	ExitThread(0);
	return 0;
}