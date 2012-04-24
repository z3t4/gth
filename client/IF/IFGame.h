



#if !defined(AFX_IFMSGWIN_H__0DC3BC64_6B6C_4FA8_9280_6B6B16D888D0__INCLUDED_)
#define AFX_IFMSGWIN_H__0DC3BC64_6B6C_4FA8_9280_6B6B16D888D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 





enum
{
	IT_MESSAGE_WIN_TITLE			=	0,
	IT_MESSAGE_WIN_U,
	
	NUM_IT_MESSAGE_WIN
};

class CIFMsgWin   : public CIFWindow
{
public:

	int				m_buttonType;
	bool			m_bInputMode;

	
	CIFText			*m_textCtl[NUM_IT_MESSAGE_WIN];
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBottom;
	CIFControl		*m_line;
	
	CIFButtonCtl	*m_okBtn;
	CIFButtonCtl	*m_cancelBtn;
	CIFButtonCtl	*m_okCenterBtn;

	
	CIFControl		*m_inputBar;

	CIFMsgWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFMsgWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
};





enum
{
	IT_STACK_WIN_TITLE		=	0,
	IT_STACK_WIN_INPUT,
	IT_STACK_WIN_INPUT_U,
	IT_STACK_WIN_INPUT_NUM_U,
	IT_STACK_WIN_NUM_POS_0,
	IT_STACK_WIN_NUM_POS_1,
	IT_STACK_WIN_NUM_POS_2,
	IT_STACK_WIN_NUM_POS_3,
	IT_STACK_WIN_NUM_POS_4,
	IT_STACK_WIN_NUM_POS_5,
	IT_STACK_WIN_NUM_POS_6,
	IT_STACK_WIN_NUM_POS_7,
	IT_STACK_WIN_NUM_POS_8,
	IT_STACK_WIN_NUM_POS_9,

	NUM_IT_STACK_WIN
};

#define MAX_COUNT_IF_STACK_WIN_NUM		10

enum
{
	IF_STACK_TYPE_INVENTORY,
	IF_STACK_TYPE_BUY_ITEM,
	IF_STACK_TYPE_BUY_SKILL,
	IF_STACK_TYPE_EPACK,
	IF_STACK_TYPE_TRADE_SE,
	IF_STACK_TYPE_DEPOT,
	IF_STACK_TYPE_DEPOT_SAVE_SE,
	IF_STACK_TYPE_DEPOT_LOAD_SE,
	IF_STACK_TYPE_BUY_BOOTH_ITEM,
	IF_STACK_TYPE_SELL_BOOTH_PRICE,
	
	IF_STACK_TYPE_BATTING_TRADE,
	

	IF_STACK_TYPE_MAILBOX_SEND,


	IF_STACK_TYPE_HELPERSYSTEM_MYPOINT,

};

class CIFStackWin  : public CIFWindow
{
public:

	
	CIFText*				m_textCtl[NUM_IT_STACK_WIN];

	CIFControl*			m_titleBar;
	CIFControl*			m_titleBottom;
	CIFControl*			m_inputBar;
	CIFControl*			m_inputNumBar;
	CIFControl*			m_numBar;
	CIFControl*			m_line;
	
	CIFButtonCtl*		m_numUpBtn[MAX_COUNT_IF_STACK_WIN_NUM];
	CIFButtonCtl*		m_numDnBtn[MAX_COUNT_IF_STACK_WIN_NUM];
	CIFButtonCtl*		m_okBtn;
	CIFButtonCtl*		m_cancelBtn;
	CIFButtonCtl*		m_closeBtn;

	int					m_stackType;			
	int					m_count;
	int					m_maxCount;
	int					m_saveInvenPos;
	int					m_saveItemPrice;

	int					m_chargeType;
	float				m_chargeCost;

	CIFStackWin( CIFControlNode* pParentControl = NULL );
	virtual ~CIFStackWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		InitStack( char* str, item_t* item, int type, int invenPos = 0, float price = 0.0f );
	int			GetCount()		{ return( m_count ); }
};





class CIFMenuWin  	: public CIFWindow
{
public:

	CIFMenuWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFMenuWin();

