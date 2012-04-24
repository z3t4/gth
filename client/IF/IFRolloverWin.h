



#if !defined(AFX_IFROLLOVERWIN_H__86C7061C_25C5_4986_93EA_9A7BCAB84897__INCLUDED_)
#define AFX_IFROLLOVERWIN_H__86C7061C_25C5_4986_93EA_9A7BCAB84897__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CIFRolloverWin  	: public CIFWindow
{
public:

	CIFControl		*m_win;
	CIFControl		*m_titleLeft;
	CIFControl		*m_titleRight;
	CIFControl		*m_titleCenter;
	CIFControl		*m_titleLeftSide;
	CIFControl		*m_titleRightSide;
	CIFControl		*m_titleBottom;

	char			m_str[2048];
	int				m_outputTextX;
	int				m_outputTextY;
	int				m_outputTextXSize;
	int				m_outputTextYSize;
	int				m_saveArragneType;
	
	CIFRolloverWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFRolloverWin();

	virtual void		PrepareControl();	
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		MakeRolloverWindow( int x, int y, char *str, int forcedWinFlag, int xsize, int ysize, int arrangeType );
};



class CIFGuildAuthoritySettingWin  	: public CIFWindow
{
public:

	CIFControl		*m_win;
	CIFControl		*m_titleLeft;
	CIFControl		*m_titleRight;
	CIFControl		*m_titleCenter;
	CIFControl		*m_titleLeftSide;
	CIFControl		*m_titleRightSide;
	CIFControl		*m_titleBottom;

	CIFButtonCtl	*m_joinBtn; 
	CIFButtonCtl	*m_disorganizeBtn; 
	CIFButtonCtl	*m_banishBtn; 
	CIFButtonCtl	*m_setMasterBtn; 
	CIFButtonCtl	*m_applyBloodPledgeBtn; 
	CIFButtonCtl	*m_breakBloodPledgeBtn; 
	CIFButtonCtl	*m_applyBloodPledgeBattleBtn; 
	CIFButtonCtl	*m_requestPvPBtn; 
	CIFButtonCtl	*m_modeSettingBtn; 
	CIFButtonCtl	*m_resetPositionBtn; 

	char			m_str[2048];
	int				m_outputTextX;
	int				m_outputTextY;
	int				m_outputTextXSize;
	int				m_outputTextYSize;
	int				m_saveArragneType;

	CIFGuildAuthoritySettingWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFGuildAuthoritySettingWin();

	virtual void		PrepareControl();	
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		MakeRolloverWindow( int x, int y, char *str, int forcedWinFlag, int xsize, int ysize, int arrangeType );
};

#endif 
