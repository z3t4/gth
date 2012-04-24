



#include "../global_def.h"
#include "IFCommon.h"
#include "../game/GolryunBattle/GolryunBattle.h"


#include "..\CTools.h"
extern CTools gcTools;
extern CItemTableData gItemTableData;




char *monsterSpawnItemName[50] = 
{
	"몬스터 RA 증가(%)",						
	"몬스터 SA 증가(%)",						
	"몬스터 RA 회복속도 증가(%)",				
	"몬스터 SA 회복속도 증가(%)",				
	"몬스터 최소 공격력 증가(%)",				
	"몬스터 최대 공격력 증가(%)",				
	"몬스터 최소 신비력 증가(%)",				
	"몬스터 최대 신비력 증가(%)",				
	"몬스터 공격력 증가(%)",					
	"몬스터 공격속도 증가(%)",					
	"몬스터 명중력 증가(%)",					
	"몬스터 방어력 증가(%)",					
	"몬스터 회피율 증가(%)",					
	"몬스터 (양)저항력 증가(%)",				
	"몬스터 (음)저항력 증가(%)",				
	"몬스터 저항력(양/음) 증가(%)",				
	"몬스터 크리티컬 데미지 증가(%)",			
	"몬스터 크리티컬 확률 증가(%)",				
	"몬스터 스턴되지 않음",						
	"몬스터 소환시간 제한없음",					
	"경험치 증가(%)",							
	"젠 경험치 증가(%)",						
	"젠 숙련도 증가(%)",						
	"나크 증가(%)",								
	"아이템 드롭률 증가(%)",					
};



void CIFManager::InitRolloverText()
{
	memset( m_rolloverText, 0, sizeof( m_rolloverText ) );
	m_rolloverAddFirstFlag = true;
}

void CIFManager::AddRolloverText( char *str, int colorIdx, int lineFeed )
{
	char szTemp[16];

	if( str[0] == 0 ) return;

	if( !m_rolloverAddFirstFlag && lineFeed ) 
	{
		strcat( m_rolloverText, "&n" );
	}

	if( colorIdx >= 0 ) 
	{
		sprintf( szTemp, "&c%d", colorIdx );
		strcat( m_rolloverText, szTemp );
		strcat( m_rolloverText, str );
		strcat( m_rolloverText, "&c0" );
	}
	else
	{
		strcat( m_rolloverText, str );
	}
	m_rolloverAddFirstFlag = false;
}

void CIFManager::MakeRolloverText( int x, int y, int forcedWinFlag, int sx, int sy, int arrangeType )
{
	if( m_rolloverText[0] == 0 ) return;
	g_ifMng->m_rolloverWin->MakeRolloverWindow( x, y, m_rolloverText, forcedWinFlag, sx, sy, arrangeType );

}

