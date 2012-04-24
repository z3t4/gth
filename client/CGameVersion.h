



#if !defined(AFX_CGAMEVERSION_H__87673604_4663_491F_A3F1_0E2C0B1B0B35__INCLUDED_)
#define AFX_CGAMEVERSION_H__87673604_4663_491F_A3F1_0E2C0B1B0B35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define VERSIONSTRING 20

class CGameVersion
{
public:	
	float m_iClientGameVersion;
	char m_szMasterServerVersion[VERSIONSTRING+1];

public:	
	CGameVersion();
	virtual ~CGameVersion();

};

#endif 
