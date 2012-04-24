


#include "global.h"
#include  <Resapi.h>
#include "LogEvent.h"


CLogEvent gLogEvent;





CLogEvent::CLogEvent()
{
	Init();
}

CLogEvent::~CLogEvent()
{
	if(NULL != m_hEventSource) 
	{	
		::DeregisterEventSource(m_hEventSource); 
	}
}

void	 CLogEvent::Init()
{
	m_hEventSource = NULL;

	AddEventSource("GTH LOG","GTHGameserver",NULL,1);
    m_hEventSource = ::RegisterEventSource(NULL,"GTH LOG"); 
}


BOOL CLogEvent::AddEventSource(
   LPTSTR pszLogName, 
   LPTSTR pszSrcName, 
   LPTSTR pszMsgDLL,  
   DWORD  dwNum)      
{
   HKEY hk; 
   DWORD dwDisp; 
   TCHAR szBuf[MAX_PATH]; 

   
   wsprintf(szBuf, 
      "SYSTEM\\CurrentControlSet\\Services\\EventLog\\%s\\%s",
      pszLogName, pszSrcName); 
 
   if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, szBuf, 
          0, NULL, REG_OPTION_NON_VOLATILE,
          KEY_WRITE, NULL, &hk, &dwDisp)) 
   {
      printf("Could not create the registry key."); 
      return FALSE;
   }
 
 
   
   RegCloseKey(hk); 
   return TRUE;
}

void CLogEvent::WriteLog(WORD wType,const char* szMessage)
{
	::ReportEvent(m_hEventSource,
				  wType,
				  1,
				  0,
				  NULL,   
				  1,
				  0,
				  &szMessage,
				  NULL);
	return;
}

void CLogEvent::LogEvent(WORD wType,LPCTSTR lpszFormat, ...)
{
#ifdef _DEBUG

	va_list argList;
	va_start(argList, lpszFormat);

	char szOutputMessage[1024];

	vsprintf(szOutputMessage,lpszFormat, argList);
	va_end(argList);

	WriteLog(wType,szOutputMessage);

#endif
	
}

void CLogEvent::ReStartLog()
{
	::ClearEventLog(m_hEventSource,"gthlog.log");
}