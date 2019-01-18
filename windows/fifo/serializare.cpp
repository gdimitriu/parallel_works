// serializare.cpp: implementation of the serializare class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fifo.h"
#include "serializare.h"
#include <iostream.h>
#include "serializare_sem.h"
#include "serializare_mutex.h"
#include "serializare_event.h"
#include "serializare_criticals.h"
#include "serializare_bariera.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

serializare::serializare()
{
	int i;
	//serializare cu semafoare
	cout<<"Serializare cu threaduri\n";cout.flush();
	cout<<"Serializare cu semafoare\n";cout.flush();
	serializare_sem *threads;
	for(i=0;i<N;i++) hthreads[i]=NULL;
	for(i=0;i<N;i++) sem[i].Lock(); //blochez threadurile
	sem[N-1].Unlock(); //deblochez ultimul thread
	for(i=0;i<N;i++)
	{
		ordine[i]=i;
		threads=new serializare_sem(N,sem); //aloc memorie
		threads->InitInstance(&ordine[i]);
		threads->CreateThread(0,0); //lansez thread
		hthreads[i]=threads->m_hThread;//capturez handler
	}
	//join
	::WaitForMultipleObjects(N,hthreads,TRUE,INFINITE);
	//serializare cu mutexuri
	cout<<endl<<"Serializare cu mutexuri\n";cout.flush();
	serializare_mutex *threadm;
	for(i=0;i<N;i++) hthreads[i]=NULL;
	for(i=0;i<N;i++) mutex[i].Lock(); //blochez threadurile
	for(i=0;i<N;i++)
	{
		ordine[i]=i;
		threadm=new serializare_mutex(N,mutex,&ordine[i]);
		threadm->CreateThread(0,0); //lansez thread
		hthreads[i]=threadm->m_hThread; //capturez handler
	}
	mutex[N-1].Unlock(); //deblochez ultimul thread
	for(i=N-1;i>=0;i--)
	{
		::WaitForSingleObject(hthreads[i],INFINITE);
		if(i!=0) mutex[i-1].Unlock(); //deblochez urmatorul thread
	}
	cout<<endl;cout.flush();
	for(i=0;i<N;i++) hthreads[i]=NULL;
	for(i=0;i<N;i++) mutex[i].Lock(); //blochez threadurile
	for(i=0;i<N;i++)
	{
		ordine[i]=i;
		threadm=new serializare_mutex(N,mutex,&ordine[i]);
		threadm->CreateThread(0,0); //lansez thread
		hthreads[i]=threadm->m_hThread; //capturez handler
	}
	mutex[0].Unlock(); //deblochez primul thread
	for(i=0;i<N;i++)
	{
		::WaitForSingleObject(hthreads[i],INFINITE);
		if(i<(N-1)) mutex[i+1].Unlock(); //deblochez urmatorul thread
	}
	cout<<endl;cout.flush();
	//serializare cu eveniment
	cout<<"Serializare cu evenimente\n";cout.flush();
	serializare_event *threade;
	for(i=0;i<N;i++) hthreads[i]=NULL;
	for(i=0;i<N;i++)
	{
		ordine[i]=i;
		threade=new serializare_event(N,event,&ordine[i]);
		threade->CreateThread(0,0); //lansez thread
		hthreads[i]=threade->m_hThread;//capturez handler
	}
	event[N-1].SetEvent();//eliberez ultimul thread
	//join
	::WaitForMultipleObjects(N,hthreads,TRUE,INFINITE);
	for(i=0;i<N;i++) event[i].Unlock(); //eliberez evenimetele
	cout<<endl;cout.flush();
	//serializare cu critical section
	cout<<endl<<"Serializare cu critical section\n";cout.flush();
	serializare_criticals *threadc;
	for(i=0;i<N;i++) hthreads[i]=NULL;
	for(i=0;i<N;i++) section[i].Lock(); //blochez threaduri
	for(i=0;i<N;i++)
	{
		ordine[i]=i;
		threadc=new serializare_criticals(N,section,&ordine[i]);
		threadc->CreateThread(0,0);//lansez thread
		hthreads[i]=threadc->m_hThread; //capturez hadler
	}
	section[N-1].Unlock(); //deblochez utimul thread
	for(i=N-1;i>=0;i--)
	{
		::WaitForSingleObject(hthreads[i],INFINITE);
		if(i!=0) section[i-1].Unlock(); //deblochez urmatorul thread
	}
	cout<<endl;cout.flush();
	for(i=0;i<N;i++) hthreads[i]=NULL;
	for(i=0;i<N;i++) section[i].Lock(); //blochez threadurile
	for(i=0;i<N;i++)
	{
		ordine[i]=i;
		threadc=new serializare_criticals(N,section,&ordine[i]);
		threadc->CreateThread(0,0);//lansez thread
		hthreads[i]=threadc->m_hThread;//capturez handler
	}
	section[0].Unlock();//deblochez primul thread
	for(i=0;i<N;i++)
	{
		::WaitForSingleObject(hthreads[i],INFINITE);
		if(i<(N-1)) section[i+1].Unlock(); //deblochez urmatorul thread
	}
	cout<<endl;cout.flush();
	cout<<"Serializare cu bariera\n";cout.flush();
	serializare_bariera *threadb;
	bariera bar;
	CSemaphore sema[N];
	bar.barrier_init(N);
	for(i=0;i<N;i++) hthreads[i]=NULL;
	for(i=0;i<N;i++) sema[i].Lock(); //blochez threadurile
	sema[N-1].Unlock(); //deblochez ultimul thread
	for(i=0;i<N;i++)
	{
		ordine[i]=i;
		threadb=new serializare_bariera(N,sema,&ordine[i],&bar);
		threadb->CreateThread(0,0); //lansez thread
		hthreads[i]=threadb->m_hThread; //capturez handler
	}
	::WaitForMultipleObjects(N,hthreads,TRUE,INFINITE);
	bar.barrier_destroy();
	cout<<endl;cout.flush();
}

serializare::~serializare()
{

}
