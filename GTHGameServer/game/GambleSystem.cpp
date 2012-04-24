




#include "../Global.h"





CGambleSystem::CGambleSystem()
{
	InitGambleSystem();
	
}

CGambleSystem::~CGambleSystem()
{
	
}

CGambleSystem::CollectGambleByType_t::CollectGambleByType_t()
{
	Type			= -1;
	TotalGamebleRate = 0.0f;	
}








CGambleSystem::CollectGambleByType_t* CGambleSystem::GetCollectGambleItem(int Type)
{
	if ( Type < 0 || Type >= MAX_COUNT_OPTION_AVAILABLE_TYPE)
		return NULL;
	
	return &m_CollectGambleItemByType[Type];
}







int	 CGambleSystem::GetTotalGamebleTableNumber(void)
{
	return m_CollectGambleTypeNumber;
}
























BOOL CGambleSystem::isActiveSystem(void)
{
	return m_bActiveSystem;
	
}







void CGambleSystem::InitGambleSystem()
{
	m_bActiveSystem = TRUE;
	m_CollectGambleTypeNumber = 0;
	
	memset(m_CollectGambleItemByType, 0x00, 
		sizeof(CollectGambleByType_t) * MAX_COUNT_OPTION_AVAILABLE_TYPE);	
	
	
	
}


int CGambleSystem::GetGambleItemAvallableOn( int availableon )
{
	if ( availableon & OPTION_AVAILABLE_ON_DAGGER )	
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_DAGGER ;
	
	if ( availableon & OPTION_AVAILABLE_ON_1HSWORD )	
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_1HSWORD ;
	
	if ( availableon & OPTION_AVAILABLE_ON_2HSWORD )	
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_2HSWORD;		
	
	if ( availableon & OPTION_AVAILABLE_ON_BOW )
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_BOW;
	
	if ( availableon & OPTION_AVAILABLE_ON_STAFF )			
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_STAFF;
	
	if ( availableon & OPTION_AVAILABLE_ON_SPIRITOR )	
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_SPIRITOR;
	
	if ( availableon & OPTION_AVAILABLE_ON_CLAW )		
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_CLAW;
	
	if ( availableon & OPTION_AVAILABLE_ON_UPPER )		
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_UPPER;
	
	if ( availableon & OPTION_AVAILABLE_ON_LOWER )		
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_LOWER;
	
	if ( availableon & OPTION_AVAILABLE_ON_AIRPROTECTOR )
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_AIRPROTECTOR;
	
	if ( availableon & OPTION_AVAILABLE_ON_BODYPROTECTOR )	
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_BODYPROTECTOR;
	
	if ( availableon & OPTION_AVAILABLE_ON_GLOVES )		
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_GLOVES;
	
	if ( availableon & OPTION_AVAILABLE_ON_BOOTS )		
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_BOOTS;
	
	if ( availableon & OPTION_AVAILABLE_ON_RING )		
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_RING;
	
	if ( availableon & OPTION_AVAILABLE_ON_NECKLACE )	
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_NECKLACE;
	
	if ( availableon & OPTION_AVAILABLE_ON_BRACELET )	
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_BRACELET;
	
	if ( availableon & OPTION_AVAILABLE_ON_EARRING )	
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_EARRING;
	
	if ( availableon & OPTION_AVAILABLE_ON_EPACK )
		return CollectGambleByType_t::AVAILABLE::OPTION_AVAILABLE_ON_EPACK;	
	
	return -1;
}







