








#pragma once

#include <windows.h>





class CCriticalSection
{
public:
	class Owner
	{
	public:
		explicit Owner (CCriticalSection& crit) ;
		~Owner () ;

	private:
		CCriticalSection& m_crit ;

		
		Owner (const Owner& rhs) ;
		Owner &operator= (const Owner& rhs) ;
	} ;

	CCriticalSection () ;
	~CCriticalSection () ;


	void Enter () ;
	void Leave () ;

private:
	CRITICAL_SECTION m_cs ;
} ;


