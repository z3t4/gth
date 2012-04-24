


#include "../global_def.h"

CLogSystem::CLogSystem()
{
	m_updateTime = 0;
	m_generationHour = -1;
}

CLogSystem::~CLogSystem()
{
}

void CLogSystem::Initialize(int createLog, int createDebug)
{
	
	m_updateTime = g_timer.GetAppMilliTime();
	m_updateCycle = 1000;  
	m_createLog = createLog;
	m_createDebug = createDebug; 

	
	m_createError = true;
}

int CLogSystem::Open()
{
	struct tm *date_tm;
	

	
    time( &m_logTime );
	date_tm = localtime( &m_logTime );
	if (m_generationHour != date_tm->tm_hour) 
	{
		m_generationHour = date_tm->tm_hour;	

		
		date_tm->tm_year += 1900;
		date_tm->tm_mon += 1;

		if ( m_createLog )
		{
			sprintf(m_logFileName, "logs/GS%02d%02d_%04d%02d%02d_%02d.log", 
				g_config.serverGroupNo, g_config.gameServerNo, date_tm->tm_year, date_tm->tm_mon, date_tm->tm_mday, date_tm->tm_hour);
		}

		if ( m_createDebug )
		{
			sprintf(m_debugFileName, "debugs/GS%02d%02d_%04d%02d%02d_%02d.dbg", 
				g_config.serverGroupNo, g_config.gameServerNo, date_tm->tm_year, date_tm->tm_mon, date_tm->tm_mday, date_tm->tm_hour);
		}
	}

	if ( m_createLog )
	{
		m_fpLog = fopen( m_logFileName , "a+" ); 
		if( !m_fpLog ) 
		{
			MessageBox(g_hWnd, "logs folder doesn't exist!!", "Error!", MB_OK);
			return false;
		}
	}

	if ( m_createDebug )
	{
		m_fpDebug = fopen( m_debugFileName , "a+" ); 
		if( !m_fpDebug )   
		{
			MessageBox(g_hWnd, "debugs folder doesn't exist!!", "Error!", MB_OK);
			return false;
		}
	}
	
	
	if( m_createError )
	{
		m_fpError = fopen( "error.dbg", "a+" );
		if( !m_fpError )
		{
			MessageBox(g_hWnd, "Cannot open error.dbg file!!", "Error!", MB_OK);
			return false;
		}
	}

	return true;
}

int CLogSystem::Close()
{
	if ( m_createLog )
	{
		if ( fclose(m_fpLog) != 0 ) 
			return false;
	}

	if (m_createDebug )
	{
		if ( fclose(m_fpDebug) != 0 ) 
			return false;
	}

	
	if( m_createError )
	{
		if( fclose( m_fpError ) != 0 )
			return false;
	}

	return true;
}


int CLogSystem::Update()
{
	if (m_updateTime > g_globalTime) return false;

	
	m_updateTime = g_globalTime + m_updateCycle;	

	
	if ( !Close() ) return false;
	if ( !Open() ) return false;

	return true;
}



void CLogSystem::Write( const char *format, ... )
{

	if ( !m_createDebug ) return;

	va_list argptr;
	char buffer[4096];
	char cTime[26];

	
	va_start(argptr, format);
	vsprintf(buffer, format, argptr);
	va_end(argptr);

    time( &m_logTime );
	strcpy(cTime, ctime(&m_logTime)); 
	cTime[24] ='\0';	

	fprintf(m_fpDebug, "[%s] %s\n", cTime, buffer);
}


void CLogSystem::WriteToLog( const char *format, ... )
{
	if ( !m_createLog ) return;

	va_list argptr;
	char buffer[4096];
	char cTimeStamp[30];
	timeStamp_t timeStamp;

	
	va_start(argptr, format);
	vsprintf(buffer, format, argptr);
	va_end(argptr);

	



	sprintf(cTimeStamp, "%4d-%02d-%02d %02d:%02d:%02d", 
		timeStamp.year, timeStamp.month, timeStamp.day, timeStamp.hour, timeStamp.minute, timeStamp.second);

	fprintf(m_fpLog, "%s;%s\n", cTimeStamp, buffer);
}


void CLogSystem::WriteToError( const char *format, ... )
{
	if ( !m_createError ) return;

	va_list argptr;
	char buffer[4096];
	char cTime[26];

	
	va_start(argptr, format);
	vsprintf(buffer, format, argptr);
	va_end(argptr);

    time( &m_logTime );
	strcpy(cTime, ctime(&m_logTime)); 
	cTime[24] ='\0';	

	fprintf(m_fpError, "[%s] %s\n", cTime, buffer);
}








































































void 
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


char* GetItemInformationLog( int isStack, int itemTableIdx, int itemIdx, int inventoryPos )
{
	static char buffer[100];

	_snprintf( buffer, 100, 
		"isStack[%d] "
		"itemTableIdx[%d] "
		"itemIdx[%d] "
		"invenPos[%d]"
		,
		isStack,
		itemTableIdx,
		itemIdx,
		inventoryPos
		);

	return buffer;
}
