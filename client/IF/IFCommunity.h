



#if !defined(AFX_IFDLGWIN_H__5108DE34_6191_4F2C_A5C2_A9FAFAB25B69__INCLUDED_)
#define AFX_IFDLGWIN_H__5108DE34_6191_4F2C_A5C2_A9FAFAB25B69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 
#include "../game/Manager.h"





#define		NUM_OUTPUT_CHAT_MESSAGE		11

#define		MAX_OUTPUT_CHAT_MESSAGE		200

#define		MAX_OUTPUT_CHAT_STRING		56

enum
{
	IT_DLG_WIN_TITLE_0		=	0,
	IT_DLG_WIN_TITLE_1,
	IT_DLG_WIN_TITLE_2,
	IT_DLG_WIN_TITLE_3,
	IT_DLG_WIN_TITLE_4,
	IT_DLG_WIN_INPUT_U,
	IT_DLG_WIN_OUTPUT_U,
	
	NUM_IT_DLG_WIN
};

class CIFDlgWin  : public CIFWindow
{
public:

	CIFText						*m_textCtl[NUM_IT_DLG_WIN];

	CIFScrollBar				*m_scrollBar;
	CIFButtonCtl				*m_optionBtn;

	
	chatMessage_t				m_allMsg[MAX_OUTPUT_CHAT_MESSAGE];
	chatMessage_t				m_viewMsg[MAX_OUTPUT_CHAT_MESSAGE];

	int							m_allMsgCurPos;
	int							m_viewMsgCurPos;
	int							m_viewOutPos;
	int							m_numAllMsg;
	int							m_numViewMsg;

	int							m_curSendType;
	bool						m_checkViewType[5][6];

	char						m_whisperName[NAMESTRING];

	CIFDlgWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFDlgWin();

	
	
	int FindMessageIndex( int outPos, int idx );

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		AddChatMessage( char* message, DWORD color = GTHCOLOR_SYSMESSAGE, int type = IF_CHATMSG_TYPE_SYSTEM );
	void		AddChatPosition();
	void		SetScrollBar();
	void		SetDlgInput( int flag );

	void		SetSendString();
	void		SetWhisper( char *name );
	void		SetWhisperFromTarget();
	bool		AddViewMsg();
	bool		UpdateViewMsg();
	bool		IsCheckedType( int type );
	void		SetScrollBarPositionMax();
	void		SetViewTypeDefault( int idx );
};






enum
{
	IT_CHATOPTION_WIN_TITLE,
	IT_CHATOPTION_WIN_NORMALCHAT,
	IT_CHATOPTION_WIN_PARTYCHAT,
	IT_CHATOPTION_WIN_DANCHAT,
	IT_CHATOPTION_WIN_TRADECHAT,
	IT_CHATOPTION_WIN_WHISPERCHAT,
	IT_CHATOPTION_WIN_WORLDCHAT,

	NUM_IT_CHATOPTION_WIN
};

class CIFChatOptionWin : public CIFWindow
{
public:
	CIFText				*m_textCtl[NUM_IT_CHATOPTION_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBottom;

	CIFButtonCtl		*m_saveBtn;
	CIFButtonCtl		*m_cancelBtn;

	CIFCheckBox			*m_checkBtn[6];

	CIFChatOptionWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFChatOptionWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual	int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

};





enum
{
	IT_WORLDCHAT_WIN_TITLE		=	0,
	IT_WORLDCHAT_WIN_NOTICE,
	IT_WORLDCHAT_WIN_FREE_COUNT,

	NUM_IT_WORLDCHAT_WIN
};

class CIFWorldChatWin  : public CIFWindow
{
	
public:
	enum enumType{
		none=0,
		speaker=1,
	};

	enum{
		MAXBYTES_TITLE=100,
		MAXBYTES_DESC=100,
	};
	char		  m_Title[MAXBYTES_TITLE+1];
	char          m_Desc[MAXBYTES_DESC+1];
	enum enumType m_Type;

	void Enable(int state = true,enum enumType Type=none);
	enum enumType GetType(void) const;
	
		
public:

	
	CIFText				*m_textCtl[NUM_IT_WORLDCHAT_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBottom;
	CIFControl			*m_bgMessage;

	CIFButtonCtl		*m_sendBtn;
	CIFButtonCtl		*m_cancelBtn;
	CIFButtonCtl		*m_exitBtn;

	CIFWorldChatWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFWorldChatWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
	bool				CheckWorldChattingCount();
};





enum
{
	IT_CHAT_WIN_TITLE		=	0,
	IT_CHAT_WIN_NPC_NAME_U,
	IT_CHAT_WIN_NPC_CHAT_U,
	IT_CHAT_WIN_SELECT_U,

