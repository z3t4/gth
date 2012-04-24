



#if !defined(AFX_LOGEVENT_H__A3DE815F_0CAF_408A_9BE7_5D60F70E33D0__INCLUDED_)
#define AFX_LOGEVENT_H__A3DE815F_0CAF_408A_9BE7_5D60F70E33D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CLogEvent  
{
public:
	enum
	{
		GTH_EVENTLOG_SUCCESS=0,
		GTH_EVENTLOG_ERROR_TYPE,
		GTH_EVENTLOG_WARNING_TYPE,
		GTH_EVENTLOG_INFORMATION_TYPE,
		GTH_EVENTLOG_AUDIT_SUCCESS,
		GTH_EVENTLOG_AUDIT_FAILURE,
	};

	CLogEvent();
	virtual ~CLogEvent();

	void	Init();

	void	LogEvent(WORD wType,LPCTSTR lpszFormat, ...);
	void	WriteLog(WORD wType,const char* szMessage);

	BOOL	AddEventSource(
			   LPTSTR pszLogName, 
			   LPTSTR pszSrcName, 
			   LPTSTR pszMsgDLL,  
			   DWORD  dwNum);     

	void	ReStartLog();
public:
	HANDLE		m_hEventSource;

};

#endif 
