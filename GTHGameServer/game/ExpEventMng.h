



#if !defined(AFX_EXPEVENTMNG_H__7EFC291A_26E7_4058_BC3E_EA094D46429A__INCLUDED_)
#define AFX_EXPEVENTMNG_H__7EFC291A_26E7_4058_BC3E_EA094D46429A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CExpEventMng  
{	

private:
	
	enum enumProtocal
	{
		CC_BROAD_CAST_SET_EVENT		= 0x0001,
		SS_BROAD_CAST_SET_EVENT		= 0x0002,
		SS_SET_EVENT				= 0x0003,
		
		SS_REQUEST_EVENT			= 0x0004,
		SS_REPLY_EVENT				= 0x0005,
		
		CC_SET_ACTIVE_SYSTEM		= 0x0006,
		SS_SET_ACTIVE_SYSTEM		= 0x0007,

		CC_REQUEST_EVENT_STATUS		= 0x0008,
		GSC_REPLY_EVENT_STATUS		= 0x0009,
	};
	
	
	
	enum enumEventType
	{
		EXP_RATE,
		ITEM_DROP_RATE,
		NAK_DROP_RATE,
		GENCAPABILITY_RATE,
		ALL_READ_LOGIC,
	};
	

private:

	
	
	float	m_fExpRate;
	float	m_fItemDropRate;
	float	m_fNakDropRate;	
	float	m_fGencapability_rate;

private:		
	BOOL	m_ActiveMng;

public:
	void	ProcessExpEvent();

	void	ProcessMessage_SetEvent();		
	void	ProcessMessage_BroadCast_SetEvent(enum enumEventType, float fvalue);	
	void	ProcessMessage_RequestAllEvent();
	void	ProcessMessage_ReplyAllEvent();
	void	ProcessMessage_RequestActiveSystem();

	void	SendMessage_SetEventTo_MemberServer(enum enumEventType, float value);
	void	SendMessage_SetEventTo_ManagerServer(enum enumEventType, float value);

	void	SetEvent(enum enumEventType, float fValue);
	void	SendMessage_RequestEventToManagerServer();
	void	SendMessage_ReplyEventToServer(int MemberServerIdx, BOOL Active = FALSE);
	void	SendMessage_SetActiveToManagerServer(BOOL Active);
	void	SendMessage_ReplyEventStatus(playerCharacter_t* pc);

public:


public:
	void	InitExpEventMng();
public:
	CExpEventMng();
	virtual ~CExpEventMng();

};

#endif 