	NUM_IT_CHAT_WIN
};

#define MAX_COUNT_IF_CHAT_SELECT_STRING		10

class CIFChatWin    : public CIFWindow
{
public:

	
	CIFText			*m_textCtl[NUM_IT_CHAT_WIN];
	
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;
	CIFControl		*m_selectBar;

	
	CIFButtonCtl	*m_closeBtn;
	CIFButtonCtl	*m_prevBtn;
	CIFButtonCtl	*m_nextBtn;


	int				m_npcIdxOnServer;
	char			m_npcName[32];

	char			m_npcMessageStr[4096];
	chatMessage		m_selectStr[MAX_COUNT_IF_CHAT_SELECT_STRING];
	chatMessage		m_saveSelectStr;
	int				m_numSelectStr;
	int				m_selectedIdx;
	
	
	int				m_msgIndex;
	int				m_curPage;
	int				m_maxPage;
	int				m_selectBarSavePos;

	CIFChatWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFChatWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		InitChat();
	void		QuitChat();
	void		MakeNpcMessage( int idx );
	void		MakeSelectMessage( int pos, int idx );
};





enum
{
	IT_PARTY_WIN				=	0,
	IT_PARTY_WIN_M0_LEVEL,
	IT_PARTY_WIN_M0_NAME,
	IT_PARTY_WIN_M1_LEVEL,
	IT_PARTY_WIN_M1_NAME,
	IT_PARTY_WIN_M2_LEVEL,
	IT_PARTY_WIN_M2_NAME,
	IT_PARTY_WIN_M3_LEVEL,
	IT_PARTY_WIN_M3_NAME,
	IT_PARTY_WIN_M4_LEVEL,
	IT_PARTY_WIN_M4_NAME,
	IT_PARTY_WIN_M5_LEVEL,
	IT_PARTY_WIN_M5_NAME,
	IT_PARTY_WIN_M6_LEVEL,
	IT_PARTY_WIN_M6_NAME,
	IT_PARTY_WIN_M7_LEVEL,
	IT_PARTY_WIN_M7_NAME,	

	NUM_IT_PARTY_WIN
};

class CIFPartyWin  : public CIFWindow
{
public:

	
	CIFText			*m_textCtl[NUM_IT_PARTY_WIN];
	
	CIFControl		*m_memberWin[MAX_NUMBER_OF_PARTY_MEMBER];
	CIFBarCtl		*m_memberRABar[MAX_NUMBER_OF_PARTY_MEMBER];
	CIFBarCtl		*m_memberSABar[MAX_NUMBER_OF_PARTY_MEMBER];

	int				m_memberIdx[MAX_NUMBER_OF_PARTY_MEMBER];

	int				m_otherMemberIdxOnServer;
	char			m_otherMemberName[256];

	int				m_leaderIdx;

	int				m_selectedMemberIdx;

	CIFPartyWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFPartyWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		SetPartyMember( int idx, char *name );
	int			GetPartyMemberIdx()		{ return( m_otherMemberIdxOnServer ); }
	char*		GetPartyMemberName()	{ return( m_otherMemberName ); }
	void		GetPartyTarget( int idx );
	void		InitMemberInfo();
	void		UpdateMemberInfo( int idx, int memberIdx, int memberActive, int curRA, int curSA, int maxRA, int maxSA );
};





enum
{
	IT_STORE_WIN_TITLE		=	0,
	IT_STORE_WIN_PAGE_1,
	IT_STORE_WIN_PAGE_2,
	IT_STORE_WIN_PAGE_3,
	IT_STORE_WIN_REPAIR_ONCE,
	IT_STORE_WIN_REPAIR_ALL,
	IT_STORE_WIN_INVEN_POS,

	NUM_IT_STORE_WIN
};

class CIFStoreWin  	: public CIFWindow
{
public:

	
	CIFText			*m_textCtl[NUM_IT_STORE_WIN];
	
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;
	CIFControl		*m_pageBar;
	CIFControl		*m_repairBar;
	CIFControl		*m_invenBar;
	
	
	CIFButtonCtl	*m_closeBtn;
	CIFButtonCtl	*m_repairOnceBtn;
	CIFButtonCtl	*m_repairAllBtn;


	storeItem_t		m_storeItem[MAX_NUMBER_OF_STORE_ITEM];
	float			m_raCost;
	float			m_saCost;
	int				m_curPage;
	int				m_buyItemIdx;
	int				m_sellItemIdx;
	
	
	int				m_StoreType;
	enum
	{
		GENERAL_ITME_STORE,
		LEARN_SKILL_STORE,
		GAMBLE_ITEM_STORE,		
	};
	int				m_PageItemCount[3];
	
	

	CIFStoreWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFStoreWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		InitStore();

	
	void		SetPageName();

	
	BOOL		m_LockGambleStore;


