 

#include "../global.h"


#include "../Tools/CTools.h"
extern CTools* gcpTools;


#include <vector>

#define ITEM_LOOT_TIME_FOR_INDIVIDUAL	10000		
#define	ITEM_DESTROY_TIME				180000		




int						g_numFixOpt;

extern float			g_optValue[NUM_OPTION_VALUE];
extern itemTable_t		g_optItem;
extern itemTable_t		g_oriItem;


extern CItemTableData gItemTableData;



using namespace std;


void InitItemOption()
{
	g_numFixOpt = 0;
}

void InitItem(item_t *item)
{  
	memset( item, 0, sizeof( item_t ) );
	for(int i = 0; i < MAX_NUMBER_OF_ITEM_OPTION; i ++ )
	{		
		item->optionIdx[i] = -1;
		item->optionValue[i] = 0;
	}
	item->itemTableIdx = -1;
	
	
	SetItemUseFlag( item, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_UN_CHECK);
	SetItemSpawnType( item, ITEM_SPAWN_INFO::enumItemSpawnType::ITEM_SPAWN_TYPE_UNKNOW);
	

}

DWORD GetItemAvailalbeOn( int itemTableIdx )
{
	int itemClassTableIdx = g_itemTable[itemTableIdx].classIdx;
	DWORD availableOn = 0;

	
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_WEAPON ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_WEAPONS;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_DANGGER ) availableOn |= OPTION_AVAILABLE_ON_DAGGER;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_ONEHAND_SWORD ) availableOn |= OPTION_AVAILABLE_ON_1HSWORD;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_TWOHAND_SWORD ) availableOn |= OPTION_AVAILABLE_ON_2HSWORD;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_MAGIC_BOW ) availableOn |= OPTION_AVAILABLE_ON_BOW;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_STAFF ) availableOn |= OPTION_AVAILABLE_ON_STAFF;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_SPIRITOR ) availableOn |= OPTION_AVAILABLE_ON_SPIRITOR;
		if( g_itemClassTable[itemClassTableIdx].weaponType == WEAPON_TYPE_CLAW ) availableOn |= OPTION_AVAILABLE_ON_CLAW;
	}
	
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_UPPER ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_UPPER;
		availableOn |= OPTION_AVAILABLE_ON_ARMORS;
	}
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_LOWER ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_LOWER;
		availableOn |= OPTION_AVAILABLE_ON_ARMORS;
	}
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_AIR_PROTECTOR ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_AIRPROTECTOR;
		availableOn |= OPTION_AVAILABLE_ON_ARMORS;
	}
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_BODY_PROTECTOR ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_BODYPROTECTOR;
		availableOn |= OPTION_AVAILABLE_ON_ARMORS;
	}
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_GLOVES ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_GLOVES;
		availableOn |= OPTION_AVAILABLE_ON_ARMORS;
	}
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_BOOTS ) 
	{
		availableOn |= OPTION_AVAILABLE_ON_BOOTS;
		availableOn |= OPTION_AVAILABLE_ON_ARMORS;
	}

	
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_EPACK ) availableOn |= OPTION_AVAILABLE_ON_EPACK;

	
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_RING ) availableOn |= OPTION_AVAILABLE_ON_RING;
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_NECKLACE ) availableOn |= OPTION_AVAILABLE_ON_NECKLACE;
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_BRACELET ) availableOn |= OPTION_AVAILABLE_ON_BRACELET;
	if( g_itemTable[itemTableIdx].wearPosition == ITEM_WEAR_POS_EARRING ) availableOn |= OPTION_AVAILABLE_ON_EARRING;

	return( availableOn );
	
}


int GetItemType( int itemTableIdx )
{
	int wearPosition = g_itemTable[itemTableIdx].wearPosition;
	int type = -1;

	
	if( wearPosition == ITEM_WEAR_POS_WEAPON ) 
	{
		type = 0;
	}
	else
	
	if( wearPosition == ITEM_WEAR_POS_UPPER || wearPosition == ITEM_WEAR_POS_LOWER || 
		wearPosition == ITEM_WEAR_POS_AIR_PROTECTOR || wearPosition == ITEM_WEAR_POS_BODY_PROTECTOR ||
		wearPosition == ITEM_WEAR_POS_GLOVES || wearPosition == ITEM_WEAR_POS_BOOTS )
	{
		type = 1;
	}
	else
	
	if( wearPosition == ITEM_WEAR_POS_EPACK )
	{
		type = 2;
	}
	else
	
	if( wearPosition == ITEM_WEAR_POS_RING || wearPosition == ITEM_WEAR_POS_NECKLACE ||
		wearPosition == ITEM_WEAR_POS_BRACELET || wearPosition == ITEM_WEAR_POS_EARRING ) 
	{
		type = 3;
	}
	return( type );
}



























































































































































































































































int SpawnItem( item_t *item, int itemTableIdx, int optionNumber, int optionGradeType, int spawnLevel, int bGamble )
{
	int						reqLevel	=	0;

	
	if(itemTableIdx < 0 || itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE ) return 0;
	itemTable_t				*pTable		=	&g_itemTable[itemTableIdx];
	itemClassTable_t		*pCTable	=	&g_itemClassTable[pTable->classIdx];
	DWORD					availableOn;

	InitItem( item );
	InitItemOption();
	
	
	
	
	if(gItemTableData.IsType(pTable,CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_SUMMONCARD_ITEM))
	{
		item_t tempItem;
		memcpy(&tempItem, item, sizeof(item_t) );
		GenerateMonsterSpawnItem( tempItem, itemTableIdx, spawnLevel);
		memcpy(item, &tempItem, sizeof(item_t));

		return 1;
	}

	item->itemTableIdx		=	itemTableIdx;
	item->reqLevel			=	pTable->level;
	item->durability		=	0;

	if( pTable->durability > 0 )
	{
		item->durability	=	(int)( GTH_Random() * (float)pTable->durability );
	}
	else
	{
		item->durability	=	0;
	}
	
	
	
	if(g_itemTable[itemTableIdx].epkChargeType == TypeItemTable_t::EPK_TYPE_MUCH_CAPACITY)
	{
		gcpTools->SetEpkChargeCount(item,pTable->epkChargeCount);
		if(pTable->epkChargeCount > 0)
		{
			item->optionNumber++;
		}
	}
	

	item->experience		=	0;
	item->timer				=	0;
	strcpy( item->name, pTable->name );

	
	if( optionNumber == 0 || optionGradeType < 0 ) return true;

	availableOn				=	GetItemAvailalbeOn( itemTableIdx );
	
	DWORD	optionAvailableOn;
	int		selectOptionList[MAX_COUNT_OPTION_TABLE];
	int		selectOptionNumber	=	0;
	int		totalProbability	=	0;

	
	for( int i = 0; i < MAX_COUNT_OPTION_TABLE; i ++ )
	{
		optionAvailableOn = 0;

		if( g_optionTable[i].idx < 0 ) break;
		
		
		
		

		
		if ( !bGamble )	
		{
			if( g_optionTable[i].dropYN <= 0 ) continue;
		}
		else
		{
			
			if( g_optionTable[i].dropYN < 2 ) continue;
		}


		optionAvailableOn	=	g_optionTable[i].availableOn;

		if( optionAvailableOn & availableOn )
		{
			selectOptionList[selectOptionNumber ++]	=	i;
			
			
			if( g_optionTable[i].fixationPro > 0 || bGamble )
			{
				totalProbability	+=	g_optionTable[i].fixationPro;
			}
		}
	}

	
	if( selectOptionNumber <= 0 ) return true;
	
	for( i = 0; i < selectOptionNumber; i ++ )
	{
		if( g_optionTable[selectOptionList[i]].fixationPro < 0 )
		{
			item->optionIdx[item->optionNumber]		=	selectOptionList[i];			

			if ( bGamble )
			{
				float r = 0.0f, accPro = 0.0f;
				r		=	GTH_Random();
				
				for( int OptionGradeNum = 0; OptionGradeNum < MAX_NUMBER_OF_OPTION_VALUE_GRADE ; OptionGradeNum ++ )
				{																
					accPro += (float) g_logic.gambleSelectOptionTable[optionNumber].SelectOptionGraderate[OptionGradeNum] * 0.01f;
					if( r < accPro ) break;
				}
				
				OptionGradeNum += 1;
				
				
				if ( OptionGradeNum < 0 ) 
					OptionGradeNum = 1;					
				
				
				if (OptionGradeNum > MAX_NUMBER_OF_OPTION_VALUE_GRADE )
					OptionGradeNum = MAX_NUMBER_OF_OPTION_VALUE_GRADE;
				
				
				item->optionValue[item->optionNumber]	=	OptionGradeNum;	
			}
			else			
				item->optionValue[item->optionNumber]	=	optionGradeType;	
			
			
			item->optionNumber ++;
			
			if( item->optionNumber >= optionNumber ) return true;
		}
	}
	
	
	float	accPro, r ;

	int j=0;

	for( i = 0; i < optionNumber; i ++ )
	{
		r		=	GTH_Random();
		accPro	=	0.0f;

		for( j = 0; j < selectOptionNumber; j ++ )
		{
	
			
			if(j >= MAX_COUNT_OPTION_TABLE) continue;
			if(selectOptionList[j] < 0 || selectOptionList[j] >= MAX_COUNT_OPTION_TABLE) continue;
			

			if( g_optionTable[selectOptionList[j]].fixationPro < 0 ) continue;
			if( selectOptionList[j] < 0 ) continue;
			
			
			if(totalProbability != 0)
			{
				accPro += (float)g_optionTable[selectOptionList[j]].fixationPro / (float) totalProbability;
			}

			if( r < accPro ) break;
		}
		
		if( j == selectOptionNumber ) continue;


		
		if(item->optionNumber < 0 || item->optionNumber >= MAX_NUMBER_OF_ITEM_OPTION) continue;
		item->optionIdx[item->optionNumber]		=	selectOptionList[j];
		
		
		
		if ( bGamble )
		{						
			float GraderandomVal = 0.0f;
			float GradeaccPro = 0.0f;
			GraderandomVal		=	GTH_Random();			
			for( int OptionGradeNum = 0; OptionGradeNum < MAX_NUMBER_OF_OPTION_VALUE_GRADE ; OptionGradeNum ++ )
			{								
				GradeaccPro += (float) g_logic.gambleSelectOptionTable[optionNumber].SelectOptionGraderate[OptionGradeNum] * 0.01;
				if( GraderandomVal < GradeaccPro ) break;
			}

			
			OptionGradeNum += 1;

			
			if ( OptionGradeNum < 0 ) 
				OptionGradeNum = 1;	


			
			if (OptionGradeNum > MAX_NUMBER_OF_OPTION_VALUE_GRADE )
				OptionGradeNum = MAX_NUMBER_OF_OPTION_VALUE_GRADE;	

			
			item->optionValue[item->optionNumber]	=	OptionGradeNum;	
		}
		else			
			item->optionValue[item->optionNumber]	=	optionGradeType;			
		

		
		
		item->optionNumber ++;

		
		totalProbability			-=	g_optionTable[selectOptionList[j]].fixationPro;
		selectOptionList[j]			=	-1;

		
		if( item->optionNumber >= optionNumber ) return true;
	}
	
	return true;
}





