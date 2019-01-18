// reducere.cpp: implementation of the reducere class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "reducere_paralela.h"
#include "reducere.h"
#include <iostream.h>
#include "suma.h"
#include <sys/timeb.h>
#include <time.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

reducere::reducere()
{
	long i;
	bool flag;
	cout<<"Cite thread-uri aveti ";cout.flush();
	cin>>threads;
	cout<<"Ce dimensiune are vectorul ";cout.flush();
	cin>>dim;
	flag=true;
	while(flag)
	{
		cout<<"Introduceti date (y) sau generare in ordine (o) sau aleatoare (a)";cout.flush();
		char test;
		cin>>test;
		vector=new int[dim];
		switch(test)
		{
		case 'y':
			for(i=0;i<dim;i++) cin>>vector[i];
			flag=false;
			break;
		case 'o':
			for(i=0;i<dim;i++) vector[i]=i;
			flag=false;
			break;
		case 'a':
			for(i=0;i<dim;i++) vector[i]=rand();
			flag=false;
			break;
		default:
			cout<<"Nu ati introdus ce trebuie";
			break;
		}
	}
	LARGE_INTEGER times1,times2,timep1,timep2;
	QueryPerformanceCounter(&times1);
	reducere_seriala();
	QueryPerformanceCounter(&times2);
	QueryPerformanceCounter(&timep1);
	reducere_paralela();
	QueryPerformanceCounter(&timep2);
	cout<<"Serial "<<serial<<" Parallel "<<paralel<<endl;
	if(serial!=paralel) cout<<"Eroare de calcul";
	timp(times1,times2,timep1,timep2);
	cout.flush();
}

reducere::~reducere()
{
	delete vector;
}

void reducere::reducere_seriala()
{
	serial=0;
	for(long i=0;i<dim;i++) serial+=vector[i];
}

void reducere::reducere_paralela()
{
	CMutex mutex;
	HANDLE *hthreads;
	hthreads=new HANDLE[threads];
	suma *thread;
	int *ordine;
	ordine=new int[threads];
	paralel=0;
	for(int i=0;i<threads;i++)
	{
		ordine[i]=i;
		thread=new suma(dim,threads,vector,&mutex,&paralel,&ordine[i]);
		thread->CreateThread(0,0);
		hthreads[i]=thread->m_hThread;
	}
	::WaitForMultipleObjects(threads,hthreads,TRUE,INFINITE);
	delete hthreads;
	delete ordine;
}

void reducere::timp(LARGE_INTEGER val1,LARGE_INTEGER val2,LARGE_INTEGER val3,LARGE_INTEGER val4)
{
	LARGE_INTEGER freq;
	double ser=0.0,par=0.0;
	QueryPerformanceFrequency(&freq);
	if((val2.HighPart-val1.HighPart)==0)
	{
		ser=(double)(val2.LowPart-val1.LowPart)/(double)freq.LowPart;
		cout<<"serial "<<dim<<' '<<ser<<endl;
	}
	if((val4.HighPart-val3.HighPart)==0)
	{
		par=(double)(val4.LowPart-val3.LowPart)/(double)freq.LowPart;
		cout<<"parallel "<<dim<<' '<<par<<endl;
	}
	if(par!=0.0 && ser!=0.0)
		cout<<"Speedup "<<dim<<' '<<ser/par<<endl;
	cout.flush();
}
