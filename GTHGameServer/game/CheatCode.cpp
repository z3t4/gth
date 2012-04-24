#include "../global.h"
extern i3socket_t gsSocket;

#define MAX_OF_CHEAT_LEVEL_SKILL		10


#include "../Tools/CTools.h"
extern CTools* gcpTools;
void GTH_DisconnectPlayer(playerCharacter_t* pPlayer,const BOOL bForceQuit);



void GTH_ProcessMessage_CheatCode_FORCEQUIT_BY_GM(void)
{
	const int bytes=NAMESTRING+100;
	char message[bytes+1];
	char targetPlayerName[NAMESTRING+1];

	strncpy(targetPlayerName,MSG_ReadString(),NAMESTRING);
	targetPlayerName[NAMESTRING]=NULL;

	playerCharacter_t* pTargetPlayer = gcpTools->GetPlayerRecordPointer(targetPlayerName);
	if(NULL != pTargetPlayer){
		GTH_DisconnectPlayer(pTargetPlayer,TRUE);		
		_snprintf(message,bytes,"complete force quit [%s]",pTargetPlayer->name);
		message[bytes]=NULL;		
	}else{
		_snprintf(message,bytes,"can't find user [%s]",targetPlayerName);
		message[bytes]=NULL;		
	}

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_CHATMESSAGE_WHISPER);
		MSG_WriteString(&netMessage,"Server");	
		MSG_WriteString(&netMessage,message);
		NET_SendUnreliableMessage(&(g_curPC->sock), &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_ProcessMessage_CheatCode()
{
	int type;
	int value;
	int skillIdx[5], tableIdx[5], initLevel[5];

	type = MSG_ReadByte();	

	
	if( !g_logic.cheatEnable ) return;
	

	switch( type )
	{
	case CHEATCODE_GETEXP :
		{
			value = MSG_ReadLong();
			
			int	OldLevel = g_curPC->level;
			GTH_PC_GetLoot( g_curPC->idx, value, 0, 0, 0 );
			
			g_logSystem->WriteToLog(g_curPC, LOG_CLASS_CHEAT, "%d;%d;%d;;;;",
				LOG_CLASS_ITEM_CHEAT_LEVEL, OldLevel, g_curPC->level);			
			
		}
		break;

	case CHEATCODE_GETGENEXP :
		{
			value = MSG_ReadLong();
			GTH_PC_GetLoot( g_curPC->idx, 0, value, 0, 0 );
		}
		break;

	case CHEATCODE_SKILLPOINT :
		{
			value = MSG_ReadLong();
			g_curPC->bonusSkillPoint ++;
			GTH_SendMessage_ReplyCheatCode( g_curPC, type );
		}
		break;

	case CHEATCODE_GETSKILL :
		{
			value = MSG_ReadLong();
			tableIdx[0] = value;
			initLevel[0] = 1;
			
			
			skillIdx[0] = g_SkillLogic.SKILL_Add( g_curPC, tableIdx[0] );
			if( skillIdx[0] >= 0 )
			{
				GTH_SendMessage_Reply_AddSkill( g_curPC, 1, skillIdx );

				
				CSkilTable	*pSkillTable = NULL;
				
				
				pSkillTable = g_SkillTableDB.FindDB(g_curPC->skill[skillIdx[0]].tableIdx);
				
				if ( pSkillTable != NULL )					
				{										
					g_logSystem->WriteToLog(g_curPC, LOG_CLASS_CHEAT, "%d;%d;;;;%s;",
						LOG_CLASS_ITEM_CHEAT_SKILL, pSkillTable->m_nId, "noName");
				}
				
				
			}
		}
		break;

	case CHEATCODE_GETMAXSE :
		{
			value = MSG_ReadLong();
			g_curPC->curChargeSE += value;
			
			if( g_curPC->curChargeSE > MAX_OWN_NAK ) g_curPC->curChargeSE = MAX_OWN_NAK;
			GTH_SendMessage_ReplyCheatCode( g_curPC, type );

			
			g_logSystem->WriteToLog(g_curPC, LOG_CLASS_CHEAT, "%d;%d;;;;;",
			LOG_CLASS_ITEM_CHEAT_NAK, value);
			

		}
		break;

	case CHEATCODE_CLEARQUEST :
		{
			value = MSG_ReadLong();
			g_curPC->questVar[value].iVar = 65280;
			GTH_ClearQuestStatus( g_curPC, value, 0 );

			
			g_logSystem->WriteToLog(g_curPC, LOG_CLASS_CHEAT, "%d;%d;;;;%s;",
				LOG_CLASS_ITEM_CHEAT_QUEST, value, "noName");
			
		}
		break;

	
	case CHEATCODE_ADDITEM :
		{
			value = MSG_ReadLong();
			int value2 = MSG_ReadShort();

			int addItemData[6];

			addItemData[0] = value;	
			addItemData[1] = -1;	
			addItemData[2] = 10;	
			addItemData[3] = 1;		

			if ( value == QUESTSCROLL_ITEM_INDEX )
			{
				if ( value2 < 0 )
				{
					switch ( g_curPC->pcJob )
					{
					case 0 : case 4 : case 5 :	
						addItemData[4] = 100;
						break;
					case 1 : case 6 : case 7 :	
						addItemData[4] = 107;
						break;
					case 2 : case 8 : case 9 : case 10 :	
						addItemData[4] = 207;
						break;
					case 3 : case 11 : case 12 : case 13 :	
						addItemData[4] = 200;
						break;
					}
					addItemData[5] = FindRewardField( addItemData[4] );
				}
				else
				{
					addItemData[4] = value2;
					addItemData[5] = FindRewardField ( value2 );
				}
			}			
			
			
			
			if ( value  == MONSTERCARD_ITEM_INDEX)
				addItemData[2] = value2;		
			


			
			
			
			GTH_CheatAddItem(g_curPC, addItemData);		
			
		}
		break;

	case CHEATCODE_GETGENCAPABILITY :
		{
			value = MSG_ReadLong();
			g_curPC->genCapability[g_curPC->selectedGen] += value;
			GTH_SendMessage_ReplyCheatCode( g_curPC, type );
		}
		break;

	case CHEATCODE_MAXRASA :
		{
			value = MSG_ReadLong();
			g_curPC->curRA = g_curPC->calMaxRA;
			g_curPC->curSA = g_curPC->calMaxSA;
		}
		break;

	case CHEATCODE_AWAKEN :
		{
			value = MSG_ReadLong();
			int awakenType[3] = { 0, 0, 0 };
			awakenType[0] = value;
			if( g_curPC->pcJob <= 3 && awakenType[0] > 3 ) AI_AwakenPlayerJob( g_curPC, awakenType );
		}
		break;

	case CHEATCODE_READYAWAKEN :
		{
			value = MSG_ReadLong();
			GTH_PC_GetLoot( g_curPC->idx, g_expTable[29].totalexp, g_expTable[14].totalGenExp, 100000000, 0 );
			g_curPC->force = 200;
			g_curPC->soulpower = 200;
			g_curPC->agility = 200;
			g_curPC->vitality = 200;
			GTH_PC_Calculate( g_curPC );
		}
		break;

	case CHEATCODE_GOMONSTER :
		{
			value = MSG_ReadLong();
			int zoneIdx;
			int monsterIdx = value;
			
			if (monsterIdx < 0 || monsterIdx >= MAX_MONSTERS)
				return;
			
			if( !g_monster[monsterIdx].active ) return;
			
			if ( g_curPC->worldIdx == g_monster[monsterIdx].worldIdx )
			{
				vec_copy( g_monster[monsterIdx].position, g_curPC->position );
				zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_curPC->idx, g_curPC->worldIdx, g_curPC->zoneIdx, g_curPC->position);
				g_curPC->zoneIdx = zoneIdx;
				
				GTH_SendPCEventMessage_Respawn( g_curPC );
				GTH_SendMessage_SyncItemObject( g_curPC );				
			}
		}
		break;
		
	
	
	case CHEATCODE_ITEM :
		{
			item_t item;
			int optionNumber;

			InitItem( &item );

			item.itemTableIdx = MSG_ReadShort();
			optionNumber = MSG_ReadByte();

			for( int i=0; i<optionNumber; i++ )
			{
				item.optionIdx[i] = MSG_ReadShort();
				item.optionValue[i] = MSG_ReadShort();
				if( item.optionIdx[i] >= 0 ) item.optionNumber = i + 1;
			}

			GTH_CheatCode_AddItem( g_curPC, &item );
		}
		break;

	case CHEATCODE_MONSTER:
		{
			int monsterTableIdx = MSG_ReadShort();
			int AIType = MSG_ReadShort();
			int idx = MONSTER_Summon( monsterTableIdx, g_curPC->worldIdx, g_curPC->position, AIType );
			if( idx < 0 ) break;

			g_monster[idx].basic.level = MSG_ReadShort();
			g_monster[idx].basic.maxRA = MSG_ReadShort();
			g_monster[idx].basic.maxSA = MSG_ReadShort();
			g_monster[idx].basic.recoveryRARate = MSG_ReadShort();
			g_monster[idx].basic.recoverySARate = MSG_ReadShort();
			g_monster[idx].basic.atkPhyMinDmg = MSG_ReadShort();
			g_monster[idx].basic.atkPhyMaxDmg = MSG_ReadShort();
			g_monster[idx].basic.atkMagMinDmg = MSG_ReadShort();
			g_monster[idx].basic.atkMagMaxDmg = MSG_ReadShort();
			g_monster[idx].basic.atkHitPower = MSG_ReadShort();
			g_monster[idx].basic.atkDelay = MSG_ReadShort();
			g_monster[idx].basic.atkLength = MSG_ReadShort();
			g_monster[idx].basic.atkRange = MSG_ReadShort();
			g_monster[idx].basic.atkCriticalPro = MSG_ReadFloat();
			g_monster[idx].basic.atkCriticalDmg = MSG_ReadShort();
			g_monster[idx].basic.defPower = MSG_ReadShort();
			g_monster[idx].basic.defAvoidPower = MSG_ReadShort();
			g_monster[idx].basic.defPositiveRes = MSG_ReadShort();
			g_monster[idx].basic.defNegativeRes = MSG_ReadShort();
			g_monster[idx].basic.velocity = MSG_ReadShort();
			g_monster[idx].basic.stunTime = MSG_ReadShort();
			g_monster[idx].basic.exp = MSG_ReadShort();
			g_monster[idx].basic.genExp = MSG_ReadShort();
			g_monster[idx].basic.lootSE = MSG_ReadShort();
			g_monster[idx].basic.pathFindingSize = MSG_ReadShort();

			MONSTER_Calculate( &g_monster[idx] );
		}
		break;

	case CHEATCODE_ATKDELAY :
		{
			int forcedAtkDelay = MSG_ReadShort();
			if( forcedAtkDelay < 0 ) g_curPC->forceAtkDelay = -1;
			else g_curPC->forceAtkDelay = forcedAtkDelay;
			
			GTH_PC_Calculate( g_curPC );
		}
		break;
	
	case CHEATCODE_LEVEL :
		{
			int exp, genexp, se, awaken, pcJob;

			exp = MSG_ReadLong();
			genexp = MSG_ReadLong();
			se = MSG_ReadLong();
			awaken = MSG_ReadShort();
			pcJob = MSG_ReadShort();

			
			int OldLevel = g_curPC->level;
			
			
			GTH_CheatCode_InitSkill( g_curPC );
			
			if ( awaken < 0 )
				g_curPC->pcJob = pcJob;
			
			GTH_PC_GetLoot_Cheat( g_curPC->idx, exp, genexp, se, 0 );
			
			int awakenType[3] = { 0, 0, 0 };
			awakenType[0] = awaken;
			if( g_curPC->pcJob <= 3 && awakenType[0] > 3 ) AI_AwakenPlayerJob( g_curPC, awakenType );			
			
			g_logSystem->WriteToLog(g_curPC, LOG_CLASS_CHEAT, "%d;%d;%d;;;;;",
				LOG_CLASS_ITEM_CHEAT_LEVEL, OldLevel, g_curPC->level);
			


		}
		break;
	
	case CHEATCODE_SKILL :
		{
			int gSkillIdx[MAX_OF_CHEAT_LEVEL_SKILL], gSkillLevel[MAX_OF_CHEAT_LEVEL_SKILL];

			int skillNumber = MSG_ReadShort();
			
			for( int i=0; i<skillNumber; i++ )
			{
				gSkillIdx[i] = MSG_ReadShort();
				gSkillLevel[i] = MSG_ReadShort();
			}
			
			GTH_CheatCode_AddSkill( g_curPC, skillNumber, gSkillIdx, gSkillLevel );


			
			for( i=0; i<skillNumber; i++ )
			{				
				CSkilTable	*pSkillTable = NULL;
				pSkillTable = g_SkillTableDB.FindDB(gSkillIdx[i]);
				
				if ( pSkillTable != NULL )					
				{
					
					
					
					
					g_logSystem->WriteToLog(g_curPC, LOG_CLASS_CHEAT, "%d;%d;;;;%s;",
						LOG_CLASS_ITEM_CHEAT_SKILL, gSkillIdx[i], "noName");
				}
			}
			


		}
		break;
	case CHEATCODE_ITEMDROPTEST :
		{
			int monsterTableIdx;
			long killNumber;

			monsterTableIdx = MSG_ReadShort();
			killNumber = MSG_ReadLong();

			GTH_CheatCode_ItemDropTest( monsterTableIdx, killNumber );
		}
		break;

		
	case CHEATCODE_MONSTERSPAWN:
		{
			int montblIdx = -1;
			int ai_idx = -1;
			montblIdx	 = MSG_ReadShort();
			ai_idx		 = MSG_ReadShort();

			if ( montblIdx < 0 ) return;			



			int idx = MONSTER_Summon( montblIdx, g_curPC->worldIdx, g_curPC->position, ai_idx );
			if( idx < 0 ) break;

			g_logSystem->Write("USE CHEAT : MonsterSpawn. %s(%s) )", g_curPC->name, g_curPC->userID );

			MONSTER_Calculate( &g_monster[idx] );;			
		}
		break;		
		
		
	case CHEATCODE_MONSTERKILL:
		{
			int range	 = MSG_ReadShort();
			GTH_CheatCode_MonsterKill(range);
		}		
		break;

		
		
	case CHEATCODE_ADDQUESTITEM:
		{	
			int itemidx	 = MSG_ReadShort();			
			GTH_CheatCode_AddQuestItem(itemidx);
		}	
		break;	
	
	case CHEATCODE_OUTPUTGUILDINFO:
		{
			OutputGuildInfomation();
		}	
		break;		
	

	
	case CHEATCODE_GET_MEMBERSHIP_TYPE:
		{
			int lpackidx = 3;

			if ( g_curPC->PremiumInfo.iMemberShipType ==
				playerCharacter_t::tagPremiumInfo::enumMeberShipType::Pay)
				lpackidx = 3;
			else
				lpackidx = 4;


			GTH_SendMessage_ChattingMsg(g_curPC, 2, 0, lpackidx );		

		}break;
	case CHEATCODE_FORCEQUIT_BY_GM:
		GTH_ProcessMessage_CheatCode_FORCEQUIT_BY_GM();
		break;		










	
	case CHEATCODE_CLEARINVEN:
		{
			for (int iInvenPos = 0; iInvenPos < MAX_INVENTORY_SIZE; iInvenPos++)
			{
				int ItemIdx = g_curPC->inventory[iInvenPos];													
				if(ItemIdx< 0 || ItemIdx>= MAX_NUMBER_OF_OWNITEM) 
					continue;									

				item_t* pTempItem = NULL;
				pTempItem= &g_curPC->item[iInvenPos];				

				if ( pTempItem->itemTableIdx >=0 || pTempItem->itemTableIdx < MAX_NUMBER_OF_ITEM_TABLE)
				{					
					g_curPC->curItemWeight -= g_itemTable[pTempItem->itemTableIdx].weight;
					if ( g_curPC->curItemWeight < 0 )
						g_curPC->curItemWeight = 0;
				}
				
				GTH_DeleteMyItem( g_curPC, pTempItem);
				g_curPC->inventory[iInvenPos] = -1;												
				g_DBGameServer->RemoveItem( g_curPC, ItemIdx );
			}

			GTH_SendMessage_ReplyCheatCode(g_curPC, CHEATCODE_CLEARINVEN);
		}
		break;

	
	case CHEATCODE_KILLME:
		{
			value = MSG_ReadLong();
			g_curPC->curRA=value;
		}
		break;
	case CHEATCODE_DANBATTLE_PCPOINT:
		{
			GTH_SendMessage_ReplyCheatCode(g_curPC, CHEATCODE_DANBATTLE_PCPOINT);
		}
		break;

	}
		
}


