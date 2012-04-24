



#ifndef _IFGOLRYUNBATTLE_H_																			
#define _IFGOLRYUNBATTLE_H_

#if _MSC_VER > 1000
#pragma once
#endif 






#define TICKET_LIST_MAX_NUM	16
#define TICKET_LIST_LINE_HEIGHT	18


struct BattleTicketList
{
	int		num;		
	byte	job;
	int     win;
	int		lose;
	float   shareRate;			
	bool	checkFlag;
};

class CIFBattleTicketWin : public CIFWindow
{
public:
	enum 
	{
		IT_TICKET_TITLE,

		IT_TICKET_LIST_NUMBER,
		IT_TICKET_LIST_CLASS,
		IT_TICKET_LIST_WAR_RECORD,
		IT_TICKET_LIST_SHARE_RATE,

		NUM_IT_TICKET,
	};

public:

	CIFText				*m_textCtl[NUM_IT_TICKET];
	CIFCheckBox			*m_checkBtn[TICKET_LIST_MAX_NUM];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBarBottom;

	CIFControl			*m_selectBar;

	CIFButtonCtl		*m_exitBtn;

	CIFButtonCtl		*m_oneWinBtn;
	CIFButtonCtl		*m_twoWinBtn;
	CIFButtonCtl		*m_oneTwoWinBtn;
	
	CIFButtonCtl		*m_renewBtn;
	CIFButtonCtl		*m_buy100Btn;
	CIFButtonCtl		*m_buy10Btn;
	CIFButtonCtl		*m_cancelBtn;

	CIFScrollBar		*m_scrollBar;

	BattleTicketList	m_ticketList[TICKET_LIST_MAX_NUM];

	int					m_interfaceType;
	int					m_winNumber;		

	
	int					m_JoinerCnt;
	
	int					m_selectedIdx[2];
	int					m_selectBarSavePos[2];

	bool				m_bDisplay;			
	
	CIFBattleTicketWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFBattleTicketWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void				UpdateTicketList();
	int					GetTicketCount();
	void				SetDisplayMode( bool mode );
	void				SetSelectBar( int idx );
};







class CIFShareWin : public CIFWindow
{
public:
	enum
	{
	IT_SHARE_TITLE,
	IT_SHARE_WIN_SERIAL_BAR,		
	IT_SHARE_WIN_GUID_BAR,			
	IT_SHARE_WIN_RATE_BAR,			
	IT_SHARE_WIN_LEVEL,				
	IT_SHARE_WIN_INVEN_POS,
	IT_SHARE_WIN_NAK,
	IT_SHARE_WIN_NAK_BAR,

	NUM_IT_SHARE,
	};

public:

	CIFText				*m_textCtl[NUM_IT_SHARE];
	
	CIFControl			*m_titleBar;
	CIFControl			*m_titleBarBottom;

	CIFButtonCtl		*m_battlePrevBtn;
	CIFButtonCtl		*m_battleNextBtn;

	CIFButtonCtl		*m_levelPrevBtn;
	CIFButtonCtl		*m_levelNextBtn;
	
	CIFButtonCtl		*m_changeBtn;
	CIFButtonCtl		*m_cancelBtn;

	bool				m_bDisplay;			
	int					m_iShareNak;		
	int					m_iBattleCnt;			
	int					m_iLevelCnt;			
	
	CIFShareWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFShareWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void				CurrentBattleInformation();
	void				RecordBattleInformation(int battleCnt);
	void				InitShareInventory();
	void				UndoShareInventory();
};









struct TournamentInfo
{
	int		number;				
	int		winNumer;			
	int     stackNum;			
	float   shareRate;			
	int		winMoney;			
	bool	checkFlag;
};

class CIFTournamentWin : public CIFWindow
{
public:
	enum
	{
		IT_TOURNAMENT_TITLE,
		IT_TOURNAMENT_STEP0_NUMBER,
		IT_TOURNAMENT_STEP1_NUMBER,
		IT_TOURNAMENT_STEP2_NUMBER,
		IT_TOURNAMENT_STEP3_NUMBER,
		IT_TOURNAMENT_STEP4_NUMBER,
		IT_TOURNAMENT_WIN_NUMBER,

