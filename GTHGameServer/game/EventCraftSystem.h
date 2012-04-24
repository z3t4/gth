



#if !defined(AFX_EVENTCRAFTSYSTEM_H__F66E8CA1_8EB6_4C13_8D43_5975DB596AD7__INCLUDED_)
#define AFX_EVENTCRAFTSYSTEM_H__F66E8CA1_8EB6_4C13_8D43_5975DB596AD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define MAX_EVENT_CRAFT_ITEM	 8
		
enum
{
	CRAFT_TYPE_NORMAL=0,
	CRAFT_TYPE_RESULT,
	CRAFT_TYPE_EVENT,
};


class CEventCraftSystem  
{
public:
	enum
	{
		CRAFT_RESISTER=1,
		CRAFT_GIVE_ITEM,
	};

public:
	CEventCraftSystem();
	virtual ~CEventCraftSystem();
	
	itemCraftTable_t		*Get(int idx)
	{
		if(idx < 0 || idx >= m_EventItemCraftTable.size()) return NULL;
		return &m_EventItemCraftTable[idx];
	}

	int GetTableSize()
	{
		return m_EventItemCraftTable.size();
	}

	void	Push(itemCraftTable_t temp)
	{
		m_EventItemCraftTable.push_back(temp);
	}

public:
	vector <itemCraftTable_t>	m_EventItemCraftTable;
};

int GTH_CheckEventCraftFormula( playerCharacter_t *pc );
int GTH_EventCheckCraftFormula( playerCharacter_t *pc );
int GTH_EventCraftProcess( playerCharacter_t *pc, int &reply, int &error , bool checkFlag = false );


extern CEventCraftSystem g_EventCraftSystem;

#endif 
