



#if !defined(AFX_CTOOLS_H__27E5ED11_4334_47F6_80B0_D401A9E1F9D4__INCLUDED_)
#define AFX_CTOOLS_H__27E5ED11_4334_47F6_80B0_D401A9E1F9D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CTools  
{
	private:
		BOOL m_bOK;
	public:
		BOOL isOK(void) const;
		skill_t* GetMySkillTpointer(playerCharacter_t* pPlayer,const int Idx);
		playerCharacter_t* GetPlayerRecordPointer(const int Idx);
		playerCharacter_t* GetPlayerRecordPointer(const querySet_t&	querySet);
		playerCharacter_t* GetPlayerRecordPointer(const char* pName);
		void IncDecChaosPoint(playerCharacter_t* pPlayer,const int changePoint);
		
		
		
		
		BOOL DeleteItemFromMyInventory(playerCharacter_t* pPlayer,const int iInventoryIdx,const int ItemTableIdx, int DeleteNum = 1);

	
	public:
		monsterCharacter_t* GetNPCRecordPointer(const int Idx);


	
	public:
		monsterCharacter_t* GetMonsterRecordPointer(const int Idx);

	
	private:
		
		
		inline BOOL AddItem_Inventory(playerCharacter_t* pPlayer,const int ItemArrayIdx);
		inline BOOL AddItem_DropMap(playerCharacter_t* pPlayer,const int ItemArrayIdx);		

	public:
		
		BOOL		 IsStackItem(const item_t* in_pItem);
		itemTable_t* GetItemTablePointer(const int in_nItemTableIdx);
		item_t*		 GetMyItem(playerCharacter_t* in_pPlayer,const int in_nMyItemArrayIdx);

		
		
		
		
		int AddItem(playerCharacter_t* pPlayer,item_t& item, BOOL bDropItem = TRUE);
		int GetMatchMyInventoryIdx_fromItemTable(playerCharacter_t* pPlayer,const int paramItemTableIdx);
		

		
		BOOL AddItemBatting(playerCharacter_t* pPlayer,item_t& item);
		int GetMatchMyItemIdx_fromINVENTORY(playerCharacter_t* pPlayer,const int paramItemTableIdx);
		
	public:
		
		int	 GetEpkChargeCount(item_t *pItem);

		
		void  SetEpkChargeCount(item_t *pItem,int count);

		
		BOOL isSummonAllow(playerCharacter_t* pPlayer);

		
		
		bool	IsTransformEntity( void *ent, int entType );

		
		item_t* GetMyInventoryItem(playerCharacter_t* pPlayer,const int InventoryPos);
		BOOL	DeleteItemFromMyInventory(playerCharacter_t* pPlayer,const int iInventoryIdx);

		BOOL    isReady(const playerCharacter_t* in_pPlayer);

	
	public:
		CTools();
		virtual ~CTools();

};

#endif 
