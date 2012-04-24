



#if !defined(AFX_CLOGSYSTEM_H__D9072BAC_8B47_4A18_9D72_5DE66B9FE3A1__INCLUDED_)
#define AFX_CLOGSYSTEM_H__D9072BAC_8B47_4A18_9D72_5DE66B9FE3A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "CLogFileCtrl.h"
class CLogSystem
{
	private:
		
		char			m_logFileName[_MAX_PATH];
		char			m_debugFileName[_MAX_PATH];
		unsigned int	m_updateTime;
		int				m_updateCycle;
		int				m_generationHour;
		CLogFileCtrl	m_UserLogFileCtrl;
		CLogFileCtrl	m_DebugLogFileCtrl;		


	public:
		void Initialize(const BOOL in_bCreateLog,const BOOL in_bCreateDebug);
		void Open(void);
		void Close(void);
		void Update(void);
		void Write( const char *format, ... );
		void WriteToLog( const char *format, ... );

	public:
		CLogSystem();
		~CLogSystem();
};






enum
{
	LOG_CLASS_LEVELUP = 0,		
	LOG_CLASS_GENLEVELUP,		
	LOG_CLASS_ITEM,				
	LOG_CLASS_PCDIE,			
	LOG_CLASS_CONNECT,			
	LOG_CLASS_DISCONNECT,		
	LOG_CLASS_PCCREATE,			
	LOG_CLASS_PCDELETE,			
};


typedef struct 
{
	SQLSMALLINT year; 
	SQLUSMALLINT month; 
	SQLUSMALLINT day; 
	SQLUSMALLINT hour; 
	SQLUSMALLINT minute; 
	SQLUSMALLINT second; 
	SQLUINTEGER fraction;
} timeStamp_t;





#endif 