void GTH_CheatCode_AddItem( playerCharacter_t* pc, item_t *cheatItem )
{
	item_t item, *upItem;

	int	bIsStack = false, stackNumber = 1;
	int	itemTableIdx = cheatItem->itemTableIdx;
	int	stackItemIdx, inventoryPos, itemIdx;
	int	reply = 1, error = 0;
	char generateID[20];
	int itemNumber = 1;

	InitItem( &item );

	
	
	if( g_itemTable[itemTableIdx].idx < 0 ) return;

	SpawnItem( &item, itemTableIdx, 1 );

	if( GTH_IsStackItem( &item ) )
	{
		item.durability = itemNumber - 1;
		
		bIsStack = true;
		stackNumber = itemNumber;
	}
	else if( g_itemTable[itemTableIdx].durability > 0 )
	{		item.durability = 100;
	}

	stackItemIdx = GTH_StackMyItem( pc, &item );

	if( stackItemIdx >= 0 )
	{
		upItem = &pc->item[stackItemIdx];
		upItem->durability += item.durability + 1;
		if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;
		
		g_DBGameServer->SaveItem( pc, upItem, stackItemIdx );
		
		
		
		



		

		
		
		int SelectInvenIdx = -1;
		for( int InvenIdx = 0; InvenIdx < MAX_INVENTORY_SIZE; InvenIdx ++ )
		{
			if( g_curPC->inventory[InvenIdx] < 0 ) continue;			
			int itemIdx = g_curPC->inventory[InvenIdx];
			
			if( GTH_CheckStackItem( upItem, &g_curPC->item[itemIdx] ) )
			{
				SelectInvenIdx = InvenIdx;
				break;
			}
		}
		
		g_logSystem->WriteToLog(g_curPC, LOG_CLASS_CHEAT, "%d;%d;%d;;;%s;%s",
			LOG_CLASS_ITEM_CHEAT_ITEM, upItem->itemTableIdx, upItem->durability, upItem->name,
			GetItemInformationLog( bIsStack, upItem->itemTableIdx, stackItemIdx, SelectInvenIdx));
		
	}
	else
	{
		item.optionNumber = cheatItem->optionNumber;

		for( int i=0; i<item.optionNumber; i++ )
		{
			item.optionIdx[i] = cheatItem->optionIdx[i];
			item.optionValue[i] = cheatItem->optionValue[i];
		}

		strcpy( item.name, g_itemTable[itemTableIdx].name );

		
		inventoryPos = GTH_FindEmptyInventoryPos( pc );
		if( inventoryPos < 0 )	
		{
			g_logSystem->Write("Obtain Quest Item Failure!! : Inventory Space not Enough. %s(%s) )", pc->name, pc->userID );
			reply = 0;
			error = ERROR_PICKUPITEM_4;
			GTH_SendMessage_AddInvItem( pc, reply, error, &item );
			return;
		}
		
		itemIdx = GTH_AddMyItem( pc, &item );

		if (itemIdx < 0)	
		{
			reply = 0;
			error = ERROR_PICKUPITEM_5;
			GTH_SendMessage_AddInvItem( pc, reply, error, &item );
			return;
		}
		
		
		pc->inventory[inventoryPos] = itemIdx;
		
		strcpy(generateID, "questGenID");
		g_DBGameServer->AddItem(pc, &item, itemIdx, generateID);

		
		
		



		

		
		g_logSystem->WriteToLog(g_curPC, LOG_CLASS_CHEAT, "%d;%d;%d;;;%s;%s",
			LOG_CLASS_ITEM_CHEAT_ITEM, item.itemTableIdx, 0, item.name,
			GetItemInformationLog( bIsStack, item.itemTableIdx, itemIdx, inventoryPos ));
		
	}

	GTH_SendMessage_AddInvItem( pc, reply, error, &item );
	
}