void CIFManager::MakeItemRollover( item_t *item, int numFlag )
{
	int itemTableIdx = item->itemTableIdx;
	int classIdx, skillTableIdx;
	int color, itemType;


	
	char szTemp[512], szTemp2[256];
	char szReward[128];
	
	char RemainTime[256];
	

	classIdx = g_itemTable[itemTableIdx].classIdx;

	itemType = GTH_GetItemType( itemTableIdx );

	OPTION_ApplyItem( item );

	
	if ( item->itemTableIdx == QUESTSCROLL_ITEM_INDEX )
		sprintf( szTemp, "[%s]", g_questscrollTable[item->optionIdx[0]].name );
	
	else if ( item->itemTableIdx == AUTOQUESTSCROLL_ITEM_INDEX )
		sprintf( szTemp, "[%s]", g_autoquestscrollTable[item->optionIdx[0]].name );
	
	
	else if ( item->itemTableIdx == CGolryunBattle::TWOGYUN_100000_ITEM_INDEX ||
		      item->itemTableIdx == CGolryunBattle::TWOGYUN_1000000_ITEM_INDEX )
	{
		sprintf(szTemp,item->name);
		AddRolloverText( szTemp, 5 );

		
		int serialNum = item->experience;
		int time = item->optionIdx[0];
		int level = item->optionIdx[1];
		int winNum = item->optionIdx[2];
		
		sprintf(szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 393 ),time,level,winNum);
		AddRolloverText( szTemp, 5 );
		
		sprintf(szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 394 ),serialNum);
		AddRolloverText( szTemp, 5 );
		return;

	}
	else if ( item->itemTableIdx == CGolryunBattle::ENTERANCE_TICKET_ITEM_INDEX )
	{
		sprintf(szTemp,item->name);
		AddRolloverText( szTemp, 5 );

		
		sprintf(szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 395 ),item->optionIdx[0],item->experience);
		AddRolloverText( szTemp, 5 );
		return;
	}
	
	else
	{
		
		
		if( g_itemTable[itemTableIdx].rarity < 5 && itemType != -1 )
			sprintf( szTemp, "%s [%s]", g_LPACK.GetMessage( 0, 773+g_itemTable[itemTableIdx].rarity ), item->name );
		else
			sprintf( szTemp, "[%s]", item->name );
	}
		Fx_CHARACTER_t *character;
		character = &g_charManager->m_Characters[0];
		if ( character->gameMaster > 0 )
		{
	sprintf( szTemp, "%s, ID.%d", szTemp, item->itemTableIdx );
		}
	
	
	AddRolloverText( szTemp, 5 );

	
	
	
	if ( item->itemTableIdx == QUESTSCROLL_ITEM_INDEX )
	{



		
		strcpy( szTemp, g_questscrollTable[item->optionIdx[0]].description );

		if( !stricmp( szTemp, "NULL" ) )
		{
			g_ifMng->m_questWin->MakeRewardString(	item->optionIdx[1], 
													g_questscrollTable[item->optionIdx[0]].data.rewardField[item->optionIdx[1]], 
													g_questscrollTable[item->optionIdx[0]].data.reward.itemNo, 
													szReward );
			sprintf(	szTemp, 
						g_LPACK.GetMassage( 0, 783 ), 
						g_monsterTable[	g_questscrollTable[item->optionIdx[0]].data.monsterIndex ].name,
						g_questscrollTable[item->optionIdx[0]].data.monsterNo,
						szReward	);
		}

		AddRolloverText( " ", 0 );
		AddRolloverText( szTemp, 8 );

		
		AddRolloverText( g_LPACK.GetMassage( 0, 186 ) );
		
		return ;
	}

	
	
	if ( item->itemTableIdx == AUTOQUESTSCROLL_ITEM_INDEX )
	{
		return;
	}

	switch( itemType )
	{
		
	case 0 :
		sprintf( szTemp, g_LPACK.GetMassage( 0, 305 ), item->durability );
		AddRolloverText( szTemp );
		
		strcpy( szTemp, GetItemRequireJobText( &g_optItem ) );
		AddRolloverText( szTemp, 6 );
		
		if( item->reqLevel > g_oriItem.level ) color = 8;
		else color = -1;
		sprintf( szTemp, g_LPACK.GetMassage( 0, 645 ), item->reqLevel );
		AddRolloverText( szTemp, color );
		
		strcpy( szTemp, GetItemRequireStatText( &g_optItem ) );
		AddRolloverText( szTemp );

		
		if( g_optItem.atkPhyMinDmg > g_oriItem.atkPhyMinDmg ||  g_optItem.atkPhyMaxDmg > g_oriItem.atkPhyMaxDmg ) color = 8;
		else color = -1;
		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 257 ), g_optItem.atkPhyMinDmg, g_optItem.atkPhyMaxDmg );
		AddRolloverText( szTemp, color );

		if( g_optItem.atkMagMinDmg > g_oriItem.atkMagMinDmg ||  g_optItem.atkMagMaxDmg > g_oriItem.atkMagMaxDmg ) color = 8;
		else color = -1;

		sprintf( szTemp, g_LPACK.GetMassage( 0, 255 ), g_optItem.atkMagMinDmg, g_optItem.atkMagMaxDmg );
		AddRolloverText( szTemp, color );
		
		if( g_optItem.atkHitPower > 0 )
		{
			if( g_optItem.atkHitPower > g_oriItem.atkHitPower ) color = 8;
			else color = -1;

			sprintf( szTemp, g_LPACK.GetMassage( 0, 652 ), g_optItem.atkHitPower );
			AddRolloverText( szTemp, color );
		}
		
		if( g_optItem.atkDelay > g_oriItem.atkDelay ) color = 8;
		else color = -1;
		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 258 ), g_optItem.atkDelay );
		AddRolloverText( szTemp, color );

		
		if( g_optItem.atkBowSACost > 0 ) 
		{
			sprintf( szTemp, g_LPACK.GetMassage( 0, 576 ), g_optItem.atkBowSACost );
			AddRolloverText( szTemp );
		}
		
		break;
		
	case 1 :
		sprintf( szTemp, g_LPACK.GetMassage( 0, 305 ), item->durability );
		AddRolloverText( szTemp );
		
		strcpy( szTemp, GetItemRequireJobText( &g_optItem ) );
		AddRolloverText( szTemp, 6 );
		
		if( item->reqLevel > g_oriItem.level ) color = 8;
		else color = -1;
		sprintf( szTemp, g_LPACK.GetMassage( 0, 645 ), item->reqLevel );
		AddRolloverText( szTemp, color );
		
		strcpy( szTemp, GetItemRequireStatText( &g_optItem ) );
		AddRolloverText( szTemp );
		
		if( g_optItem.defPower > 0 ) 
		{
			if( g_optItem.defPower > g_oriItem.defPower ) color = 8;
			else color = -1;

			sprintf( szTemp, g_LPACK.GetMassage( 0, 262 ), g_optItem.defPower );
			AddRolloverText( szTemp, color );
		}
		if( g_optItem.defAvoidPower > 0 )
		{
			if( g_optItem.defAvoidPower > g_oriItem.defAvoidPower ) color = 8;
			else color = -1;

			sprintf( szTemp, g_LPACK.GetMassage( 0, 264 ), g_optItem.defAvoidPower );
			AddRolloverText( szTemp, color );
		}
		if( g_optItem.defHitPower > 0 )
		{
			if( g_optItem.defHitPower > g_oriItem.defHitPower ) color = 8;
			else color = -1;

			sprintf( szTemp, g_LPACK.GetMassage( 0, 652 ), g_optItem.defHitPower );
			AddRolloverText( szTemp, color );
		}
		if( g_optItem.defVelocity > 0 )
		{
			if( g_optItem.defVelocity > g_oriItem.defVelocity ) color = 8;
			else color = -1;

			sprintf( szTemp, g_LPACK.GetMassage( 0, 653 ), (int)g_optItem.defVelocity );
			AddRolloverText( szTemp, color );
		}
		break;

		
		
	case 2 :
		{
			
			strcpy( szTemp, GetItemRequireJobText( &g_optItem ) );
			AddRolloverText( szTemp, 6 );
			
			if( item->reqLevel > g_oriItem.level ) color = 8;
			else color = -1;
			sprintf( szTemp, g_LPACK.GetMassage( 0, 645 ), item->reqLevel );
			AddRolloverText( szTemp, color );

			
			strcpy( szTemp, GetItemRequireStatText( &g_optItem ) );
			AddRolloverText( szTemp );

			
			
			bool EquipEpack=false;
			int epackIdx = g_cgv.myCharacterInfo->equipment[EQUIP_EPACK];
			if(epackIdx >= 0 && epackIdx < MAX_NUMBER_OF_OWNITEM)
			{
				if(&g_cgv.myCharacterInfo->item[epackIdx] == item)
				{
					EquipEpack = true;
				}
			}

			if( g_optItem.epkRecoveryRA > 0 )
			{
				
				sprintf( szTemp, g_LPACK.GetMassage( 0, 265 ), g_optItem.epkRecoveryRA, g_optItem.epkMaxChargeRA );
				AddRolloverText( szTemp );
			}

			if( g_optItem.epkRecoverySA > 0 )
			{
				
				sprintf( szTemp, g_LPACK.GetMassage( 0, 266 ), g_optItem.epkRecoverySA, g_optItem.epkMaxChargeSA );
				AddRolloverText( szTemp );
			}

			
			switch(g_optItem.epkPackType) 
			{
			case TypeItemTable_t::EPK_TYPE_NORMAL:
				
				AddRolloverText( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 404 )  );
				break;
			case TypeItemTable_t::EPK_TYPE_MUCH_CAPACITY :
				
				AddRolloverText( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 413 )  );
				break;
			}

			
			switch(g_optItem.epkChargeType) 
			{
			case TypeItemTable_t::EPK_TYPE_UNCHARGE:
				
				AddRolloverText( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 414 )  );
				break;
			case TypeItemTable_t::EPK_TYPE_CHARGE :
				
				sprintf( szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 415 ) ,gcTools.GetEpkChargeCount(item),g_optItem.epkChargeCount);
				AddRolloverText( szTemp );
				break;
			}

			
			switch(g_optItem.epkChargeMethod) 
			{
			case TypeItemTable_t::EPK_CHARGE_TYPE_NPC:
				
				AddRolloverText( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 416 )  );
				break;
			case TypeItemTable_t::EPK_CHARGE_TYPE_ITEM :
				
				AddRolloverText( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 417 )  );
				break;
			case TypeItemTable_t::EPK_CHARGE_TYPE_BOTH :
				
				AddRolloverText( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 418 ) );
				break;			
			}

			
			switch(g_optItem.epkRecycleType) 
			{
			case TypeItemTable_t::EPK_RECYCLE_TYPE_ENABLE:
				
				AddRolloverText( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 419 ) );
				break;
			case TypeItemTable_t::EPK_RECYCLE_TYPE_DISABLE :
				
				AddRolloverText( g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 420 ) );
				break;
			}

			
			if(g_optItem.weight > 0)
			{
				
				sprintf( szTemp, "%s %d",  g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 470 ), g_optItem.weight );				
				AddRolloverText(szTemp);
			}			
		}


		break;
	
	case 3 :
		
		strcpy( szTemp, GetItemRequireJobText( &g_optItem ) );
		AddRolloverText( szTemp, 6 );
		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 645 ), item->reqLevel );
		AddRolloverText( szTemp, 8 );
		
		strcpy( szTemp, GetItemRequireStatText( &g_optItem ) );
		AddRolloverText( szTemp, 0 );
		break;
	default :
		if( GTH_IsStackItem( item ) || numFlag )
		{
			sprintf( szTemp, g_LPACK.GetMassage( 0, 267 ), item->durability + 1 );
			AddRolloverText( szTemp );
		}
		if( g_optItem.etcItemType == ETCITEM_TYPE_SKILLSPHERE )
		{
			
			strcpy( szTemp, GetItemRequireJobText( &g_optItem ) );
			AddRolloverText( szTemp, 6 );
			skillTableIdx = g_optItem.skillSphere.skillTableIndex;
			

			
			CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
			if(pSkillTable!=NULL) 
			{
				sprintf( szTemp, g_LPACK.GetMassage( 0, 268 ), pSkillTable->GetSkillRequire()->m_nGenLevel);
				
				

				
				AddRolloverText( g_LPACK.Get(LPACK_TYPE_SKILL,pSkillTable->m_DescIdx),9);
			}
			
			
			
		}
		
		else if( g_optItem.etcItemType == ETCITEM_TYPE_PRECOCITY )
		{
			
			AddRolloverText( g_optItem.document, 9 );
			
			
			
			if(gGameConfigFlag.m_bPrecocity_Time)
			{
				strcpy( szTemp2, g_ifMng->GetTimeString( g_optItem.precocity.precocityTime * g_cgv.myCharacterInfo->m_fItemPrecocityTimeRatio ) );
			}
			else
			{
				strcpy( szTemp2, g_ifMng->GetTimeString( g_optItem.precocity.precocityTime) );
			}
			
			
			sprintf( szTemp, g_LPACK.GetMassage( 0, 592 ), szTemp2 );
			AddRolloverText( szTemp );
			
			sprintf( szTemp, g_LPACK.GetMassage( 0, 593 ), g_itemTable[g_optItem.precocity.outComeItemIndex].name );
			AddRolloverText( szTemp, 2 );
		}
		
		else if( g_optItem.etcItemType == ETCITEM_TYPE_MONSTERSPHERE )
		{
			memset(szTemp, 0, 256);
			
			
			
			

			
			
			SummonMonster_t* pTable = &g_summonMonstertable[item->optionIdx[0]];			
			sprintf(szTemp, g_LPACK.GetMessage(0,948), g_monsterTable[pTable->idx].name);
			AddRolloverText( szTemp , 8);			

			sprintf(szTemp, "%s", pTable->doc);
			AddRolloverText( szTemp , 9);

			sprintf(szTemp, g_LPACK.GetMessage(0,949), (int)pTable->SummonTime);
			AddRolloverText( szTemp , 9);

			sprintf(szTemp, g_LPACK.GetMessage(0,950), g_cgv.worldTable[pTable->SummonWorldIdx].hanName, 7);
			AddRolloverText( szTemp , 2);
			AddRolloverText( g_LPACK.GetMassage( 0, 186 ) );
			return;
		}
		else 
		{
			
			AddRolloverText( g_optItem.document, 9 );
			
			

			
			int UseItemType = gItemTableData.GetUseItemType(&g_itemTable[itemTableIdx]);
			if(UseItemType == CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_RECYCLE)
			{
				sprintf( szTemp, g_LPACK.GetMassage( 0, 680 ), item->reqLevel );
				AddRolloverText( szTemp, 8 );
			}
			
		}
		if( GTH_IsConsumptiveItem( item ) )
		{
			AddRolloverText( g_LPACK.GetMassage( 0, 186 ) );
		}
		break;
	}

	
	if ( IsItemUseFlag(item, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_TIME) )
	{
		
		if ( item->ItemExtendInfo.AllowUseTime > 0)
		{
			strncpy(szTemp, g_ifMng->GetTimeString(item->ItemExtendInfo.AllowUseTime), 512 );
			szTemp[511] = NULL;
			
			_snprintf(RemainTime, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 551),  szTemp);
			RemainTime[255] = NULL;
			AddRolloverText(RemainTime, 6);
		}		
		else		
			
			AddRolloverText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 552), 6);			
		
	}
	
	if ( IsItemUseFlag(item, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_USE_DATE) )
	{
		if ( item->ItemExtendInfo.CalclateUseDateToSec > 0)
		{			
			szTemp[511] = NULL;
			
			
			
			
			struct tm *newtime;
			newtime = localtime((time_t*)&item->ItemExtendInfo.AllowUseDate);
			







			_snprintf(RemainTime, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 553),
				newtime->tm_year + 1900, 
				newtime->tm_mon + 1, 
				newtime->tm_mday, 
				newtime->tm_hour, 
				newtime->tm_min);
			
			AddRolloverText(RemainTime, 6);
		}
		else
			
			AddRolloverText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 554), 6);
	}
	

	
	if ( itemType != 2)
		MakeItemOptionRender( item );
	
	
	
	if ( itemType == 0 || itemType == 1 || itemType == 2)	
		AddRolloverText( g_itemTable[item->itemTableIdx].document , 8);
	


	
	

	
	
	

	

}