	CIFButtonCtl		*m_charBtn;
	CIFButtonCtl		*m_itemBtn;
	CIFButtonCtl		*m_skillBtn;
	CIFButtonCtl		*m_questBtn;
	CIFButtonCtl		*m_communityBtn;
	CIFButtonCtl		*m_guildBtn;
	CIFButtonCtl		*m_optionBtn;

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
};





enum
{
	IT_MAP_WIN_MINI_TITLE,
	IT_MAP_WIN_MINI_MAP_POS,
	IT_MAP_WIN_POSITION,

	NUM_IT_MINI_MAP_WIN
};

class CIFMiniMapWin  : public CIFWindow
{
public:

	CIFText					*m_textCtl[NUM_IT_MINI_MAP_WIN];
	
	CIFControl				*m_titleBar;
	CIFButtonCtl			*m_closeBtn;
	CIFButtonCtl			*m_zoomInBtn;
	CIFButtonCtl			*m_zoomOutBtn;
	CIFButtonCtl			*m_mapOpenBtn;

	CIFControl				*m_myImg;
	CIFControl				*m_entityImg[MAX_CHAR_A_SCENE];
	
	CIFMiniMapWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFMiniMapWin();

	
	CD3DBuffer				*m_miniMapVexbuf;			
	float					m_realMapSize;
	
	virtual void		PrepareControl();
	virtual	void		PrepareBuffer();
	virtual	void		UpdateBuffer();
	virtual void		Release();

	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	
	int			Selection();
};





enum
{
	IT_MAP_WIN_TITLE,
	
	NUM_IT_MAP_WIN
};

#define	MAX_NUMBER_OF_MAP_NPCICON		100

typedef struct 
{
	vec3_t		position;
	int			iconType;			
	int			tableIdx;
	int			linkedWorldIdx;		
	int			size;
} mapNpcIconInfo_t;

class CIFMapWin  : public CIFWindow
{
public:

	CIFText					*m_textCtl[NUM_IT_MAP_WIN];
	
	CIFControl				*m_titleBar;
	CIFButtonCtl			*m_closeBtn;

	CIFControl				*m_myImg;
	CIFControl				*m_npcImg[MAX_NUMBER_OF_MAP_NPCICON];

	mapNpcIconInfo_t		m_npcInfo[MAX_NUMBER_OF_MAP_NPCICON];
	int						m_numNpcInfo;
	
	int						m_npcZoomType;
	float					m_npcZoomSize;

	CIFMapWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFMapWin();

	
	CD3DBuffer				*m_mapVexbuf;				

	int						m_mapTextureID;			
	
	void		AddTextureToManager();
	void		UpdateTextureID();

	virtual void		PrepareControl();
	virtual	void		PrepareBuffer();
	virtual	void		UpdateBuffer();
	virtual void		Release();

	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
};





enum
{
	IT_GM_WIN_TITLE		=	0,
	IT_GM_WIN_NAME_U,
	IT_GM_WIN_U,
	IT_GM_WIN_SELECT_U,

	
	IT_GM_WIN_GOLRYUN_U,	
	

	NUM_IT_GM_WIN
};

#define MAX_NUMBER_OF_GM_CALLER_NAME	16

class CIFGmWin     : public CIFWindow
{
public:
	
	enum GM_GolryunCommand
	{
		start_battle	=	0,
		start_join,
		start_recall_fighter,
		check_join_num,
		close_join,
		start_betting,
		check_betting_nak,
		ready_tournament,
		start_tournament,
		cancel_tournament,
		close_battle,
		force_join_fighter,
		force_eject,
		check_status_pvpMode,
	};
	
	
	CIFText			*m_textCtl[NUM_IT_GM_WIN];
	
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;
	CIFControl		*m_selectBar;
	CIFControl		*m_nameSelectBar;

	
	CIFButtonCtl	*m_closeBtn;
	CIFButtonCtl	*m_prevBtn;
	CIFButtonCtl	*m_nextBtn;


	int				m_selectedIdx;
	int				m_nameSelectedIdx;
	int				m_golryunSelectedIdx;
	
	
	int				m_curPage;
	int				m_maxPage;
	int				m_selectBarSavePos;
	int				m_nameSelectBarSavePos;

	int				m_numCaller;

	CIFGmWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFGmWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		AddCaller( char* name, char* reason );
	void		DelCaller( char* name );
	void		ProcessGM();
	void        ProcessGMGolryunCommand(void);
};





enum
{
	IT_HELP_WIN_TITLE		=	0,
	IT_HELP_WIN_HOME_BUTTON,
	IT_HELP_WIN_PREV_BUTTON,
	IT_HELP_WIN_NEXT_BUTTON,
	IT_HELP_WIN_HELP,

	NUM_IT_HELP_WIN
};

class CIFHelpWin    : public CIFWindow
{
public:

	
	CIFText			*m_textCtl[NUM_IT_HELP_WIN];
	
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;

	
	CIFButtonCtl	*m_closeBtn;
	CIFButtonCtl	*m_homeBtn;
	CIFButtonCtl	*m_prevBtn;
	CIFButtonCtl	*m_nextBtn;
	CIFScrollBar	*m_scrollBar;

	CIFHyperLink	*m_textHyperLink;

	int				m_helpIndex;
	int				m_prevIndex, m_nextIndex;

	CIFHelpWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFHelpWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
};





enum
{
	IT_TIP_WIN_TITLE,
	IT_TIP_WIN_TIP,

	NUM_IT_TIP_WIN
};

class CIFTipWin : public CIFWindow
{
public:
	CIFText					*m_textCtl[NUM_IT_TIP_WIN];
	
	CIFControl				*m_titleBar;
	CIFButtonCtl			*m_closeBtn;
	CIFButtonCtl			*m_nextBtn;

	char			m_tipStr[256];
	int				m_tipIdx;
	int				m_maxtip;
	float			m_curTick;
	int				m_winOriginYPos;
	int				m_winRelativeYPos;

	bool			m_bTip;

	CIFTipWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFTipWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	
	int			Selection();
	void		Open();
	bool		CanOpen();
	void		SetTipState(bool bTip);
};




enum
{
	AUTO_QUEST_SCROLL_TITLE			=	0,
	AUTO_QUEST_SCROLL_STORY,	
	AUTO_QUEST_SCROLL_MESSAGE,
	NUM_AUTO_QUEST_WIN
};

typedef enum
{
	AUTO_WIDE_START,
	AUTO_SCROLL_ORDER,
	AUTO_SCROLL_START,
	AUTO_SCROLL_FAIL,
	AUTO_SCROLL_COMPLETE,
	AUTO_EFFECT_PROCESS,
	AUTO_EFFECT_END,
	AUTO_WIDE_END,
	AUTO_SCROLL_END,
	
	AUTO_SCROLL_ENDMESSAGE,
	NUM_AUTO_SCROLL
}eAutoScrollType;


class CIFAutoQuestWin : public CIFWindow
{
public:
	CIFText				*m_textCtl[NUM_AUTO_QUEST_WIN];

	CIFControl			*m_wideUp;		
	CIFControl			*m_wideDown;	

	CIFButtonCtl		*m_startBtn;
	CIFButtonCtl		*m_closeBtn;

	float			m_totalTime;
	float			m_curTick;
	int				m_arrEffIdx;
	int				m_questIdx;

	float           m_fWideUpPosY;
	float           m_fWideDownPosY;
	float           m_fAngle;
	bool			m_wideEnable;
	bool			m_scrollEnable;
	
	int				m_iautoflag;
	
	
	BOOL m_bComplete;

	eAutoScrollType		m_eType;
	eAutoScrollType		m_eMsgtype;

	CIFAutoQuestWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFAutoQuestWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
   
	void WideRender();
	void ScrollRender();
	void MessageRender(void);

	void		WideEnable( int state = true )					{ m_wideEnable = state; }
	int			WideState()										{ return( m_wideEnable ); }

	void		ScrollEnable( int state = true )				{ m_scrollEnable = state; }
	int			ScrollState()									{ return( m_scrollEnable ); }

};





enum
{
	IT_CRAFTHELP_WIN_TITLE		=	0,
	IT_CRAFTHELP_WIN_RESULT_INVEN_0_POS,
	IT_CRAFTHELP_WIN_RESULT_INVEN_1_POS,
	IT_CRAFTHELP_WIN_SOURCE_INVEN_POS,
	IT_CRAFTHELP_WIN_SEARCH_POS,
	
	
	IF_CRAFTHELP_WIN_MEMBERSHIP_POS,

