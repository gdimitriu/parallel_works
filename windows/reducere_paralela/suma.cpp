// suma.cpp : implementation file
//

#include "stdafx.h"
#include "reducere_paralela.h"
#include "suma.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// suma

IMPLEMENT_DYNCREATE(suma, CWinThread)

suma::suma()
{
}
suma::suma(long dimensiune,int nrthreads,int *vectori,CMutex *mutexuri,long *paralel,int *poz)
{
	dim=dimensiune;
	threads=nrthreads;
	vector=vectori;
	mutex=mutexuri;
	rezultat=paralel;
	index=*poz;
}
suma::~suma()
{
}

BOOL suma::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int suma::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(suma, CWinThread)
	//{{AFX_MSG_MAP(suma)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// suma message handlers

int suma::Run()
{
	long local;
	local=0;
	for(long i=index;i<dim;i=i+threads) local+=vector[i];
	mutex->Lock();
	*rezultat+=local;
	mutex->Unlock();
	ExitThread(0);
	return 0;
}