int SpawnItemByOptionGroup( item_t *item, int itemTableIdx, int optionGroupIdx )
{
	itemTable_t				*pTable		=	&g_itemTable[itemTableIdx];

	InitItem( item );
	InitItemOption();
	
	item->itemTableIdx		=	itemTableIdx;
	item->reqLevel			=	pTable->level;
	item->durability		=	0;

	if( pTable->durability > 0 )
	{
		item->durability	=	(int)( GTH_Random() * (float)pTable->durability );
	}
	else
	{
		item->durability	=	0;
	}
	
	item->experience		=	0;
	item->timer				=	0;
	strcpy( item->name, pTable->name );

	if( g_optionGroupTable[optionGroupIdx].idx < 0 ) return false;

	for( int i = 0; i < MAX_NUMBER_OF_ITEM_OPTION; i ++ )
	{
		if( g_optionGroupTable[optionGroupIdx].optionIdx[i] < 0 ) continue;

		item->optionIdx[item->optionNumber]		=	g_optionGroupTable[optionGroupIdx].optionIdx[i];
		item->optionValue[item->optionNumber]	=	g_optionGroupTable[optionGroupIdx].optionGrade[i];
		item->optionNumber ++;
	}
	
	return true;
}

#define		GENERATE_MONSTER_UP_LEVEL		10



float	GetItemDropPenalty( int pcLevel, int monsterLevel )
{
	float	dropPenalty	=	1.0f;
	int		levelDiffer	=	pcLevel	-	monsterLevel;

	if( levelDiffer == 10 )	dropPenalty	=	0.9f;
	if( levelDiffer == 11 )	dropPenalty	=	0.7f;
	if( levelDiffer == 12 )	dropPenalty	=	0.6f;
	if( levelDiffer == 13 )	dropPenalty	=	0.5f;
	if( levelDiffer == 14 )	dropPenalty	=	0.4f;
	if( levelDiffer == 15 )	dropPenalty	=	0.3f;
	if( levelDiffer == 16 )	dropPenalty	=	0.25f;
	if( levelDiffer == 17 )	dropPenalty	=	0.20f;
	if( levelDiffer == 18 )	dropPenalty	=	0.15f;
	if( levelDiffer == 19 )	dropPenalty	=	0.10f;
	if( levelDiffer == 20 )	dropPenalty	=	0.05f;
	if( levelDiffer > 20 )	dropPenalty	=	0.0f;

	return( dropPenalty );
}



































































































































































int GTH_GenerateItemFromMonster( monsterCharacter_t *monster )
{
	playerCharacter_t* pc = NULL;
	lootItem_t* pLoot=NULL;
	item_t spawnItem[MAX_NUMBER_OF_ITEM_LOOT];
	int				nSpawnItem = 0;

	vec3_t			position[MAX_NUMBER_OF_ITEM_LOOT];	
	
	int		i, j;
	float	r, accPro, dropPenalty;
	int		rx, ry, generateIdx, itemTableIdx;
	int		lootType, lootIdx;

	int		optionNumber, optionGrade;	
	
	bool	bUsedropPenalty = TRUE;
	float	TotalDroupPenalty = 0.0;


	lootType		=	LOOTTYPE_EVERYONE;
	lootIdx			=	-1;
	dropPenalty		=	1.0f;

	
	float fAddItemDropRatio =1.0f;
		
	
	if( monster->mostConEntityType != ENTITY_PC || monster->mostConEntityIdx < 0 )
	{
		return 0;
	}

	
	else if( monster->mostConEntityType == ENTITY_PC && SAFE_PCS(monster->mostConEntityIdx))
	{
		if( g_pc[monster->mostConEntityIdx].organizeServer >= 0 )
		{
			lootType = LOOTTYPE_PARTY;
			lootIdx = monster->mostConEntityIdx;
		}
		else
		{
			lootType = LOOTTYPE_INDIVIDUAL;
			lootIdx = monster->mostConEntityIdx;
		}

		
		
		dropPenalty	=	GetItemDropPenalty( g_pc[monster->mostConEntityIdx].level, monster->basic.level );

		
		if(gGameConfigFlag.m_bMap_Attr_Ratio)
		{
			PremiumMap	*pMap = g_pc[monster->mostConEntityIdx].GetCurMap();
			if(pMap)
			{
				fAddItemDropRatio =  pMap->Cur.fAddItemDropRatio;
			}
		}
		
	}
	
	
	
	if( GTH_Random() < 0.8f )		
	{
		GTH_SpawnNak( monster->basic.lootSE, monster->worldIdx, monster->position, lootType, lootIdx );
	}
	
	pLoot = &monster->basic.lootItem;
	if( pLoot == NULL || pLoot->maxLootNumber <= 0 ) return( false );

	
	
	for( i = 0; i < pLoot->maxLootNumber; i ++ )
	{
		accPro = 0.0f;
		r = GTH_Random();

		float generatePro;
		for( j = 0; j < pLoot->generateIdxNumber; j ++ )
		{
			bUsedropPenalty = true;
			if( pLoot->generateProbaility[j] == 0.0f ) continue;
			
			
			generatePro =	pLoot->generateProbaility[j] * (g_logic.itemDropRate * fAddItemDropRatio);
			

			
			
			
			generateIdx = pLoot->generateIdx[j];
			for( int ItemIdxInGroup = 0; ItemIdxInGroup < g_itemGenerateTable[generateIdx].itemNumber; ItemIdxInGroup ++ )
			{
				 int itemidx = g_itemGenerateTable[generateIdx].itemIdx[ItemIdxInGroup];				 
				 if ( itemidx < 0 ) continue;

				 if ( g_itemTable[itemidx].level >= 200 )
				 {
					 bUsedropPenalty = false;
					 break;				 
				 }
			}			
			if ( bUsedropPenalty )
			{
				
				TotalDroupPenalty += generatePro - (generatePro	* dropPenalty);
				
				generatePro	*=	dropPenalty;						
			}			
			else			
			{
				
				
				
				accPro += TotalDroupPenalty;
				TotalDroupPenalty = 0.0;
			}
			
			
			accPro		+=	generatePro;		
			
			if( r < accPro ) break;
		}
		if( j == pLoot->generateIdxNumber ) continue;

		
		
		generateIdx = pLoot->generateIdx[j];		

		
		accPro = 0.0f;
		r = GTH_Random();

		
		
		for( j = 0; j < g_itemGenerateTable[generateIdx].itemNumber; j ++ )
		{
			if( g_itemGenerateTable[generateIdx].itemPro[j] == 0.0f ) continue;

			accPro += g_itemGenerateTable[generateIdx].itemPro[j];
			if( r < accPro ) break;
		}
		if( j == g_itemGenerateTable[generateIdx].itemNumber ) continue;
		itemTableIdx = g_itemGenerateTable[generateIdx].itemIdx[j];
		

		if( itemTableIdx < 0 ) continue;

		
		
		
		if ( itemTableIdx == 479 || itemTableIdx == 573)
		{
			if ( g_pc[monster->mostConEntityIdx].level > 1)
				continue;
		}
		

		
		
		optionNumber = monster->basic.rareItemGrade;
		if( optionNumber <= 0 ) optionNumber = 1;
		optionGrade = monster->basic.optionGrade;
		if( optionGrade <= 0 ) optionGrade = 1;
	

		
		
		if( !SpawnItem( &spawnItem[nSpawnItem], itemTableIdx, 
			GTH_RandomizePower( 1, optionNumber ),
			GTH_RandomizePower( 1, optionGrade ),
			monster->basic.level ) ) continue;

		
		
		if( itemTableIdx == 849 || itemTableIdx == 850 )
		{
			spawnItem[nSpawnItem].reqLevel = monster->basic.level;
		}
		
		else if( itemTableIdx >= 864 && itemTableIdx <= 867 )
		{
			spawnItem[nSpawnItem].reqLevel = monster->basic.level;
		}

		rx = GTH_Randomize( 3 ) - 1;
		ry = GTH_Randomize( 3 ) - 1;
	
		vec_copy( monster->position, position[nSpawnItem] );
		position[nSpawnItem][2] = GTH_GetHeightFromCollisionMap(monster->worldIdx, monster->position);

		position[nSpawnItem][0] += (float)rx * 40.0f;
		position[nSpawnItem][1] += (float)ry * 40.0f;

		g_logSystem->Write( "Item spawn Success (Monster:[%s](%d), Item:[%s])",
			g_monsterTable[monster->tableIdx].name, monster->idx, g_itemTable[spawnItem[nSpawnItem].itemTableIdx].name );

		nSpawnItem ++;
	}

	if( nSpawnItem > 0 )
		GTH_SpawnMultiItemObject( spawnItem, monster->worldIdx, position, lootType, lootIdx, nSpawnItem, monster->position );

	return( true );
}


