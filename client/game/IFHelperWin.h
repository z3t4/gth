



#if !defined(AFX_IFHELPERWIN1_H__F690DEF0_1DFE_4451_AA11_4B48A517B910__INCLUDED_)
#define AFX_IFHELPERWIN1_H__F690DEF0_1DFE_4451_AA11_4B48A517B910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "..\Game\HelperSystem.h"

enum
{
	IT_HELPER_WIN_TITLE,
	IT_HELPER_WIN_NAME,
	IT_HELPER_WIN_LEVEL,
	IT_HELPER_WIN_SPAWN_BTN,
	IT_HELPER_WIN_DELETE_BTN,
	IT_HELPER_WIN_SHUTDOWN_BTN,
	
	NUM_IT_HELPER_WIN,
};

class CIFHelperWin : public CIFWindow  
{
public:
	CIFHelperWin(CIFControlNode *pParentControl = NULL);
	virtual ~CIFHelperWin();
	CIFText				*m_textCtl[NUM_IT_HELPER_WIN];
	
	CIFControl			*m_titleBar;
	CIFControl			*m_titleBarBottom;
	
	CIFButtonCtl		*m_closeBtn;
	CIFButtonCtl		*m_SpawnBtn;
	CIFButtonCtl		*m_DeleteBtn;
	CIFButtonCtl		*m_ShutDownBtn;
	
	CIFControl			*m_onlineBar;
	CIFControl			*m_offlineBar;
	
	CIFControl			*m_nameLevelBar;
	
	int					m_Mode;   
	int					m_selectedHelperIdx;
	char				m_selectedHelperName[NAMESTRING+1];
	int					m_totalOnlineHelperCount;	
	int					m_totalHelperCount;			
	
	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();
	
	void				AddHelper(HelperEntry* newHelper);
	void				RemoveHelper(char *HelperName);
	void				UpdateHelper(HelperEntry* oldHelper);
	
	inline	char*		GetSelectedHelperName()			{ return m_selectedHelperName; }
};

#endif 