	enum
	{
		STORE_STRLEN	= 256
	};
	
	
};





enum
{
	IT_TRADE_WIN_TITLE			=	0,
	IT_TRADE_WIN_OTHER_NAME_U,
	IT_TRADE_WIN_OTHER_INVEN_POS,
	IT_TRADE_WIN_OTHER_SE,
	IT_TRADE_WIN_OTHER_SE_U,
	IT_TRADE_WIN_OTHER_OK,
	IT_TRADE_WIN_OTHER_ADMIT,
	IT_TRADE_WIN_MY_NAME_U,
	IT_TRADE_WIN_MY_INVEN_POS,
	IT_TRADE_WIN_MY_SE,
	IT_TRADE_WIN_MY_SE_U,
	IT_TRADE_OK,
	IT_TRADE_CANCEL,
	IT_TRADE_ADMIT,
	IT_TRADE_SEINPUT,
	
	NUM_IT_TRADE_WIN
};

class CIFTradeWin  : public CIFWindow
{
public:
	
	
	CIFText				*m_textCtl[NUM_IT_TRADE_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBottom;
	CIFControl			*m_otherBar;
	CIFControl			*m_otherInvenBar;
	CIFControl			*m_otherLine;
	CIFControl			*m_otherSENumBar;
	CIFControl			*m_myBar;
	CIFControl			*m_myInvenBar;
	CIFControl			*m_myLine;
	CIFControl			*m_mySENumBar;

	CIFButtonCtl		*m_okBtn;
	CIFButtonCtl		*m_cancelBtn;
	CIFButtonCtl		*m_admitBtn;
	CIFButtonCtl		*m_closeBtn;
	CIFButtonCtl		*m_seInputBtn;


	int					m_otherIdxOnServer;
	char				m_otherName[256];
	int					m_myTradeSE;
	int					m_otherTradeSE;

	int					m_readyFlag;
	int					m_otherReadyFlag;
	int					m_confirmFlag;
	int					m_otherConfirmFlag;

	float				m_prepareReadyTime;

	CIFTradeWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFTradeWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		SetTradeInfo( int otherIdxOnServer, char* name );
	void		InitTrade();
	void		CancelTrade();
	void		ConfirmTrade();
	int			GetTradeItem( int pos );
	void		SetReady( int type, int flag );
	void		SetConfirm( int flag );
};


class CIFBattingWindow  : public CIFWindow
{
public:
	
	
	CIFText				*m_textCtl[NUM_IT_TRADE_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBottom;
	CIFControl			*m_otherBar;
	CIFControl			*m_otherInvenBar;
	CIFControl			*m_otherLine;
	CIFControl			*m_otherSENumBar;
	CIFControl			*m_myBar;
	CIFControl			*m_myInvenBar;
	CIFControl			*m_myLine;
	CIFControl			*m_mySENumBar;

	CIFButtonCtl		*m_okBtn;
	CIFButtonCtl		*m_cancelBtn;
	CIFButtonCtl		*m_admitBtn;
	CIFButtonCtl		*m_closeBtn;
	CIFButtonCtl		*m_seInputBtn;


	int					m_otherIdxOnServer;
	char				m_otherName[256];
	int					m_myTradeSE;
	int					m_otherTradeSE;

	BOOL					m_readyFlag;
	BOOL				m_otherReadyFlag;
	BOOL					m_confirmFlag;
	BOOL					m_otherConfirmFlag;

	float				m_prepareReadyTime;

	CIFBattingWindow( CIFControlNode *pParentControl = NULL );
	virtual ~CIFBattingWindow();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		SetTradeInfo( int otherIdxOnServer, char* name );
	void		InitTrade();
	void		CancelTrade();
	void		ConfirmTrade();
	void		ConfirmTradeDefeat();
	int			GetTradeItem( int pos );
	void		SetReady( BOOL type, BOOL flag );
	void		SetConfirm( int flag );
};





enum
{
	IT_BOOTH_WIN_TITLE,
	IT_BOOTH_WIN_NAME_BAR,
	IT_BOOTH_WIN_NAME,
	IT_BOOTH_WIN_INFO_BAR_0,
	IT_BOOTH_WIN_INFO_0,
	IT_BOOTH_WIN_ANIM,
	IT_BOOTH_WIN_INVEN_POS,
	IT_BOOTH_WIN_NAK_BAR,
	IT_BOOTH_WIN_NAK,
	IT_BOOTH_WIN_EDIT,
	IT_BOOTH_WIN_CANCEL,

	NUM_IT_BOOTH_WIN,
};

typedef struct 
{
	item_t		item;
	int			price;
} boothBuyItem_t;

class CIFBoothWin  : public	CIFWindow
{
public:

