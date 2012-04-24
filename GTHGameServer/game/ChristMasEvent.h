



#if !defined(AFX_CHRISTMASEVENT_H__9C30DEFF_53FB_47D7_9913_165046629D93__INCLUDED_)
#define AFX_CHRISTMASEVENT_H__9C30DEFF_53FB_47D7_9913_165046629D93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 



#define CHRISTMASEVENT_LIMITTIME	60 * 1000 * 5  

class CChristMasEvent  
{		

public:
	
	enum enumChristMasProtocol		
	{
		CC_CHRISTMAS_EVENT_REQUEST_USE_SNOWITEM,				
		GSC_CHRISTMAS_EVENT_REPLY_USE_SNOWITEM,					
		GSC_CHRISTMAS_EVENT_REPLY_CHRISTEVENT,					
		CC_CHRISTMAS_EVENT_REQUEST_EQUIPE_ITEM,					
		GSC_CHRISTMAS_EVENT_REPLY_EQUIPE_ITEM,					
		CC_CHRISTMAS_EVENT_SETING,								

		CC_CHRISTMAS_EVENT_REQUEST_WORLD_STATE,					
		GSC_CHRISTMAS_EVENT_REQUEST_WORLD_STATE,

		CC_CHRISTMAS_EVENT_NOTICE,
		CC_CHRISTMAS_BROAD_CASTING_EVENT_SETING,
	};

	
	enum enumEventWorldState
	{
		CHRISTMAS_EVENT_WORLD_PROGRESS,									
		CHRISTMAS_EVENT_WORLD_NONE										
	};

	enum enumChristMasEventState
	{
		EVENT_INACTIVE,
		EVENT_ACTIVE		
	};	

	
	
	enum enumChristMasEventError
	{
		ERROR_CHRISTMAS_EVENT_USE_ITEM,		
		ERROR_CHRISTMAS_EVENT_USE_ITEM2,
		ERROR_CHRISTMAS_EVENT_EQUIP_ITEM,
		ERROR_CHRISTMAS_EVENT_ACTIVE,
	};
	
	enum enumChristMasItemIdx
	{
		SANTA_WEAPON	=	384,		
		SANTA_UPPER		=	497,
		SANTA_LOWER		=   507,		
		SANTA_GLOVES	=	385,
		SANTA_BOOTS		=	405,		
	};
		
public:
	struct EventWorld
	{
		int						m_WorldIdx;
		float					m_EventLimitTime;
		enumEventWorldState		m_EventState;
	}m_EventWorld[MAX_WORLDS];


	enumChristMasEventState		m_EventState;

public:
	void GTH_Process_ChristMas_Event();

	
	void GTH_ProcessMessage_ChristMas_UseItem(void);

	
	void GTH_SendMessage_Reply_ChristMas_UseItem(playerCharacter_t*, int reply, int error, int invenpos);	

	
	void GTH_SendMessage_BroadCast_ChristMasEvent(playerCharacter_t*, int worldidx);

	
	void GTH_ProcessMessage_Request_WordlState();

	
	void GTH_SendMessage_Reply_WordlState(playerCharacter_t*, int reply, int state);

	
	void GTH_SendMessage_Reply_WordlStateToMemberServer(int state);

	
	void GTH_SendMessage_Reply_WordlStateToManagerServer(int state);
	
	void GTH_ProcessMessageBroadCastEventSeting();

	
	void GTH_ProcessMessageEventSeting();

	
	BOOL GTH_IsChristMasItem(int ItemIdx);

	
	BOOL GTH_IsEnableEquipChristMasItem();

	
	BOOL GTH_IsEquipChristMasItem();

	BOOL GTH_IsAllowItemWerePos(int ItemIdx);
	
	void GTH_ProcessMessage_Event_UserNotice();	

		
public:
	
	void ProcessEventTIme();
	void InitChristMasEvent();


public:
	CChristMasEvent();
	virtual ~CChristMasEvent();

};


#endif 
 