void CIFManager::MakeItemOptionRender( item_t *item )
{
	char szTemp[256], szTemp2[256];
	int i, j, temp;
	int itemTableIdx = item->itemTableIdx;
	int wearPosition = g_itemTable[itemTableIdx].wearPosition;
	int optionIdx[MAX_NUMBER_OF_ITEM_OPTION], optionValue[MAX_NUMBER_OF_ITEM_OPTION];

	
	
	if( g_itemTable[itemTableIdx].etcItemType == ETCITEM_TYPE_OPTIONSPHERE ) return;

	
	for( i = 0; i < item->optionNumber; i ++ )
	{
		optionIdx[i] = item->optionIdx[i];
		if( optionIdx[i] < 0 ) optionValue[i] = 0;
		else if( item->optionValue[i] > 10 ) optionValue[i] = 10;
		else optionValue[i] = item->optionValue[i];

		

	}

	
	for( i = 0; i < item->optionNumber; i ++ )
	{
		for( j = i; j < item->optionNumber; j ++ )
		{
			

			if( optionIdx[i] < optionIdx[j] )
			{
				temp = optionIdx[i];
				optionIdx[i] = optionIdx[j];
				optionIdx[j] = temp;

				temp = optionValue[i];
				optionValue[i] = optionValue[j];
				optionValue[j] = temp;
			}
		}
	}

	int optColor, optSphereType;
	
	optSphereType = OPTION_GetOptionStoneGrade( item );

	if( item->optionNumber > 0 )
	{
		
		if( !( wearPosition == ITEM_WEAR_POS_RING || wearPosition == ITEM_WEAR_POS_NECKLACE ||
			wearPosition == ITEM_WEAR_POS_BRACELET || wearPosition == ITEM_WEAR_POS_EARRING ) )
		{
			sprintf( szTemp, g_LPACK.GetMessage(0,932), g_LPACK.GetMessage( 0, 778 + optSphereType ) );
			AddRolloverText( szTemp, 0 );
		}
		AddRolloverText( "&a0", false );
	}
	
	for( i = 0; i < item->optionNumber; i ++ )
	{

		
		memset( szTemp2, 0, sizeof(szTemp2) );

		
		if( strstr( g_optionTable[optionIdx[i]].printing, "%" ) != NULL )
		{
			sprintf( szTemp, g_optionTable[optionIdx[i]].printing, 
				(int)OPTION_GetOptionValue( item, optionIdx[i], optionValue[i] ) );
		}
		else
		{
			strcpy( szTemp, g_optionTable[optionIdx[i]].printing );
		}

		
		optColor = 2;
		


		if( optSphereType >= 0 && optSphereType < 5 )
		{


			
			sprintf( szTemp2, "[+%d] %s", optionValue[i], szTemp );
			optColor = 2;

		}
		else
		{
			sprintf( szTemp2, "%s", szTemp );
		}

		AddRolloverText( szTemp2, optColor );
	}
}

