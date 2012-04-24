



#include "../global_def.h"

CItemRecycleProc		gItemRecycleProc;


extern CItemTableData gItemTableData;





CItemRecycleProc::CItemRecycleProc()
{

}

CItemRecycleProc::~CItemRecycleProc()
{

}

int CItemRecycleProc::RecycleProc(item_t *_item)
{
	int itemTableIdx	= _item->itemTableIdx;
	int number			= _item->durability + 1;
	int level			= _item->reqLevel;
	
	char szTemp[256];
	
	szTemp[0]='\0'; 

	int UseItemType = gItemTableData.GetUseItemType(&g_itemTable[itemTableIdx]);

	
	if(UseItemType == CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_QUESTSCROLL)
	{
		sprintf( szTemp, g_LPACK.GetMessage(0,947), _item->name );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp );
		return IF_CTL_NONE;
	}

	if(UseItemType == CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_RECYCLE)
	{
		char Temp[256];
		Temp[0]='\0';

		int LpackTab = gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::LPACK_TAB);
		int LpackIdx = gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::LPACK_INDEX);

		if(gItemTableData.IsType(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_DURABILITY_MULTIPLY))
		{	
			int iVal= (int)gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_DURABILITY_MULTIPLY);
			sprintf( Temp, g_LPACK.GetMassage( LpackTab, LpackIdx ), iVal * number );
			strcat(szTemp,Temp);
		}
		else if(gItemTableData.IsType(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_LEVEL_MULTIPLY))
		{
			int iVal= (int)gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_LEVEL_MULTIPLY);
			sprintf( Temp, g_LPACK.GetMassage( LpackTab, LpackIdx ), 1 * level + iVal );
			strcat(szTemp,Temp);
		}
		else if(gItemTableData.IsType(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_LEVEL_EXP_AND_GENEXP_MULTIPLY))
		{
			int iExpVal= (int)gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_LEVEL_EXP_MULTIPLY);
			int iGenExpVal= (int)gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_LEVEL_GENEXP_MULTIPLY);

			sprintf( Temp, g_LPACK.GetMassage( LpackTab, LpackIdx ), iExpVal * level ,  iGenExpVal * level );
			strcat(szTemp,Temp);
		}
		else if(gItemTableData.IsType(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_PARAM_ITEM_FUNCTION))
		{
			int iVal= (int)gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataValue::RECYCLE_PARAM_ITEM_FUNCTION);
			sprintf( Temp, g_LPACK.GetMassage( LpackTab, LpackIdx ), level * iVal);
			strcat(szTemp,Temp);
		}		
	}
	else
	{
		
		
		int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
		if( epackIdx >= 0 && epackIdx < MAX_NUMBER_OF_OWNITEM )
		{
			int ePackItemTableIdx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;
			if(ePackItemTableIdx >= 0 && ePackItemTableIdx <  MAX_NUMBER_OF_ITEM_TABLE)
			{
				if(g_itemTable[ePackItemTableIdx].epkRecycleType == TypeItemTable_t::EPK_RECYCLE_TYPE_DISABLE)
				{
					
					sprintf( szTemp,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,403), g_cgv.myCharacterInfo->item[epackIdx].name );
					g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp );
					return IF_CTL_NONE;
				}
			}
		}

		
		
		int sellCost, supplyRA, supplySA, onceRecoveryRA, onceRecoverySA;
		int raCount, saCount;

		sellCost = g_itemTable[itemTableIdx].sellCost;
		onceRecoveryRA = g_cgv.myCharacterInfo->onceRecoveryRA;
		onceRecoverySA = g_cgv.myCharacterInfo->onceRecoverySA;
		supplyRA = g_itemTable[itemTableIdx].supplyRA;
		supplySA = g_itemTable[itemTableIdx].supplySA;

		
		int valueRa = (int)GetEpackRechargePrice_RA( onceRecoveryRA, 1.0f, (float)g_itemTable[itemTableIdx].level ); 
		int valueSa = (int)GetEpackRechargePrice_SA( onceRecoverySA, 1.0f, (float)g_itemTable[itemTableIdx].level ); 							
		
		
		if( onceRecoveryRA > 0 )
			raCount = sellCost * 3/10 * supplyRA * 0.01 / valueRa;
		else
			raCount = 0;
		if( onceRecoverySA > 0 )
			saCount = sellCost * 3/10 * supplySA * 0.01 / valueSa;
		else
			saCount = 0;

		sprintf( szTemp, g_LPACK.GetMassage(0,135), _item->name, raCount, saCount );
		
	}

	g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, 0, IF_MESSAGE_EPACK_HOLE_ITEM );
	return IF_CTL_NONE;
}