int GTH_SpawnMultiItemObject( item_t item[], int worldIdx, vec3_t position[], int lootType, int lootPCIdx , int number, vec3_t origin )
{
	int i, j, nItemObject = 0;
	itemObject_t *pItemObject;
	itemObject_t itemObject[MAX_NUMBER_OF_ITEM_LOOT];
	
	for( i = 0; i < number; i ++ )
	{
		pItemObject = &g_object[0];
		for ( j = 0; j < MAX_OBJECTS; j++, pItemObject++)
		{
			if ( !pItemObject->active ) break;
		}
		if ( j >= MAX_OBJECTS ) break;
		if ( g_objectNumber >= MAX_OBJECTS ) break;
		
		pItemObject->idx = j;

		
		pItemObject->objectType = OBJECT_TYPE_ITEM;

		
		
		pItemObject->worldIdx = worldIdx;

		vec_copy( position[i], pItemObject->position );
		memcpy( &pItemObject->item, &item[i], sizeof( item_t ) );		

		pItemObject->entityType = ENTITY_OBJECT;
		vec_set(pItemObject->angles, 0, 0, 0);

		pItemObject->zoneIdx = GTH_Zone_GetZoneIdx(pItemObject->position);

		if ( !GTH_Zone_AddEntityList(pItemObject->worldIdx, pItemObject->zoneIdx, ENTITY_OBJECT, pItemObject->idx) )
		{
			g_logSystem->Write( "Item Spawn Failure!! : World : %d, Zone : %d, Idx : %d, Pos(%.2f,%.2f,%.2f)", 
				pItemObject->worldIdx, pItemObject->zoneIdx, pItemObject->idx, 
				pItemObject->position[0], pItemObject->position[1], pItemObject->position[2] );
			break;
		}

		
		
		pItemObject->active = true;

		pItemObject->lootPCIdx = lootPCIdx;
		pItemObject->lootType = lootType;

		
		if( lootType == LOOTTYPE_PARTY )
		{
			
			
			strncpy( pItemObject->organizerName, g_pc[lootPCIdx].organizerName,  NAMESTRING);
			pItemObject->organizeServer = g_pc[lootPCIdx].organizeServer;
			pItemObject->organizeTime = g_pc[lootPCIdx].organizeTime;
		}
		ShowLogInfo("ITEMNAME=%s,organizerName=%s,objidx =%d", pItemObject->item.name, pItemObject->organizerName, pItemObject->idx);

		pItemObject->lootTime = g_globalTime + ITEM_LOOT_TIME_FOR_INDIVIDUAL;		
		pItemObject->destroyTime = g_globalTime + ITEM_DESTROY_TIME;				

		memcpy( &itemObject[nItemObject], pItemObject, sizeof( itemObject_t ) );
		nItemObject ++;
		g_objectNumber++;
	}

	if( nItemObject > 0 ) 
		GTH_SendMessage_AddMultiItemObject( itemObject, nItemObject, origin );

	return( true );
}




int GTH_SpawnNak( int nak, int worldIdx, vec3_t position, int lootType, int lootPCIdx )
{
	int i;

	
	itemObject_t *itemObject;

	if ( g_objectNumber >= MAX_OBJECTS ) return -1;	
	
	itemObject = &g_object[0];

	for (i=0; i < MAX_OBJECTS; i++, itemObject++)
	{
		if ( !itemObject->active ) break;
	}

	if ( i >= MAX_OBJECTS ) return -1;

	itemObject->idx				=	i;
	itemObject->active			=	true;
	itemObject->objectType		=	OBJECT_TYPE_NAK;
	itemObject->worldIdx		=	worldIdx;
	itemObject->nak				=	nak;

	vec_copy(position, itemObject->position);
	vec_set(itemObject->angles, 0, 0, 0);

	itemObject->entityType		=	ENTITY_OBJECT;
	itemObject->zoneIdx			=	GTH_Zone_GetZoneIdx(itemObject->position);

	if ( !GTH_Zone_AddEntityList(itemObject->worldIdx, itemObject->zoneIdx, ENTITY_OBJECT, itemObject->idx) )
	{
		return -1;
	}


	itemObject->lootPCIdx		=	lootPCIdx;
	itemObject->lootType		=	lootType;
	itemObject->lootTime		=	g_globalTime	+	ITEM_LOOT_TIME_FOR_INDIVIDUAL;
	itemObject->destroyTime		=	g_globalTime	+	ITEM_DESTROY_TIME;

	
	if( lootType == LOOTTYPE_PARTY && lootPCIdx >= 0 )
	{
		strcpy( itemObject->organizerName, g_pc[lootPCIdx].organizerName );
		itemObject->organizeServer = g_pc[lootPCIdx].organizeServer;
		itemObject->organizeTime = g_pc[lootPCIdx].organizeTime;
	}

	g_objectNumber++;
	
	GTH_SendMessage_AddItemObject( itemObject );

	return( 1 );
}

int GTH_SpawnItemObject(item_t *item, int worldIdx, vec3_t position, int lootType, int lootPCIdx)
{
	int i;
	itemObject_t *itemObject;

	if ( g_objectNumber >= MAX_OBJECTS ) return -1;	
	
	itemObject = &g_object[0];
	for (i=0; i < MAX_OBJECTS; i++, itemObject++)
	{
		if ( !itemObject->active ) break;
	}

	if ( i >= MAX_OBJECTS ) return -1;

	itemObject->idx = i;
	
	
	
	itemObject->objectType = OBJECT_TYPE_ITEM;

	itemObject->worldIdx = worldIdx;
	vec_copy(position, itemObject->position);
	memcpy( &itemObject->item, item, sizeof(item_t) );

	itemObject->entityType = ENTITY_OBJECT;
	vec_set(itemObject->angles, 0, 0, 0);

	itemObject->zoneIdx = GTH_Zone_GetZoneIdx(itemObject->position);

	if ( !GTH_Zone_AddEntityList(itemObject->worldIdx, itemObject->zoneIdx, ENTITY_OBJECT, itemObject->idx) )
	{
		g_logSystem->Write( "Item Spawn Failure!! : World : %d, Zone : %d, Idx : %d, Pos(%.2f,%.2f,%.2f)", 
				itemObject->worldIdx, itemObject->zoneIdx, itemObject->idx, 
				itemObject->position[0], itemObject->position[1], itemObject->position[2] );

		return -1;
	}

	
	
	itemObject->active = true;
	itemObject->lootPCIdx = lootPCIdx;
	itemObject->lootType = lootType;
	itemObject->lootTime = g_globalTime + ITEM_LOOT_TIME_FOR_INDIVIDUAL;		
	itemObject->destroyTime = g_globalTime + ITEM_DESTROY_TIME;					

	g_objectNumber++;

	
	GTH_SendMessage_AddItemObject( itemObject );

	return i;  
}

int GTH_RemoveItemObject(itemObject_t *itemObject)
{

	if ( !itemObject->active ) return false;

	
	GTH_Zone_DeleteEntityList(itemObject->worldIdx, itemObject->zoneIdx, ENTITY_OBJECT, itemObject->idx);

	itemObject->active = false;
	g_objectNumber --;

	
	GTH_SendMessage_DeleteItemObject( itemObject );

	return true;
}

void GTH_UpdateItemObject()
{
	itemObject_t *itemObject;
	int i;

	itemObject = &g_object[0];
	for (i=0; i<MAX_OBJECTS; i++, itemObject++)
	{
		if ( !itemObject->active ) continue;

		if ( g_globalTime >= itemObject->destroyTime )
		{
			
			GTH_RemoveItemObject( itemObject );
		}

		if ( itemObject->lootType == LOOTTYPE_INDIVIDUAL || itemObject->lootType == LOOTTYPE_PARTY )
		{
			if ( g_globalTime >= itemObject->lootTime )
			{
				itemObject->lootType = LOOTTYPE_EVERYONE;
				itemObject->lootPCIdx = -1;
				itemObject->lootTime = 0;
			}
		}
	}
}

