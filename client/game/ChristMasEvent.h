



#if !defined(AFX_CHRISTMASEVENT_H__9C30DEFF_53FB_47D7_9913_165046629D93__INCLUDED_)
#define AFX_CHRISTMASEVENT_H__9C30DEFF_53FB_47D7_9913_165046629D93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 



#define CHRISTMASEVENT_LIMITTIME	300000  

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
		EVENT_ACTIVE,		
	};	
	
	

	
	
	enum enumChristMasEventError
	{
		ERROR_CHRISTMAS_EVENT_USE_ITEM,		
		ERROR_CHRISTMAS_EVENT_USE_ITEM2,		
		ERROR_CHRISTMAS_EVENT_EQUIP_ITEM,
		ERROR_CHRISTMAS_EVENT_ACTIVE,
	};	

	
	enumChristMasEventState m_EventState;

public:
	void GTH_Process_ChristMas_Event();

	
	void GTH_ProcessMessage_Reply_ChristMas_UseItem(void);
	void GTH_SendMessage_Request_ChristMas_UseItem(int invenpos);	
	
	void GTH_ProcessMessage_ChristMasEvent();

	void GTH_SendMessageEventSeting(int event);

	void GTH_SendMessage_Request_WorldState(int worldidx);		
	void GTH_ProcessMessage_Request_WorldState();		

	void GTH_SendUserNotice(char *notice);

		
public:
	
	void InitChristMasEvent();

public:
	CChristMasEvent();
	virtual ~CChristMasEvent();

};


#endif 
 