	NUM_IT_CRAFTHELP_WIN
};

class CIFCraftHelpWin  : public CIFWindow
{
public:

	
	CIFText			*m_textCtl[NUM_IT_CRAFTHELP_WIN];
	
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;
	CIFControl		*m_searchText;

	
	CIFControl		*m_MemberShipBar;

	CIFControl		*m_searchItemBox[32];

	
	CIFButtonCtl	*m_prevBtn;
	CIFButtonCtl	*m_nextBtn;
	CIFButtonCtl	*m_closeBtn;
	CIFButtonCtl	*m_totalViewBtn;
	CIFButtonCtl	*m_searchViewBtn;
	CIFButtonCtl	*m_searchBtn;

	
	int				m_totalMaxCraftFormula;
	int				m_totalCurCraftFormula;
	int				m_searchMaxCraftFormula;
	int				m_searchCurCraftFormula;
	int				m_curType;
	char			m_searchStr[128];
	int				m_checkInventory[MAX_INVENTORY_SIZE];
	
	vector<itemCraftTable_t *>	m_sSearchCraftTable;


	CIFCraftHelpWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFCraftHelpWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		MakeCraftItem( item_t *item, itemCraftTable_t *pCraftTbl, int type, int itemIdx );
	void		SetCraftHelpWin( char* searchStr );
	void		SearchCraftFormula();

	int			CheckMyItem( int itemTableIdx, int count );
};






enum
{
	IT_TUTORIAL_WIN_TITLE		=	0,
	IT_TUTORIAL_WIN_NAME_U,
	IT_TUTORIAL_WIN_CHAT_U,
	IT_TUTORIAL_WIN_SELECT_U,

	NUM_IT_TUTORIAL_WIN
};

#define MAX_COUNT_IF_HELP_SELECT_STRING		10

class CIFTutorialWin : public CIFWindow
{
public:

	
	CIFText			*m_textCtl[NUM_IT_TUTORIAL_WIN];
	
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;
	CIFControl		*m_selectBar;

	
	CIFButtonCtl	*m_closeBtn;
	CIFButtonCtl	*m_prevBtn;
	CIFButtonCtl	*m_nextBtn;


	char			m_messageStr[4096];
	chatMessage		m_selectStr[MAX_COUNT_IF_HELP_SELECT_STRING];
	int				m_numSelectStr;
	int				m_selectedIdx;
	
	
	int				m_msgIndex;
	int				m_curPage;
	int				m_maxPage;

	int				m_selectBarSavePos;

	int				m_tutorialFlag;
	int				m_tipFlag;

	CIFTutorialWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFTutorialWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		InitHelp();
	void		QuitHelp();
	void		MakeMessage( int idx );
	void		MakeSelectMessage( int pos, int idx );
};





enum
{
	IT_OPTION_WIN_TITLE		=	0,
	IT_OPTION_WIN_HELP,
	IT_OPTION_WIN_OPTION,
	IT_OPTION_WIN_RESTART,
	IT_OPTION_WIN_EXIT,

	NUM_IT_OPTION_WIN
};

class CIFOptionWin  : public CIFWindow
{
public:

	
	CIFText				*m_textCtl[NUM_IT_OPTION_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBottom;
	CIFButtonCtl		*m_helpBtn;
	CIFButtonCtl		*m_optionBtn;
	CIFButtonCtl		*m_restartBtn;
	CIFButtonCtl		*m_exitBtn;
	CIFButtonCtl		*m_closeBtn;

	CIFOptionWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFOptionWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
};






enum
{
	IT_SETTING_WIN_TITLE,
	IT_SETTING_WIN_AUTOTIP,
	IT_SETTING_WIN_BGMVOLUME,
	IT_SETTING_WIN_EFFECTVOLUME,
	IT_SETTING_WIN_ON1,
	IT_SETTING_WIN_OFF1,
	IT_SETTING_WIN_ON2,
	IT_SETTING_WIN_OFF2,
	IT_SETTING_WIN_ON3,
	IT_SETTING_WIN_OFF3,