void GTH_DeleteMyItem( playerCharacter_t *pc, item_t *item )
{
	int i;

	memset(item, 0, sizeof(item_t));

	item->itemTableIdx = -1;
	for (i=0; i<MAX_NUMBER_OF_ITEM_OPTION; i++)
	{
		item->optionIdx[i] = -1;
	}

	pc->itemNumber --;

}

int GTH_AddMyItem( playerCharacter_t *pc, item_t *item )
{
	int i;

	for (i=0; i<MAX_NUMBER_OF_OWNITEM; i++)
	{
		if ( pc->item[i].itemTableIdx == -1 ) break;
	}
	
	if (i >= MAX_NUMBER_OF_OWNITEM)
		return -1;

	memcpy(&pc->item[i], item, sizeof(item_t));

	pc->itemNumber++;

	return i;
}





int GTH_StackMyItem( playerCharacter_t* pc, item_t *item )
{
	int i;
	int itemIdx;

	
	if( GTH_IsStackItem( item ) )
	{
		for( i = 0; i < MAX_INVENTORY_SIZE; i ++ )
		{
			if( pc->inventory[i] < 0 ) continue;
			itemIdx = pc->inventory[i];

			if( GTH_CheckStackItem( item, &pc->item[itemIdx] ) )
			{
				return( itemIdx );
			}
		}
	}
	return( -1 );
}

int GTH_StackMyItemForDepot( playerCharacter_t* pc, item_t *item )
{
	int i;
	int itemIdx;

	
	if( GTH_IsStackItem( item ) )
	{
		for( i = 0; i < MAX_DEPOT_SIZE; i ++ )
		{
			if( pc->depot[i] < 0 ) continue;
			itemIdx = pc->depot[i];

			if( GTH_CheckStackItem( item, &pc->item[itemIdx] ) )
			{
				return( itemIdx );
			}
		}
	}
	return( -1 );
}





int GTH_AddQuestItem( playerCharacter_t *pc, int itemTableIdx, int questIdx, int itemNumber, int maxNumber )
{
	int i, pos;

	
	for( i = 0; i < MAX_COUNT_QUEST_INVENTORY; i ++ )
	{
		
		if( questIdx == 0 || questIdx == 1 || questIdx == 2 || questIdx == 3 )
		{
			if( pc->questInventory[i].item.itemIdx == itemTableIdx ) break;
		}

		if( pc->questInventory[i].item.itemIdx == itemTableIdx &&
			pc->questInventory[i].item.questIdx == questIdx ) break;
	}
	
	if( i == MAX_COUNT_QUEST_INVENTORY )
	{
		for( i = 0; i < MAX_COUNT_QUEST_INVENTORY; i ++ )
		{
			if( pc->questInventory[i].item.itemIdx < 0 ) break;
		}
		
		if( i == MAX_COUNT_QUEST_INVENTORY ) return 0;

		pc->questInventory[i].item.itemIdx = itemTableIdx;
		pc->questInventory[i].item.questIdx = questIdx;
		pc->questInventory[i].item.itemCount = 0;
		pos = i;
	}
	else
	{
		pos = i;
	}

	pc->questInventory[pos].item.itemCount += itemNumber;

	
	if( pc->questInventory[pos].item.itemCount > maxNumber )
	{
		pc->questInventory[pos].item.itemCount = maxNumber;
		return 0;
	}

	GTH_SendMessage_QuestItemInfo( pc, itemTableIdx, itemNumber );

	return 1;
}

int GTH_DeleteQuestItem( playerCharacter_t *pc, int itemTableIdx, int questIdx, int itemNumber )
{
	int i;

	
	for( i = 0; i < MAX_COUNT_QUEST_INVENTORY; i ++ )
	{
		
		if( questIdx == 0 || questIdx == 1 || questIdx == 2 || questIdx == 3 )
		{
			if( pc->questInventory[i].item.itemIdx == itemTableIdx ) break;
		}

		if( pc->questInventory[i].item.itemIdx == itemTableIdx &&
			pc->questInventory[i].item.questIdx == questIdx ) break;
	}
	
	if( i == MAX_COUNT_QUEST_INVENTORY ) return 0;

	pc->questInventory[i].item.itemCount -= itemNumber;

	if( itemNumber <= 0 || pc->questInventory[i].item.itemCount <= 0 )
	{
		pc->questInventory[i].item.itemIdx = -1;
		pc->questInventory[i].item.questIdx = 0;
		pc->questInventory[i].item.itemCount = 0;
	}

	itemNumber = -itemNumber;

	GTH_SendMessage_QuestItemInfo( pc, itemTableIdx, itemNumber );

	return 1;
}

int GTH_GetQuestItemCount( playerCharacter_t* pc, int itemTableIdx, int questIdx )
{
	int i;

	
	for( i = 0; i < MAX_COUNT_QUEST_INVENTORY; i ++ )
	{
		
		if( questIdx == 0 || questIdx == 1 || questIdx == 2 || questIdx == 3 )
		{
			if( pc->questInventory[i].item.itemIdx == itemTableIdx ) break;
		}
		if( pc->questInventory[i].item.itemIdx == itemTableIdx &&
			pc->questInventory[i].item.questIdx == questIdx ) break;
	}
	
	if( i == MAX_COUNT_QUEST_INVENTORY ) return -1;

	return pc->questInventory[i].item.itemCount;
}

int GTH_GenerateQuestItemFromMonster( monsterCharacter_t *monster, int conIdx )
{
	int entityIdx;
	int questIdx, step, itemTableIdx, maxNumber;
	playerCharacter_t *pc;


	
	if( g_qigTable[monster->tableIdx].itemNumber == 0 ) return 0;

	
	entityIdx = monster->conInfo[conIdx].entityIdx;
	pc = &g_pc[entityIdx];
	if( !pc->active || !pc->alive ) return 0;
	if( stricmp( pc->name, monster->conInfo[conIdx].entityName ) ) return 0;

	
	for( int i = 0 ; i < g_qigTable[monster->tableIdx].itemNumber; i ++ )
	{
		if( g_qigTable[monster->tableIdx].itemTableIdx[i] < 0 ) continue;
		if( g_qigTable[monster->tableIdx].questIdx[i] < 0 ) continue;

		questIdx = g_qigTable[monster->tableIdx].questIdx[i];
		step = g_qigTable[monster->tableIdx].step[i];
		
		for( int j = 0; j < MAX_COUNT_QUEST_STATUS; j ++ )
		{
			
			if( pc->questStatus[j].questIdx == questIdx &&
				pc->questVar[questIdx].var.step == step ) break;
		}
		if( j == MAX_COUNT_QUEST_STATUS ) continue;
		
		
		if( GTH_Random() < g_qigTable[monster->tableIdx].generateProbaility[i] * 0.01f )
		{
			itemTableIdx = g_qigTable[monster->tableIdx].itemTableIdx[i]; 
			maxNumber = g_qigTable[monster->tableIdx].maxItemCount[i];
			
			
			GTH_AddQuestItem( pc, itemTableIdx, questIdx, 1, maxNumber );

			return 1;
		}
	}

	return 0;
}





int GTH_CheckCraftFormula( playerCharacter_t *pc )
{
	int i, j, k, itemIdx;
	int itemTableIdx1, itemTableIdx2, checkedNumber;
	int numSourceItem, checkedItem[MAX_CRAFT_SOURCE_ITEM_SIZE], checkedIdx[MAX_CRAFT_SOURCE_ITEM_SIZE];
	item_t *sourceItem;
	
	
	numSourceItem = 0;
	for( i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		if( pc->craftSourceInventory[i] < 0 ) continue;
		
		sourceItem = &pc->item[pc->craftSourceInventory[i]];

		
		
		checkedIdx[numSourceItem] = i;
		numSourceItem ++;
	}
	if( numSourceItem == 0 ) return -1;

	for( i = 0; i < g_itemCraftTableNumber; i ++ )
	{
		
		if( numSourceItem != g_itemCraftTable[i].numSourceItem ) continue;
		
		checkedNumber = numSourceItem;
		
		for( j = 0; j < MAX_CRAFT_SOURCE_ITEM_SIZE; j ++ )
		{
			checkedItem[j] = false;
		}
		
		for( j = 0; j < MAX_CRAFT_SOURCE_ITEM_SIZE; j ++ )
		{
			for( k = 0; k < numSourceItem; k ++ )
			{
				
				if( checkedItem[k] ) continue;

				itemIdx = pc->craftSourceInventory[checkedIdx[k]];
				sourceItem = &pc->item[itemIdx];
				
				itemTableIdx1 = sourceItem->itemTableIdx;
				itemTableIdx2 = g_itemCraftTable[i].sourceItem[j].itemIndex;
				if( itemTableIdx1 != itemTableIdx2 ) continue;
				
				if( g_itemTable[itemTableIdx1].stackFlag && g_itemTable[itemTableIdx2].stackFlag )
				{
					if( g_itemCraftTable[i].sourceItem[j].itemCount != sourceItem->durability + 1 ) continue;
				}
				

				break;
			}
			if( k == numSourceItem ) break;
			checkedItem[k] = true;
			checkedNumber --;
		}
		if( checkedNumber == 0 ) return i;
	}

	return -1;
}


