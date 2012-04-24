








#include "CriticalSection.h"





CCriticalSection::CCriticalSection ()
{
	::InitializeCriticalSection (&m_cs) ;
}

CCriticalSection::~CCriticalSection ()
{
	::DeleteCriticalSection (&m_cs) ;
}



void CCriticalSection::Enter ()
{
	::EnterCriticalSection (&m_cs) ;
}

void CCriticalSection::Leave ()
{
	::LeaveCriticalSection (&m_cs) ;
}





CCriticalSection::Owner::Owner (CCriticalSection& cs)
: m_crit (cs)
{
	m_crit.Enter () ;
}

CCriticalSection::Owner::~Owner ()
{
	m_crit.Leave () ;
}


