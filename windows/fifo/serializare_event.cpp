// serializare_event.cpp : implementation file
//

#include "stdafx.h"
#include "fifo.h"
#include "serializare_event.h"
#include <iostream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// serializare_event

IMPLEMENT_DYNCREATE(serializare_event, CWinThread)

serializare_event::serializare_event()
{
}

serializare_event::~serializare_event()
{
}

serializare_event::serializare_event(int nr,CEvent *ev,int *indexare)
{
	index=*indexare;
	event=ev;
	threads=nr;
}

BOOL serializare_event::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int serializare_event::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(serializare_event, CWinThread)
	//{{AFX_MSG_MAP(serializare_event)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// serializare_event message handlers

int serializare_event::Run()
{
	HANDLE eveniment;
	eveniment=event[index].m_hObject;
	::WaitForSingleObject(eveniment,INFINITE);
	cout<<index<<' ';
	cout.flush();
	if(index!=0) event[index-1].SetEvent();
	ExitThread(0);
	return 0;
}
