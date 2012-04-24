#if _MSC_VER >= 1000
#pragma once
#endif

#ifndef _CDAUMGAMEAUTH_H_
#define _CDAUMGAMEAUTH_H_

#include "DaumGameCrypt.h"

class CDaumGameAuth
{
public:
	CDaumGameAuth( void );
	~CDaumGameAuth( void );

	BOOL	Init( IN LPCTSTR szKey );
	BOOL	SetSource( IN LPCTSTR szSourceString );
	BOOL	GetData( IN LPCTSTR szKeyName, OUT LPTSTR szBuffer, IN int nMaxLength );
	BOOL	IsTimeExpired( void );
	CDaumGameCrypt	*GetGameCrypt(){return m_pCryptor;};

protected:
	enum {
		MAX_SOURCE_LENGTH	= 1024,
		MAX_KEY_LENGTH		= 256
	};
	enum {
		EXPIRED_SECS		= 4 * 60 * 60		
	};

protected:
	CDaumGameCrypt	*m_pCryptor;
	TCHAR			m_szSourceString[MAX_SOURCE_LENGTH];

};

#endif 
