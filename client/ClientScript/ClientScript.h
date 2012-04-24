#if !defined(AFX_TOTUORIAL_H__DB41A13D_1745_4B62_BF85_9344651C32DF__INCLUDED_)
#define AFX_TOTUORIAL_H__DB41A13D_1745_4B62_BF85_9344651C32DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 








#define				MAX_NUMBER_OF_SCRIPT_VARIABLE			100		
#define				MAX_NUMBER_OF_SCRIPT_PROCEDURE			100		
#define				MAX_CLIENT_SCRIPT_COUNT					30
#define				MAX_CLIENT_SCRIPT_BUFFER_SIZE			4000
#define				MAX_SCRIPT_VAR							100		





enum CS_EVENT
{
	CS_WAIT,
	CS_RUN
};

struct sCS_FUNC
{
	int				(*pFunc)(void* pc,int* pParam);
	int				argCount;
};

typedef struct
{
	int				Type;				

	
	char            ScriptData			[MAX_CLIENT_SCRIPT_BUFFER_SIZE];	

	
	CS_EVENT		ScriptEvent;

	
	unsigned int	saveTime;			
	int				thinkTime;			
	int				ScriptAddr;			

	
	int				ScriptVariable		[MAX_NUMBER_OF_SCRIPT_VARIABLE];

	
	int				ScriptProcedureAddr	[MAX_NUMBER_OF_SCRIPT_PROCEDURE];
	
	
	
	int				callProcedureAddr;

	
	bool			isCall;
	int				msgWaiting;
	int				pcReplyIdx;

	int				msgFlag;
	int				sendMsgIdx;
	int				replyMsgIdxNumber;
	int				replyMsgIdx[5];

} ClientScript_t;			





enum TIP_TYPES			
{
	TIP_INTERFACE		= 0	,
	TIP_INVENTORY			,
	TIP_CHAR_WINDOW			,
	TIP_QUEST_WINDOW		,
	TIP_PARTY_WINDOW		,
	TIP_COMMAND_WINDOW		,
	TIP_TRADE_WINDOW		,
	TIP_ENERGY_PACK			,
	TIP_SPELL				,
	TIP_SECRET_TALK			,
	TIP_PARTY_TALK			,
	TIP_QUEST_NPC			,
	TIP_REPAIRITEM			,
	TIP_NSE					,
	TIP_SKILL_POINT			,
	TIP_GEN					,
	TIP_STATUS_POINT		,
	TIP_REPAIR_ITEM			,	
	TIP_COUNT			
};


enum TutorialQuests
{
	Tutorial_Walk				= 0	,	
	Tutorial_OpenInvenTory		,	
	Tutorial_TakeItem			,	
	Tutorial_MoveCamera			,	
	Tutorial_SitDown			,	
	Tutorial_SetTarget			,	
	Tutorial_Shopkeeper			,	
	Tutorial_MeetNpc			,	
	Tutorial_Count				,		

};

enum ScriptType
{
	TutorialScript,
	TipScript
};



struct sTutorial_Script
{
	int				TutorialType;
	ClientScript_t	Script;
};

	

struct sTutorual_SaveBaseInfo
{
	int		CharIdx;
	char	Charname[256];

	int		TutorialNum;
	bool	bTutorialMode;
};

struct sTutorial_History
{
	char	LoginName[256];
	vector <sTutorual_SaveBaseInfo>	vBaseInfo;
};




class CTutorialMng
{

protected:
	CFileMng			*m_fileMng;				

public:	
	int					m_NowTutorialNum;
	int					m_NowTipNum;

protected:	
	bool				m_bAlwaysRun;		
	static bool			m_bTutorialMode;

	int					m_CompleteTutorial;
	int					m_Tip_Count;
	int					m_Tutorial_Count;
	int					m_LogInIdx;
	

public:
	
	int					ScriptVar				[MAX_SCRIPT_VAR];			
	int					QuestNumber;										

protected:
	sTutorial_Script	m_Tip_Script			[MAX_CLIENT_SCRIPT_COUNT];
	sTutorial_Script	m_Tutorial_Script		[MAX_CLIENT_SCRIPT_COUNT];

public:	
	void				SetCompleteTutorial		(int type);
	BOOL				IsCompleteTutorial		(int type);

	
	void				SetFileMng				(CFileMng*	mng	)	{ m_fileMng	= mng;}	
	void				SetReplyMessage			(int reply);
	void				SettingTutorial			(int idx);
	bool				GetTotutirlMode()		{return m_bTutorialMode; }			
	void				SetTotutirlMode			(int active);
	

public:
	BOOL				Initialize				();
	BOOL				InitScript				(int type, int idx);

	BOOL				LoadScriptFile			(int Type, int idx, char* Filename);
	
	bool				SaveHistory				(char *filename);
	bool				LoadHistory				(char *filename);
	
	BOOL				Process();

	vector<sTutorial_History>					m_vTutorialHistory;
	int					FindInLoginID			(char *ID);
	int					FindInCharName			(int idx , char *Name);

public:	
	CTutorialMng();
	~CTutorialMng(){};
};






void	CS_LoadScript				( ClientScript_t* pCS, char* filename );
int		CS_InterpretorFunction		( ClientScript_t* pCS );
int		CS_InterpretorDefine		( ClientScript_t* pCS, int paramAddr );
char	CS_GetCharData				( ClientScript_t* pCS );
int		CS_GetIntData				( ClientScript_t* pCS );
int		CS_Interpretor				( ClientScript_t* pCS );
void	CS_InterpretorCall			( ClientScript_t* pCS );
void	CS_InterpretorOperation		( ClientScript_t* pCS );
void	CS_InterpretorParameter		( ClientScript_t* pCS, int param[], int n );
int		CS_InterpretorCase			( ClientScript_t* pCS );
void	CS_InterpretorThen			( ClientScript_t* pCS, int addrInfo[] );
void	CS_InterpretorElse			( ClientScript_t* monster, int addrInfo[] );
void	CS_InterpretorIF			( ClientScript_t* pCS );







int		CS_IsMessageWating			( void *pPc, int *pParam );	
int		CS_SendMessage				( void *pPc, int *pParam );
int		CS_SendMessageButton		( void *pPc, int *pParam );
int		CS_GetMessageSelect			( void *pPc, int *pParam );
int		CS_CloseMessage				( void *pPc, int *pParam );
int		CS_GetPcEvent				( void *pPc, int *pParam );
int		CS_SetQuestStep				( void *pPc, int *pParam );	
int		CS_CheckQuestStep			( void *pPc, int *pParam );	
int		CS_GetTutorialNum			( void *pPC, int *pParam );
int		CS_SetTutorialNum			( void *pPC, int *pParam );
int		CS_ChkOpenInvenWindow		( void *pPC, int *pParam );
int		CS_ChkEquipItem				( void *pPc, int *pParam );
int		CS_ChkMoveCamera			( void *pPc, int *pParam );
int		CS_IsSelectObject			( void *pPc, int *pParam );
int		CS_IsRequestNpcMenu			( void *pPc, int *pParam );
int		CS_EndTutorial				( void *pPc, int *pParam );
int		CS_IsOpenSkillWindow		( void *pPc, int *pParam );
int		CS_ChkRegSitDownIcon		( void *pPc, int *pParam );
int		CS_SEND_CHAT_MESSAGE (void *pPc, int *pParam);

int		CS_SendMessage_TutorialMessage(int type, ClientScript_t* pCS );


extern	sCS_FUNC					CS_FUNC[];

#endif 