	CIFText				*m_textCtl[NUM_IT_BOOTH_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBottom;
	CIFComboBox			*m_animCombo;

	CIFButtonCtl		*m_editBtn;
	CIFButtonCtl		*m_cancelBtn;

	
	boothBuyItem_t		m_boothBuyItem[MAX_BOOTH_INVENTORY_SIZE];
	chatMessage			m_buyerName[50];
	int					m_buyerNumber;
	
	char				m_sellerName[128];

	char				m_boothName[128];
	char				m_boothInfo[128];

	int					m_boothType;
	int					m_boothState;
	
	int					m_saveItemIdx;
	int					m_totalSellNak;

	CIFBoothWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFBoothWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	
	void		InitBooth( int type );
	void		SetBoothState( int type );
	void		CloseBooth();
	void		TradeItem( int type, int pos, int itemCount );
	void		ExitBooth( int type );
	void		AddBuyer( char* name );
	void		RemoveBuyer( char* name );
};






enum
{
	IT_COMMUNITY_WIN_TITLE,
	IT_COMMUNITY_MAIL,
	IT_COMMUNITY_FRIEND,
	IT_COMMUNITY_CLOSE,
	IT_COMMUNITY_HELPER,

	NUM_IT_COMMUNITY_WIN,
};

class CIFCommunityWin : public CIFWindow
{
public:

	CIFText				*m_textCtl[NUM_IT_COMMUNITY_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBarBottom;

	
	CIFButtonCtl		*m_mailBtn;
	CIFButtonCtl		*m_friendBtn;
	CIFButtonCtl		*m_closeBtn;
	CIFButtonCtl		*m_exitBtn;
	
	CIFButtonCtl		*m_helperBtn;
	
	CIFCommunityWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFCommunityWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
	
};





#define MAIL_WIN_LIST_MAX_NUM		17
#define MAIL_WIN_LIST_LINE_HEIGHT	17

enum
{
	IT_MAIL_WIN_TITLE,
		
	IT_MAIL_WIN_RECVMAIL_TITLE,
	IT_MAIL_WIN_RECVMAIL_MIN,
	IT_MAIL_WIN_RECVMAIL_MAX,

	IT_MAIL_WIN_STOREMAIL_TITLE,
	IT_MAIL_WIN_STOREMAIL_MIN,
	IT_MAIL_WIN_STOREMAIL_MAX,

	IT_MAIL_WIN_LIST_SENDER,
	IT_MAIL_WIN_LIST_TITLE,
	IT_MAIL_WIN_LIST_DATE,
	IT_MAIL_WIN_LIST_STATUS,

	NUM_IT_MAIL_WIN,
};

enum
{
	MAIL_INTERFACE_TYPE_RECVMAIL = 0,
	MAIL_INTERFACE_TYPE_STOREMAIL,
};

class CIFMailWin : public CIFWindow
{
public:

	CIFText				*m_textCtl[NUM_IT_MAIL_WIN];
	CIFCheckBox			*m_checkBtn[MAIL_WIN_LIST_MAX_NUM];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBarBottom;

	CIFControl			*m_recvMailInfo;
	CIFControl			*m_storeMailInfo;

	CIFControl			*m_selectBar;

	CIFButtonCtl		*m_exitBtn;

	CIFButtonCtl		*m_recvMailBtn;
	CIFButtonCtl		*m_storeMailBtn;

	CIFButtonCtl		*m_writeMailBtn;
	CIFButtonCtl		*m_saveMailBtn;
	CIFButtonCtl		*m_deleteMailBtn;
	CIFButtonCtl		*m_cancelBtn;

	CIFScrollBar		*m_scrollBar;

	int					m_interfaceType;

	int					m_mailListCount[2];
	MailEntry			m_mailList[2][MAX_MAIL_COUNT];

	int					m_selectedIdx[2];
	int					m_selectBarSavePos[2];

	bool				m_bDisplay;
	
	CIFMailWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFMailWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void				SetInterfaceType( int type );
	void				UpdateMailList();
	void				DeleteMailList();
	void				SetDisplayMode( bool mode );
	void				DeleteSelectedMails();
	void				SetSelectBar( int idx );
};





#define MAX_MAIL_LINE	22

enum
{
	IT_MAILEDIT_WIN_TITLE,

	IT_MAILEDIT_WIN_DATE_LABEL,
	IT_MAILEDIT_WIN_USERNAME_LABEL,
	IT_MAILEDIT_WIN_MAILTITLE_LABEL,

	IT_MAILEDIT_WIN_DATE,
	IT_MAILEDIT_WIN_USERNAME,
	IT_MAILEDIT_WIN_MAILTITLE,

	IT_MAILEDIT_WIN_CONTENT,
	
	NUM_IT_MAILEDIT_WIN,
};

enum
{
	MAILEDIT_INTERFACE_TYPE_READMAIL = 0,
	MAILEDIT_INTERFACE_TYPE_WRITEMAIL,
};

class CIFMailEditWin : public CIFWindow
{
public:

	CIFText				*m_textCtl[NUM_IT_MAILEDIT_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBarBottom;

	CIFControl			*m_mailInfo;

	CIFButtonCtl		*m_exitBtn;

	CIFButtonCtl		*m_sendMailBtn;
	CIFButtonCtl		*m_cancelBtn;

	CIFScrollBar		*m_scrollBar;

	int					m_interfaceType;

	MailEntry			m_mail;

	bool				m_bDisplay;

	int					m_mailLine[2];

	bool				m_bVisibleCursor;

	CIFMailEditWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFMailEditWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void				SetInterfaceType( int type );
	void				UpdateMailContent();
	void				SetMailContent( MailEntry &mail );
	void				DeleteMailContent();
	void				SetDisplayMode( bool mode );
	
};






enum
{
	IT_FRIEND_WIN_TITLE,
	IT_FRIEND_WIN_NAME,
	IT_FRIEND_WIN_LEVEL,

	NUM_IT_FRIEND_WIN,
};

class CIFFriendWin : public	CIFWindow
{
public:

	CIFText				*m_textCtl[NUM_IT_FRIEND_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBarBottom;

	
	CIFButtonCtl		*m_closeBtn;

	CIFControl			*m_onlineBar;
	CIFControl			*m_offlineBar;

	CIFControl			*m_nameLevelBar;

	int					m_selectedFriendIdx;
	char				m_selectedFriendName[NAMESTRING];
	int					m_totalOnlineFriendCount; 
	int					m_totalFriendCount; 

	CIFFriendWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFFriendWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

			void		AddFriend(friend_t* newFriend);
			void		RemoveFriend(char *friendName);
			void		UpdateFriend(friend_t* oldFriend);

	inline	char*		GetSelectedFriendName()			{	return m_selectedFriendName; }
};






enum
{
	IT_GUILD_WIN_TITLE,

	IT_GUILD_WIN_INFOTAB1,
	IT_GUILD_WIN_INFOTAB2,
	IT_GUILD_WIN_INFOTAB3,
	IT_GUILD_WIN_INFOTAB4,

	IT_GUILD_WIN_GUILDINFO,

	IT_GUILD_WIN_FAMEPOINT,
	IT_GUILD_WIN_RANK,
	IT_GUILD_WIN_EXPPOINT,
	IT_GUILD_WIN_RAIDRPOINT,
	IT_GUILD_WIN_HUNTERPOINT,
	IT_GUILD_WIN_PVPPOINT,

	IT_GUILD_WIN_GUILDSCHEDULETITLE,
	IT_GUILD_WIN_GUILDSCHEDULE,

	IT_GUILD_WIN_USERINFOTITLE,
	IT_GUILD_WIN_ONLINETITLE,
	IT_GUILD_WIN_OFFLINETITLE,

	IT_GUILD_WIN_NAMETITLE,
	IT_GUILD_WIN_LEVELETITLE,
	IT_GUILD_WIN_JOBTITLE,
	IT_GUILD_WIN_LOCATIONTITLE,
	IT_GUILD_WIN_RANKTITLE,

	IT_GUILD_WIN_ONLINECOUNT,
	IT_GUILD_WIN_OFFLINECOUNT,
	IT_GUILD_WIN_TOTALCOUNT,

	
	IT_GUILD_WIN_SECEDE,
	IT_GUILD_WIN_SCHEDULE,
	IT_GUILD_WIN_AUTHORITY,
	IT_GUILD_WIN_CLOSE,

	NUM_IT_GUILD_WIN,
};

#define GUILD_WIN_SCHEDULE_STRING_SIZE	100
#define MAX_SCROLL_LINE1				8
#define MAX_SCROLL_LINE2				7
#define SCROLL_LINE_HEIGHT				17


class CIFGuildWin : public	CIFWindow
{
public:

	CIFText				*m_textCtl[NUM_IT_GUILD_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBarBottom;

	
	CIFButtonCtl		*m_closeBtn;

	
	CIFButtonCtl		*m_secedeBtn;
	CIFButtonCtl		*m_scheduleBtn;
	CIFButtonCtl		*m_authorityBtn;
	CIFButtonCtl		*m_close2Btn;

	CIFControl			*m_infoTab;
	CIFControl			*m_questRaidTab;
	CIFControl			*m_hunterPvPTab;
	CIFControl			*m_pledgeBattleTab;

	CIFControl			*m_countLabelBar;

	CIFScrollBar		*m_scrollBar1;
	CIFScrollBar		*m_scrollBar2;

	CIFControl			*m_selectBar;

	int					m_selectedIdx[2];
	int					m_selectBarSavePos[2];

	char				m_guildName[GUILDNAMESTRING];
	char				m_scheduleString[128];
	
	GuildMemberList_t	m_onlineMember[MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES];
	GuildMemberList_t	m_offlineMember[MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES];
	int					m_onlineMemberNumber;
	int					m_offlineMemberNumber;

	CIFGuildWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFGuildWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	int					GetRankInGuild( int idx );
	
	
	void				PutLabelText();
	void				PutContentText();

	void				UpdateMemberList();
	void				SetSelectBar( int idx, int base );
	int					GetSelectecIdx();

};




enum
{
	IT_GUILDAUTH_WIN_SUBSCRIBE,
	IT_GUILDAUTH_WIN_DANDIS,
	IT_GUILDAUTH_WIN_BANISH,
	IT_GUILDAUTH_WIN_SETMASTER,
	IT_GUILDAUTH_WIN_BLOODREQ,
	IT_GUILDAUTH_WIN_BLOODDIS,
	IT_GUILDAUTH_WIN_BLOODBAT,
	IT_GUILDAUTH_WIN_PVPREQ,
	IT_GUILDAUTH_WIN_SETMODE,
	IT_GUILDAUTH_WIN_DEPRIVE,

