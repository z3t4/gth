



#if !defined(AFX_GAMECONFIGFLAG_H__64911BCF_348E_4592_AE6E_C851A9E5E8B1__INCLUDED_)
#define AFX_GAMECONFIGFLAG_H__64911BCF_348E_4592_AE6E_C851A9E5E8B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


class CGameConfigFlag
{
public:
	CGameConfigFlag()
	{
		Init();
	}

	virtual ~CGameConfigFlag()
	{
	}

	void Init()
	{
		
		m_bCharacter_Storage	=FALSE;

		
		m_bPrecocity_Time		=FALSE;

		
		m_bDie_Reduce_panality	=FALSE;

		
		m_bMap_Attr_Ratio		=FALSE;

		
		m_bItem_Upgrade_Limit	=FALSE;

		
		m_bItem_Craft_Limit		=FALSE;

		
		m_bWorld_Chat_Limit		=FALSE;

		m_nFlag=0;
	}

	int		LoadFlagFile();
	int		GetFlag()
	{
		return m_nFlag;
	}

	void	SetFlag(int flag);
	void	SetFlag();

public:
		
		BOOL	m_bCharacter_Storage;

		
		BOOL	m_bPrecocity_Time;

		
		BOOL	m_bDie_Reduce_panality;

		
		BOOL	m_bMap_Attr_Ratio;

		
		BOOL	m_bItem_Upgrade_Limit;

		
		BOOL	m_bItem_Craft_Limit;

		
		BOOL	m_bWorld_Chat_Limit;	
		
		int		m_nFlag;
	
};

extern CGameConfigFlag gGameConfigFlag;


#endif 