	NUM_IT_SETTING_WIN
};

class CIFSettingWin : public CIFWindow
{
public:
	CIFText				*m_textCtl[NUM_IT_SETTING_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBarBottom;
	CIFButtonCtl		*m_closeBtn;

	CIFCheckBox			*m_autoTipOnCheckBox;
	CIFCheckBox			*m_autoTipOffCheckBox;
	CIFCheckBox			*m_bgmOnCheckBox;
	CIFCheckBox			*m_bgmOffCheckBox;
	CIFCheckBox			*m_effectOnCheckBox;
	CIFCheckBox			*m_effectOffCheckBox;

	CIFScrollBar		*m_bgmScrollBar;
	CIFScrollBar		*m_effectScrollBar;
	
	OptionSet_t			m_optionSetInfo;

	CIFSettingWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFSettingWin();

	virtual void		PrepareControl();
	virtual void		Render();
	int			WindowFrame();
	int			WindowProcess();
	int			WindowSelection();

	
	int			Selection();

	void		SetOptionSet();
	void		GetOptionSet();

};




enum
{
	IT_WORLD_CHANGE_WIN_TITLE		=	0,
	IT_WORLD_CHANGE_WIN_MESSAGE,
	IT_WORLD_CHANGE_WIN_SELECT,
	IT_WORLD_CHANGE_WIN_CANCEL,
	IT_WORLD_CHANG_WIN_LIST_POS,
	NUM_IT_WORLD_CHANGE_WIN,

	WORLD_LIST_WIN_MAX_NUM			= 13,	
	WORLD_LIST_WIN_HEIGHT			= 17,
	WORLD_LIST_SKIP_NUM				= 2,

	
	
};

class CIFWorld_ChangeWin   : public CIFWindow
{
public:
	int				m_SelectWorldIdx;
	int				m_WorldCount;
	int				m_selectBarSavePos;
	int				m_selectedIdx;
	int				m_WorldItemIdx;

public:

	
	CIFText			*m_textCtl[NUM_IT_WORLD_CHANGE_WIN];	
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;
	CIFControl		*m_ButtonBgBar;
	CIFControl		*m_MessageBgBar;
	
	CIFButtonCtl	*m_closeBtn;
	CIFButtonCtl	*m_selectBtn;
	CIFButtonCtl	*m_cancelBtn;	

	CIFScrollBar	*m_scrollBar;
	CIFControl		*m_selectBar;

public:
	int				m_MaxDrawWorldListNum;
	void			WorldListRender();
	void			SetSelectBar( int idx );
	CIFWorld_ChangeWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFWorld_ChangeWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
};








enum
{
	IT_SERVER_LIST_WIN_TITLE	=	0,
	IT_SERVER_LIST_TEXT_MESSAGE,
	IT_SERVER_LIST_WIN_SELECT,
	IT_SERVER_LIST_WIN_CANCEL,
	IT_SERVER_LIST_WIN_LIST_POS,
	IT_SERVER_LIST_TEXT,
	NUM_IT_SERVERLIST_WIN
};



#define MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN		20
#define SERVER_LIST_SELECTBAR_HEIGHT            15
#define SERVER_LIST_WIN_TOP                     70
#define SERVER_LIST_WIN_WIDTH                  304
#define SERVER_LIST_WIN_SELECT_LIST            210

class CIFServer_ListWin   : public CIFWindow
{	
	enum
	{
		SERVERNAME_LEN = 256,
		MAX_RENDER_SERVER_LIST_COUNT = 8
	};
	
	enum
	{
		SERVER_ASCENDINGFLAG	=	0,  
		SERVER_DESCENDINGFLAG	=	1,  
		STATE_ASCENDINGFLAG		=	2,  
		STATE_DESCENDINGFLAG	=	3   
	};
	
public:
	std::vector<serverGroup_t> m_vpServerList;	
	
	int				m_selectBarSavePos;
	int				m_selectedIdx;
	int				m_ActiveServerCount;
	
	
	int            m_Server_Sort;					
	bool           m_ServerSort_Flag;				
	bool           m_Server_State_Sort_Flag;		
	
	
	
public: 
	CIFControl			*m_login;
	CIFText             *m_ServerText[MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN];
	CIFText             *m_ServerClientState[MAX_COUNT_IF_LOGIN_WIN_SERVER_BTN];
	CIFText				*m_textCtl[NUM_IT_SERVERLIST_WIN];
	