void GTH_CheatCode_AddSkill( playerCharacter_t* pc, int skillNumber, int* skillTableIdx, int* skillLevel )
{
	int idx, skillIdx[MAX_OF_CHEAT_LEVEL_SKILL];

	for (int i=0; i<skillNumber; i++)
	{
		
		idx = g_SkillLogic.SKILL_Add( pc, skillTableIdx[i] );

		pc->skill[idx].level.gen[0] = skillLevel[i];
		pc->skill[idx].level.gen[1] = skillLevel[i];
		pc->skill[idx].level.gen[2] = skillLevel[i];
		pc->skill[idx].level.gen[3] = skillLevel[i];

		skillIdx[i] = idx;
	}

	GTH_SendMessage_Reply_AddSkill( pc, skillNumber, skillIdx );

}


void GTH_CheatCode_InitSkill( playerCharacter_t* pc )
{
	for( int i = 0; i < MAX_NUMBER_OF_SKILL; i ++ )
	{
		pc->skill[i].tableIdx = -1;
	}

	for( i=0; i<MAX_NUMBER_OF_HOTKEY; i++ )
	{
		pc->hotkeyIdx[i] = -1;
	}

	pc->skillNumber = 0;

}


void GTH_TestItemSpawnCode()
{
	
	
	item_t	testItem;
	int		testItemNumber = 0;
	FILE	*fp;
	char	cTime[26], optionPrinting[128];
	time_t	writeTime;

	int		optionValue;

	init_genrand( (unsigned) time( NULL ) );
	
	fp = fopen( "testItem.txt", "w" );

    time( &writeTime );	strcpy( cTime, ctime( &writeTime ) );
	
	cTime[24]		=	'\0';

	fprintf( fp, "%s Test Item ----------------------\n", cTime);

	while( 1 )
	{
		int itemTableIdx	=	GTH_Randomize( MAX_NUMBER_OF_ITEM_TABLE );
		int optionNumber	=	GTH_Randomize( 6 ) + 1;
		int optionGrade		=	GTH_Randomize( 10 );

		if( g_itemTable[itemTableIdx].idx < 0 ) continue;			
		if( g_itemTable[itemTableIdx].name[0] == 0 ) continue;		
		if( g_itemTable[itemTableIdx].name[0] == 'B' ) continue;	
		if( g_itemTable[itemTableIdx].etcItemType != ETCITEM_TYPE_NONE ) continue;	

		SpawnItem( &testItem, itemTableIdx, optionNumber, optionGrade, 100 );

		if( testItem.optionNumber == 0 || itemTableIdx == 868 ) continue;					

		fprintf( fp, "[%d] %s(Lv.%d)\n", testItemNumber, testItem.name, testItem.reqLevel );

		for( int k = 0; k < testItem.optionNumber; k ++ )
		{
			optionValue			=	OPTION_GetOptionValue(	&testItem, testItem.optionIdx[k], testItem.optionValue[k] - 1 );
			
			sprintf( optionPrinting, g_optionTable[testItem.optionIdx[k]].printing, optionValue );
			if( optionValue < 0 )
			{
				fprintf( fp, "[%d, +%d]%s*\n", testItem.optionIdx[k], testItem.optionValue[k], optionPrinting );
			}
			else
			{
				fprintf( fp, "[%d, +%d]%s\n", testItem.optionIdx[k], testItem.optionValue[k], optionPrinting );
			}
		}
		fprintf( fp, "\n" );

		testItemNumber ++;
		if( testItemNumber >= 10000 ) break;
	}

	fclose( fp );
	
}


