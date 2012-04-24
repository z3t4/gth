
#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>


#include "global.h"

CLogSystem::CLogSystem()
{
	m_updateTime = 0;
	m_generationHour = -1;

	m_fpLog = NULL;
	m_fpDebug = NULL;
	m_bCreateLog = FALSE;
	m_bCreateDebug = FALSE;
}

CLogSystem::~CLogSystem()
{
	if(NULL != m_fpLog){ fclose(m_fpLog); m_fpLog=NULL; }
	if(NULL != m_fpDebug){ fclose(m_fpDebug); m_fpDebug=NULL; }
}






void CLogSystem::Initialize(const BOOL in_bCreateLog,const BOOL in_bCreateDebug)
{
	
	m_updateTime = g_globalTime;
	m_updateCycle = 1000;  
	m_bCreateLog = in_bCreateLog;
	m_bCreateDebug = in_bCreateDebug; 
}





BOOL CLogSystem::Open(void)
{
	struct tm* date_tm=NULL;
	

	
    time( &m_logTime );
	date_tm = localtime( &m_logTime );
	if (m_generationHour != date_tm->tm_hour) 
	{
		m_generationHour = date_tm->tm_hour;	

		
		date_tm->tm_year += 1900;
		date_tm->tm_mon += 1;

		if(TRUE == m_bCreateLog ){
			sprintf(m_logFileName, "logs/MS%02d_%04d%02d%02d_%02d.log", 
				g_config.serverNo, date_tm->tm_year, date_tm->tm_mon, date_tm->tm_mday, date_tm->tm_hour);
		}

		if(TRUE == m_bCreateDebug ){
			sprintf(m_debugFileName, "debugs/MS%02d_%04d%02d%02d_%02d.dbg", 
				g_config.serverNo, date_tm->tm_year, date_tm->tm_mon, date_tm->tm_mday, date_tm->tm_hour);
		}
	}

	if(TRUE == m_bCreateLog ){
		m_fpLog = fopen( m_logFileName , "a+" ); 
		if(NULL == m_fpLog) return FALSE;		
	}

	if(TRUE == m_bCreateDebug ){
		m_fpDebug = fopen( m_debugFileName , "a+" ); 
		if(NULL == m_fpDebug) return FALSE;		
	}

	return true;
}


BOOL CLogSystem::Close(void)
{
	if(NULL != m_fpLog){ fclose(m_fpLog); m_fpLog=NULL; }
	if(NULL != m_fpDebug){ fclose(m_fpDebug); m_fpDebug=NULL; }
	return TRUE;
}


int CLogSystem::Update()
{
	if (m_updateTime > g_globalTime) return false;

	
	
	m_updateTime = g_globalTime + m_updateCycle;	

	
	if(FALSE == Close()) return FALSE;
	if(FALSE == Open()) return false;

	return true;
}



void CLogSystem::Write( const char *format, ... )
{
	if ( !m_bCreateDebug ) return;



	va_list argptr;	
	const int maxbytes=4095;
	char buffer[maxbytes+1];
	char cTime[26];

	
	va_start(argptr, format);	
	_vsnprintf(buffer, maxbytes, format, argptr);
	buffer[maxbytes]=NULL;

	va_end(argptr);
	

    time( &m_logTime );
	strcpy(cTime, ctime(&m_logTime)); 
	cTime[24] ='\0';	

	fprintf(m_fpDebug, "[%s] %s\n", cTime, buffer);
}



void CLogSystem::WriteToLog( const char *format, ... )
{
	if ( !m_bCreateLog ) return;

	va_list argptr;
	const int maxbytes=4095;
	char buffer[maxbytes+1];
	char cTimeStamp[30];
	timeStamp_t timeStamp;

	
	va_start(argptr, format);
	
	_vsnprintf(buffer, maxbytes, format, argptr);
	buffer[maxbytes]=NULL;
	va_end(argptr);

	GetTimeStamp( &timeStamp );





	sprintf(cTimeStamp, "%4d-%02d-%02d %02d:%02d:%02d", 
		timeStamp.year, timeStamp.month, timeStamp.day, timeStamp.hour, timeStamp.minute, timeStamp.second);

	fprintf(m_fpLog, "%s;%s\n", cTimeStamp, buffer);
}






void GetTimeStamp(timeStamp_t *timeStamp)
{
	time_t	curTime;
	struct tm *date_tm;

	
    time( &curTime );
	date_tm = localtime( &curTime );

	timeStamp->year = date_tm->tm_year += 1900;
	timeStamp->month = date_tm->tm_mon += 1;
	timeStamp->day = date_tm->tm_mday;
	timeStamp->hour = date_tm->tm_hour;
	timeStamp->minute = date_tm->tm_min;
	timeStamp->second = date_tm->tm_sec;
	timeStamp->fraction = 0; 
}
