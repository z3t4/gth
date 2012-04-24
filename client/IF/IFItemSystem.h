



#if !defined(AFX_IFITEMWIN_H__C28FE369_9F8E_4EC7_B0D2_3902FAD9D689__INCLUDED_)
#define AFX_IFITEMWIN_H__C28FE369_9F8E_4EC7_B0D2_3902FAD9D689__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 







































































enum
{
	IT_DEPOT_WIN_TITLE		=	 0,
	IT_DEPOT_WIN_DEPOT,
	IT_DEPOT_WIN_INVEN_POS,
	IT_DEPOT_WIN_SE,
	IT_DEPOT_WIN_SE_U,

	NUM_IT_DEPOT_WIN
};

class CIFDepotWin  : public CIFWindow
{
public:

	CIFText					*m_textCtl[NUM_IT_DEPOT_WIN];

	CIFControl				*m_titleBar;
	CIFControl				*m_titleBottom;
	CIFControl				*m_depotBar;
	CIFControl				*m_invenBar1;
	CIFControl				*m_invenBar2;
	CIFControl				*m_seBar;
		
	CIFButtonCtl			*m_saveBtn;
	CIFButtonCtl			*m_loadBtn;
	CIFButtonCtl			*m_closeBtn;

	int						m_saveDepotPos;
	int						m_depotSE;

	CIFDepotWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFDepotWin();

	virtual void	PrepareControl();
	virtual	void	Render();
	virtual int		WindowFrame();
	virtual int		WindowProcess();
	virtual int		WindowSelection();
};





enum
{
	IT_PRECOCITY_WIN_TITLE		=	0,
	IT_PRECOCITY_WIN_INVEN_POS_0,
	IT_PRECOCITY_WIN_INVEN_POS_1,
	IT_PRECOCITY_WIN_INVEN_POS_2,
	IT_PRECOCITY_WIN_INVEN_POS_3,
	IT_PRECOCITY_WIN_INVEN_POS_4,
	IT_PRECOCITY_WIN_INVEN_POS_5,

	NUM_IT_PRECOCITY_WIN
};

class CIFPrecocityWin  : public CIFWindow
{
public:

	
	CIFText			*m_textCtl[NUM_IT_PRECOCITY_WIN];
	
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;

	
	CIFButtonCtl	*m_closeBtn;

	int				m_savePrecocityPos;


	CIFPrecocityWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFPrecocityWin();

	virtual void		PrepareControl();	
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

};





enum
{
	IT_CRAFT_WIN_TITLE		=	0,
	IT_CRAFT_WIN_RESULT_INVEN_0_POS,
	IT_CRAFT_WIN_RESULT_INVEN_1_POS,
	IT_CRAFT_WIN_SOURCE_INVEN_POS,
	IT_CRAFT_WIN_PROGRESS,
	IT_CRAFT_WIN_OK,
	IT_CRAFT_WIN_CANCEL,

	NUM_IT_CRAFT_WIN
};

class CIFCraftWin  : public CIFWindow
{
public:

	
	CIFText			*m_textCtl[NUM_IT_CRAFT_WIN];
	
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;

	
	CIFControl		*m_progressWin;
	CIFBarCtl		*m_progressBar;
	CIFControl		*m_progressAni;

	
	CIFButtonCtl	*m_okBtn;
	CIFButtonCtl	*m_cancelBtn;

	
	int				m_craftingFlag;
	float			m_craftCurrentTime;
	float			m_craftMaxTime;
	
	
	int				m_nCraftIdx;

	CIFCraftWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFCraftWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		InitCraft();
	void		UndoCraft();
	void		StartCraft( float craftMaxTime );

};





enum
{
	IT_UPGRADE_WIN_TITLE		=	0,
	IT_UPGRADE_WIN_INVEN_0_POS,
	IT_UPGRADE_WIN_INVEN_1_POS,
	IT_UPGRADE_WIN_INVEN_2_POS,
	IT_UPGRADE_WIN_RESULT_INVEN_POS,
	IT_UPGRADE_WIN_PROGRESS,
	IT_UPGRADE_WIN_OK,
	IT_UPGRADE_WIN_CANCEL,

	NUM_IT_UPGRADE_WIN
};
 
class CIFUpgradeWin  : public CIFWindow
{
public:

	
	CIFText			*m_textCtl[NUM_IT_UPGRADE_WIN];
	
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;

	
	CIFControl		*m_progressWin;
	CIFBarCtl		*m_progressBar;
	CIFControl		*m_progressAni;

	
	CIFButtonCtl	*m_okBtn;
	CIFButtonCtl	*m_cancelBtn;

	CIFComboBox		*m_optionCombo;

	
	int				m_upgradeFlag;
	float			m_upgradeCurrentTime;
	float			m_upgradeMaxTime;

	bool			m_bDeleteOption;

	CIFUpgradeWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFUpgradeWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		InitUpgrade();
	void		UndoUpgrade();
	void		StartUpgrade( float upgradeMaxTime );
	void		SetOptionComboBox();
	void		ClearOptionComboBox();

};





 
enum
{
	IT_NPCINV_WIN_TITLE		=	0,
	IT_NPCINV_WIN_INVEN_0_POS,
	IT_NPCINV_WIN_INVEN_1_POS,
	IT_NPCINV_WIN_INVEN_2_POS,
	IT_NPCINV_WIN_INVEN_3_POS,
	IT_NPCINV_WIN_INVEN_4_POS,
	IT_NPCINV_WIN_INVEN_5_POS,
	IT_NPCINV_WIN_OK,
	IT_NPCINV_WIN_CANCEL,

