#ifndef _LOG_H_
#define _LOG_H_


class CLogSystem
{
private:
	time_t			m_logTime;
	FILE			*m_fpLog;
	FILE			*m_fpDebug;
	char			m_logFileName[64];
	char			m_debugFileName[64];
	unsigned int	m_updateTime;
	int				m_updateCycle;
	int				m_generationHour;
	int				m_createLog;
	int				m_createDebug;

	
	FILE			*m_fpError;
	int				m_createError;


public:
	CLogSystem();
	~CLogSystem();

	void Initialize(int createLog, int createDebug);
	int Open();
	int Close();
	int Update();
	void Write( const char *format, ... );
	void WriteToError( const char *format, ... );
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
	LOG_CLASS_PCERROR,			
};

enum
{
	LOG_CLASS_ITEM_PICKUP	=	0,
	LOG_CLASS_ITEM_DROP,
	LOG_CLASS_ITEM_STACK,
	LOG_CLASS_ITEM_BUY_STORE,
	LOG_CLASS_ITEM_SELL_STORE,
	LOG_CLASS_ITEM_LEARN_SKILL,
	LOG_CLASS_ITEM_SKILL_SCROLL,
	LOG_CLASS_ITEM_USE_PORTAL,
	LOG_CLASS_ITEM_DESTROY,
	LOG_CLASS_ITEM_CONFIRM_TRADE,
	LOG_CLASS_ITEM_UNDO_TRADE,
	LOG_CLASS_ITEM_ADD_QUEST,
	LOG_CLASS_ITEM_DEL_QUEST,
	LOG_CLASS_ITEM_DIE_DROP,
	LOG_CLASS_ITEM_DIE_DESTROY,
	
	
	LOG_CLASS_ITEM_COMPLETE_CRAFT,
	LOG_CLASS_ITEM_CRAFT_SOURCE,
	LOG_CLASS_ITEM_CRAFT_RESULT,
	LOG_CLASS_ITEM_UNDO_CRAFT,
	
	
	LOG_CLASS_ITEM_UNDO_BOOTH,
	LOG_CLASS_ITEM_SELL_BOOTH,
	LOG_CLASS_ITEM_BUY_BOOTH,

	
	LOG_CLASS_ITEM_COMPLETE_UPGRADE,
	LOG_CLASS_ITEM_UPGRADE_SOURCE,
	LOG_CLASS_ITEM_UPGRADE_RESULT,
	LOG_CLASS_ITEM_UNDO_UPGRADE,
};

enum
{
	LOG_CLASS_ITEM_MISSMATCH	=	0,
};


enum
{
	LOG_CLASS_ITEM_MISSMATCH_TYPE_SAVE	=	0,
	LOG_CLASS_ITEM_MISSMATCH_TYPE_TRADE,
	LOG_CLASS_ITEM_MISSMATCH_TYPE_CRAFT,
	LOG_CLASS_ITEM_MISSMATCH_TYPE_UPGRADE,
	LOG_CLASS_ITEM_MISSMATCH_TYPE_BOOTH,
	LOG_CLASS_ITEM_MISSMATCH_TYPE_AUTOQUESTCRAFT,
};
















char* GetItemInformationLog( int isStack, int itemTableIdx, int itemIdx, int inventoryPos );

#endif

