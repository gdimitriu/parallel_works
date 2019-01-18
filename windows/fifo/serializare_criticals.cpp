// serializare_criticals.cpp : implementation file
//

#include "stdafx.h"
#include "fifo.h"
#include "serializare_criticals.h"
#include <iostream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// serializare_criticals

IMPLEMENT_DYNCREATE(serializare_criticals, CWinThread)

serializare_criticals::serializare_criticals()
{
}

serializare_criticals::~serializare_criticals()
{
}

serializare_criticals::serializare_criticals(int nr,CCriticalSection *crit,int *indexare)
{
	critical=crit;
	threads=nr;
	index=*indexare;
}

BOOL serializare_criticals::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int serializare_criticals::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(serializare_criticals, CWinThread)
	//{{AFX_MSG_MAP(serializare_criticals)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// serializare_criticals message handlers

int serializare_criticals::Run()
{
	critical[index].Lock();
	cout<<index<<' ';
	cout.flush();
	critical[index].Unlock();
	ExitThread(0);
	return 0;
}
