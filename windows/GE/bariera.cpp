// bariera.cpp: implementation of the bariera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bariera.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bariera::bariera()
{

}

bariera::~bariera()
{

}
int bariera::barrier_init(int Count)
{
	bCond=new CEvent(false,true);
	nWaiters=0;
	bCount=Count;
	return 0;
}
int bariera::barrier_reset(int Count)
{
	bMutex.Lock();
	if(nWaiters>0)
	{
		bMutex.Lock();
		return(nWaiters);
	}
	bCount=Count;
	bMutex.Unlock();
	return 0;
}
int bariera::barrier_sync()
{
	bMutex.Lock();
	++nWaiters;
	if(nWaiters==bCount)
	{
		/* last thread tuch the barrier */
		nWaiters=0;
		bMutex.Unlock();
		bCond->PulseEvent();
		return 0;
	}
	else
	{
		bMutex.Unlock();
		HANDLE eveniment;
		eveniment=bCond->m_hObject;
		::WaitForSingleObject(eveniment,INFINITE);
		return 0;
	}
	return 0;
}
void bariera::barrier_destroy()
{
	delete [] bCond;
}
