



#if !defined(AFX_SKILLITEM_H__000E36C4_7627_4D3D_A8A5_00D611C82BBF__INCLUDED_)
#define AFX_SKILLITEM_H__000E36C4_7627_4D3D_A8A5_00D611C82BBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#define MAX_SKILLITEM_UNIT	10

class CSkillItemUnit
{
public:
	CSkillItemUnit()
	{
		Init();
	}

	~CSkillItemUnit()
	{
	}

	void Init()
	{
		m_nItemTableIdx = -1;
		m_nLevel = -1;
		m_nType=-1;
	}

	void Set(int Index,int Level,int nType=-1)
	{
		m_nItemTableIdx = Index;
		m_nLevel = Level;
		m_nType = nType;
	}

public:
	int		m_nItemTableIdx;
	int		m_nLevel;
	int		m_nType;
};

class CSkillItemSet
{
public:
	CSkillItemSet()
	{	
		Init();
	}

	~CSkillItemSet()
	{
	}

	void			Init()
	{
		m_nSkillID=-1;
		for(int i=0;i<MAX_SKILLITEM_UNIT;i++)
		{
			m_ItemList[i].Init();
		}
	}

	void			Insert(CSkillItemUnit temp)
	{
		for(int i=0;i<MAX_SKILLITEM_UNIT;i++)
		{
			if(m_ItemList[i].m_nItemTableIdx < 0)
			{
				m_ItemList[i] = temp;
				break;
			}
		}
	}

	void			Delete(int Idx)
	{
		if(Idx <0 || Idx >= MAX_SKILLITEM_UNIT) return;
		m_ItemList[Idx].Init();
		
		for(int i=Idx;i<MAX_SKILLITEM_UNIT-1;i++)
		{
			m_ItemList[i] = m_ItemList[i+1];
		}

		m_ItemList[MAX_SKILLITEM_UNIT-1].Init();
	}


	int				Size()
	{
		int count=0;
		for(int i=0;i<MAX_SKILLITEM_UNIT;i++)
		{
			if(m_ItemList[i].m_nItemTableIdx>= 0)
			{
				count ++;
			}
		}
		return count;
	}

public:
	int				m_nSkillID;
	CSkillItemUnit  m_ItemList[MAX_SKILLITEM_UNIT];
};

class CSkillItemData
{
public:
	CSkillItemData()
	{		
	}

	~CSkillItemData()
	{
	}

	CSkillItemSet * Get(int Index)
	{
		if(Index < 0 || Index >= m_SkillItemList.size())
		{
			return NULL;
		}

		return m_SkillItemList[Index];
	}	

	void Insert(CSkillItemSet *temp)
	{
		m_SkillItemList.push_back(temp);
	}

	void	Clear()
	{
		int size = m_SkillItemList.size();

		for(int i=0;i<size;i++)
		{
			CSkillItemSet *temp = m_SkillItemList[i];
			SAFE_DELETE(temp);
		}

		m_SkillItemList.clear();
	}

public:
	vector <CSkillItemSet *> m_SkillItemList;
};


class CSkillItem  
{
public:

	enum 
	{
		ITEM_TYPE_NONE=0,
		ITEM_TYPE_ENABLE_DEAD_SELF_ITEM,		

		
		ITEM_TYPE_TRANSFORM_ITEM,

		NUM_OF_ITEM_TYPE,
	};

	CSkillItem();
	virtual ~CSkillItem();

	CSkillItemData *	GetSkillItemList()
	{
		return &m_SkillItemData;
	}

	bool				IfSkillItem(int itemTableIdx);
	void				SetDataList();
	bool				CheckValid_UseSkillItem(const int _InventoryIdx);
	int					GetSkillTableIndex(int itemTableIdx);
	int					GetLevel(int itemTableIdx);
	int					GetSkillItemTableIndex(int InvenPos);

	
	CSkillItemSet *		GetSkillItem(int itemTableIdx);
	bool				IsType(int itemTableIdx, int Type);

public:
	typedef map<int , int> CMap_SkillItemData;
	CMap_SkillItemData	m_mapSkillItem;	

	CSkillItemData		m_SkillItemData;
};

extern CSkillItem g_SkillItem;

#endif 
