



#include "CLogFileCtrl.h"
#include <TIME.H>










CLogFileCtrl::CLogFileCtrl()
{
	m_fp = NULL;
	m_bActive = FALSE;
	InitializeCriticalSection(&m_critcalsection);
}






CLogFileCtrl::~CLogFileCtrl()
{
	DeleteCriticalSection(&m_critcalsection);

	if( NULL != m_fp)
	{	
		fclose(m_fp); 
		m_fp = NULL;
	}


}






void CLogFileCtrl::SetActive(const BOOL in_bActive)
{
	m_bActive = in_bActive;
}







BOOL CLogFileCtrl::IsActive(void)
{
	return m_bActive;
}







BOOL CLogFileCtrl::Open(const char* in_strfileName)
{
	
	EnterCriticalSection(&m_critcalsection);		

	if ( FALSE == m_bActive)
	{
		LeaveCriticalSection(&m_critcalsection);
		return FALSE;
	}

	if(NULL != m_fp)
	{
		fclose(m_fp); 
		m_fp=NULL; 
	}
	
	m_fp = fopen( in_strfileName, "a+" ); 
	if(NULL == m_fp )	
	{
		LeaveCriticalSection(&m_critcalsection);
		return FALSE;
	}


	LeaveCriticalSection(&m_critcalsection);
	return TRUE;
}







BOOL CLogFileCtrl::Close(void)
{
	EnterCriticalSection(&m_critcalsection);
	
	if(NULL != m_fp)
	{
		fclose(m_fp); 
		m_fp=NULL; 
	}

	LeaveCriticalSection(&m_critcalsection);

	return TRUE;
}






void CLogFileCtrl::WriteLog(const char* in_strMessage, const char* in_strUserInfo)
{
	EnterCriticalSection(&m_critcalsection);

	if(FALSE == m_bActive) 
	{
		LeaveCriticalSection(&m_critcalsection);
		return;
	}
	if(NULL == m_fp)
	{
		LeaveCriticalSection(&m_critcalsection);
		return;
	}

	char strTimeStamp[30]="";
	
	timeStamp_t timeStamp;
	GetTimeStamp( &timeStamp );

	sprintf(strTimeStamp, "%4d-%02d-%02d %02d:%02d:%02d", 
	timeStamp.year, timeStamp.month, timeStamp.day, timeStamp.hour, timeStamp.minute, timeStamp.second);
	
	strTimeStamp[29] = NULL;
	

	char MessageBUffer[MAX_MESSAGE_BUFFER];
	char LogBuffer[MAX_LOG_BUFFER];
	char UserBuffer[MAX_USERINFO_BUFFER];

	memset(MessageBUffer, 0x00, MAX_MESSAGE_BUFFER);
	memset(LogBuffer, 0x00, MAX_LOG_BUFFER);	
	memset(UserBuffer, 0x00, MAX_USERINFO_BUFFER);

	if ( NULL != in_strMessage)
	{
		_snprintf(MessageBUffer, MAX_MESSAGE_BUFFER, in_strMessage);
		MessageBUffer[MAX_MESSAGE_BUFFER-1] = NULL;
	}
	if ( NULL != in_strUserInfo)
	{
		_snprintf(UserBuffer, MAX_MESSAGE_BUFFER, in_strUserInfo);
		UserBuffer[MAX_USERINFO_BUFFER-1] = NULL;
	}

	
	int size = strlen(UserBuffer);

	if ( NULL == in_strUserInfo)
		fprintf(m_fp, "%s;%s\n", strTimeStamp, in_strMessage);
	else	                                                                                                  
		fprintf(m_fp, "%s;%s%s\n", strTimeStamp, in_strUserInfo, in_strMessage);
	LogBuffer[MAX_LOG_BUFFER-1] = NULL;

	





	if ( m_fp != NULL)
	fprintf(m_fp, LogBuffer);

	LeaveCriticalSection(&m_critcalsection);
	return;

}






void CLogFileCtrl::GetTimeStamp(timeStamp_t* out_pTimeStamp)
{
	time_t	curTime;
	struct tm* date_tm=NULL;

	
    time( &curTime );
	date_tm = localtime( &curTime );

	out_pTimeStamp->year		= date_tm->tm_year += 1900;
	out_pTimeStamp->month		= date_tm->tm_mon += 1;
	out_pTimeStamp->day			= date_tm->tm_mday;
	out_pTimeStamp->hour		= date_tm->tm_hour;
	out_pTimeStamp->minute		= date_tm->tm_min;
	out_pTimeStamp->second		= date_tm->tm_sec;
	out_pTimeStamp->fraction	= 0; 
}
