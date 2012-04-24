



#if !defined(AFX_CLOGFILECTRL_H__509985E6_F7C3_4BA8_9399_5E5D4617A358__INCLUDED_)
#define AFX_CLOGFILECTRL_H__509985E6_F7C3_4BA8_9399_5E5D4617A358__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include <WINDOWS.H>
#include <STDIO.H>
#include <sqltypes.h>

class CLogFileCtrl
{

protected:
	typedef struct 
	{
		SQLSMALLINT		year; 
		SQLUSMALLINT	month; 
		SQLUSMALLINT	day; 
		SQLUSMALLINT	hour; 
		SQLUSMALLINT	minute; 
		SQLUSMALLINT	second; 
		SQLUINTEGER		fraction;
	} timeStamp_t;

protected:
	FILE*				m_fp;
	BOOL				m_bActive;
	CRITICAL_SECTION	m_critcalsection;	

	
public:
	BOOL	Open(const char* in_strfileName);
	BOOL	Close(void);

	enum
	{
			MAX_MESSAGE_BUFFER  = 4096,
			MAX_USERINFO_BUFFER = 4096,
			MAX_LOG_BUFFER		= MAX_MESSAGE_BUFFER + MAX_USERINFO_BUFFER,
	};

public:
	void	SetActive(const BOOL in_bActive);
	BOOL	IsActive(void);

	
	void GetTimeStamp(timeStamp_t* out_pTimeStamp);


public:
	void WriteLog(const char* in_strMessage, const char* in_strUserInfo = NULL);	


public:
	CLogFileCtrl();
	~CLogFileCtrl();	
};



#endif 
