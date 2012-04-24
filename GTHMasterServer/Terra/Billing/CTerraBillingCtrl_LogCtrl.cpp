



#include "../../global.h"
#include "CTerraBillingCtrl_LogCtrl.h"


#include "tagTerraBilling.h"
extern struct tagTerraBilling gTerraBilling;




CTerraBillingCtrl_LogCtrl::CTerraBillingCtrl_LogCtrl()
{
	m_bOK=FALSE;
	m_generationHour=25;
	m_fp=NULL;
	m_NextUpdateTime=g_globalTime-100;


	m_bOK=TRUE;

}





CTerraBillingCtrl_LogCtrl::~CTerraBillingCtrl_LogCtrl()
{
	CloseFile();
}





BOOL CTerraBillingCtrl_LogCtrl::isOK(void) const
{
	return m_bOK;
}




void CTerraBillingCtrl_LogCtrl::Update(void)
{
	if(FALSE == gTerraBilling.Log.bOutput) return;
	if(m_NextUpdateTime > g_globalTime) return;

	
	m_NextUpdateTime=g_globalTime + gTerraBilling.Log.UpdateCycle;
	
	CloseFile();
	OpenFile();

}




inline void CTerraBillingCtrl_LogCtrl::CloseFile(void)
{
	if(NULL != m_fp){ fclose(m_fp); m_fp=NULL; }		
}





void CTerraBillingCtrl_LogCtrl::Write(const char* format, ... )
{
	if(FALSE == gTerraBilling.Log.bOutput) return;
	if(NULL == m_fp){ 
		if(FALSE == OpenFile()) return; 		
	}


	const int buffer_maxbytes=4095;
	char buffer[buffer_maxbytes+1];

	va_list argptr;
	

	
	va_start(argptr, format);
	_vsnprintf(buffer,buffer_maxbytes,format,argptr);
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




char* CTerraBillingCtrl_LogCtrl::GetYESNOstring(BOOL bYESorNO)
{
	static char* pYESstring = "yes";
	static char* pNOstring = "no";

	if(TRUE == bYESorNO) return pYESstring;
	return pNOstring;
}




inline BOOL CTerraBillingCtrl_LogCtrl::OpenFile(void)
{
	struct tm* date_tm;
	long Time;
	
	
    
	time(&Time);
	date_tm = localtime(&Time);


	if(m_generationHour != date_tm->tm_hour){
		m_generationHour = date_tm->tm_hour;
	
		date_tm->tm_year += 1900;
		date_tm->tm_mon += 1;

		#ifdef _GAME_SERVER
		wsprintf(m_szFileName,"billinglogs/Terra%02d%02d_%04d%02d%02d_%02d.log",g_config.serverGroupNo,g_config.gameServerNo,date_tm->tm_year,date_tm->tm_mon,date_tm->tm_mday,date_tm->tm_hour);
		#endif

		#ifdef _MASTER_SERVER
		wsprintf(m_szFileName,"billinglogs/Terra%02d_%04d%02d%02d_%02d.log",g_config.serverNo,date_tm->tm_year,date_tm->tm_mon,date_tm->tm_mday,date_tm->tm_hour);
		#endif
	}
	
	m_fp = fopen(m_szFileName,"a+");
	if(NULL == m_fp) return FALSE;

	return TRUE;
}