	NUM_IT_GUILDAUTH_WIN,
};

class CIFGuildAuthWin : public	CIFWindow
{
public:

	CIFText				*m_textCtl[NUM_IT_GUILDAUTH_WIN];
	CIFControl			*m_titleBar;

	
	CIFButtonCtl		*m_exitBtn;
	CIFButtonCtl		*m_inviteBtn;
	CIFButtonCtl		*m_danDisBtn;
	CIFButtonCtl		*m_banishBtn;
	CIFButtonCtl		*m_setMasterBtn;
	CIFButtonCtl		*m_bloodReqBtn;
	CIFButtonCtl		*m_bloodDisBtn;
	CIFButtonCtl		*m_bloodBatBtn;
	CIFButtonCtl		*m_pvpReqBtn;
	CIFButtonCtl		*m_setModeBtn;
	CIFButtonCtl		*m_depriveBtn;

	char				m_targetName[128];

	CIFGuildAuthWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFGuildAuthWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void				SetAllButtonDisable();
	int					GetMyGuildAuthority();
	void				GetTargetName( char name[] );

};





enum
{
	IT_GUILDPVP_WIN_TITLE,

	IT_GUILDPVP_WIN_TIME_TITLE,
	IT_GUILDPVP_WIN_ATEAM_TITLE,
	IT_GUILDPVP_WIN_BTEAM_TITLE,

	NUM_IT_GUILDPVP_WIN,
};

class CIFGuildPvPWin : public CIFWindow
{
private:

	char				m_szTime[24][16];
	char				m_szATeam[24][GUILDNAMESTRING];
	char				m_szBTeam[24][GUILDNAMESTRING];

public:

	CIFText				*m_textCtl[NUM_IT_GUILDPVP_WIN];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBarBottom;

	CIFButtonCtl		*m_closeBtn;
	CIFButtonCtl		*m_enterBtnA;
	CIFButtonCtl		*m_enterBtnB;
	CIFButtonCtl		*m_cancelBtn;

	CIFSpinCtl			*m_timeSelectBtn;

	CIFScrollBar		*m_scrollBar;

	CIFGuildPvPWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFGuildPvPWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
};














enum
{
	IT_GUILD_WAR_WIN_TITLE_TEXT,
	IT_GUILD_WAR_WIN_INFO_TITLE_TEXT,
	IT_GUILD_WAR_WIN_ATEAMTEXT,
	IT_GUILD_WAR_WIN_BTEAMTEXT,

	IT_GUILD_WAR_WIN_ATEAM_NAMETEXT,	
	IT_GUILD_WAR_WIN_BTEAM_NAMETEXT,		
	IT_GUILD_WAR_WIN_ATEAM_INFOTEXT,		
	IT_GUILD_WAR_WIN_BTEAM_INFOTEXT,	
	
	IT_GUILD_WAR_WIN_STANDBY_TITLE_TEXT, 	
	IT_GUILD_WAR_WIN_STANDBY_LIST1_TEXT, 
	IT_GUILD_WAR_WIN_STANDBY_LIST2_TEXT,
	IT_GUILD_WAR_WIN_STANDBY_LIST3_TEXT, 
	IT_GUILD_WAR_WIN_STANDBY_LIST4_TEXT, 
	IT_GUILD_WAR_WIN_STANDBY_LIST5_TEXT, 
	IT_GUILD_WAR_WIN_STANDBY_LIST6_TEXT, 
	IT_GUILD_WAR_WIN_STANDBY_LIST7_TEXT, 
	IT_GUILD_WAR_WIN_STANDBY_LIST8_TEXT, 
	IT_GUILD_WAR_WIN_STANDBY_LIST9_TEXT, 
	IT_GUILD_WAR_WIN_STANDBY_LIST10_TEXT,	
	IT_GUILD_WAR_WIN_STANDBY_LIST11_TEXT,
	IT_GUILD_WAR_WIN_STANDBY_LIST12_TEXT,
	IT_GUILD_WAR_WIN_STANDBY_LIST13_TEXT,
	IT_GUILD_WAR_WIN_STANDBY_LIST14_TEXT,
	IT_GUILD_WAR_WIN_STANDBY_LIST15_TEXT,
	IT_GUILD_WAR_WIN_STANDBY_LIST16_TEXT,
	IT_GUILD_WAR_WIN_STANDBY_LIST17_TEXT,
	IT_GUILD_WAR_WIN_STANDBY_LIST18_TEXT,
	IT_GUILD_WAR_WIN_STANDBY_LIST19_TEXT,
	IT_GUILD_WAR_WIN_STANDBY_LIST20_TEXT,

