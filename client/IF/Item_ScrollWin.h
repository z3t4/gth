



#if !defined(AFX_ITEM_SCROLLWIN_H__94940B9C_E74B_4F63_8E6A_17BEFE9D387E__INCLUDED_)
#define AFX_ITEM_SCROLLWIN_H__94940B9C_E74B_4F63_8E6A_17BEFE9D387E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


class CItem_ScrollWin  : public CIFWindow
{
public:

	
	enum	TEXT_TYPE_ENUM
	{
		IT_ITEM_SCROLLWIN_TITLE		=	0,
		IT_ITEM_SCROLLWIN_ITEM_NAME,
		IT_ITEM_SCROLLWIN_OK,
		IT_ITEM_SCROLLWIN_CANCEL,

		NUM_IT_ITEM_SCROLLWIN
	};

	
	enum	enumWINDOW_TYPE_ENUM
	{
		WT_NORMAL=0,
		WT_PC_SUMMON,
		WT_PC_MOVE_SUMMON,
	};

	enum enumPacketID{

		
		CC_REQUEST_PC_SUMMON=0x0001,
		GSC_REQUEST_PC_SUMMON,
		CC_REPLY_PC_SUMMON,
		GSC_REPLY_PC_SUMMON,

		
		GSC_ERROR_MES,
		GSC_STATE_MES,

		CC_ERROR_MES,
		CC_STATE_MES,

		
		CC_REQUEST_PC_MOVE_SUMMON,
		GSC_REQUEST_PC_MOVE_SUMMON,
		CC_REPLY_PC_MOVE_SUMMON,
		GSC_REPLY_PC_MOVE_SUMMON,

		CC_REDAY_PC_MOVE_SUMMON,
		GSC_REPLY_REDAY_PC_MOVE_SUMMON,
	};

	enum enumErrorCode
	{
		ERROR_NULL_TARGET_PLAYER=1,
		ERROR_ITEM_NOT_HAVE,
		ERROR_INVALID_ITEM,
		ERROR_NOT_SUMMON_ITEM,
		ERROR_NULL_REQUEST_PLAYER,
		
		ERROR_TARGET_PLAYER_NOT_USE_AREA,
		ERROR_REQUEST_PLAYER_NOT_USE_AREA,

		ERROR_NOT_USE_ME_TO_ME,
		ERROR_NOT_MOVE_SUMMON_ITEM,
	};

	enum enumStateCode
	{
		STATE_SEND_REQUEST=1,
		STATE_TARGET_PLAYER_REFUSE,
		STATE_TARGET_PLAYER_OK,


		STATE_MSGBOX_POPUP_SUCC,
		STATE_MSGBOX_POPUP_FAIL,		

		STATE_TARGET_PLAYER_MOVE_REFUSE,
		STATE_TARGET_PLAYER_MOVE_OK,	
	};
		
	enum enumTargetPlayerReply
	{
		REPLY_OK=1,
		REPLY_REFUSE,
	};

public:
	CItem_ScrollWin(CIFControlNode *pParentControl = NULL);
	virtual ~CItem_ScrollWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void				Enable(int state,const item_t *_item,int _InventoryItemPos,int _type);
	void				Decoder();

	void				GSC_Error_Mes();
	void				GSC_State_Mes();
	void				GSC_PC_Summon_Mes();
	void				GSC_PC_Move_Summon_Mes();

	void				Init();	
	void				Select_PC_Summon(int _flag);
	void				Send_Reply_PC_Summon(int _flag);
	void				Select_PC_Move_Summon(int _flag);
	void				Send_Reday_PC_Move_Summon(int _flag);
	void				PC_Move_Summon_Proc();

	bool				IsAllowWorldChange();
	bool				IfWorldChangEffecting();
	bool				IfWorldChangeing();
	void				Gsc_Reply_Ready_Pc_Move_Summon();
		
public:
	enum{
		MAXBYTES_TITLE=100,
		MAXBYTES_ITEM_NAME=32,
	};

	
	CIFText				*m_textCtl[NUM_IT_ITEM_SCROLLWIN];
	CIFControl			*m_bgMessage;
	CIFButtonCtl		*m_OkBtn;
	CIFButtonCtl		*m_cancelBtn;

	char				m_Title[MAXBYTES_TITLE+1];
	char				m_ItemName[MAXBYTES_ITEM_NAME+1];
	int					m_InventoryItemPos;
	char				m_RequestPlayerName[NAMESTRING+1];
	char				m_TargetPlayerName[NAMESTRING+1];
	int					m_nWindowType;

	int					m_TargetPlayerWorldIdx;
	vec3_t				m_TargetPlayerPosition;

	int					m_SendTime;
	bool				m_bSending;

};



#endif 



