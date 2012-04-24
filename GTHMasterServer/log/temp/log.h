#ifndef _LOG_H_
#define _LOG_H_


class CLogSystem
{
private:
	time_t			m_logTime;
	FILE*			m_fpLog;
	FILE*			m_fpDebug;
	char			m_logFileName[PATHSTRING];
	char			m_debugFileName[PATHSTRING];
	unsigned int	m_updateTime;
	int				m_updateCycle;
	int				m_generationHour;

	BOOL			m_bCreateLog;
	BOOL			m_bCreateDebug;
	
	


public:
	CLogSystem();
	~CLogSystem();

	void Initialize(const BOOL in_bCreateLog,const BOOL in_bCreateDebug);
	
	BOOL Open(void);
	BOOL Close(void);
	int Update();
	void Write( const char *format, ... );
	void WriteToLog( const char *format, ... );
	

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


void GetTimeStamp(timeStamp_t *timeStamp);


#endif

