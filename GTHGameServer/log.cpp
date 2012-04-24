

#include "global.h"

#include "CTools.h"
extern CTools* gcpTools;




CLogSystem::CLogSystem()
{
	m_updateTime		= 0;
	m_generationHour	= -1;
}




CLogSystem::~CLogSystem()
{
	
}





void CLogSystem::Initialize(const BOOL in_bCreateLog,const BOOL in_bCreateDebug)
{
	m_updateTime	= g_globalTime;
	m_updateCycle	= 1000;	

	m_UserLogTTCtrl.SetActive(in_bCreateLog);	
	m_UserLogCtrl.SetActive(in_bCreateLog);	
	m_DebugLogCtrl.SetActive(in_bCreateDebug);
	m_ErrorLogCtrl.SetActive(TRUE);
	m_NewDanbattleLogCtrl.SetActive(TRUE);
	m_HackingUserLogCtrl.SetActive(TRUE);


}

BOOL CLogSystem::Close(void)
{
	m_UserLogTTCtrl.Close();
	m_UserLogCtrl.Close();
	m_DebugLogCtrl.Close();
	m_ErrorLogCtrl.Close();
	m_NewDanbattleLogCtrl.Close();
	m_HackingUserLogCtrl.Close();
	
	
	return TRUE;
}




BOOL CLogSystem::Open(void)
{
	struct tm* date_tm=NULL;
	
	
	
	time_t	logTime;
    time( &logTime );
	date_tm = localtime( &logTime );
	
	if (m_generationHour != date_tm->tm_hour) 
	{		
		
		m_generationHour = date_tm->tm_hour;	
		
		
		
		date_tm->tm_year += 1900;
		date_tm->tm_mon += 1;
		
		sprintf(m_logFileName, "logs/GS%02d%02d_%04d%02d%02d_%02d.log", 
			g_config.serverGroupNo, 
			g_config.gameServerNo, 
			date_tm->tm_year, 
			date_tm->tm_mon, 
			date_tm->tm_mday, 
			date_tm->tm_hour);
			
		sprintf(m_logTTFileName, "TTlogs/TT%02d%02d_%04d%02d%02d_%02d.log", 
			g_config.serverGroupNo, 
			g_config.gameServerNo, 
			date_tm->tm_year, 
			date_tm->tm_mon, 
			date_tm->tm_mday, 
			date_tm->tm_hour);
			
		sprintf(m_debugFileName, "debugs/GS%02d%02d_%04d%02d%02d_%02d.dbg", 
			g_config.serverGroupNo, 
			g_config.gameServerNo, 
			date_tm->tm_year, 
			date_tm->tm_mon, 
			date_tm->tm_mday, 
			date_tm->tm_hour);

		sprintf(m_NewDanBattleFileName, "NewDanBattleLog/GS%02d%02d_%04d%02d%02d_%02d.dbg", 
			g_config.serverGroupNo, 
			g_config.gameServerNo, 
			date_tm->tm_year, 
			date_tm->tm_mon, 
			date_tm->tm_mday, 
			date_tm->tm_hour);

		sprintf(m_HackingUserFileName, "HackingLog/GS%02d%02d_%04d%02d%02d_%02d.dbg", 
			g_config.serverGroupNo, 
			g_config.gameServerNo, 
			date_tm->tm_year, 
			date_tm->tm_mon, 
			date_tm->tm_mday, 
			date_tm->tm_hour);
		
	}		
		
	m_UserLogCtrl.Open(m_logFileName);
	m_UserLogTTCtrl.Open(m_logTTFileName);
	m_DebugLogCtrl.Open(m_debugFileName);	
	m_ErrorLogCtrl.Open("error.dbg");
	m_NewDanbattleLogCtrl.Open(m_NewDanBattleFileName);
	m_HackingUserLogCtrl.Open(m_HackingUserFileName);

	return TRUE;
}






BOOL CLogSystem::Update(void)
{
	if (m_updateTime > g_globalTime) return FALSE;

	
	m_updateTime = g_globalTime + m_updateCycle;	
	Close();
	Open();
	return TRUE;
}





void CLogSystem::Write( const char *format, ... )
{

	va_list argptr;
	const int buffer_maxbytes=4095;
	char buffer[buffer_maxbytes+1]="";	

	
	va_start(argptr, format);	
	_vsnprintf(buffer,buffer_maxbytes, format, argptr);
	buffer[buffer_maxbytes]=NULL;
	va_end(argptr);
	
	m_DebugLogCtrl.WriteLog(buffer);

}




