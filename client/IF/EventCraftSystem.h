



#if !defined(AFX_EVENTCRAFTSYSTEM_H__197B247E_DF66_4A04_A431_DDBF6EE55464__INCLUDED_)
#define AFX_EVENTCRAFTSYSTEM_H__197B247E_DF66_4A04_A431_DDBF6EE55464__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

enum
{
	IT_EVENT_CRAFT_WIN_TITLE		=	0,

	IT_EVENT_CRAFT_WIN_INVEN_0_POS,
	IT_EVENT_CRAFT_WIN_INVEN_1_POS,
	IT_EVENT_CRAFT_WIN_INVEN_2_POS,
	IT_EVENT_CRAFT_WIN_INVEN_3_POS,

	IT_EVENT_CRAFT_WIN_PROGRESS,
	
	IT_EVENT_CRAFT_WIN_OK,
	IT_EVENT_CRAFT_WIN_CANCEL,

	
	IF_EVENT_CRAFT_WIN_RESULT_TEXT_POS,

	NUM_IT_EVENT_CRAFT_WIN
};

#define EVENT_CRAFT_ITEM_SIZE	38
#define MAX_EVENT_CRAFT_ITEM	8

class CIFEventCraftWin  : public CIFWindow
{
public:
	enum
	{
		CRAFT_RESISTER=1,
		CRAFT_GIVE_ITEM,
	};
public:

	CIFEventCraftWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFEventCraftWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void				InitCraft();
	void				UndoCraft();
	void				StartCraft( float craftMaxTime );
	int					GetEventCraftFormula();

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

	
	CIFText			*m_textCtl[NUM_IT_EVENT_CRAFT_WIN];
	
	
	CIFControl		*m_ifcItem[MAX_EVENT_CRAFT_ITEM];

	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;

	
	CIFControl		*m_ResultWin;

	
	CIFControl		*m_progressWin;
	CIFBarCtl		*m_progressBar;
	CIFControl		*m_progressAni;

	
	CIFButtonCtl	*m_okBtn;
	CIFButtonCtl	*m_cancelBtn;

	
	int				m_craftingFlag;
	float			m_craftCurrentTime;
	float			m_craftMaxTime;

	int				m_nSelectEventCraftIndex;

	vector <itemCraftTable_t>	m_EventItemCraftTable;
};


#endif 
