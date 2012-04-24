



#if !defined(AFX_WORLDCHANGESYSTEM_H__01998234_9413_49D5_83C6_823234A6BAB6__INCLUDED_)
#define AFX_WORLDCHANGESYSTEM_H__01998234_9413_49D5_83C6_823234A6BAB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

struct WorldChangeSystem
{
public:	
	
	enum
	{ 
		WORLD_CHANGE_EFFECT_IDX	 = 73
	};
	
	enum enumWorldChangeMode
	{
			WORLD_CHANGESYSTEM_NONE,
			WORLD_CHANGESYSTEM_EFFECT_MODE,
	};

	
	enum enumWOrldChangeTime
	{
		WORLD_CHANGE_EFFECT_TIME = 3000,
		ADDITION_TIME			 = 1000,
		WORLD_CHANGE_TOTAL_TIME  = WORLD_CHANGE_EFFECT_TIME + ADDITION_TIME
	};

	enumWorldChangeMode m_SystemMode;
	int					m_SelectWorldIdx;
	int					m_SaveInvenPos;

	WorldChangeSystem();
};

	






#endif 
