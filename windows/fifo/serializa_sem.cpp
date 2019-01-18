// serializa_sem.cpp : implementation file
//

#include "stdafx.h"
#include "fifo.h"
#include "serializa_sem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// serializa_sem

IMPLEMENT_DYNCREATE(serializa_sem, CWinThread)

serializa_sem::serializa_sem()
{
}

serializa_sem::~serializa_sem()
{
}

BOOL serializa_sem::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int serializa_sem::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(serializa_sem, CWinThread)
	//{{AFX_MSG_MAP(serializa_sem)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// serializa_sem message handlers
