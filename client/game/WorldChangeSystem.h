



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
		WORLD_CHANGE_EFFECT_IDX = 73
	};

	enum enumWorldChangeSystemMode
	{
		WORLD_CHANGESYSTEM_NONE,
		WORLD_CHANGESYSTEM_EFFECT_MODE,		

		
		WORLD_CHANGESYSTEM_ITEM_MODE,

		WORLD_CHANGESYSTEM_CHECK, 
	};
	
public:
	enumWorldChangeSystemMode	m_SystemMode;

	int		m_SaveInvenPos;
	int		m_SelectWorldIdx;

public:
	WorldChangeSystem();
	 ~WorldChangeSystem(){};

};




#endif 
