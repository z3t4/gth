



#include "..\global.h"
#include "CGolryunBattle_Log.h"


#include "tagGolryunBattle.h"
extern struct tagGolryunBattleVar gGolryunBattleVar;




CGolryunBattle_Log::CGolryunBattle_Log()
{
	m_bOK=FALSE;
	m_generationHour=25;
	m_fp=NULL;
	m_NextUpdateTime=g_globalTime-100;


	m_bOK=TRUE;

}





CGolryunBattle_Log::~CGolryunBattle_Log()
{
	CloseFile();
}





BOOL CGolryunBattle_Log::isOK(void) const
{
	return m_bOK;
}




void CGolryunBattle_Log::Update(void)
{
	if(FALSE == gGolryunBattleVar.Log.bOutput) return;



	
	if(m_NextUpdateTime > (int)g_globalTime) return;

	
	m_NextUpdateTime=g_globalTime + gGolryunBattleVar.Log.UpdateTimeMiliSecond;
	
	CloseFile();
	OpenFile();

}




inline void CGolryunBattle_Log::CloseFile(void)
{
	if(NULL != m_fp){ fclose(m_fp); m_fp=NULL; }		
}





void CGolryunBattle_Log::Write(const char* format, ... )
{
	if(FALSE == gGolryunBattleVar.Log.bOutput) return;
	if(NULL == m_fp){ 
		if(FALSE == OpenFile()) return; 		
	}


	va_list argptr;
	

	const int buffer_maxbytes=4095;
	char buffer[buffer_maxbytes+1];


	
	va_start(argptr, format);
	_vsnprintf(buffer,buffer_maxbytes, format, argptr);
	buffer[buffer_maxbytes]=NULL;	
	va_end(argptr);

    
	const int bytes_strTime=24;
	char strTime[bytes_strTime+1];

	time_t Time;	
	time( &Time );
	strncpy(strTime, ::ctime(&Time),bytes_strTime);
	strTime[bytes_strTime] = NULL;

	fprintf(m_fp,"[%s] %s\n",strTime,buffer);

}




char* CGolryunBattle_Log::GetYESNOstring(BOOL bYESorNO)
{
	static char* pYESstring = "yes";
	static char* pNOstring = "no";

	if(TRUE == bYESorNO) return pYESstring;
	return pNOstring;
}




inline BOOL CGolryunBattle_Log::OpenFile(void)
{
	struct tm* date_tm;
	long Time;
	
	
    
	time(&Time);
	date_tm = localtime(&Time);


	if(m_generationHour != date_tm->tm_hour){
		m_generationHour = date_tm->tm_hour;	
		date_tm->tm_year += 1900;
		date_tm->tm_mon += 1;		
		wsprintf(m_szFileName,"GolryunLogs/Golryun%02d%02d_%04d%02d%02d_%02d.log",g_config.serverGroupNo,g_config.gameServerNo,date_tm->tm_year,date_tm->tm_mon,date_tm->tm_mday,date_tm->tm_hour);
		
	}
	
	m_fp = fopen(m_szFileName,"a+");
	if(NULL == m_fp) return FALSE;

	return TRUE;
}