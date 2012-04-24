


#include "../global_def.h"


CItemUseProc g_ItemUseProc;
#include "../game/GolryunBattle/GolryunBattle.h"
extern CItemTableData gItemTableData;


#include "..\CTools.h"
extern CTools gcTools;





CItemUseProc::CItemUseProc()
{

}

CItemUseProc::~CItemUseProc()
{

}




int CItemUseProc::RepairItem(const item_t *_item,const int _Pos)
{
	int itemTableIdx=-1;
	char szTemp[256];

	itemTableIdx = _item->itemTableIdx;
	if(itemTableIdx <0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return( IF_CTL_NONE );


	
	
	g_cgv.saveInventoryIdx = _Pos;
	sprintf( szTemp, g_LPACK.GetMassage(0,356), g_itemTable[itemTableIdx].name );
	g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_MAGIC_BOOK);
	return( IF_CTL_NONE );
	
}


int CItemUseProc::EpkItem(const item_t *_item,const int _Pos)
{
	int itemTableIdx=-1;


	itemTableIdx = _item->itemTableIdx;
	if(itemTableIdx <0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return( IF_CTL_NONE );


	
	
	

	
	int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
	if( epackIdx < 0 )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
			g_LPACK.GetMessage(LPACK_TYPE_NORMAL,103));
		return 0;
	}
	else
	{
		int EpackItemTableIdx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;

		
		if(g_itemTable[EpackItemTableIdx].epkPackType != TypeItemTable_t::EPK_TYPE_NORMAL)
		{
			
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 448 ));
			return 0;
		}

		
		
		if(g_itemTable[EpackItemTableIdx].epkChargeMethod == TypeItemTable_t::EPK_CHARGE_TYPE_NPC)
		{
			
			
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 405 ));
			return 0;
		}
	}
	
	g_cgv.saveInventoryIdx = _Pos;
	
	g_ifMng->SetMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 322 ), 
		g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 257 ), 
		MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_USE_EPACK_CHARG_SOUL );	
	return( IF_CTL_NONE );

}


int CItemUseProc::EpkMuchItem(const item_t *_item,const int _Pos)
{
	int itemTableIdx=-1;


	itemTableIdx = _item->itemTableIdx;
	if(itemTableIdx <0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return( IF_CTL_NONE );


	
	
	

	
	int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
	if( epackIdx < 0 )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
			g_LPACK.GetMessage(LPACK_TYPE_NORMAL,103));
		return 0;
	}
	else
	{
		int EpackItemTableIdx = g_cgv.myCharacterInfo->item[epackIdx].itemTableIdx;

		
		if(g_itemTable[EpackItemTableIdx].epkPackType != TypeItemTable_t::EPK_TYPE_MUCH_CAPACITY)
		{
			
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 447 ));
			return 0;
		}

		
		
		if(g_itemTable[EpackItemTableIdx].epkChargeMethod == TypeItemTable_t::EPK_CHARGE_TYPE_NPC)
		{
			
			
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 405 ));
			return 0;
		}
	}
	
	g_cgv.saveInventoryIdx = _Pos;
	
	g_ifMng->SetMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 322 ), 
		g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 257 ), 
		MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_USE_EPACK_CHARG_SOUL );	
	return( IF_CTL_NONE );

}

int CItemUseProc::WeatherSnowItem(const item_t *_item,const int _Pos)
{
	
	
	
	g_cgv.saveInventoryIdx = _Pos;

	
	g_ifMng->SetMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 322 ), 
		g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 246 ), 					
		MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_USE_EVENT_SNOW_ITEM );															
	return( IF_CTL_NONE );

}

int CItemUseProc::WeatherRainItem(const item_t *_item,const int _Pos)
{
	
	g_cgv.saveInventoryIdx = _Pos;				
	
	g_ifMng->SetMessage( g_LPACK.GetMassage( LPACK_TYPE_NORMAL, 322 ), 
		g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 292 ), 					
		MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_USE_EVENT_RAIN_ITEM );															
	return( IF_CTL_NONE );
	
}