	NUM_IT_GUILDARENA_WIN
};

#define MAX_STANDBYCOUNT	20


class CIFGuildArenaWin : public CIFWindow
{	




























public:
	
	char				m_szATeamList[NAMESTRING];						
	char				m_szBTeamList[NAMESTRING];						

	int					m_RegAteamIdx;
	int					m_RegBteamIdx;
	int					m_MyTextIdxinStandby;							

public:
	CIFText				*m_textCtl[NUM_IT_GUILDARENA_WIN];				
	char				m_szStandbyList[MAX_STANDBYCOUNT][NAMESTRING];	
	int					m_szStandby_danIdx_List[MAX_STANDBYCOUNT];		
	int					m_StandByCount;												
	CIFControl			*m_title;
	CIFControl			*m_title_bar;

	CIFScrollBar		*m_scrollBarStandby;

	CIFButtonCtl		*m_requestbtn;
	CIFButtonCtl		*m_startBtn;
	CIFButtonCtl		*m_cancelBtn;
	CIFButtonCtl		*m_closeBtn;

	
	int					m_selectStandbyIdx;								
	int					m_ReadyBattleFlag;								

	enum
	{
			STATE_NONE,
			STATE_REQUEST,
			STATE_READY,
			STATE_BATTLE,
			STATE_END,
			STATE_EVENT		

	};


	CIFGuildArenaWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFGuildArenaWin();

	void				ClearStandbyList();
	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();	

	
	void				ClearRegTeams();
};






class CUserInfo{

public:
	char m_czUserID[NAMESTRING];
	BOOL m_bisLeader;
	short m_iwin;
	short m_idraw;
	short m_idefeat;
	short m_iindex;
	CIFText m_textNameView;
	CUserInfo():m_bisLeader(FALSE),m_iwin(0),m_idraw(0),
		m_idefeat(0),m_iindex(0)
	{
		memset(m_czUserID,0,NAMESTRING);
	}

	CUserInfo(CUserInfo &ruserinfo): m_bisLeader(ruserinfo.m_bisLeader),m_iwin(ruserinfo.m_iwin),
		m_idraw(ruserinfo.m_idraw),m_idefeat(ruserinfo.m_idefeat),m_iindex(ruserinfo.m_iindex),m_textNameView(ruserinfo.m_textNameView)
	{
		strcpy(m_czUserID,ruserinfo.m_czUserID);
	}

	virtual~CUserInfo(){}
	
	CUserInfo& operator=(const CUserInfo &ruserinfo)
	{
		if(this==&ruserinfo) return *this;
		 strcpy(m_czUserID,ruserinfo.m_czUserID);
		 m_bisLeader=ruserinfo.m_bisLeader;
		 m_iwin=ruserinfo.m_iwin;
		 m_idraw=ruserinfo.m_idraw;
		 m_idefeat=ruserinfo.m_idefeat;
		 m_iindex=ruserinfo.m_iindex;
		 m_textNameView=ruserinfo.m_textNameView;
		 
		 return *this;

	}

	BOOL operator==(const CUserInfo &ruserinfo) const
	{
		if(strcmp(m_czUserID,ruserinfo.m_czUserID)==0) return TRUE;
		else return FALSE;
	}


};



class CIFGonryunBattlePractice : public CIFWindow
{	

public:

  
	char				m_szATeamList[NAMESTRING+1];						
	char				m_szBTeamList[NAMESTRING+1];						

	int					m_RegAteamIdx;
	int					m_RegBteamIdx;
	int					m_MyTextIdxinStandby;							
	BOOL				m_isLeader;
	int					m_currentUserCount;
	BOOL				m_bbattleMode;									

public:
	
	CIFText			*m_textCtl[NUM_IT_GUILDARENA_WIN];				
   	char				m_szStandbyList[MAX_STANDBYCOUNT][NAMESTRING+1];	
	struct tagRecorder{		
		int win;
		int draw;
		int lose;
		int level;
		struct tagRecorder():win(0),draw(0),lose(0),level(0){}
	};

	enum{
		Max_Sugestor=20,
	};
	
	struct tagRecorder m_arrayUserInfo[Max_Sugestor];
	
