



#if !defined(AFX_IFCHARACTERINVENTORY_H__52C08A6B_D362_44D3_B191_88DF0090DD64__INCLUDED_)
#define AFX_IFCHARACTERINVENTORY_H__52C08A6B_D362_44D3_B191_88DF0090DD64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 





enum
{
	IT_ITEM_WIN_TITLE			=	0,
	IT_ITEM_WIN_WEAPON,
	IT_ITEM_WIN_WEAPON_CHANGE,
	IT_ITEM_WIN_DEF,
	IT_ITEM_WIN_INVEN,
	IT_ITEM_WIN_EQUIP_ATK_POS_0,
	IT_ITEM_WIN_EQUIP_ATK_POS_1,
	IT_ITEM_WIN_EQUIP_UPPER_POS,
	IT_ITEM_WIN_EQUIP_LOWER_POS,
	IT_ITEM_WIN_EQUIP_AIRPROT_POS,
	IT_ITEM_WIN_EQUIP_PROT_POS,
	IT_ITEM_WIN_EQUIP_GLOVES_POS,
	IT_ITEM_WIN_EQUIP_BOOTS_POS,
	IT_ITEM_WIN_EQUIP_ACC_POS_0,
	IT_ITEM_WIN_EQUIP_ACC_POS_1,
	IT_ITEM_WIN_EQUIP_ACC_POS_2,
	IT_ITEM_WIN_EQUIP_ACC_POS_3,
	IT_ITEM_WIN_EQUIP_TRANS_POS,
	IT_ITEM_WIN_EQUIP_EPACK_POS,
	IT_ITEM_WIN_EQUIP_RECYCLE_POS,
	IT_ITEM_WIN_SE,
	IT_ITEM_WIN_SE_U,
	IT_ITEM_WIN_WEIGHT,
	IT_ITEM_WIN_WEIGHT_U,
	IT_ITEM_WIN_INVEN_POS,

	NUM_IT_ITEM_WIN
};

class CIFItemWin  : public CIFWindow
{
public:

	CIFControl			*m_titleBar;
	CIFBarCtl			*m_weightBar;
	CIFButtonCtl		*m_closeBtn;
	CIFButtonCtl		*m_weaponChangeBtn;

	
	CIFText				*m_textCtl[NUM_IT_ITEM_WIN];
	
	int					m_repairType;
	int					m_repairPos;

	CIFItemWin( CIFControlNode *pParentControl = NULL );
	virtual ~CIFItemWin();

	virtual void	PrepareControl();
	virtual	void	Render();
	virtual int		WindowFrame();
	virtual int		WindowProcess();
	virtual int		WindowSelection();


	
	
	struct sLockItem{
		int  nItemTableIdx;
		int  nInvenPos;
		BOOL bLock;
	};	

	sLockItem			m_LockItem[MAX_INVENTORY_SIZE];
	void				ClearLockItems(void);
	void				LockItem(const sLockItem lockitem);
	void				UnLockItem(const int in_nItemTableIdx, const int in_nInvenPos);
	int					IsLockItem(const int in_nInvenPos) const;

	
	int			InventoryProcess();
	int			EquipProcess();
};

#endif 

