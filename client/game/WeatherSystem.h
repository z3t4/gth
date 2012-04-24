



#if !defined(AFX_WEATHERSYSTEM_H__67AE6DCF_783D_4C64_B489_3E8CC40113F9__INCLUDED_)
#define AFX_WEATHERSYSTEM_H__67AE6DCF_783D_4C64_B489_3E8CC40113F9__INCLUDED_

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
		CC_WEATHER_SYSTEM_BROAD_CASTING_EVENT_SETING,
		
	};

	
	enum enumEventWorldState
	{
		WEATHER_SYSTEM_WORLD_PROGRESS,									
		WEATHER_SYSTEM_WORLD_NONE										
	};

	enum enumWeatherSystemEventState
	{
		EVENT_INACTIVE,
		EVENT_ACTIVE,		
	};	

	
	
	enum enumWeatherSystemEventError
	{
		ERROR_WEATHER_SYSTEM_USE_ITEM,		
		ERROR_WEATHER_SYSTEM_USE_ITEM2,		
		ERROR_WEATHER_SYSTEM_EQUIP_ITEM,
		ERROR_WEATHER_SYSTEM_ACTIVE,
	};	

	enum enumProgressLimitTime
	{
		SNOW_LITMIT_TIME = 60 * 5 * 1000, 
		RAIN_LITMIT_TIME = 60 * 5 * 1000,
		DEFAULT_LIMIT_TIME = 60 * 5 * 1000,
	};


private:	
	
	float m_Snow_LimitTime;
	float m_Rain_LimitTime;


public:
	
	void GTH_Process_Weather_System();
	

	
	void GTH_ProcessMessage_Reply_WeatherSetting(void);		
	void GTH_SendMessage_Request_WeatherSetting
	( TIMESTATE weather, float limittime = DEFAULT_LIMIT_TIME, bool IsUseItem = 0, int invenpos = -1 );
	
	void GTH_ProcessMessage_WeatherSystemEvent();

	void GTH_SendMessageEventSeting(int event);

	void GTH_SendMessage_Request_WorldState(int worldidx);		
	void GTH_ProcessMessage_Request_WorldState();		

	void GTH_SendUserNotice(char *notice);

		
public:
	
	void InitWeatherSystem();
	


public:
	CWeatherSystem();
	virtual ~CWeatherSystem();

};

#endif 