void CGambleSystem::CollectGambleItems(void)
{		
	for (int tableIdx = 0; tableIdx < g_itemTableNumber; tableIdx ++)
	{		
		if ( g_itemTable[tableIdx].idx < 0 )
			continue;
		
		
		int availableOn	= GetItemAvailalbeOn( g_itemTable[tableIdx].idx );
		int type		= GetGambleItemAvallableOn(availableOn);		
		
		
		if ( !strcmp (g_itemTable[tableIdx].name, "BLANK" ) ) 
			continue;
		
		
		if ( g_itemTable[tableIdx].sellYn < 2)
			continue;		
		
		
		if ( type == -1)
			type = CollectGambleByType_t.OPTION_AVAILABLE_ON_ETC;		
		
		
		m_CollectGambleItemByType[type].Type = availableOn;
		
		
		
		if ( m_CollectGambleItemByType[type].vGambleItem.empty() )
			m_CollectGambleTypeNumber++;
		
		
		float GamebleRate = g_logic.gambleitemSelectRate[g_itemTable[tableIdx].rarity];
		
		
		CollectGambleByType_t::GambleItem_t GambleItem;
		
		GambleItem.ItemIdx		= g_itemTable[tableIdx].idx;		
		GambleItem.GambleRate	= GamebleRate;				
		
		m_CollectGambleItemByType[type].vGambleItem.push_back(GambleItem);		
		
		
		m_CollectGambleItemByType[type].TotalGamebleRate +=	GamebleRate;
	}	
	
}







void CGambleSystem::CollectGambleOptions(void)
{
	for (int tableIdx =0; tableIdx < MAX_COUNT_OPTION_AVAILABLE_TYPE; tableIdx ++)
	{			
		if ( m_CollectGambleItemByType[tableIdx].Type  <= 0 ) continue;
		
		
		for( int OptionTableIdx = 0; OptionTableIdx < MAX_COUNT_OPTION_TABLE; OptionTableIdx ++ )
		{												
			if( g_optionTable[OptionTableIdx].idx < 0 ) continue;
			
			
			
			
			
			
			if( !( g_optionTable[OptionTableIdx].availableOn & m_CollectGambleItemByType[tableIdx].Type ) ) 
				continue;
			
			m_CollectGambleItemByType[tableIdx].vAvailableOption.push_back(
				g_optionTable[OptionTableIdx].idx);
		}	
	}
}







void CGambleSystem::InitMakeStore(storeStruct_t* pMakestore)
{
	if ( pMakestore == NULL )
		return;
	
	memset(pMakestore, 0, sizeof(storeStruct_t));
	pMakestore->itemNumber = 0;		
	
	for (int PageIdx = 0; PageIdx < MAX_NUMBER_OF_STORE_ITEM_PAGE; PageIdx++)
	{
		memset(&pMakestore->defaultItem[PageIdx], 0, sizeof(storeDefaultItemStruct_t));
		memset(&pMakestore->defaultItem[PageIdx].itemIndex, -1, 
			sizeof(int) * MAX_NUMBER_OF_STORE_DEFAULT_ITEM);
		
		pMakestore->defaultItem[PageIdx].defaultNumber =0;
	}	
	
	for (int ItemIdx = 0; ItemIdx < MAX_NUMBER_OF_STORE_ITEM; ItemIdx++)
		pMakestore->item[ItemIdx].item.itemTableIdx = -1;
	
}











BOOL CGambleSystem::MakeGambleStore(storeStruct_t* pMakeGamblestore, 
									storeStruct_t* pGamblestore, playerCharacter_t* pPc)
{
	if ( pPc == NULL )
		return FALSE;
	
	if ( pMakeGamblestore == NULL)
		return FALSE;
	
	if ( pGamblestore == NULL)
		return FALSE;

	if ( m_bActiveSystem == FALSE)
		return FALSE;
	
	
	
	memset(pPc->RegGambleItems, -1, sizeof(int) *  MAX_NUMBER_OF_STORE_ITEM);
	memset(pPc->RegGamblePrice, 0,	sizeof(int) *  MAX_NUMBER_OF_STORE_ITEM);
	
	
	InitMakeStore(pMakeGamblestore);	
	
	
    
	srand( (unsigned) time( NULL ) );
	for (int StoreSlotIdx = 0; StoreSlotIdx < MAX_NUMBER_OF_STORE_ITEM; StoreSlotIdx ++)
	{
		storeItem_t* pStoreItem = &pGamblestore->item[StoreSlotIdx];
		
		RandomRegsterItemToDefaultStore(pMakeGamblestore, 2 + ( rand() % MAX_REGSTER_ITEMCOUNT ), pStoreItem);
	}
	
	
	return DefaultItemToStore(pPc, pMakeGamblestore);
}