	CIFControl			*m_ServerList;
	CIFButtonCtl		*m_selectBtn;
	CIFButtonCtl		*m_cancelBtn;
	CIFButtonCtl		*m_ServerButton;
	CIFButtonCtl		*m_ServerState;
	
	CIFScrollBar		*m_scrollBar;
	CIFControl			*m_selectBar;

	RECT				m_ServerListCheckRect;
	
	
public:	
	void			ServerListRender();
	void			SetSelectBar( int idx );
	CIFServer_ListWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFServer_ListWin();
	
	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
	
	BOOL				CheckSelectServer();
};



#include "NewDanBattleSystem.h"
class CIFDanWarRequestWin   : public CIFWindow
{	
public:
	enum DanWarRequestWinType
	{
		REQUEST,			
		EXAMINATION,		
		RESULT,				
	} m_DanWarWinType;
	
	
public:
	CIFText				*m_textCtlBoxDoc;			
	CIFText				*m_textCtlDanNameTitle;		
	CIFText				*m_textCtlDanName;			
	CIFText				*m_textCtlWorldTitle;		
	CIFText				*m_textCtlWorldName;		
	CIFText				*m_textCtlDanMemberNumTitle; 
	CIFText				*m_textCtlDanMemberNum;		

	CIFText				*m_textCtlDanWarTypeTitle;	
	CIFComboBox			*m_ComboCtlDanWarType;		

	CIFText				*m_textCtlDanWarTime;		
	CIFComboBox			*m_ComboCtlDanWarTime;		

	CIFText				*m_textCtlWinDanPointTitle;	
	CIFText				*m_textCtlWinDanPoint;		
	CIFText				*m_textCtlLoseDanPointTitle;
	CIFText				*m_textCtlLoseDanPoint;		
	CIFText				*m_textCtlMyPointTitle;		
	CIFText				*m_textCtlMyPoint;			
	CIFText				*m_textCtlBestIDTitle;		
	CIFText				*m_textCtlBestID;			

	
	CIFCheckBox			*m_PrimiumItemCheckBox;
	CIFText				*m_textPrimiumItem;	


	int					m_SaveOkBtnXPos;

	
	CIFButtonCtl		*m_OkBtn;					
	CIFButtonCtl		*m_cancelBtn;

public:
	void				InitDanWarWin(enum DanWarRequestWinType type, class NewDanBattleSystem* pNewDanBattleSystem);	
	void				InitDanWarRequestWin(enum DanWarRequestWinType type, class NewDanBattleSystem* pNewDanBattleSystem);
	void				InitDanWarExaminationWin(enum DanWarRequestWinType type, class NewDanBattleSystem* pNewDanBattleSystem);
	void				InitDanWarResultWin(enum DanWarRequestWinType type, class NewDanBattleSystem* pNewDanBattleSystem);


	
public:			
	CIFDanWarRequestWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFDanWarRequestWin();
	
	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();	
	
};



class CIFDanWarBattleWin   : public CIFWindow
{	
	
public:		
	CIFText				*m_textCtlREMAINTIME_TITLE;
	CIFText				*m_textCtlREMAINTIME_TEXT;	
	CIFText				*m_textCtlREMAINPC_TITLE;	
	CIFText				*m_textCtlREMAINPC_TEXT;		
	CIFText				*m_textCtlMY_GUILD_KILLPOINT_TITLE;	
	CIFText				*m_textCtlMY_GUILD_KILLPOINT_TEXT;	
	CIFText				*m_textCtlPC_KILLPOINT_TITLE;		
	CIFText				*m_textCtlPC_KILLPOINT_TEXT;		
	CIFText				*m_textCtlENEMY_GUILD_KILLPOINT_TITLE;	
	CIFText				*m_textCtlENEMY_GUILD_KILLPOINT_TEXT;

public:
	
	
public:			
	CIFDanWarBattleWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFDanWarBattleWin();
	
	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();	
	
};




#endif 