void CIFManager::MakeActionRollover( int idx )
{
	
	
	
		g_ifMng->AddRolloverText( g_LPACK.GetMassage( g_actionTable[idx].nLPackID, g_actionTable[idx].nDescIdx ) );

}

void CIFManager::MakeHotRollover( int idx )
{
	int hotIdx;
	int hotType;
	hotType = g_cgv.myCharacterInfo->hotkeyType[idx];
	hotIdx = g_cgv.myCharacterInfo->hotkeyIdx[idx];
	
	switch( g_cgv.myCharacterInfo->hotkeyType[idx] )
	{
	case IF_HOTTYPE_SKILL :
		MakeSkillRollover( g_cgv.myCharacterInfo->hotkeyIdx[idx], false );
		break;
	case IF_HOTTYPE_ACTION :
		MakeActionRollover( g_cgv.myCharacterInfo->hotkeyIdx[idx] );
		break;
	case IF_HOTTYPE_ITEM :
		break;
	}
}




void CIFManager::MakeSkillRollover_GetValue(int _MySkillIndex,int skillTableIdx,SKILL_GEN_LEVEL &skillLevel)
{
	int i;
	char szTemp[256], genStr[256], szName[256];

	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable==NULL) return;

	CSkillDataFormat *pDataFormat			= pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return;

	int genType = g_SkillDataGlobal.GetGrade(pSkillTable->m_nJobType);
	if(genType <0 ) return;

	int limitLevel = g_SkillDataGlobal.GetLimitlevel(skillTableIdx,genType);
	int GradeClass = g_SkillDataGlobal.GetGradeClass(skillTableIdx,genType);

	
	if(pDataFormat->m_MessageType != SKILL_MSG_TYPE_PASSIVE)
	{
		CSkillCastValue * pCastValue = g_SkillDataGlobal.GetCastValue(SKILL_CAST_TABLE_INDEX,skillTableIdx,skillLevel.gen[genType]);
		if(pCastValue==NULL) return;

		
		if( !pSkillTable->m_nUsageType )
		{
			
			float castingTime, coolTime, saCost;

			
			
			coolTime		= pCastValue->m_fCool_Time;
			castingTime		= pCastValue->m_fCasting_Time;
			saCost			= pCastValue->m_nSa_Comsumption;

			if( castingTime > 0 )
			{			
				
				sprintf( szTemp, g_LPACK.Get(LPACK_TYPE_SKILL,4003), castingTime );	
				g_ifMng->AddRolloverText( szTemp );
			}

			if( saCost > 0 )
			{
				
				sprintf( szTemp, g_LPACK.Get(LPACK_TYPE_SKILL,4001), saCost );		
				g_ifMng->AddRolloverText( szTemp );
			}

			if( coolTime > 0 )
			{
				
				sprintf( szTemp, g_LPACK.Get(LPACK_TYPE_SKILL,4002), coolTime );		
				g_ifMng->AddRolloverText( szTemp );
			}
			g_ifMng->AddRolloverText(" ");
		}	
	}

	
	for(i=0;i<MAX_SKILL_VALUE_LIST;i++)
	{	
		if(pDataFormat->m_ValueList[i].m_Type == SKILL_CAST_TABLE_INDEX) continue;

		
		if(pDataFormat->m_ValueList[i].m_Type == SKILL_PHYMAXDMG) continue;

		CSkillTableData *pValue = g_SkillDataGlobal.GetValue(pDataFormat->m_ValueList[i].m_Type,skillTableIdx,_MySkillIndex,skillLevel.gen[genType]);
		if(pValue == NULL) continue;

		
		if(pValue->m_bMaxLv <= 0) continue;

		
		if(pDataFormat->m_ValueList[i].m_Type == SKILL_PHYMINDMG)
		{
			sprintf(szName,g_LPACK.Get(LPACK_TYPE_SKILL,4004));

			CSkillTableData *pMaxValue = g_SkillDataGlobal.GetValue(SKILL_PHYMAXDMG,skillTableIdx,_MySkillIndex,skillLevel.gen[genType]);
			
			if(pMaxValue == NULL)
			{	
				sprintf( szTemp, "%s:%.1f ", szName,pValue->m_fValue);
			}
			else if( pValue->m_fValue == pMaxValue->m_fValue)
			{
				sprintf( szTemp, "%s:%.1f ", szName,pValue->m_fValue);
			}
			
			else
			{
				sprintf( szTemp, "%s:%.1f~%.1f ", szName,pValue->m_fValue,pMaxValue->m_fValue);
			}
		}
		else
		{
			sprintf(szName,g_LPACK.Get(LPACK_TYPE_SKILL,pDataFormat->m_ValueList[i].m_Type + SKILL_DEFAULT_TYPE_INDEX));
			if(szName == "NULL") continue;
			sprintf( szTemp, "%s:%.1f ", szName,pValue->m_fValue);
		}

		if( pValue->m_bMaxLv > 1 )
		{
			
			int nGen_Level = skillLevel.gen[pValue->m_bGenGrade];
			if(nGen_Level > pValue->m_bMaxLv)	nGen_Level = pValue->m_bMaxLv;

			sprintf( genStr, "(%s %d/%d)", g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_GEN_ATTRIBUTE, pValue->m_bGenGrade ), nGen_Level, pValue->m_bMaxLv );
			strcat( szTemp, genStr );

			g_ifMng->AddRolloverText( szTemp );
		}
	}
}