void GTH_CheatCode_ItemDropTest( int monsterTableIdx, long killNumber )
{

	monsterCharacter_t monster;
	memcpy( &monster.basic, &g_monsterTable[monsterTableIdx], sizeof( monsterBasicTable_t ) );

	item_t			spawnItem[MAX_NUMBER_OF_ITEM_LOOT];
	int				nSpawnItem;

	int resultItem[1000], resultNumber[1000];
	int m = 0;

	for( int i=0; i<1000; i++ )
	{
		resultItem[i] = -1;
		resultNumber[i] = 0;
	}

	for( long idx=0; idx<killNumber; idx++ )
	{
		for(int j=0;j<MAX_NUMBER_OF_ITEM_LOOT;j++)
		{
			memset( &spawnItem[j], 0, sizeof( item_t ) );
		}
		nSpawnItem = 0;

		GTH_SimulateGenerateItemFromMonster( &monster, spawnItem, nSpawnItem );

		for(int k=0; k<nSpawnItem; k++)
		{
			int bAlready = 0;
			for( int l=0; l<1000; l++ )
			{
				if ( spawnItem[k].itemTableIdx == resultItem[l] )
				{
					resultNumber[l]++;
					bAlready = 1;
				}
			}
			if ( bAlready == 0 )
			{
				resultItem[m] = spawnItem[k].itemTableIdx;
				resultNumber[m] = 1;
				m++;
			}

		}

	}

	GTH_SendMessage_ItemDropTest( g_curPC, resultItem, resultNumber );

}