BOOL CGambleSystem::DefaultItemToStore(playerCharacter_t* pPc, storeStruct_t* pMakestore)
{
	if ( pMakestore == NULL)
		return FALSE;				
	
	
	int ItemPos				=0;	
	
	for( int StorePageIdx = 0; StorePageIdx < MAX_NUMBER_OF_STORE_ITEM_PAGE; StorePageIdx ++ )
	{	
		
		int PageRegCount	=0;
		
		
		for( int SlotIdx = 0; SlotIdx < MAX_NUMBER_OF_STORE_GENERATE_ITEM ; SlotIdx ++ )
		{
			ItemPos = pMakestore->itemNumber;
			
			if ( ItemPos < 0 || ItemPos >= MAX_NUMBER_OF_STORE_ITEM)				
				return FALSE;
			
			InitItem( &pMakestore->item[ItemPos].item );			
			int itemTableIdx = pMakestore->defaultItem[StorePageIdx].itemIndex[SlotIdx];
			
			if ( itemTableIdx < 0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)
				continue;	
			
			SpawnItem ( &pMakestore->item[ItemPos].item, itemTableIdx );
			
			pMakestore->item[ItemPos].page		= StorePageIdx;			
			pMakestore->item[ItemPos].maxNumber = pMakestore->defaultItem[StorePageIdx].itemNumber[SlotIdx];
			pMakestore->item[ItemPos].curNumber = pMakestore->defaultItem[StorePageIdx].itemNumber[SlotIdx];			
			
			
			int Selecteditemidx = SelectCollectedItem(&pMakestore->item[ItemPos]);					
			
			
			
			float AddPrice = (rand() %( (int)g_logic.GamblePriceRate ) )* 0.01;								
			int CalPrice = 0;
			
			
			
			if ( pMakestore->item[ItemPos].item.itemTableIdx == QUESTION_MARK_ITEM_TABLE_IDX)
			{												
				pPc->RegGamblePrice[ItemPos]	= g_itemTable[QUESTION_MARK_ITEM_TABLE_IDX].gambleCost;
				pMakestore->item[ItemPos].price = g_itemTable[QUESTION_MARK_ITEM_TABLE_IDX].gambleCost;
			}
			else
			{						
				if ( GTH_Random() > 0.5 )											
				{
					CalPrice = g_itemTable[Selecteditemidx].gambleCost + 
						(int)( ((float)(g_itemTable[Selecteditemidx].gambleCost) * AddPrice ) + 0.5f);
				}
				else
				{
					CalPrice = g_itemTable[Selecteditemidx].gambleCost - 
						(int)( ((float)(g_itemTable[Selecteditemidx].gambleCost) * AddPrice ) + 0.5f);
				}
				
				
				if ( CalPrice < 0)
					CalPrice = 1;
				
				pPc->RegGamblePrice[ItemPos]	= CalPrice;				
				pMakestore->item[ItemPos].price = CalPrice;		
				
			}
			PageRegCount++;
			pPc->RegGambleItems[ItemPos]		= Selecteditemidx;			
			pMakestore->itemNumber++;
			
		}			
	}	 
	return TRUE;
}







BOOL CGambleSystem::RandomRegsterItemToDefaultStore(storeStruct_t* pMakestore, int MaxCount, storeItem_t *pStoreItem)
{	
	int RegsterCount	=  0;
	int	SkipCount		=  0;		
	
	
	if ( pStoreItem->page < 0 ||  pStoreItem->page >= MAX_NUMBER_OF_STORE_ITEM_PAGE)
		return FALSE;	
	
	
	if (pStoreItem->item.itemTableIdx < 0 || pStoreItem->item.itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE)		
		return FALSE;		
	
	if (strlen(pStoreItem->item.name) <= 0 )
		return FALSE;		
	
	if ( MaxCount >= MAX_REGSTER_ITEMCOUNT)
		MaxCount = MAX_REGSTER_ITEMCOUNT;
	
	storeDefaultItemStruct_t* pDefaultItem = &pMakestore->defaultItem[pStoreItem->page];
	
	while ( 1 )	
	{
		
		if ( pMakestore->defaultItem[pStoreItem->page].defaultNumber>= MAX_NUMBER_OF_STORE_GENERATE_ITEM)
			return FALSE;		
		
		
		
		if ( SkipCount >= MAX_NUMBER_OF_STORE_GENERATE_ITEM )
			return FALSE;
		
		
		if ( RegsterCount >= MaxCount )
			return TRUE;		
		
		
		int SlotIdx = rand() % MAX_NUMBER_OF_STORE_GENERATE_ITEM;
		
		if ( pDefaultItem->itemIndex[SlotIdx] < 0 )
		{
			pDefaultItem->itemIndex[SlotIdx]  = pStoreItem->item.itemTableIdx;
			pDefaultItem->itemNumber[SlotIdx] = 1;	
			pDefaultItem->defaultNumber++;
			RegsterCount++;			
	
	
			continue;
		}
		SkipCount++;		
	}	
	return TRUE;
}