int CItemUseProc::UseProc(const item_t *_item,const int _Pos)
{
	int itemTableIdx=-1;

	itemTableIdx = _item->itemTableIdx;

	if(itemTableIdx <0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return( IF_CTL_NONE );

	int UseItemType = gItemTableData.GetUseItemType(&g_itemTable[itemTableIdx]);

	switch(UseItemType) {
	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_REPAIR :
		return RepairItem(_item,_Pos);
		
	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_EPK_CHARGE :
		return EpkItem(_item,_Pos);	

	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_EPK_MUCH_CHARGE :
		return EpkMuchItem(_item,_Pos);

	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_WEATHER_SNOW :
		return WeatherSnowItem(_item,_Pos);

	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_WEATHER_RAIN :
		return WeatherRainItem(_item,_Pos);
		
	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_OPTION_MANAGE:
		{		
			
			int OptionType  = 
				gItemTableData.GetData(_item->itemTableIdx, 
				CItemTableData::ItemTable_FluxDataValue::OPTION_MANAGE_TYPE);

			switch ( OptionType)
			{
			case 0:	GTH_SendMessageRequest_StoneOfSage(_Pos);		break;
			case 1:	GTH_SendMessageRequest_StoneOfLucky(_Pos);		break;				
			case 2:	GTH_SendMessageRequest_StoneOfEachName(_Pos);	break;
			}
			
		}break;
		
	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_AWAKEN:
		{
			g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx = _Pos;
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), "정말 각성을 초기화 하시겠습니까?",
				MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_INIT_AWEAKEN );				
		}
		break;
		
	}	

	return( IF_CTL_NONE );
}






int CItemUseProc::EtcItemTypeWorldPortal(const item_t *_item,const int _Pos)
{
	
	g_cgv.myCharacterInfo->m_WorldChageSystem.m_SaveInvenPos = _Pos;
	int ret =IsAllowWorldChange_ForPcEvent();

	
	
	if(true == g_ifMng->m_Item_ScrollWin->IfWorldChangeing())
	{	
		g_ifMng->AddSysMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 588 ));
		return( IF_CTL_NONE );
	}

	switch(ret)
	{
	case 1:
		GTH_SendMessage_Request_WorldChangeUseItem(&g_cgv.myCharacterInfo->m_WorldChageSystem);
		break;

		
	case 2:
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,352) );
		break;												
		
		
	case 3:
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,232) );
		break;
	}
	
	
	return( IF_CTL_NONE );
}





int CItemUseProc::EtcItemTypeSkillSphere(const item_t *_item,const int _Pos)
{
	int itemTableIdx=-1;
	char szTemp[256];

	itemTableIdx = _item->itemTableIdx;
	if(itemTableIdx <0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return( IF_CTL_NONE );


	int skillTableIdx = g_itemTable[itemTableIdx].skillSphere.skillTableIndex;
	g_cgv.saveInventoryIdx = _Pos;
	

	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable!=NULL) 
	{
		
		
		
		sprintf( szTemp, g_LPACK.GetMassage(0,355), g_LPACK.Get(LPACK_TYPE_SKILL,pSkillTable->m_NameIdx) );
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_LEARN_SKILL );
	}
	return( IF_CTL_NONE );
}


int CItemUseProc::EtcItemTypePortal(const item_t *_item,const int _Pos)
{
	int itemTableIdx=-1;
	char szTemp[256];

	itemTableIdx = _item->itemTableIdx;
	if(itemTableIdx <0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return( IF_CTL_NONE );
	
	

	if( g_ifMng->CheckConditionalWindow() ) 
		return 0;
	
	
	
	
	
	if(TRUE == g_pApp->m_myCharacter->m_bWaitingRoom)
	{



		
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ),g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 392 ) );
		return( IF_CTL_NONE );
	}
	
	
	g_cgv.saveInventoryIdx = _Pos;
	
	sprintf( szTemp, g_LPACK.GetMassage(0,356), g_itemTable[itemTableIdx].name );
	g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_USE_PORTAL );
	return( IF_CTL_NONE );
}



int CItemUseProc::EtcItemTypeMonsterSphere(const item_t *_item,const int _Pos)
{
	int itemTableIdx=-1;
	char szTemp[256];

	itemTableIdx = _item->itemTableIdx;
	if(itemTableIdx <0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return( IF_CTL_NONE );
	
	
	
	

	int SummonTableIdx = _item->optionIdx[0];
	int monsteridx = g_summonMonstertable[SummonTableIdx].idx;
	int SummonTime = (int)g_summonMonstertable[SummonTableIdx].SummonTime;
	g_cgv.saveInventoryIdx = _Pos;
	sprintf( szTemp, g_LPACK.GetMassage(0,963), g_monsterTable[monsteridx].name, SummonTime);
	g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_MONSTER_SUMMON );
	return( IF_CTL_NONE );
}