int GTH_CraftProcess( playerCharacter_t *pc, int &reply, int &error, int &reqNak, float &pro, int resultItemIdx[], int checkFlag ,int &craftIdx )
{
	int i, itemIdx, inventoryCount, specialType, optionGrade, rareItemGrade;
	item_t resultItem;
	char generateID[20];

	int bIsStack, stackNumber;

	if( pc->curItemWeight >= pc->calItemWeight )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_3;			
		return 0;
	}
	if( pc->craftResultInventory[0] >= 0 || pc->craftResultInventory[1] >= 0 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_0;			
		return 0;
	}

	
	craftIdx = GTH_CheckCraftFormula( pc );
	if( craftIdx < 0 ) 
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_0;			
		return 0;
	}
	
	
	reqNak = g_itemCraftTable[craftIdx].craftCost;
	pro = g_itemCraftTable[craftIdx].successProbability;
	for( i = 0; i < g_itemCraftTable[craftIdx].numResultItem; i ++ )
	{
		resultItemIdx[i] = g_itemCraftTable[craftIdx].resultItem[i].itemIndex;
	}
	if( pc->curChargeSE < g_itemCraftTable[craftIdx].craftCost )
	{   
		reply = 0;
		error = ERROR_ITEMSYSTEM_1;			
		return 0;
	}

	
	inventoryCount = 0;
	for( i = 0; i < MAX_INVENTORY_SIZE; i ++ )
	{
		if( pc->inventory[i] < 0 ) inventoryCount ++;
	}
	
	if( g_itemCraftTable[craftIdx].numResultItem > inventoryCount )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_2;			
		return 0;
	}

	
	if(gGameConfigFlag.m_bItem_Craft_Limit)
	{
		if( g_itemCraftTable[craftIdx].m_nMembership_type > -1)
		{
			if( g_itemCraftTable[craftIdx].m_nMembership_type > pc->PremiumInfo.iMaxLevelItemCraft)
			{
				reply = 0;
				error = ERROR_ITEMSYSTEM_24;
				return 0;
			}
		}
	}
	

	if( checkFlag ) return 1;

	
	for( i = 0; i < MAX_CRAFT_SOURCE_ITEM_SIZE; i ++ )
	{
		if( pc->craftSourceInventory[i] < 0 ) continue;
		itemIdx = pc->craftSourceInventory[i];
		
		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[pc->item[itemIdx].itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = pc->item[itemIdx].durability + 1;
		}
		
		
		
		
		g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_CRAFT_SOURCE, stackNumber, pc->item[itemIdx].name,
			GetItemInformationLog( bIsStack, pc->item[itemIdx].itemTableIdx, itemIdx, -1 ));
		

		GTH_DeleteMyItem( pc, &pc->item[itemIdx] );
		pc->craftSourceInventory[i] = -1;

		g_DBGameServer->RemoveItem( pc, itemIdx );
	}
	specialType = g_itemCraftTable[craftIdx].specialType;
	
	if( GTH_Random() <= g_itemCraftTable[craftIdx].successProbability * 0.01f )
	{
		
		for( i = 0; i < g_itemCraftTable[craftIdx].numResultItem; i ++ )
		{
			InitItem( &resultItem );
			
			
			

			
			if( g_itemCraftTable[craftIdx].resultItem[i].optionGroupIdx >= 0 )
			{
				if( !SpawnItemByOptionGroup( &resultItem, 
											g_itemCraftTable[craftIdx].resultItem[i].itemIndex, 
											g_itemCraftTable[craftIdx].resultItem[i].optionGroupIdx ) ) continue;
			}
			else
			{
				optionGrade		=	g_itemCraftTable[craftIdx].resultItem[i].optionGrade;
				rareItemGrade	= 	g_itemCraftTable[craftIdx].resultItem[i].rareItemGrade;
				
				
				if( optionGrade > 0 && rareItemGrade > 0 )
				{
					if( !SpawnItem( &resultItem, 
									g_itemCraftTable[craftIdx].resultItem[i].itemIndex,
									GTH_RandomizePower( 1, rareItemGrade ),
									GTH_RandomizePower( 1, optionGrade ) ) ) continue;
				}
				
				else
				{
					if( !SpawnItem( &resultItem, 
									g_itemCraftTable[craftIdx].resultItem[i].itemIndex ) ) continue;
				}
			}			
			if( g_itemTable[resultItem.itemTableIdx].stackFlag )
			{
				resultItem.durability = g_itemCraftTable[craftIdx].resultItem[i].itemCount - 1;
			}
			itemIdx = GTH_AddMyItem( pc, &resultItem );
			pc->craftResultInventory[i] = itemIdx;
			strcpy( generateID, "craftItem" );

			g_DBGameServer->AddItem( pc, &resultItem, itemIdx, generateID );
			
			bIsStack = false;
			stackNumber = 1;

			if( g_itemTable[resultItem.itemTableIdx].stackFlag )
			{
				bIsStack = true;
				stackNumber = resultItem.durability + 1;
			}

			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
				LOG_CLASS_ITEM_CRAFT_RESULT, stackNumber, resultItem.name,
				GetItemInformationLog( bIsStack, resultItem.itemTableIdx, itemIdx, -1 ));
			
		}
	}
	
	pc->curChargeSE -= reqNak;

	return 1;
}


int GTH_AutoCraftProcess( playerCharacter_t *pc, int idx,int &reply, int &error)
{

	int i=0;
	item_t *sourceItem;
	int autoQuestindex=pc->questStatus[idx].questIdx+pc->questStatus[idx].dField[0];
	
	if(pc->questStatus[idx].dField[3] < pc->questStatus[idx].dField[2]){
		reply=0;
		error=ERROR_ITEMSYSTEM_23;
		return 0;

	}
	
	for( i = 0; i < MAX_AUTO_TRADEITEMFILD; i ++ ){
		if( pc->npcAutoSourceInventory[i] < 0 ) continue;
		
		sourceItem = &pc->item[pc->npcAutoSourceInventory[i]];
        
		
		if(sourceItem->itemTableIdx == pc->questStatus[idx].dField[8]){
		
			reply=1;
			break;
		}
		
		else{
			reply=0;
			break;
		}
		
	
	}
	
	if(	reply==1){
		for(int j=i+1; j<MAX_AUTO_TRADEITEMFILD; j++){
			
			if( pc->npcAutoSourceInventory[j] < 0 ) continue;
			
			sourceItem = &pc->item[pc->npcAutoSourceInventory[j]];
			
			if(sourceItem->itemTableIdx != pc->questStatus[idx].dField[8]){
				reply=0;
				break;
			}
		}
		
	}





	if(reply==1){	
		error=ERROR_ITEMSYSTEM_NONE;
		return 1;
	}
	

	reply=0;
	error=ERROR_ITEMSYSTEM_21;
	return 0;
}


#define REPLY_FAILE					0
#define REPLY_UPGRADE				1
#define REPLY_GAMBLE				2






int FineOptionStoneType( int itemIdx )
{
	int ret;

	switch ( itemIdx )
	{
	case 451 :	
		ret = OPTION_SPHERE_LEAF;
		break;
	case 452 :	
		ret = OPTION_SPHERE_MAD;
		break;
	case 453 :	
		ret = OPTION_SPHERE_WAVE;
		break;
	case 454 :	
		ret = OPTION_SPHERE_FIRE;
		break;
	case 455 :	
		ret = OPTION_SPHERE_ART;
		break;
	default :
		ret = -1;
		break;
	}
	
	return ret;
}



