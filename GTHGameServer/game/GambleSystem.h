



#if !defined(AFX_GAMBLESYSTEM_H__8B1781DF_A9DA_4671_9184_04530D64A692__INCLUDED_)
#define AFX_GAMBLESYSTEM_H__8B1781DF_A9DA_4671_9184_04530D64A692__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <VECTOR>

class CGambleSystem  
{

public:	
	enum
	{
			GOLD_ITEM_TABLE_IDX				=	459,
			LUCKY_STONE_ITEM_TABLE_IDX		=	620,
			QUESTION_MARK_ITEM_TABLE_IDX	=	97,
			MAX_REGSTER_ITEMCOUNT			=	3,
			GAMBLE_STORE_IDX				=	23
	};

private:	

	
	struct CollectGambleByType_t
	{
		enum AVAILABLE 
		{		
				OPTION_AVAILABLE_ON_DAGGER			= 1,
				OPTION_AVAILABLE_ON_1HSWORD,			
				OPTION_AVAILABLE_ON_2HSWORD,			
				OPTION_AVAILABLE_ON_BOW,				
				OPTION_AVAILABLE_ON_STAFF,			
				OPTION_AVAILABLE_ON_SPIRITOR,		
				OPTION_AVAILABLE_ON_CLAW,			
				OPTION_AVAILABLE_ON_ARMORS,			
				OPTION_AVAILABLE_ON_UPPER,			
				OPTION_AVAILABLE_ON_LOWER,			
				OPTION_AVAILABLE_ON_AIRPROTECTOR,	
				OPTION_AVAILABLE_ON_BODYPROTECTOR,	
				OPTION_AVAILABLE_ON_GLOVES,			
				OPTION_AVAILABLE_ON_BOOTS,			
				OPTION_AVAILABLE_ON_RING,			
				OPTION_AVAILABLE_ON_NECKLACE,		
				OPTION_AVAILABLE_ON_BRACELET,		
				OPTION_AVAILABLE_ON_EARRING,			
				OPTION_AVAILABLE_ON_EPACK,	
				OPTION_AVAILABLE_ON_ETC,				
				MAX_COUNT_OPTION_AVAILABLE_TYPE  = 21,
		};

		
		struct GambleItem_t
		{						
			int		ItemIdx;								
			float	GambleRate;								
			
			GambleItem_t()
			{
				ItemIdx		= -1;
				GambleRate	= 0.0F;
			};
		};		
		int							Type;					
		std::vector<int>			vAvailableOption;				
		std::vector<GambleItem_t>	vGambleItem;			
		float						TotalGamebleRate;		

		CollectGambleByType_t();
	};	

private:	
	CollectGambleByType_t	m_CollectGambleItemByType[MAX_COUNT_OPTION_AVAILABLE_TYPE];
	int						m_CollectGambleTypeNumber;
	BOOL					m_bActiveSystem;

	
public:
	
	void					CollectGambleItems(void);

	
	void					CollectGambleOptions(void);	

	
	BOOL					MakeGambleStore(storeStruct_t* Makestore, 
							storeStruct_t* Gamblestore, playerCharacter_t* pPc);

	
	BOOL					RandomRegsterItemToDefaultStore(storeStruct_t* pMakestore, int MaxCount, storeItem_t *pStoreItem);

	
	BOOL					DefaultItemToStore(playerCharacter_t* pPc, storeStruct_t* pMakestore);

	
	int						SelectCollectedItem(storeItem_t* pItem);

	
	int						GetGambleItemAvallableOn(int availableon);

	
	void					InitMakeStore(storeStruct_t* pMakestore);

	
	void					GTH_ProcessMessage_Request_Buy_GambleItem(playerCharacter_t* pPc);

public:	
	CollectGambleByType_t*	GetCollectGambleItem(int Type);	
	int						GetTotalGamebleTableNumber(void);
	BOOL					isActiveSystem(void);
	void					SetActiveSystem(BOOL bActive);

public:	
	void					InitGambleSystem();	


public:
	CGambleSystem();
	virtual ~CGambleSystem();

};

#endif 