int CItemUseProc::QuestScroll(const item_t *_item,const int _Pos)
{
	int itemTableIdx=-1;
	char szTemp[256];

	itemTableIdx = _item->itemTableIdx;
	if(itemTableIdx <0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return( IF_CTL_NONE );

	
	
	
	g_cgv.saveInventoryIdx = _Pos;
	sprintf( szTemp, g_LPACK.GetMassage( 0, 764 ), g_questscrollTable[_item->optionIdx[0]].name );
	g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, 0, IF_MESSAGE_QUESTSCROLL_ACTIVE );

	return( IF_CTL_NONE );
}

int CItemUseProc::AutoQuestScroll(const item_t *_item,const int _Pos)
{
	
	
	
	g_ifMng->m_autoQuestWin->m_iautoflag=AUTOQUESTSCROLL_START;
	

	GTH_SendMessage_AutoQuestScroll_Progress(AUTOQUESTSCROLL_START);

	g_cgv.saveInventoryIdx = _Pos;
	g_ifMng->m_autoQuestWin->m_questIdx = _item->optionIdx[0];	
	
	return( IF_CTL_NONE );
}

int CItemUseProc::SurgeryItem(const item_t *_item,const int _Pos)
{
	
	
	int itemTableIdx  = _item->itemTableIdx;
	if(itemTableIdx < 0 ) return( IF_CTL_NONE );

	int MaxHarCountType = gItemTableData.GetData(&g_itemTable[itemTableIdx],CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_SURGERY_MAXHAIRCOUNT_TYPE);
	
	const int gentype[NUM_JOB_TYPE] = { 0,1,2,3,0,0,1,1,2,2,2,3,3,3};	
	int job = g_cgv.syncCharacter[0].pcJob;
	int mygentype = gentype[job];

	g_ifMng->m_surgeryWin->m_maxFaceCount = g_charManager->m_pSkinManager->m_defSkinList[mygentype].count[SKIN_PART_HEAD];

	
	if(MaxHarCountType == CItemTableData::Surgery_HairItem_Type::SURGERY1)
	{
		g_ifMng->m_surgeryWin->m_maxHairCount = g_charManager->m_pSkinManager->m_defSkinList[mygentype].count[SKIN_PART_HAIR];
	}
	
	else if(MaxHarCountType == CItemTableData::Surgery_HairItem_Type::SURGERY2)
	{
		g_ifMng->m_surgeryWin->m_maxHairCount = g_charManager->m_pSkinManager->m_defSkinList[mygentype].count[SKIN_PART_HAIR]-1;
	}
	
	g_ifMng->m_surgeryWin->m_BackUpFaceIdx = g_charManager->m_Characters[0].skinPart[1];
	g_ifMng->m_surgeryWin->m_BackUpHairIdx = g_charManager->m_Characters[0].skinPart[0];
	g_ifMng->m_surgeryWin->m_FaceCount = g_ifMng->m_surgeryWin->m_BackUpFaceIdx;
	g_ifMng->m_surgeryWin->m_HairCount = g_ifMng->m_surgeryWin->m_BackUpHairIdx;
	g_cgv.saveInventoryIdx = _Pos;
	g_ifMng->m_surgeryWin->Enable();
	

	return( IF_CTL_NONE );
}

int CItemUseProc::ChatItem(const item_t *_item,const int _Pos)
{
	
	
	if(TRUE == g_pApp->m_myCharacter->m_bWaitingRoom)
	{
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ),g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 392 ) );
		return( IF_CTL_NONE );
	}
	
	g_cgv.saveInventoryIdx = _Pos;
	g_ifMng->m_worldChatWin->Enable(true,CIFWorldChatWin::enumType::speaker);
	g_ifMng->SetFocus( g_ifMng->m_worldChatWin );					
	g_chatManager->InitInputStr( INPUT_INDEX_WORLDCHAT );
	g_chatManager->SetInputState( INPUT_INDEX_WORLDCHAT, 1 );
	g_chatManager->ChangeInputMessage( INPUT_INDEX_WORLDCHAT );

	
	return( IF_CTL_NONE );
}






int CItemUseProc::UseItemMessage(const item_t *_item,const int _Pos)
{
	int itemTableIdx=-1;
	char szTemp[256];

	itemTableIdx = _item->itemTableIdx;
	if(itemTableIdx <0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return( IF_CTL_NONE );

	g_cgv.saveInventoryIdx = _Pos;
	sprintf( szTemp, g_LPACK.GetMassage(0,356), g_itemTable[itemTableIdx].name );

	
	g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, 
		IF_MESSAGE_ITEM_USE_RESERVED + itemTableIdx);

	
	if(IF_MESSAGE_ITEM_USE_RESERVED + itemTableIdx >= 65535)
	{
		g_ifMng->SetMessage("Error!","Message ID .. Range Overflow");
	}

	return( IF_CTL_NONE );
}