int GTH_UpgradeProcess( playerCharacter_t *pc, int selectedOptionIdx, int &reply, int &error, int &reqNak, float &pro, float &vanishPro, float &downGradePro, int &currentGrade, int &nextGrade, int& resultItemIdx, int checkFlag )
{
	int optionItemIdx, upItemIdx, preItemIdx, changeOptionIndex = -1;
	int maxValue;
	DWORD availableOn;

	int i, itemIdx, inventoryCount;
	item_t resultItem;
	char generateID[20];

	int bIsStack, stackNumber;

	
	itemUpgradeProbabilityTable_t * pUpgradeTable;

	
	int optionType, optionStoneType;

	if( pc->upgradeResultInventory >= 0 )
	{
		
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_12;
		return 0;
	}

	if( pc->curItemWeight >= pc->calItemWeight )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_3;			
		return 0;
	}

	upItemIdx = pc->upgradeSourceInventory[0];
	optionItemIdx = pc->upgradeSourceInventory[1];
	preItemIdx = pc->upgradeSourceInventory[2];

	
	if( upItemIdx < 0 || optionItemIdx < 0 )
	{
		
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_12;
		return 0;
	}

	
	availableOn = GetItemAvailalbeOn( pc->item[upItemIdx].itemTableIdx );
	if( availableOn == 0 )
	{
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_12;
		return 0;
	}

	
	if( g_itemTable[pc->item[optionItemIdx].itemTableIdx].etcItemType != ETCITEM_TYPE_OPTIONSPHERE )
	{
		
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_12;
		return 0;
	}

	
	
	if( pc->item[optionItemIdx].itemTableIdx == 863 )
	{
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_12;
		return 0;
	}

	
	int itemType = GetItemType( pc->item[upItemIdx].itemTableIdx );
	if( itemType != 0  && itemType != 1 )
	{
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_12;
		return 0;
	}

	

	
	if( pc->item[upItemIdx].optionNumber < 1 || pc->item[upItemIdx].optionNumber <= selectedOptionIdx )
	{
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_12;
		return 0;
	}

	
	
	itemIdx			=	pc->upgradeSourceInventory[1];	
	
	if( g_itemTable[pc->item[itemIdx].itemTableIdx].stackFlag )
	{
		bIsStack		= true;
		stackNumber		=	pc->item[itemIdx].durability + 1;
		if ( stackNumber >= 2)
		{
			reply = 0;
			error = ERROR_ITEMSYSTEM_12;
			return 0;
		}
	}
	
	


	
	
	optionType		=	OPTION_GetOptionStoneGrade( &pc->item[upItemIdx] );
	optionStoneType =	FineOptionStoneType( pc->item[optionItemIdx].itemTableIdx );

	if( optionType != optionStoneType )
	{
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_21;
		
		resultItemIdx = pc->item[upItemIdx].itemTableIdx;
		return 0;
	}

	switch( optionType )
	{
	case OPTION_SPHERE_LEAF :	
		pUpgradeTable = &g_logic.leafStoneProTbl;
		break;
	case OPTION_SPHERE_MAD :	
		pUpgradeTable = &g_logic.madStoneProTbl;
		break;
	case OPTION_SPHERE_WAVE :	
		pUpgradeTable = &g_logic.waveStoneProTbl;
		break;
	case OPTION_SPHERE_FIRE :	
		pUpgradeTable = &g_logic.fireStoneProTbl;
		break;
	case OPTION_SPHERE_ART :	
		pUpgradeTable = &g_logic.artStoneProTbl;
		break;

	default :
		{
			
			reply = 0;
			error = ERROR_ITEMSYSTEM_12;
			return 0;
		}
	}

	maxValue = 10;





	
	
	
	
	
	
	
	


	
	
	if(gGameConfigFlag.m_bItem_Upgrade_Limit)
	{
		int nFreeUserMaxValue = pc->PremiumInfo.ItemOptionUpgrade.iMaxlevel;
		if( pc->item[upItemIdx].optionValue[selectedOptionIdx] >= nFreeUserMaxValue )
		{
			
			reply = 0;
			error = ERROR_ITEMSYSTEM_25;
			return 0;			
		}
	}
	
	

	if( pc->item[upItemIdx].optionValue[selectedOptionIdx] >= maxValue )
	{
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_12;
		return 0;
	}

	currentGrade			=	pc->item[upItemIdx].optionValue[selectedOptionIdx];
	nextGrade				=	currentGrade + 1;
	
	
	pro						=	pUpgradeTable->successPro[nextGrade];
	reqNak					=	pUpgradeTable->cost[nextGrade];
	vanishPro				=	pUpgradeTable->vanishPro[nextGrade];
	downGradePro			=	pUpgradeTable->gradeDownPro[nextGrade];

	
	
	
	if(gGameConfigFlag.m_bItem_Upgrade_Limit)
	{
		int ratio = (pc->PremiumInfo.ItemOptionUpgrade.fAddPriceRatio + 0.000001f) * 100;
		reqNak = ( ratio * reqNak) / 100;
	}
	

	if( currentGrade > nextGrade )
	{
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_14;
		return 0;
	}
	if( pc->curChargeSE < reqNak )
	{   
		reply = 0;
		error = ERROR_ITEMSYSTEM_1;			
		return 0;
	}

	
	inventoryCount			=	GTH_FindEmptyInventoryPos( pc );
	if( inventoryCount < 0 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_2;			
		return 0;
	}

	memcpy( &resultItem, &pc->item[upItemIdx], sizeof( item_t ) );
	resultItemIdx			=	resultItem.itemTableIdx;
	
	if( checkFlag ) return 1;

	
	for( i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
	{
		if( pc->upgradeSourceInventory[i] < 0 ) continue;
		
		itemIdx			=	pc->upgradeSourceInventory[i];
		bIsStack		=	false;
		stackNumber		=	1;

		if( g_itemTable[pc->item[itemIdx].itemTableIdx].stackFlag )
		{
			bIsStack		= true;
			stackNumber		=	pc->item[itemIdx].durability + 1;
		}

		
		
		
		g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_UPGRADE_SOURCE, stackNumber, pc->item[itemIdx].name,
			GetItemInformationLog( bIsStack, pc->item[itemIdx].itemTableIdx, itemIdx, -1 ));
		

		GTH_DeleteMyItem( pc, &pc->item[itemIdx] );
		
		pc->upgradeSourceInventory[i]	=	-1;
		g_DBGameServer->RemoveItem( pc, itemIdx );
	}

	

	pc->curChargeSE -= reqNak;

	
	
	if( GTH_Random() <= pro * 0.01f )
	{
		
		if( resultItem.optionValue[selectedOptionIdx] < 0 )
		{
			resultItem.optionValue[selectedOptionIdx] = 0;
		}

		if( resultItem.optionValue[selectedOptionIdx] < maxValue )
		{
			resultItem.optionValue[selectedOptionIdx] = resultItem.optionValue[selectedOptionIdx] + 1;
		}
		itemIdx						=	GTH_AddMyItem( pc, &resultItem );
		pc->upgradeResultInventory	=	itemIdx;
		
		strcpy( generateID, "upgradeItem" );
		g_DBGameServer->AddItem( pc, &resultItem, itemIdx, generateID );
		
		bIsStack		=	false;
		stackNumber		=	1;
		if( g_itemTable[resultItem.itemTableIdx].stackFlag )
		{
			bIsStack		=	true;
			stackNumber		=	resultItem.durability + 1;
		}
		
		
		
		g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_UPGRADE_RESULT, stackNumber, resultItem.name,
			GetItemInformationLog( bIsStack, resultItem.itemTableIdx, itemIdx, -1 ));
		
	}
	else
	{
		
		if( GTH_Random() <= 1.0f - ( vanishPro * 0.01f ) || vanishPro <= 0.0f )
		{
			
			if ( downGradePro > 0.0f && GTH_Random() <= ( downGradePro * 0.01f ) )
			{
				resultItem.optionValue[selectedOptionIdx] -= pUpgradeTable->gradeDownLevel[nextGrade];

				if ( resultItem.optionValue[selectedOptionIdx] < 0 )
					resultItem.optionValue[selectedOptionIdx] = 0;
			}

			itemIdx = GTH_AddMyItem( pc, &resultItem );
			pc->upgradeResultInventory = itemIdx;
			strcpy( generateID, "upgradeItem" );
			g_DBGameServer->AddItem( pc, &resultItem, itemIdx, generateID );

			bIsStack = false;
			stackNumber = 1;
			if( g_itemTable[resultItem.itemTableIdx].stackFlag )
			{
				bIsStack = true;
				stackNumber = resultItem.durability + 1;
			}
			
			
			
			g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
				LOG_CLASS_ITEM_UPGRADE_RESULT, 
				stackNumber, 
				resultItem.name,
				GetItemInformationLog( bIsStack, resultItem.itemTableIdx, itemIdx, -1 ));
			
		}
		
		return 0;
	}
	
	return 1;
}