		NUM_IT_TOURNAMENT,
	};

public:

	CIFText				*m_textCtl[NUM_IT_TOURNAMENT];

	CIFControl			*m_loseX[30];
	

	CIFButtonCtl		*m_closeBtn;
	
	CIFTournamentWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFTournamentWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void				UpdateTournamentList();
	void				SetDisplayMode( bool mode );
	void				SetSelectBar( int idx );
};











struct PrizeInfo
{
	int		number;				
	int		winNumer;			
	int     stackNum;			
	float   shareRate;			
	int		winMoney;			
	bool	checkFlag;
};

class CIFPrizeWin : public CIFWindow
{
public:
	enum
	{
	IT_PRIZE_GRADE,
	IT_PRIZE_ITEM_IMAGE,
	IT_PRIZE_STACK_NUM,
	IT_PRIZE_ITEM_NAME,
	IT_PRIZE_GRADE_POS,	
	IT_PRIZE_ITEM_IMAGE_POS,
	IT_PRIZE_STACK_NUM_POS,	
	IT_PRIZE_ITEM_NAME_POS,
	IT_PRIZE_USER_NAME,
	IT_PRIZE_USER_NAME_POS,

	NUM_IT_PRIZE,
	};

public:

	CIFText				*m_textCtl[NUM_IT_PRIZE];
	
	CIFButtonCtl		*m_closeBtn;

	CIFScrollBar		*m_scrollBar;
	
	CIFPrizeWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFPrizeWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void				UpdatePrizeList();
	void				UpdatePrizeUserList();
	void				SetSelectBar( int idx );
};




class CIFGolryunInfoWin : public CIFWindow
{
public:

	
	CIFText				*m_textCtl;	
	CIFControl			*m_title;

	int					m_RemainTime;
	bool				m_bTimeOver;

	CIFGolryunInfoWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFGolryunInfoWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
};







class CIFObserverMenuWin : public CIFWindow
{
public:
	
	CIFObserverMenuWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFObserverMenuWin();

	CIFButtonCtl		*m_twogyunBtn;
	CIFButtonCtl		*m_shareBtn;
	CIFButtonCtl		*m_seedtableBtn;
	CIFButtonCtl		*m_closeBtn;

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

};








class CIFObserverInfoWin : public CIFWindow
{
public:
	enum
	{
	IT_BATTLE_TIME,
	IT_RED_FIGHTER_NAME,
	IT_BLUE_FIGHTER_NAME,

	NUM_IT_OBSERVER_INFO,
	};

public:
	
	CIFObserverInfoWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFObserverInfoWin();

	CIFText			*m_textCtl[NUM_IT_OBSERVER_INFO];


	CIFBarCtl		*m_redFighterRaBar;
	CIFBarCtl		*m_redFighterSaBar;
	CIFBarCtl		*m_blueFighterRaBar;
	CIFBarCtl		*m_blueFighterSaBar;

	int				m_RemainTime;

	int				m_redFighterNumber;
	int				m_blueFighterNumber;

	int				m_redFighterCurRa;
	int				m_redFighterMaxRa;
	int				m_redFighterCurSa;
	int				m_redFighterMaxSa;

	int				m_blueFighterCurRa;
	int				m_blueFighterMaxRa;
	int				m_blueFighterCurSa;
	int				m_blueFighterMaxSa;

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

};






#define TOURNAMENT_NUMBER_MAX_NUM		31
#define LOSE_DISPLAY_MAX_NUM			30


class CIFTournamentMiniWin : public CIFWindow
{
public:
	enum
	{
	IT_TOURNAMENT_MINI_TITLE,
	IT_TOURNAMENT_MINI_WIN_NUMBER,

	NUM_IT_TOURNAMENT_MINI,
	};

public:

	CIFText				*m_textCtl[NUM_IT_TOURNAMENT_MINI];

	CIFText				*m_textNumCtl[TOURNAMENT_NUMBER_MAX_NUM];
	CIFControl			*m_loseX[LOSE_DISPLAY_MAX_NUM];
	
	CIFButtonCtl		*m_closeBtn;
	
	CIFTournamentMiniWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFTournamentMiniWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void				UpdateTournamentList();
};


#endif 