void GTH_CheatCode_MonsterKill( int range )
{	
	if ( range <= 0)
		range = 10;

	
	
	
	int localWorldIdx = GTH_GetLocalWorldIdx( g_curPC->worldIdx );		
	
	for( int i = 0; i < MAX_MONSTERS; i ++ )
	{		
		if( !g_monster[i].active ) continue;
		int localWorldIdx = GTH_GetLocalWorldIdx( g_monster[i].worldIdx );	
		if( g_monster[i].worldIdx != g_curPC->worldIdx) continue;
		
		float dist = GTH_GetDistance2D( g_monster[i].position, g_curPC->position);
		
		if ( dist < range  * g_cmap[localWorldIdx].scale)
		{
			g_monster[i].aliveState = MON_ALIVE_STATE_NORMAL;
			g_monster[i].ai.aiEvent = AI_DIE;
			g_monster[i].event = GTH_EV_CHAR_DIE;
			g_monster[i].mostConEntityType = ENTITY_PC;
			g_monster[i].conInfo[0].entityIdx = g_curPC->idx;
			g_monster[i].conInfo[0].atkPoint =1;		
			g_monster[i].conInfoNumber = 1;

		
			
			sprintf(g_monster[i].conInfo[0].entityName, "%s", g_curPC->name);		
			g_monster[i].deathTime = g_globalTime;
			g_monster[i].maxAtkContributionPoint = 1;			
		}
	}	
}