void CIFManager::MakeSkillRollover( int idx, int tableFlag )
{
	
	char szTemp[256], genStr[256];
	int i,  skillTableIdx;
	
	
	SKILL_GEN_LEVEL skillLevel;

	CSkilTable *pSkillTable = NULL;
	
	if( tableFlag ) 
	{
		skillTableIdx = idx;

		
		pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);

		for(int i=0;i<GEN_HALF_NUMBER;i++)	skillLevel.gen[i] = 1;
	}
	else
	{
		skillTableIdx = g_cgv.myCharacterInfo->skill[idx].tableIdx;
		
		
		pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);

		for(int i=0;i<GEN_HALF_NUMBER;i++)
			skillLevel.gen[i] = g_cgv.myCharacterInfo->skill[idx].level.gen[i];
	}

	if(pSkillTable==NULL) return;

	char str[256];		

	
	sprintf(str,"%s",g_LPACK.Get(LPACK_TYPE_SKILL,pSkillTable->m_NameIdx));

	
	
	if( !pSkillTable->m_nUsageType)
	{
				Fx_CHARACTER_t *character;
		character = &g_charManager->m_Characters[0];
		if ( character->gameMaster > 0 )
		{
		
		sprintf( szTemp, "%s(%s), ID.%d", str, g_LPACK.GetMassage( 0, 1 ),pSkillTable->m_nId);
		}

	else
	{
		sprintf( szTemp, "%s(%s)", str, g_LPACK.GetMassage( 0, 1 ) );
	}
	}
	else
	{
				Fx_CHARACTER_t *character;
		character = &g_charManager->m_Characters[0];
		if ( character->gameMaster > 0 )
		{
		
		sprintf( szTemp, "%s(%s), ID.%d",str, g_LPACK.GetMassage( 0, 2 ) ,pSkillTable->m_nId);
		}
		else
		{
		
		sprintf( szTemp, "%s(%s)", str, g_LPACK.GetMassage( 0, 2 ) );
		}

	}
	g_ifMng->AddRolloverText( szTemp, 5 );

	
	g_ifMng->AddRolloverText( g_LPACK.Get(LPACK_TYPE_SKILL,pSkillTable->m_DescIdx), 9 );	
	
	szTemp[0] = 0;

	
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 565 ), pSkillTable->GetSkillRequire()->m_nGenLevel);
	sprintf( genStr, "%s(%s)&n", g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, pSkillTable->m_nJobType ), szTemp );
	g_ifMng->AddRolloverText( genStr, 6 );

	if( tableFlag )
	{
		sprintf( szTemp, "%s&n", g_LPACK.GetMassage( 0, 651 ) );		
		g_ifMng->AddRolloverText( szTemp, 5 );
	}
	else
	{
		sprintf( szTemp, "%s&n", g_LPACK.GetMassage( 0, 650 ) );		
		g_ifMng->AddRolloverText( szTemp, 5 );
	}	

	MakeSkillRollover_GetValue(idx,skillTableIdx,skillLevel);
	if( tableFlag ) return;

	

	g_ifMng->AddRolloverText( " " );

	sprintf( szTemp, "%s&n", g_LPACK.GetMassage( 0, 651 ) );	
	g_ifMng->AddRolloverText( szTemp, 5 );

	for(i=0;i<GEN_HALF_NUMBER;i++)	skillLevel.gen[i]++;
	MakeSkillRollover_GetValue(idx,skillTableIdx,skillLevel);


	


}