	int					m_StandByCount;												
	CIFControl			*m_title;
	CIFControl			*m_title_bar;

	CIFScrollBar		*m_scrollBarStandby;

	CIFButtonCtl		*m_requestbtn;
	CIFButtonCtl		*m_startBtn;
	CIFButtonCtl		*m_cancelBtn;
	CIFButtonCtl		*m_closeBtn;

	
	int					m_selectStandbyIdx;								
	int					m_ReadyBattleFlag;								

	enum
	{
			STATE_NONE,
			STATE_REQUEST,
			STATE_READY,
			STATE_BATTLE,
			STATE_END
	};


	CIFGonryunBattlePractice( CIFControlNode *pParentControl = NULL );
	virtual ~CIFGonryunBattlePractice();

	void				ClearStandbyList();
	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();	

	
	void				ClearRegTeams();
};


class CIFPostWin : public CIFWindow
{
public:

	CIFText				*m_textCtl[NUM_IT_MAIL_WIN];
	CIFCheckBox			*m_checkBtn[MAIL_WIN_LIST_MAX_NUM];

	CIFControl			*m_titleBar;
	CIFControl			*m_titleBarBottom;

	CIFControl			*m_recvMailInfo;
	CIFControl			*m_storeMailInfo;

	CIFControl			*m_selectBar;

	CIFButtonCtl		*m_exitBtn;

	CIFButtonCtl		*m_recvMailBtn;
	CIFButtonCtl		*m_storeMailBtn;

	CIFButtonCtl		*m_writeMailBtn;
	CIFButtonCtl		*m_saveMailBtn;
	CIFButtonCtl		*m_deleteMailBtn;
	CIFButtonCtl		*m_cancelBtn;

	CIFScrollBar		*m_scrollBar;

	int					m_interfaceType;

	
	
	int					m_selectedIdx;
	int					m_selectBarSavePos;

	bool				m_bDisplay;
	
	CIFPostWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFPostWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void				SetInterfaceType( int type );
	void				UpdateMailList();
	void				DeleteMailList();
	void				SetDisplayMode( bool mode );
	
	void				SetSelectBar( int idx );
	void				InitIFPost();				
	
};

enum
{
	IT_MAILBOX_WIN_TITLE			=	0,	
	IT_MAILBOX_WIN_SENDER_TEXT	,
	IT_MAILBOX_WIN_SENDER_NAME,

	IT_MAILBOX_WIN_MESSAGETITLE_TEXT,
	IT_MAILBOX_WIN_MESSAGETITLE,

	IT_MAILBOX_WIN_MESSAGE_TEXT,
	IT_MAILBOX_WIN_MESSAGE	,
	IT_MAILBOX_WIN_MY_INVEN_POS,
	IT_MAILBOX_WIN_PRICE_TEXT,
	IT_MAILBOX_WIN_PRICE,
	IT_MAILBOX_WIN_OK_TEXT,
	IT_MAILBOX_WIN_RECV_TEXT,
	NUM_IT_MAILBOX_WIN
};


class CIFMailBoxWin  : public CIFWindow
{
public:
	
	
	CIFText				*m_textCtl[NUM_IT_TRADE_WIN];


	CIFControl			*m_titleBar;
	
	CIFButtonCtl		*m_okBtn;	
	CIFButtonCtl		*m_recvBtn;

	int					m_otherIdxOnServer;		


	char				m_message[CPostManager::POST_STRSIZE+1];
	char                m_name[NAMESTRING+1];
	char			m_title[CPostManager::POST_TITLESIZE+1];
	int					m_nak;
	bool				m_bDisplay;
	bool				m_bVisibleCursor;



	CIFScrollBar* m_scrollBar;
	int m_mailLine[2];

	CIFMailBoxWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFMailBoxWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		SetTradeInfo( int otherIdxOnServer, char* name );
	void		InitMailBoxWin();
	void		CancelTrade();
	void		ConfirmTrade();
	int			GetPostItem( int pos );
	void		SetReady( int type, int flag );
	void		SetConfirm( int flag );

	void		SetDisplayMode( bool mode );


	void		GeneralItem_Render(CPostManager::PostPackage_t* pPostPackage, int MaxItemCount);
	void		StackItem_Render(CPostManager::PostPackage_t* pPostPackage, int MaxItemCount, char *szTemp);
	void        InputText_Render(CPostManager::PostPackage_t* pPostPackage, char *szTemp);
	int			OKButtonProcess(CPostManager::PostPackage_t* pPostPackage);
	int			RecvButtonProcess(CPostManager::PostPackage_t* pPostPackage);
	void		WriteModeProcess(void);
	int			ItemInfomationProcess(CPostManager::PostPackage_t* pPostPackage);


	void		UndoMailBoxInventory(void);
	void		SendItemDelete(void);

};





#endif 