	NUM_IT_NPCINV_WIN
};

class CIFNpcInvWin  : public CIFWindow
{
public:

	
	CIFText			*m_textCtl[NUM_IT_NPCINV_WIN];
	
	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;

	
	CIFButtonCtl	*m_okBtn;
	CIFButtonCtl	*m_cancelBtn;

	






	CIFNpcInvWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFNpcInvWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		InitNpcInventory();
	void		UndoNpcInventory();
	void		StartNpcInventory( float upgradeMaxTime );

};






enum
{
	IT_QUESTSCROLL_WIN_TITLE		=	0,
	IT_QUESTSCROLL_WIN_DEPART,
	IT_QUESTSCROLL_WIN_LEVEL,
	IT_QUESTSCROLL_WIN_REWARD,
	IT_QUESTSCROLL_WIN_PROGRESS,
	IT_QUESTSCROLL_WIN_OK,
	IT_QUESTSCROLL_WIN_CANCEL,

	NUM_IT_QUESTSCROLL_WIN
};

class CIFQuestScrollWin : public CIFWindow
{
public:

	
	CIFControl		*m_titleBar;
	CIFControl		*m_titleBarBottom;
	
	
	CIFText			*m_textCtl[NUM_IT_QUESTSCROLL_WIN];

	
	CIFComboBox		*m_comboDepart;
	CIFComboBox		*m_comboLevel;
	CIFComboBox		*m_comboReward;

	
	CIFControl		*m_progressWin;
	CIFBarCtl		*m_progressBar;
	CIFControl		*m_progressAni;

	
	CIFButtonCtl	*m_okBtn;
	CIFButtonCtl	*m_cancelBtn;

	
	CInterfacePhysics	*m_physics;

	
	int				m_writingFlag;
	bool			m_itemAniFlag;
	bool			m_completeFlag;
	bool			m_viewItemFlag;

	float			m_writingMaxTime;
	float			m_writingCurrentTime;

	float			m_itemAniMaxTime;
	float			m_itemAniCurrentTime;
	
	int				m_selectedQSTableIdx;
	int				m_dptList[3];

	CIFQuestScrollWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFQuestScrollWin();

	virtual void		PrepareControl();
	virtual void		Render();
	virtual int			WindowFrame();
	virtual int			WindowProcess();
	virtual int			WindowSelection();

	void		InitWriting();
	void		StartWriting( float writingMaxTime );
	void		StartItemAni();
	
	int			FindDepartIndex();
};


class CIFCashMallItem  : public CIFWindow
{

private:
	enum enumTextControl
	{
		TITLE_ITEM_NAME			= 0,
		TITLE_OPTION,		
		TITLE_CASHMAILL_DOC,	
		
		MAX_NUM_TEXT_CONTROL,
	};

	enum
	{
		MAX_OPTION_LIST_LEN		= 256,
		OPTION_LIST_WIN_HEIGHT	= 19,			
	};

public:
	enum	enumUseItemMode
	{
		STONE_OF_SAGE,
		STONE_OF_LUCKY,	
		STONE_OF_EACH_NAME,

		NONE_ITEM,
	};

	int					m_iUseItemMode;
public:	

	CIFControl			*m_titleBar;	
	CIFControl			*m_BottomBar;
	CIFControl			*m_BottomBgBar;
	CIFControl			*m_SelectBar;
	CIFControl			*m_Myinven_bg;
	

	CIFButtonCtl		*m_OkBtn;
	POINT				m_OkBtnPos;
	CIFButtonCtl		*m_Cancel;
	POINT				m_CanBtnPos;
	CIFButtonCtl		*m_Exitbtn;
	POINT				m_ExitBtnPos;

	CIFText				*m_textCtl[MAX_NUM_TEXT_CONTROL];
	CIFControl			*m_ItemIven;
	POINT				m_ItemIvenPos;
	CIFText				*m_OptionListPos;

	
	int					m_iSelectIdx;

	
	int					m_selectBarSavePos;

	
	int					m_OptionArryIdx;
	item_t*				m_pEditItem;

	int					m_MaxOptionStrLen;
	float				m_ResizeRatio;	
	float				m_ResizeOldRatio;



public:	

	virtual void				PrepareControl();
	virtual	void				Render();
	virtual	int					WindowFrame();
	virtual	int					WindowProcess();
	virtual	int					WindowSelection();

	void						SortItemOptionList();
	void						RenderItemOptionList();
	
	int							ItemProcess();	
	void						InitCashMallItem();
	int							FIndOptionArryIdx();
	item_t*						GetEditItem();
	void						GetItemOptionString(char* szText, int Len, int OptionArryIdx);

	void						MoveControl(IFControlInfo_t* pIfControlInfo, 
		CIFControl* pIfControl, float XMoveRatio, float YMoveRatio, POINT& OriginalPos);

	void						ReSizeWindow();
	
	
	

public:
	CIFCashMallItem( CIFControlNode *pParentControl = NULL );
	virtual ~CIFCashMallItem(){};

};



#endif 
