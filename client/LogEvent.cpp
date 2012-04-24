


#include "global_def.h"
#include  <Resapi.h>
#include "LogEvent.h"


CLogEvent gLogEvent;





CLogEvent::CLogEvent()
{
#ifdef _DEBUG
	Init();
#endif
}

CLogEvent::~CLogEvent()
{
#ifdef _DEBUG
	if(NULL != m_hEventSource) 
	{	
		::DeregisterEventSource(m_hEventSource); 
	}
#endif
}

void	 CLogEvent::Init()
{
#ifdef _DEBUG
	m_hEventSource = NULL;

	AddEventSource("GTH log","GTHGameserver",NULL,1);
    m_hEventSource = ::RegisterEventSource(NULL,"GTH log"); 
#endif
}


BOOL CLogEvent::AddEventSource(
   LPTSTR pszLogName, 
   LPTSTR pszSrcName, 
   LPTSTR pszMsgDLL,  
   DWORD  dwNum)      
{
   HKEY hk; 
   DWORD dwData, dwDisp; 
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
 
   
   if (RegSetValueEx(hk,              
           "EventMessageFile",        
           0,                         
           REG_EXPAND_SZ,             
           (LPBYTE) pszMsgDLL,        
           (DWORD) lstrlen(pszMsgDLL)+1)) 
   {
      printf("Could not set the event message file."); 
      return FALSE;
   }
 
   
 
   dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | 
        EVENTLOG_INFORMATION_TYPE; 
 
   if (RegSetValueEx(hk,      
           "TypesSupported",  
           0,                 
           REG_DWORD,         
           (LPBYTE) &dwData,  
           sizeof(DWORD)))    
   {
      printf("Could not set the supported types."); 
      return FALSE;
   }
 
   

   if (RegSetValueEx(hk,              
           "CategoryMessageFile",     
           0,                         
           REG_EXPAND_SZ,             
           (LPBYTE) pszMsgDLL,        
           (DWORD) lstrlen(pszMsgDLL)+1)) 
   {
      printf("Could not set the category message file."); 
      return FALSE;
   }
 
   if (RegSetValueEx(hk,      
           "CategoryCount",   
           0,                 
           REG_DWORD,         
           (LPBYTE) &dwNum,   
           sizeof(DWORD)))    
   {
      printf("Could not set the category count."); 
      return FALSE;
   }

   RegCloseKey(hk); 
   return TRUE;
}

void CLogEvent::WriteLog(WORD wType,const char* szMessage)
{
#ifdef _DEBUG
	::ReportEvent(m_hEventSource,
				  wType,
				  1,
				  0,
				  NULL,   
				  1,
				  0,
				  &szMessage,
				  NULL);
#endif
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