int CItemUseProc::PcSummonItem(const item_t *_item,const int _Pos,const int _type)
{
	
	if( g_ifMng->m_Item_ScrollWin->State() ) 
	{
		return false;
	}

	g_cgv.saveInventoryIdx = _Pos;	

	g_ifMng->m_Item_ScrollWin->Enable( true , _item,_Pos,_type);
	g_ifMng->SetFocus( g_ifMng->m_Item_ScrollWin );
	
	return( IF_CTL_NONE );
}


int CItemUseProc::EtcItemTypeNone(const item_t *_item,const int _Pos)
{
	int itemTableIdx=-1;


	itemTableIdx = _item->itemTableIdx;
	if(itemTableIdx <0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return( IF_CTL_NONE );

	int UseItemType = gItemTableData.GetUseItemType(&g_itemTable[itemTableIdx]);

	switch(UseItemType) {
	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_QUESTSCROLL :
		return QuestScroll(_item,_Pos);

	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_AUTOQUESTSCROLL :
		return AutoQuestScroll(_item,_Pos);

	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_SURGERY :
		return SurgeryItem(_item,_Pos);

	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_CHAT :
		return ChatItem(_item,_Pos);
	
	
	
	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_PC_SUMMON :
		return PcSummonItem(_item,_Pos,CItem_ScrollWin::enumWINDOW_TYPE_ENUM::WT_PC_SUMMON);

	
	
	case CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_PC_MOVE_SUMMON :
		return PcSummonItem(_item,_Pos,CItem_ScrollWin::enumWINDOW_TYPE_ENUM::WT_PC_MOVE_SUMMON);
	}
		
	return( IF_CTL_NONE );
}


int CItemUseProc::EtcItemTypeAutoQuestItem(const item_t *_item,const int _Pos)
{
	int itemTableIdx=-1;


	itemTableIdx = _item->itemTableIdx;
	if(itemTableIdx <0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return( IF_CTL_NONE );


	
	int questIdex=-1;
	for(int index=0; index<4; index++)
	{
		if(questIdex!=-1)break;
		for(int i=0; i<g_autoquestscrollTableNumber; i++)
		{ 
			if(g_itemTable[itemTableIdx].commonField[index]==g_autoquestscrollTable[i].Index)
				
			if(g_autoquestscrollTable[i].data.reward.regClass==-1||g_cgv.myCharacterInfo->pcClass ==g_autoquestscrollTable[i].data.reward.regClass)
			{
				
				questIdex=g_autoquestscrollTable[i].Index;
				break;
			}
		}
	}
	
	if(questIdex==-1)	return(IF_CTL_NONE);
	
	g_ifMng->m_autoQuestWin->m_iautoflag=AUTOQUESTSCROLL_START;
	GTH_SendMessage_AutoQuestScroll_Progress(AUTOQUESTSCROLL_START);
	g_cgv.saveInventoryIdx = _Pos;
	g_ifMng->m_autoQuestWin->m_questIdx = questIdex;

	return(IF_CTL_NONE);
}







int CItemUseProc::UseEtcTypeProc(const item_t *_item,const int _Pos)
{
	int itemTableIdx=-1;

	itemTableIdx = _item->itemTableIdx;
	if(itemTableIdx <0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) return( IF_CTL_NONE );

	switch( g_itemTable[itemTableIdx].etcItemType )
	{
	case ETCITEM_TYPE_WORLD_PORTAL :
		return EtcItemTypeWorldPortal(_item,_Pos);
				
	case ETCITEM_TYPE_SKILLSPHERE :
		return EtcItemTypeSkillSphere(_item,_Pos);

	case ETCITEM_TYPE_PORTAL :
		return EtcItemTypePortal(_item,_Pos);

	case ETCITEM_TYPE_MONSTERSPHERE :
		return EtcItemTypeMonsterSphere(_item,_Pos);

	case ETCITEM_TYPE_NONE :
		return EtcItemTypeNone(_item,_Pos);
		
	case ETCITEM_TYPE_AUTOQUESTITEM:
		return EtcItemTypeAutoQuestItem(_item,_Pos);
	}

	return(IF_CTL_NONE);
}





int CItemUseProc::MessageProcessOk(int _msgType)
{
	
	if(gcTools.IfSkillCasting_NotItemUse()) return 0;

}