char* CIFManager::GetItemRequireJobText( itemTable_t *pItem )
{
	char szTemp[256];
	int reqBeforeAT, req1st, req2nd, req3rd;
	int reqMenCount = 0, reqWinCount = 0, reqWatCount = 0, reqEarCount = 0;

	reqBeforeAT = pItem->reqBeforeAT;
	req1st = pItem->req1stAT;
	req2nd = pItem->req2ndAT;
	req3rd = pItem->req3rdAT;

	memset( szTemp, 0, sizeof( szTemp ) );
	if( reqBeforeAT & AT_MENTALITY ) 
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 0 ) );
		reqMenCount = 1;
	}
	if( reqBeforeAT & AT_WIND ) 
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 1 ) );
		reqWinCount = 1;
	}
	if( reqBeforeAT & AT_WATER ) 
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 2 ) );
		reqWatCount = 1;
	}
	if( reqBeforeAT & AT_EARTH ) 
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 3 ) );
		reqEarCount = 1;
	}

	
	
	DWORD afterAwakenAll = 0;
	
	afterAwakenAll	|= AT_DEVOTIONAL_KNIGHT;
	afterAwakenAll	|= AT_ROMING_KNIGHT;
	afterAwakenAll	|= AT_PIECING_EYES;
	afterAwakenAll	|= AT_WINDY_ROMER;
	afterAwakenAll	|= AT_DESTROYER;
	afterAwakenAll	|= AT_GUARDIAN;
	afterAwakenAll	|= AT_TRANSFORMER;
	afterAwakenAll	|= AT_DEFENDER;

	if( req1st == afterAwakenAll )
	{
		strcpy( szTemp, g_LPACK.GetMassage( 0, 909 ) );
		return( &szTemp[0] );
	}

	
	if( reqMenCount && reqWinCount && reqWatCount && reqEarCount )
	{
		strcpy( szTemp, g_LPACK.GetMassage( 0, 566 ) );
		return( &szTemp[0] );
	}
	if( ( req1st & AT_DEVOTIONAL_KNIGHT ) && reqMenCount != 1 )
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 4 ) );
		reqMenCount = 2;
	}
	if( ( req1st & AT_ROMING_KNIGHT ) && reqMenCount != 1 )
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 5 ) );
		reqMenCount = 2;
	}
	if( ( req1st & AT_PIECING_EYES ) && reqWinCount != 1 )
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 6 ) );
		reqWinCount = 2;
	}
	if( ( req1st & AT_WINDY_ROMER ) && reqWinCount != 1 )
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 7 ) );
		reqWinCount = 2;
	}
	if( ( req1st & AT_DESTROYER ) && reqWatCount != 1 )
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 8 ) );
		reqWatCount = 2;
	}
	if( ( req1st & AT_GUARDIAN ) && reqWatCount != 1 )
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 9 ) );
		reqWatCount = 2;
	}
	if( ( req1st & AT_PROPHECIER ) && reqWatCount != 1 )
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 10 ) );
		reqWatCount = 2;
	}
	if( ( req1st & AT_TRANSFORMER ) && reqEarCount != 1 )
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 11 ) );
		reqEarCount = 2;
	}
	if( ( req1st & AT_DEFENDER ) && reqEarCount != 1 )
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 12 ) );
		reqEarCount = 2;
	}
	if( ( req1st & AT_STRANGER ) && reqEarCount != 1 )
	{
		if( szTemp[0] != 0 ) strcat( szTemp, "&n" );
		strcat( szTemp, g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, 13 ) );
		reqEarCount = 2;
	}
	return( &szTemp[0] );
}

char* CIFManager::GetItemRequireStatText( itemTable_t *pItem )
{
	char szTemp[256];
	char tempStr[256];
	int force, soul, agi, vit;

	force = pItem->reqForce;
	soul = pItem->reqSoulpower;
	agi = pItem->reqAgility;
	vit = pItem->reqVitality;

	memset( szTemp, 0, sizeof( szTemp ) );

	if( force > 0 ) 
	{
		sprintf( tempStr, g_LPACK.GetMassage( 0, 272 ), force );
		strcat( szTemp, tempStr );
	}
	if( soul > 0 ) 
	{
		sprintf( tempStr, g_LPACK.GetMassage( 0, 273 ), soul );
		strcat( szTemp, tempStr );
	}
	if( agi > 0 ) 
	{
		sprintf( tempStr, g_LPACK.GetMassage( 0, 274 ), agi );
		strcat( szTemp, tempStr );
	}
	if( vit > 0 ) 
	{
		sprintf( tempStr, g_LPACK.GetMassage( 0, 275 ), vit );
		strcat( szTemp, tempStr );
	}

	return( &szTemp[0] );
}