void CLogSystem::WriteToLogTT( const char *format, ... )
{

	va_list argptr;
	const int buffer_maxbytes=4095;
	char buffer[buffer_maxbytes+1]="";

	
	va_start(argptr, format);
	_vsnprintf(buffer,buffer_maxbytes, format, argptr);
	buffer[buffer_maxbytes]=NULL;
	va_end(argptr);

	m_UserLogTTCtrl.WriteLog(buffer);

	


}



void CLogSystem::WriteToLog( const char *format, ... )
{

	va_list argptr;
	const int buffer_maxbytes=4095;
	char buffer[buffer_maxbytes+1]="";

	
	va_start(argptr, format);
	_vsnprintf(buffer,buffer_maxbytes, format, argptr);
	buffer[buffer_maxbytes]=NULL;
	va_end(argptr);

	m_UserLogCtrl.WriteLog(buffer);

	


}





void CLogSystem::WriteToError( const char *format, ... )
{


	va_list argptr;
	const int buffer_maxbytes=4095;
	char buffer[buffer_maxbytes+1]="";	

	
	va_start(argptr, format);
	_vsnprintf(buffer, buffer_maxbytes,format, argptr);
	buffer[buffer_maxbytes]=NULL;
	va_end(argptr);

	m_ErrorLogCtrl.WriteLog(buffer);

}
void CLogSystem::WriteToNewDanBattleLog( const char *format, ... )
{


	va_list argptr;
	const int buffer_maxbytes=4095;
	char buffer[buffer_maxbytes+1]="";	

	
	va_start(argptr, format);
	_vsnprintf(buffer, buffer_maxbytes,format, argptr);
	buffer[buffer_maxbytes]=NULL;
	va_end(argptr);

	m_NewDanbattleLogCtrl.WriteLog(buffer);

}



void CLogSystem::WriteToHackingLog( const char *format, ... )
{
	va_list argptr;
	const int buffer_maxbytes=4095;
	char buffer[buffer_maxbytes+1]="";	

	
	va_start(argptr, format);
	_vsnprintf(buffer, buffer_maxbytes,format, argptr);
	buffer[buffer_maxbytes]=NULL;
	va_end(argptr);

	m_HackingUserLogCtrl.WriteLog(buffer);

}



void CLogSystem::
WriteToLog(const playerCharacter_t* in_pPlayer,const int in_logClass,const char* in_format, ...  )
{
	if(NULL == in_pPlayer) return;
	

	va_list argptr;
	const int buffer_maxbytes=4095;
	char buffer[buffer_maxbytes+1]="";
	char cTimeStamp[30]="";
	

	
	va_start(argptr, in_format);
	_vsnprintf(buffer,buffer_maxbytes, in_format, argptr);
	buffer[buffer_maxbytes]=NULL;
	va_end(argptr);

	char logBuffer[4096]="";	
	
	_snprintf( logBuffer, 4096, 
		
		"%d;"			
		"%s;"			
		"%s;"			
		"%d;"			
		"%d;"			
		"%d;"			
		"%.0f;"			
		"%.0f;"			
		"%.0f;"			
		"%d;"			
		"%I64d;"			
		"%d;"			
		"%I64d;"			
		"%d;"			
		
		,
		
		g_config.serverGroupNo,	
		in_pPlayer->userID,		
		in_pPlayer->name,		
		in_pPlayer->pcJob,		
		in_logClass,		
		in_pPlayer->worldIdx,	
		in_pPlayer->position[0],
		in_pPlayer->position[1],
		in_pPlayer->position[2],
		in_pPlayer->level,		
		in_pPlayer->exp,		
		in_pPlayer->genLevel,	
		in_pPlayer->genExp,		
		in_pPlayer->curChargeSE
		
		);
		
	m_UserLogCtrl.WriteLog(buffer, logBuffer);	
}






void CLogSystem::WriteRLog( const char *format, ... )				
{

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




void CLogSystem::WriteItemDeleteLog(const playerCharacter_t* in_pPlayer,const item_t& in_item)
{
	itemTable_t* pItemTableRecord = gcpTools->GetItemTablePointer(in_item.itemTableIdx);
	if(NULL == pItemTableRecord) return;

	int number=1;
	if(pItemTableRecord->stackFlag) number = in_item.durability+1;
	char name[20]="unknown";
	strncpy(name,in_item.name,20); name[20-1]=NULL;



	WriteToLog(
		in_pPlayer, 
		LOG_CLASS_ITEM, "%d;%d;;;;%s;itemTableIdx[%d]", 
		LOG_CLASS_ITEM_DESTROY, 
		number, 
		name,
		in_item.itemTableIdx);
}





