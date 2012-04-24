








#pragma once

#include <windows.h>





class CThread
{
public:
	CThread () ;
	virtual ~CThread () ;

	HANDLE GetHandle () const ;
	
	bool Wait () const ;
	bool Wait (DWORD timeoutMillis) const ;

	bool Start () ;

private:
	virtual int Run () = 0 ;
	static unsigned int __stdcall ThreadFunction (void* pV) ;

	HANDLE m_hThread ;

	
	CThread (const CThread& rhs) ;
	CThread &operator= (const CThread& rhs) ;
} ;


