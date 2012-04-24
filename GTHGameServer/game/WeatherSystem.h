



#if !defined(AFX_WEATHERSYSTEM_H__43CEA2FD_9BE8_474B_9CF2_AF26C54FBCDF__INCLUDED_)
#define AFX_WEATHERSYSTEM_H__43CEA2FD_9BE8_474B_9CF2_AF26C54FBCDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CWeatherSystem  
{	
public:
	
	enum enumWeatherSystemProtocol		
	{
		CC_WEATHER_SYSTEM_REQUEST_USE_WEATHER_ITEM,				
		GSC_WEATHER_SYSTEM_REPLY_USE_WEATHER_ITEM,					
		GSC_WEATHER_SYSTEM_REPLY_CURRENT_WEATHER,					
		

		CC_WEATHER_SYSTEM_REQUEST_WORLD_STATE,						
		GSC_WEATHER_SYSTEM_REQUEST_WORLD_STATE,
		CC_WEATHER_SYSTEM_CHEAT_SETING,

		CC_WEATHER_SYSTEM_NOTICE,
		CC_WEATHER_SYSTEM_CASTING_EVENT_SETING,
	};

	
	enum enumEventWorldState
	{
		WEATHER_SYSTEM_WORLD_PROGRESS,									
		WEATHER_SYSTEM_WORLD_NONE										
	};

	enum enumWeatherSystemEventState
	{
		EVENT_INACTIVE,
		EVENT_ACTIVE		
	};	

	
	
	enum enumWeatherSystemEventError
	{
		ERROR_WEATHER_SYSTEM_USE_ITEM,		
		ERROR_WEATHER_SYSTEM_USE_ITEM2,
		ERROR_WEATHER_SYSTEM_EQUIP_ITEM,
		ERROR_WEATHER_SYSTEM_ACTIVE,
	};	

public:
	enum TIMESTATE
	{
		TIME_DAY       = 0,
		TIME_DAWN      = 1,
		TIME_DUSK      = 2,
		TIME_NIGHT     = 3,
		TIME_FOG       = 4,		
		TIME_RAIN      = 5,
		TIME_SNOW      = 6,			
	};

	struct EventWorld
	{
		int						m_WorldIdx;
		float					m_EventLimitTime;
		enumEventWorldState		m_EventState;		
		TIMESTATE				m_WeatherType;

	}m_EventWorld[MAX_WORLDS];


	enumWeatherSystemEventState		m_EventState;

public:
	void GTH_Process_Weather_System();

	
	
	void GTH_ProcessMessage_WeatherSetting(void);	

	
	void GTH_SendMessage_Reply_WeatherSystem_UseItem(playerCharacter_t*, int reply, int error, int invenpos);	

	
	void GTH_SendMessage_BroadCast_WeatherSystemEvent(playerCharacter_t*, int worldidx);

	
	void GTH_ProcessMessage_Request_WordlState();

	
	void GTH_SendMessage_Reply_WordlState(playerCharacter_t*, int reply, int state);

	
	void GTH_SendMessage_Reply_WordlStateToMemberServer(int state);

	
	void GTH_SendMessage_Reply_WordlStateToManagerServer(int state);
	
	void GTH_ProcessMessageBroadCastEventSeting();

	
	void GTH_ProcessMessageEventSeting();	
	
	void GTH_ProcessMessage_Event_UserNotice();	
		
public:
	
	void ProcessEventTIme();
	void InitWeatherSystem();


		
public:
	CWeatherSystem();
	virtual ~CWeatherSystem();

};

#endif 