int GTH_DeleteProcess( playerCharacter_t *pc, int &reply, int &error, int optionIndex, int checkFlag )
{
	int upItemIdx = pc->upgradeSourceInventory[0];
	int optionItemIdx = pc->upgradeSourceInventory[1]; 
	int itemIdx;
	int resultItemIdx;
	item_t resultItem;
	char generateID[20];

	int bIsStack, stackNumber;

	
	if( pc->upgradeResultInventory >= 0 )
	{
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_17;
		return 0;
	}

	
	int itemType = GetItemType( pc->item[upItemIdx].itemTableIdx );
	if( itemType != 0 && itemType != 1 )
	{
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_15;
		return 0;
	}

	
	if( pc->item[optionItemIdx].itemTableIdx != 861 ) 
	{
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_17;
		return 0;
	}

	if( optionIndex < 0 || optionIndex >= pc->item[upItemIdx].optionNumber)
	{
		
		reply = 0;
		error = ERROR_ITEMSYSTEM_16;
		return 0;
	}

	memcpy( &resultItem, &pc->item[upItemIdx], sizeof( item_t ) );
	resultItemIdx = resultItem.itemTableIdx;

	if( checkFlag ) return 1;

	
	for( int i = 0; i < MAX_UPGRADE_SOURCE_ITEM_SIZE; i ++ )
	{
		if( pc->upgradeSourceInventory[i] < 0 ) continue;
		itemIdx = pc->upgradeSourceInventory[i];
		
		bIsStack = false;
		stackNumber = 1;

		if( g_itemTable[pc->item[itemIdx].itemTableIdx].stackFlag )
		{
			bIsStack = true;
			stackNumber = pc->item[itemIdx].durability + 1;
		}

		
		
		
		g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
			LOG_CLASS_ITEM_UPGRADE_SOURCE, stackNumber, pc->item[itemIdx].name,
			GetItemInformationLog( bIsStack, pc->item[itemIdx].itemTableIdx, itemIdx, -1 ));
		

		GTH_DeleteMyItem( pc, &pc->item[itemIdx] );
		pc->upgradeSourceInventory[i] = -1;
		g_DBGameServer->RemoveItem( pc, itemIdx );
	}

	
	
	for( i=0; i<resultItem.optionNumber; i++)
	{
		if( i == optionIndex )
		{
			resultItem.optionIdx[optionIndex] = -1;
			resultItem.optionValue[optionIndex] = -1;
		}
		else if( i > optionIndex )
		{
			resultItem.optionIdx[i-1] = resultItem.optionIdx[i];
			resultItem.optionValue[i-1] = resultItem.optionValue[i];

			resultItem.optionIdx[i] = -1;
			resultItem.optionValue[i] = -1;
		}
	}

	resultItem.optionNumber--;

	
	
	
	
	itemIdx = GTH_AddMyItem( pc, &resultItem );
	pc->upgradeResultInventory = itemIdx;
	strcpy( generateID, "deleteOptionItem" );
	g_DBGameServer->AddItem( pc, &resultItem, itemIdx, generateID );
	
	bIsStack = false;
	stackNumber = 1;
	if( g_itemTable[resultItem.itemTableIdx].stackFlag )
	{
		bIsStack = true;
		stackNumber = resultItem.durability + 1;
	}
	
	
	
	g_logSystem->WriteToLog( pc, LOG_CLASS_ITEM, "%d;%d;;;;%s;%s", 
		LOG_CLASS_ITEM_UPGRADE_RESULT, stackNumber, resultItem.name,
		GetItemInformationLog( bIsStack, resultItem.itemTableIdx, itemIdx, -1 ));
	


	return 1;
}


int GTH_QuestScroll_FindTableIndex( playerCharacter_t *pc, int depart, int level, int reward, int selectedQSTableIdx, int checkFlag )
{
	int number = 0;
	int	idx[10];

	int ret = 0;
	switch ( depart )
	{
	case 1 :
		if ( pc->pcClass != GEN_MENTALITY && pc->pcClass != GEN_WIND )
			ret = 1;
		break;
	case 2 :
		if ( pc->pcClass != GEN_WATER && pc->pcClass != GEN_EARTH )
			ret = 1;
		break;
	default :
		break;
	}

	if ( ret == 1 )
		return -2;

	
	for (int i=0; i<g_questscrollTableNumber; i++)
	{
		if ( g_questscrollTable[i].data.scrollType == depart && g_questscrollTable[i].data.level == level )
		{
			idx[number] = i;
			number++;

			
			
			if ( number > 9 )
				break;
		}
	}

	if ( number == 1 )
		return idx[0];

	
	if ( number > 1 )
	{
		if ( checkFlag )
			return idx[GTH_Randomize(number)];
		else
		{
			for( i=0; i<number; i++ )
			{
				if ( idx[i] == selectedQSTableIdx )
					return idx[i];
			}
			
		}
	}

	return -1; 
}


int GTH_QuestScroll_ObtainProcess( playerCharacter_t *pc,  int &reply, int &error, int &reqNak, int &tableIdx, int depart, int level, int reward, int selectedQSTableIdx, int checkFlag )
{

	tableIdx = GTH_QuestScroll_FindTableIndex( pc, depart, level, reward, selectedQSTableIdx, checkFlag );

	if ( tableIdx == -1 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_18;		
		return 0;
	}

	if ( tableIdx == -2 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_20;		
		return 0;
	}

	if ( g_questscrollTable[tableIdx].data.rewardField[reward] == -1 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_19;		
		return 0;
	}
		
	
	if( pc->curItemWeight + g_itemTable[QUESTSCROLL_ITEM_INDEX].weight > pc->calItemWeight )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_3;			
		return 0;
	}

	
	reqNak = g_questscrollTable[tableIdx].data.price;
	if( pc->curChargeSE < reqNak )
	{   
		reply = 0;
		error = ERROR_ITEMSYSTEM_1;			
		return 0;
	}

	
	int inventoryCount = 0;
	for( int i = 0; i < MAX_INVENTORY_SIZE; i ++ )
	{
		if( pc->inventory[i] < 0 ) inventoryCount ++;
	}

	if( inventoryCount < 1 )
	{
		reply = 0;
		error = ERROR_ITEMSYSTEM_2;			
		return 0;
	}

	if( checkFlag ) return 1;

	
	pc->curChargeSE -= reqNak;

	int addItemData[6];

	addItemData[0] = QUESTSCROLL_ITEM_INDEX;	
	addItemData[1] = -1;						
	addItemData[2] = 1;							
	addItemData[3] = 1;							
	addItemData[4] = tableIdx;					
	addItemData[5] = reward;					






	AI_NPC_AddItem( pc, addItemData );

	return 1;
}





int GTH_AutoQuestScroll_ObtainProcess( playerCharacter_t* pPlayer, const int& tableIdx)
{
	
	if( pPlayer->curItemWeight + g_itemTable[AUTOQUESTSCROLL_ITEM_INDEX].weight > pPlayer->calItemWeight ) 
		return ERROR_ITEMSYSTEM_3;
			

	
	for(int  idx = 0; idx < MAX_NUMBER_OF_OWNITEM; idx ++ ) {

		if( pPlayer->item[idx].itemTableIdx != AUTOQUESTSCROLL_ITEM_INDEX )  continue;

		
		if( pPlayer->item[idx].optionIdx[0] == tableIdx) 
			return ERROR_ITEMSYSTEM_22;
	}

	
	
	for(idx = 0; idx < MAX_INVENTORY_SIZE; idx ++ ) {
		if(0 > pPlayer->inventory[idx]) return ERROR_ITEMSYSTEM_NONE;
	}

	
	return ERROR_ITEMSYSTEM_2;
}





void GenerateMonsterSpawnItem(item_t& item, int itemTableIdx, int monsterLevel)
{
	itemTable_t *pTable = &g_itemTable[itemTableIdx];
	itemClassTable_t *pCTable = &g_itemClassTable[pTable->classIdx];

	InitItem( &item );
	InitItemOption();

	item.itemTableIdx	= itemTableIdx;
	strcpy( item.name, pTable->name );

	
	
	
	
	
	



	
	
	
	


	
	int tableIdx = GTH_RandomizePower( 1, g_summonMonstertableNumber );
	item.optionIdx[0] = tableIdx;	
	item.timer = g_summonMonstertable[tableIdx].SummonTime;				
	item.optionNumber ++;	

	chASSERT(item.optionIdx[0] != - 1);
	if( item.optionIdx[0] == - 1)	return;								

	
}

int DecideMonsterType(int monsterLevel, int & selectedMonsterLevel)
{	
	int	monsterIdx;
	
	selectedMonsterLevel  = CalcMonsterLevel(monsterLevel);
	bool bBoss = IsBossMonster();

	if( bBoss ) {
		monsterIdx = FindBossMonster(selectedMonsterLevel);
	} else {
		monsterIdx = FindNormalMonster(selectedMonsterLevel);
	}	


	return monsterIdx;
}

int	CalcMonsterLevel(int monsterLevel)
{
	return GTH_RandomizePower( 1, monsterLevel + 10 );
}


bool	IsBossMonster()
{
	return false;

	float rate = GTH_Random();
	
	if( rate <= 0.008 ) return true;
	
	return false;
}


int GetSpawnMapID(int monsterIdx)
{
	monsterCharacter_t* pMonster = &g_monster[monsterIdx];
	if( pMonster == NULL )
		return -1;

	return pMonster->spawnWorldIdx;
}


int FindBossMonster(int level)
{
	return 0;
}


int FindNormalMonster(int level)
{
	int index = -1;
	int templevel = level;
	bool down = true;

	while( index == -1 ) {
		index = FindMonster(level);
		
		if( down )	level--;
		else	    level++;

		if( level == 0 ) {
			level = templevel + 1;
			down = false;
		} else if( level > 100 ) {	
			index = -1;	
			break;
		}
	}

	return index;
}


int FindMonster(int level)
{
	std::vector<int> List;
	for(int i = 0; i < MAX_NUMBER_OF_MONSTER_TABLE; i++ ) {
		if( g_monsterTable[i].level == level )
			List.push_back(i);
	}

	int size = List.size();

	if( size == 0 ) 	return -1;

	return List[GTH_RandomizePower(0, size - 1)];
}


void CalcMonsterOptionNum(item_t& item)
{
	int monsterLevel = item.optionIdx[0];
	chASSERT(monsterLevel >= 0);

	int level = g_monsterTable[monsterLevel].level;

	int monsterOptionCount = GetOptionNum(monsterLevel, 4);
	int playerOptionCount  = GetOptionNum(monsterLevel, 5 - monsterOptionCount);

	item.optionNumber =  monsterOptionCount + playerOptionCount + 1;	
	
	float point = 1000 * (1 - pow(0.98, monsterLevel)) * (1 + (item.optionNumber - 1)*0.1) * (GTH_RandomizePower(0.7, 1.0) + 0.2) ;
	float totalPoint = point * (1 + playerOptionCount * 0.1);

	ShareTotalPoint(item, monsterOptionCount, playerOptionCount, (int)totalPoint);

}