int CGambleSystem::SelectCollectedItem(storeItem_t* pItem)
{			
	
	int StorItemIdx		= pItem->item.itemTableIdx;
	int availableOn		= GetItemAvailalbeOn( g_itemTable[StorItemIdx].idx );
	
	
	int type			= GetGambleItemAvallableOn(availableOn);	
	
	
	
	
	
	if ( type < 0 || type > CollectGambleByType_t.MAX_COUNT_OPTION_AVAILABLE_TYPE)	
	{
		float	TakeGoldItemRate = 0.8f;
		
		
		if ( GTH_Random() < TakeGoldItemRate )
			return GOLD_ITEM_TABLE_IDX;
		
		
		
		
		
		
		int SelecttypeNum = CollectGambleByType_t::OPTION_AVAILABLE_ON_RING + 
			(rand() % (CollectGambleByType_t::MAX_COUNT_OPTION_AVAILABLE_TYPE - 
			CollectGambleByType_t::OPTION_AVAILABLE_ON_RING) );
		
		
		
		
		int	CurSelectTypeNum = CollectGambleByType_t::OPTION_AVAILABLE_ON_RING;
		for (int typeIdx = CollectGambleByType_t::OPTION_AVAILABLE_ON_RING; 
		typeIdx < CollectGambleByType_t::MAX_COUNT_OPTION_AVAILABLE_TYPE; typeIdx ++)
		{
			if ( m_CollectGambleItemByType[typeIdx].vGambleItem.size() )
				CurSelectTypeNum++;
			
			if ( SelecttypeNum <= CurSelectTypeNum)
			{
				type = typeIdx;			
				break;
			}
		}
		
		
		
		if ( !m_CollectGambleItemByType[type].vGambleItem.size() )
			return GOLD_ITEM_TABLE_IDX;
	}	
	
	
	
	
	
	
	float	SelectRate		= 0.0f;
	float	accPro			= 0.0f;
	
	SelectRate				= GTH_Random();
	
	float TotalRate			= m_CollectGambleItemByType[type].TotalGamebleRate;
	int		selectGambleItemidx = -1;
	int selectRanNum = rand() % m_CollectGambleItemByType[type].vGambleItem.size();
	
	for (int i =0; i < m_CollectGambleItemByType[type].vGambleItem.size(); i ++, selectRanNum++)
	{				
		if ( selectRanNum >= m_CollectGambleItemByType[type].vGambleItem.size())
			selectRanNum = 0;
		
		accPro += m_CollectGambleItemByType[type].vGambleItem[selectRanNum].GambleRate / TotalRate;		
		
		if ( SelectRate <  accPro )		
			break;
		
		TotalRate -= m_CollectGambleItemByType[type].vGambleItem[selectRanNum].GambleRate;
	}	
	
	if ( selectRanNum < 0 )
		selectRanNum = 0 ;
	
	if ( selectRanNum > MAX_NUMBER_OF_ITEM_TABLE)
	{
		if ( m_CollectGambleItemByType[type].vGambleItem.size() > 0)
			selectRanNum = m_CollectGambleItemByType[type].vGambleItem.size() -1;
	}
	
	
	
	
	return  m_CollectGambleItemByType[type].vGambleItem[selectRanNum].ItemIdx;
	
}




#include "../CTools.h"
extern CTools* gcpTools;





























































































































































void CGambleSystem::SetActiveSystem(BOOL bActive)
{
	m_bActiveSystem = bActive;
}
