





#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>


#include "..\global.h"
#include "CLogSystem.h"




CLogSystem::CLogSystem()
{
	m_updateTime = 0;
	m_generationHour = -1;
}





CLogSystem::~CLogSystem()
{

}











void CLogSystem::Initialize(const BOOL in_bCreateLog,const BOOL in_bCreateDebug)
{
	
	m_updateTime = g_globalTime;
	m_updateCycle = 1000;  

	m_UserLogFileCtrl.SetActive(in_bCreateLog);
	m_DebugLogFileCtrl.SetActive(in_bCreateDebug);
}






void CLogSystem::Open(void)
{
	struct tm* date_tm=NULL;
	

	time_t	logTime;
    time( &logTime );
	date_tm = localtime( &logTime );

	if(m_generationHour != date_tm->tm_hour){

		m_generationHour = date_tm->tm_hour;	
		
		date_tm->tm_year += 1900;
		date_tm->tm_mon  += 1;

		
		sprintf(m_logFileName, "logs/MS%02d_%04d%02d%02d_%02d.log", 
			g_config.serverNo, 
			date_tm->tm_year, 
			date_tm->tm_mon, 
			date_tm->tm_mday, 
			date_tm->tm_hour);


		
		sprintf(m_debugFileName, "debugs/MS%02d_%04d%02d%02d_%02d.dbg", 
			g_config.serverNo, 
			date_tm->tm_year, 
			date_tm->tm_mon, 
			date_tm->tm_mday, 
			date_tm->tm_hour);		
	}

	m_UserLogFileCtrl.Open(m_logFileName);
	m_DebugLogFileCtrl.Open(m_debugFileName);
}









void CLogSystem::Close(void)
{
	m_UserLogFileCtrl.Close();
	m_DebugLogFileCtrl.Close();	
}




void CLogSystem::Update(void)
{
	if (m_updateTime > g_globalTime) return;

	
	
	m_updateTime = g_globalTime + m_updateCycle;	

	
	Close();
	Open();
}




void CLogSystem::Write( const char *format, ... )
{
	if(FALSE == m_DebugLogFileCtrl.IsActive()) return;

	va_list argptr;	
	const int maxbytes=4095;
	char buffer[maxbytes+1];
	

	
	va_start(argptr, format);	
	_vsnprintf(buffer, maxbytes, format, argptr);
	buffer[maxbytes]=NULL;
	va_end(argptr);
	
	
	m_DebugLogFileCtrl.WriteLog(buffer);
}





void CLogSystem::WriteToLog( const char *format, ... )
{
	

	if(FALSE == m_UserLogFileCtrl.IsActive()) return;

	va_list argptr;
	const int maxbytes=4095;
	char buffer[maxbytes+1];
	
	

	
	va_start(argptr, format);
	
	_vsnprintf(buffer, maxbytes, format, argptr);
	buffer[maxbytes]=NULL;
	va_end(argptr);


	


	m_UserLogFileCtrl.WriteLog(buffer);
}


