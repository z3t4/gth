



#include "stdafx.h"
#include "CGameVersion.h"





CGameVersion::CGameVersion()
{		

	

	
	
	
	
	
	
	
	m_iClientGameVersion = 2.12 ;
	strncpy(m_szMasterServerVersion, "lol", VERSIONSTRING);
	
	
	
	
	
	
	m_szMasterServerVersion[VERSIONSTRING] = NULL;	
}


CGameVersion::~CGameVersion()
{

}