void GTH_CheatCode_AddQuestItem( int idx )
{
	int itemidx	 = idx;	
	
	if ( g_curPC == NULL)
		return ;

	if ( itemidx < 0) 
		return;	
	
	
	
	for (int i = 0; i < MAX_NUMBER_OF_MONSTER_TABLE; i ++)
	{				
		for (int itemnumber = 0; itemnumber < g_qigTable[i].itemNumber; itemnumber++ )				
		{
			if ( g_qigTable[i].itemTableIdx[itemnumber] == itemidx )
			{
				
				if ( TRUE == GTH_AddQuestItem( g_curPC, itemidx, g_qigTable[i].questIdx[itemnumber], 
					g_qigTable[i].maxItemCount[itemnumber], g_qigTable[i].maxItemCount[itemnumber] )
					)					
				{
					
					g_logSystem->WriteToLog(g_curPC, LOG_CLASS_CHEAT, "%d;%d;%d;;;%s",
						LOG_CLASS_ITEM_CHEAT_ITEM, itemidx, 1, 
						g_itemTable[g_qigTable[i].itemTableIdx[itemnumber]].name,
						GetItemInformationLog( 1, itemidx, -1, -1 ));
				}
				
				return;
			}					
		}
	}			
}


#include <shlwapi.h>
void OutputGuildInfomation(FILE* fp,const int GuildIdx)
{
	if(0 > GuildIdx) return;
	if(MAX_GUILDS <= GuildIdx) return;
	guild_t* pGuild=&g_guild[GuildIdx];

	char buffer[4096];
	

	fprintf(fp,"\n**********************************************************************");
	
	fprintf(fp,"\n idx=[%d]",pGuild->idx);
	
	fprintf(fp,"\n server group=[%d]",pGuild->serverGroupID);


	
	fprintf(fp,"\n name=[%s]",pGuild->name);

	
	
	
	StrCpyN(buffer,pGuild->notice,GUILDNOTICESTRING); buffer[GUILDNOTICESTRING]=NULL;
	fprintf(fp,"\n notice=[%s]",buffer);

	
	fprintf(fp,"\n famePoint=[%d]",pGuild->famePoint);

	
	fprintf(fp,"\n expPoint=[%d]",pGuild->expPoint);

	
	fprintf(fp,"\n raidPoint=[%d]",pGuild->raidPoint);

	
	fprintf(fp,"\n pvpPoint=[%d]",pGuild->pvpPoint);

	
	fprintf(fp,"\n hunterPoint=[%d]",pGuild->hunterPoint);

	
	fprintf(fp,"\n level=[%d]",pGuild->level);

	
	StrCpyN(buffer,pGuild->masterName,NAMESTRING); buffer[NAMESTRING]=NULL;
	fprintf(fp,"\n masterName=[%s]",buffer);

	
	for(int SubMasterIdx=0; SubMasterIdx < MAX_GUILD_SUBMASTERS; SubMasterIdx++){
		StrCpyN(buffer,pGuild->subMasterName[SubMasterIdx],NAMESTRING); buffer[NAMESTRING]=NULL;
		fprintf(fp,"\n subMaster[%02d] name=[%s]",SubMasterIdx,buffer);
	}


	
	for(int factionMasterIdx=0; factionMasterIdx < MAX_GUILD_FACTIONMASTERS; factionMasterIdx++){
		StrCpyN(buffer,pGuild->factionMasterName[factionMasterIdx],NAMESTRING); buffer[NAMESTRING]=NULL;
		fprintf(fp,"\n factionMaster[%02d] name=[%s]",factionMasterIdx,buffer);
	}
	
	for(int mateIdx=0; mateIdx < MAX_GUILD_MATES; mateIdx++){
		StrCpyN(buffer,pGuild->mateName[mateIdx],NAMESTRING); buffer[NAMESTRING]=NULL;
		fprintf(fp,"\n mate[%2d] name=[%s]",mateIdx,buffer);
	}
	
	
	

	fprintf(fp,"\n**********************************************************************");
}