void	ShareTotalPoint(item_t &item, int monsterOptNum, int playerOptNum, int totPoint)
{
	int tempSlot[5]  = { -1, -1, -1, -1, -1};
	int tempSlot1[5] = { -1, -1, -1, -1, -1};
	int optionIdx = -1, tempIndex = 0;
	int maxValue = 0, minValue = 0, resValue = 0, tempValue = 0;

	
	minValue = 10;
	maxValue = totPoint - minValue * (monsterOptNum + playerOptNum);
	resValue = maxValue - minValue;

	srand( (unsigned)time( NULL ) );

	int MonGrade = (g_monsterTable[item.optionIdx[0]].level / 10) + 1;

	
	do {
AA:
		optionIdx = rand() % MONSTER_OPTION;
		if( !CheckOptionGrade(MonGrade, optionIdx) ) goto AA;
		
		if( CheckSameOptionIdx(optionIdx, tempSlot, 5) ) {
			tempSlot[tempIndex] = optionIdx;
			tempIndex++;
		}

	} while( tempIndex != monsterOptNum);

	
	optionIdx = -1, tempIndex = 0;
	do {
BB:
		optionIdx = (rand() % (PLAYER_OPTION - MONSTER_OPTION)) + MONSTER_OPTION;

		if( !CheckOptionGrade(MonGrade, optionIdx) ) goto BB;

		if( CheckSameOptionIdx(optionIdx, tempSlot1, 5) ) {
			tempSlot1[tempIndex] = optionIdx;
			tempIndex++;
		}

	} while( tempIndex != playerOptNum);

	
	if( totPoint < (monsterOptNum + playerOptNum) * 10 || resValue < 0 ) {
		minValue = 10;

		for(int i = 0; i < monsterOptNum; i++) {
			item.optionIdx[i+1] = tempSlot[i];
			item.optionValue[i+1] = minValue;
		}

		for(i = 0; i < playerOptNum; i++) {
			item.optionIdx[monsterOptNum+1 + i] = tempSlot1[i];
			item.optionValue[monsterOptNum+1 + i] = minValue;
		}
		
		return;
	}

	
	for(int i = 0; i < monsterOptNum; i++) {
		item.optionIdx[i+1] = tempSlot[i];
		CheckMinMaxValue(item.optionIdx[i+1], item.optionValue[i+1], resValue, maxValue);
	}

	
	int max;
	for(i = 0; i < playerOptNum; i++) {
		item.optionIdx[monsterOptNum+1 + i] = tempSlot1[i];
		max = CheckMinMaxValue(item.optionIdx[monsterOptNum+1 + i], item.optionValue[monsterOptNum+1 + i], resValue, maxValue);

		
		if( i +1 == playerOptNum )
		{
			if( resValue > max ) resValue = max;
			if( resValue < 0 )	resValue = minValue;
			item.optionValue[monsterOptNum+1 + i] = resValue;
				
		}
	}

}



bool	CheckOptionGrade(int MonGrade, int optionIdx)
{
	if( g_monsterSpawnOption[optionIdx].optionGrade <= MonGrade )
		return true;

	return false;
}


bool	CheckSameOptionIdx(int index, int *temp, int tempcount )
{
	for(int i = 0; i < tempcount; i++)
	{
		if( index == temp[i] )
			return false;
	}

	return true;
}

int GetOptionNum(int monsterLevel, int limit)
{
	int		random;
	float	rate;

	int mul[4] = { 100, 90, 70, 50 };

	for(int i = 1; i < limit; i++ )
	{
		random = (int)(GTH_Random() * 100 );
		rate = mul[i] * (float)(100.0f / (100.0f + (float)monsterLevel));

		if( random > rate ) return i;
	}

	return limit;
}


int		CheckMinMaxValue(int optionIdx, int& optionValue, int& resValue, int maxValue)
{
	int min, max, tempValue;

	FindSpawnCardMinMaxValue(optionIdx, min, max);

	tempValue = rand() % maxValue;

	optionValue = min + tempValue;
	if( optionValue > max )	optionValue = max;

	resValue -= optionValue;

	return max;
}



void	FindSpawnCardMinMaxValue(int optionIdx, int& minValue, int& maxValue)
{
	switch(optionIdx)
	{
	case MONSTER_OPTION_RA_INC:			minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_SA_INC:			minValue = 10, maxValue =  50;	break;
	case MONSTER_OPTION_RA_REC:			minValue = 10, maxValue =  50;	break;
	case MONSTER_OPTION_SA_REC:			minValue = 10, maxValue =  70;	break;
	case MONSTER_OPTION_MIN_ATT:		minValue = 10, maxValue =  30;	break;
	case MONSTER_OPTION_MAX_ATT:		minValue = 10, maxValue =  50;	break;
	case MONSTER_OPTION_MIN_MAG:		minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_MAX_MAG:		minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_ATT_INC:		minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_ASP_INC:		minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_DEX_INC:		minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_DEF_INC:		minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_ESC_INC:		minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_PREG_INC:		minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_NREG_INC:		minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_REG_INC:		minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_CRTDMG_INC:		minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_CRTRAT_INC:		minValue = 10, maxValue = 400;	break;
	case MONSTER_OPTION_NOSTUN:			minValue = 10, maxValue = 400;	break;


	case PLAYER_OPTION_EXP_INC:			minValue = 10, maxValue = 500;	break;
	case PLAYER_OPTION_GENEXP_INC:		minValue = 10, maxValue = 500;	break;
	case PLAYER_OPTION_GENSKILL_INC:	minValue = 10, maxValue = 500;	break;
	case PLAYER_OPTION_NAC_INC:			minValue = 10, maxValue = 500;	break;
	case PLAYER_OPTION_DROPRATE_INC:	minValue = 10, maxValue = 1000;	break;
	}
}



int GTH_SimulateGenerateItemFromMonster( monsterCharacter_t *monster, item_t* spawnItem, int& nSpawnItem )
{
	playerCharacter_t *pc = NULL;
	float addPro = 0.0f;

	lootItem_t		*pLoot;



	int i, j;
	float r, accPro;
	int generateIdx, itemTableIdx, optionNumber;
	int lootType, lootIdx;

	lootType = LOOTTYPE_EVERYONE;
	lootIdx = -1;

	pLoot = &monster->basic.lootItem;
	if( pLoot == NULL || pLoot->maxLootNumber <= 0 ) return( false );


	
	float fItemDropRate =	1.0f;
	if(gGameConfigFlag.m_bMap_Attr_Ratio && SAFE_PCS(monster->mostConEntityIdx))
	{
		PremiumMap	*pMap = g_pc[monster->mostConEntityIdx].GetCurMap();
		{
			fItemDropRate = pMap->Cur.fAddItemDropRatio;
		}
	}
	

	
	for( i = 0; i < pLoot->maxLootNumber; i ++ )
	{
		accPro = 0.0f;
		r = GTH_Random();

		float generatePro;
		for( j = 0; j < pLoot->generateIdxNumber; j ++ )
		{
			if( pLoot->generateProbaility[j] == 0.0f ) continue;

			
			generatePro = pLoot->generateProbaility[j] * (g_logic.itemDropRate * fItemDropRate);
			
			
			accPro += ( generatePro * ( 1.0f + addPro ) );
			
			if( r < accPro ) break;
		}
		if( j == pLoot->generateIdxNumber ) continue;

		generateIdx = pLoot->generateIdx[j];

		
		accPro = 0.0f;
		r = GTH_Random();
		for( j = 0; j < g_itemGenerateTable[generateIdx].itemNumber; j ++ )
		{
			if( g_itemGenerateTable[generateIdx].itemPro[j] == 0.0f ) continue;

			accPro += g_itemGenerateTable[generateIdx].itemPro[j];
			if( r < accPro ) break;
		}
		if( j == g_itemGenerateTable[generateIdx].itemNumber ) continue;
		itemTableIdx = g_itemGenerateTable[generateIdx].itemIdx[j];

		if( itemTableIdx < 0 ) continue;

		
		optionNumber = monster->basic.rareItemGrade;
		optionNumber = GTH_RandomizePower( 1, ( optionNumber + 1 ) * 10 ) / 10;

		if( !SpawnItem( &spawnItem[nSpawnItem], itemTableIdx, monster->basic.optionGrade, monster->basic.level ) )
			continue;

		
		
		if( itemTableIdx == 849 || itemTableIdx == 850 )
		{
			spawnItem[nSpawnItem].reqLevel = monster->basic.level;
		}
		
		else if( itemTableIdx >= 864 && itemTableIdx <= 867 )
		{
			spawnItem[nSpawnItem].reqLevel = monster->basic.level;
		}

		nSpawnItem ++;
	}




	return( true );
}



int Get_EmptyInventoryCount(playerCharacter_t *pc)
{	
	int empytcount = 0;
	for( int i = 0; i < MAX_INVENTORY_SIZE; i ++ )
	{
		if( pc->inventory[i] >= 0 ) continue;
		empytcount++;
	}
	return empytcount;
}


