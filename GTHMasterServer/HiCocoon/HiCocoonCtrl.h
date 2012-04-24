



#if !defined(AFX_HICOCOONCTRL_H__E11D0F86_3974_4625_BFB5_3527DCA2E2A1__INCLUDED_)
#define AFX_HICOCOONCTRL_H__E11D0F86_3974_4625_BFB5_3527DCA2E2A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "DaumGameAuth.h"

class CHiCocoonCtrl 
{	
private:
	BOOL	m_bOK;

private:
	CDaumGameAuth	m_DaumGameAuth;

public:
	enum tagCoCoonAUth
	{
		maxbytes_CoCoonAuth=200,
	};


public:
	BOOL			IsOK();	

public:
	BOOL			SetAuth(char* auth);
	BOOL			GetTimeExpired();
	CDaumGameAuth	GetDaumGameAuth();

	
	

public:
	CHiCocoonCtrl();
	virtual ~CHiCocoonCtrl();

};
BOOL ProcessHiCocoonLogin(CDaumGameAuth* DaumGameAuth, char* IN_Hicocoonauth, char* OUT_ID);




#endif 