void OutputGuildInfomation(void)
{
	char szFileName[_MAX_PATH];
	struct tm *date_tm;
	time_t system_time;
	time(&system_time);
	date_tm = localtime(&system_time );

	sprintf(szFileName,
		"GS%02d%02d_%04d%02d%02d_%02d%02d%02d.Guild.Txt",
		g_config.serverGroupNo, 
		g_config.gameServerNo, 
		date_tm->tm_year, 
		date_tm->tm_mon, 
		date_tm->tm_mday, 
		date_tm->tm_hour,
		date_tm->tm_min,
		date_tm->tm_sec);

	FILE* fp = fopen(szFileName,"at+");
	if(NULL == fp) return;


	for(int GuildIdx=0; GuildIdx < MAX_GUILDS; GuildIdx++){
		if(NULL == g_guild[GuildIdx].name[0]) continue;

		OutputGuildInfomation(fp,GuildIdx);
	}

	fclose(fp); fp=NULL;
}




int GTH_CheatAddItem(void *pPc, int *pParam )
{
	playerCharacter_t* pc	= static_cast< playerCharacter_t* >( pPc );

	int		itemTableIdx	= pParam[0];	
	int		optionGroupIdx	= pParam[1];	
	int		itemNumber		= pParam[2];	
	int		messageFlag		= pParam[3];	
	int		stackItemIdx, inventoryPos, itemIdx;
	int		reply = 1, error = 0;
	int		bIsStack = false, stackNumber = 1;
	char	generateID[20];
	item_t	item, *upItem;


	itemTable_t* pItemTable = gcpTools->GetItemTablePointer(itemTableIdx);
	if(NULL == pItemTable) return FALSE;
	
	if(pItemTable->idx < 0) return FALSE;

	
	
	if( optionGroupIdx >= 0 )
	{
		SpawnItemByOptionGroup( &item, itemTableIdx, optionGroupIdx );
	}
	else
	{
		
		
		SpawnItem( &item, itemTableIdx );
	}
	
	
	
	if (itemTableIdx == MONSTERCARD_ITEM_INDEX )
	{
		item.optionIdx[0] = itemNumber;	
		item.timer = g_summonMonstertable[itemNumber].SummonTime;				
		item.optionNumber ++;		
	}	
	
	

	if( GTH_IsStackItem( &item ) )
	{
		item.durability = itemNumber - 1;
		
		bIsStack = true;
		stackNumber = itemNumber;
	}
	else if( g_itemTable[itemTableIdx].durability > 0 )
	{
		item.durability = 100;
	}

	stackItemIdx = GTH_StackMyItem( pc, &item );

	if( stackItemIdx >= 0 )
	{
		upItem = &pc->item[stackItemIdx];
		upItem->durability += item.durability + 1;
		if( upItem->durability + 1 >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;
		
		g_DBGameServer->SaveItem( pc, upItem, stackItemIdx );

		
		
		
		int SelectInvenIdx = -1;
		for( int InvenIdx = 0; InvenIdx < MAX_INVENTORY_SIZE; InvenIdx ++ )
		{
			if( g_curPC->inventory[InvenIdx] < 0 ) continue;			
			int itemIdx = g_curPC->inventory[InvenIdx];
			
			if( GTH_CheckStackItem( upItem, &g_curPC->item[itemIdx] ) )
			{
				SelectInvenIdx = InvenIdx;
				break;
			}
		}
		g_logSystem->WriteToLog(g_curPC, LOG_CLASS_CHEAT, "%d;%d;%d;;;%s;%s",
			LOG_CLASS_ITEM_CHEAT_ITEM, upItem->itemTableIdx, upItem->durability, upItem->name,
			GetItemInformationLog( bIsStack, upItem->itemTableIdx, stackItemIdx, SelectInvenIdx));
		
		
	}
	else
	{
		strcpy( item.name, g_itemTable[itemTableIdx].name );

		
		if (itemTableIdx == QUESTSCROLL_ITEM_INDEX)
		{
			item.optionIdx[0] = pParam[4];	
			item.optionIdx[1] = pParam[5];	

			item.optionNumber = 2;
		}
		
		
		if (itemTableIdx == AUTOQUESTSCROLL_ITEM_INDEX)
		{
			item.optionIdx[0] = pParam[4];	
			item.optionIdx[1] = pParam[5];	

			item.optionNumber = 2;
		}
		
		
		inventoryPos = GTH_FindEmptyInventoryPos( pc );
		if( inventoryPos < 0 )	
		{
			
			int itemObjectIdx = GTH_SpawnItemObject( &item, pc->worldIdx, pc->position, LOOTTYPE_INDIVIDUAL, pc->idx );
		
			if ( itemObjectIdx < 0 )
			{
				g_logSystem->Write("Can't Obtain Item Error!!(pc name:%s, item index:%d, option group index:%d", pc->name, item.itemTableIdx, optionGroupIdx );
			}
			return 0;

		}
		
		itemIdx = GTH_AddMyItem( pc, &item );

		if (itemIdx < 0)	
		{
			
			int itemObjectIdx = GTH_SpawnItemObject( &item, pc->worldIdx, pc->position, LOOTTYPE_INDIVIDUAL, pc->idx );

			if ( itemObjectIdx < 0 )
			{
				g_logSystem->Write("Can't Obtain Item Error!!(pc name:%s, item index:%d, option group index:%d", pc->name, item.itemTableIdx, optionGroupIdx );
			}
			return 0;
		}
		
		
		pc->inventory[inventoryPos] = itemIdx;
		
		strcpy(generateID, "questGenID");
		g_DBGameServer->AddItem(pc, &item, itemIdx, generateID);

		
		g_logSystem->WriteToLog(g_curPC, LOG_CLASS_CHEAT, "%d;%d;%d;;;%s;%s",
			LOG_CLASS_ITEM_CHEAT_ITEM, item.itemTableIdx, 0, item.name,
			GetItemInformationLog( bIsStack, item.itemTableIdx, itemIdx, inventoryPos ));
		
		
	}

	
	if ( reply == 1 )
		error = 1 - pParam[3];	

	GTH_SendMessage_AddInvItem( pc, reply, error, &item );

	return 1;
}