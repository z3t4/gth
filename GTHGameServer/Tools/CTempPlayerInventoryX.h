



#if !defined(AFX_CTEMPPLAYERINVENTORYX_H__D5B5B114_473F_4E3F_A4D0_B88814CB1771__INCLUDED_)
#define AFX_CTEMPPLAYERINVENTORYX_H__D5B5B114_473F_4E3F_A4D0_B88814CB1771__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 



class CTempPlayerInventoryX  
{
	public:
		enum ErrorCode{
			ErrorCode_Success=0,
			ErrorCode_unknown,
			ErrorCode_Over_Weight,
			ErrorCode_NotEnough_Inventory,
			ErrorCode_stacknum_overflow,

		};
	
	private:
		struct tagTempInventory{
			int TableIdx;
			int numberIdx;
		};
		struct tagTempInventory m_TempInventory[MAX_INVENTORY_SIZE];
		BOOL m_bOK;
		int  m_curWeight;
		int  m_maxWeight;
	private:
		BOOL Init(const playerCharacter_t* pPlayer);

	public:
		BOOL isOK(void) const;
		const ErrorCode Insert(const item_t* pItem);

		
	
	

	public:
		CTempPlayerInventoryX(const playerCharacter_t* pPlayer);
		virtual ~CTempPlayerInventoryX();

};

#endif 
