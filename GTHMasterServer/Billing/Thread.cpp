








#include "Thread.h"
#include <process.h>



CThread::CThread ()
: m_hThread (INVALID_HANDLE_VALUE)
{
}

CThread::~CThread ()
{
	if (m_hThread != INVALID_HANDLE_VALUE)
	{
		::CloseHandle (m_hThread) ;
	}
}

HANDLE CThread::GetHandle () const
{
	return m_hThread ;
}

bool CThread::Start ()
{
	if (m_hThread == INVALID_HANDLE_VALUE)
	{
		unsigned int threadID = 0 ;

		m_hThread = (HANDLE)::_beginthreadex (0, 0, ThreadFunction, (void*)this, 0, &threadID) ;

		if (m_hThread == INVALID_HANDLE_VALUE)
		{
			return false ;
		}
	}
	else
	{
		return false ;
	}

	return true ;
}

bool CThread::Wait () const
{
	if (!Wait (INFINITE))
	{
		return false ;
	}

	return true ;
}

bool CThread::Wait (DWORD timeoutMillis) const
{
	bool ok ;

	DWORD result = ::WaitForSingleObject (m_hThread, timeoutMillis) ;
	
	if (WAIT_TIMEOUT == result)
	{
		ok = false ;
	}
	else if (WAIT_OBJECT_0 == result)
	{
		ok = true ;
	}
	else
	{
		ok = false ;
	}

	return ok ;
}

unsigned int __stdcall CThread::ThreadFunction (void* pV)
{
	int result = 0 ;

	CThread* pThis = (CThread*)pV ;

	if (pThis)
	{
		result = pThis->Run () ;
	}

	return result ;
}


