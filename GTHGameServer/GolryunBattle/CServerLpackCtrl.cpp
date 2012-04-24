



#include "..\global.h"
#include "CServerLpackCtrl.h"
#include <windowsx.h>

extern char *g_lTableToken[];

const char CServerLpackCtrl::m_OverflowMessageLine[MaxBytes_per_Line+1]="Server Lpack Overflow";


#include "CCSVFileCtrl.h"



CServerLpackCtrl::CServerLpackCtrl()
{
	m_bOK=FALSE;

	
	
	for(int idx=0; idx < MaxLineNum; idx++){
		strncpy(m_LineBuffer[idx],m_OverflowMessageLine,MaxBytes_per_Line);
		m_LineBuffer[idx][MaxBytes_per_Line]=NULL;
	}



	if(FALSE == Init()) return;

	m_bOK=TRUE;

}






CServerLpackCtrl::~CServerLpackCtrl()
{

}




BOOL CServerLpackCtrl::Init(void)
{
	char szFileName[_MAX_PATH];
	wsprintf(szFileName,"l_tables/ServerLpack%s.csv",g_lTableToken[g_config.languageType]);
	if(FALSE == LoadFile(szFileName)) return FALSE;

	return TRUE;
}





BOOL CServerLpackCtrl::isOK(void) const
{
	return m_bOK;
}





char* CServerLpackCtrl::GetLpack(const unsigned int Idx)
{
	if(Idx < 0) return (char*)m_OverflowMessageLine;
	if(Idx >= MaxLineNum) return (char*)m_OverflowMessageLine;
	return m_LineBuffer[Idx];
}








BOOL CServerLpackCtrl::LoadFile(const char* szpFileName)
{
	CCSVFileCtrl* cpCSVFileCtrl=NULL;
	cpCSVFileCtrl = new CCSVFileCtrl(szpFileName);
	if(NULL == cpCSVFileCtrl) return FALSE;
	if(FALSE == cpCSVFileCtrl->isOK()){ delete cpCSVFileCtrl; cpCSVFileCtrl=NULL; return FALSE; }


	int idx=0;
	const unsigned char* pToken=NULL;
	while(TRUE){
		pToken = cpCSVFileCtrl->NextToken();
		if(NULL == pToken) break;		

		idx = atoi((char*)pToken);
		if(idx < 0) break;
		if(idx >= MaxLineNum) break;

		pToken = cpCSVFileCtrl->NextToken();
		if(NULL == pToken) break;		
		
		strncpy(m_LineBuffer[idx],(char*)pToken,MaxBytes_per_Line);
		m_LineBuffer[idx][MaxBytes_per_Line]=NULL;		
	}



	if(NULL != cpCSVFileCtrl){ delete cpCSVFileCtrl; cpCSVFileCtrl=NULL; }	
	return TRUE;
}
