#include "../global_def.h"

globalVariable_t	g_cgv;

void GTH_InitClientGlobalVariable()
{
	g_cgv.state = CLIENT_START_PROCESS;

	
	g_cgv.onMasterServer = false;
	g_cgv.onGameServer = false;

	g_cgv.displayDebugMessage = 0;

	g_cgv.enableChat = false;			
	g_cgv.chatColor = 0xffffffff;
	g_cgv.whisperType = 0;				

	g_cgv.myCharacterInfo = &g_cgv.savedData[0];

	g_cgv.flagChangeWorld = false;

	g_cgv.offLineStartFlag = false;
	g_cgv.processChangeType = true;

	g_cgv.clickEvent = GTH_CEV_CHAR_NONE;

	GTH_InitSavedData();	

	g_cgv.inputMode = CLIENT_INPUT_MOUSE_MODE;

	g_cgv.resendFlag = false;
	g_cgv.enablePK = false;
	g_cgv.isReceiveAllSavedData = false;
	g_cgv.resendTargeting = false;

	VectorClear( g_cgv.oldTargetPosition );

	g_cgv.targetItemIdxOnServer = -1;
	g_cgv.epackRADelayTime = 0.0f;
	g_cgv.epackSADelayTime = 0.0f;
	
	GTH_UnlockSending();
	
	g_cgv.resendAttack = false;
	g_cgv.saveEvent = -1;
	g_cgv.sendSkillFlag = false;
	
	
	memset( g_cgv.targetName, 0, NAMESTRING );
	g_cgv.callGMTimer = 0;


	for( int i = 0; i < 20; i ++ )
	{
		memset( g_cgv.callerName[i], 0, sizeof( chatMessage ) );
	}
	g_cgv.isGmBusy = false;
	g_cgv.isGmHide = false;

	
	g_cgv.summonMonsterIdx = 0;
	g_cgv.summonAiType = 2;
	g_cgv.summonValidTime = -1.0f;

	
	g_cgv.saveMenuTime = -1.0f;

	
	g_cgv.restartState = RESTART_NONE;
	g_cgv.savedDataNumber = 0;

	g_cgv.currentSyncTime = -1.0f;
	g_cgv.lastSyncTime = -1.0f;
	g_cgv.minSyncTime = 10000.0f;
	g_cgv.maxSyncTime = 0.0f;
	g_cgv.missSyncTimeCount = 0;

	memset( g_cgv.syncTimeList, 0, sizeof(g_cgv.syncTimeList) );
	g_cgv.syncSampleCount		=	0;
	g_cgv.syncAverage			=	0.0f;
	g_cgv.syncOverAverageCount	=	0;

	
	
	
	
	
	
	g_cgv.myCharacterInfo->m_bChatBan_Flag=false;


	g_cgv.myCharacterInfo->m_dwLimite_ChatTime=0.0f;
	
	
	g_cgv.myCharacterInfo->itemDivideType=0;
	g_cgv.myCharacterInfo->m_bischange=false;


	g_cgv.myCharacterInfo->m_bbattleMode=FALSE;

	
	g_cgv.m_fClientGameVersion =1.16f;

	
	g_cgv.Skill.iRecvSkillCastingCount=-1;
	g_cgv.Skill.iSkillExecuteIndex=-1;
	g_cgv.Skill.iSkillExecuteTargetType=0;
	g_cgv.Skill.iSkillExecuteUseType=0;
	g_cgv.Skill.iSelectSkillUseType=0;
	g_cgv.Skill.iSelectSkillTableIdx=-1;
	g_cgv.Skill.iUseInvenPos=0;	

	
	g_cgv.characterSlotMode = globalVariable_t::CharacterSlotMode::FRONT;
	
}

int GTH_InputState()
{
	return( g_cgv.inputMode );
}

void GTH_InitSavedData()
{
	int i, j, k;

	for( i = 0; i < MAX_SAVED_DATA; i ++ )
	{
		memset( &g_cgv.savedData[i], 0, sizeof( myCharacterInfo_t ) );

		
		g_cgv.savedData[i].m_CashMall_OptionStone.InitCashMallItem_OptionStone();
		

		g_cgv.savedData[i].characterID = -1;
		g_cgv.savedData[i].targetIdx = -1;
		
		for( j = 0; j < MAX_INVENTORY_SIZE; j ++ )
			g_cgv.savedData[i].inventory[j] = -1;
		
		for( j = 0; j < MAX_DEPOT_SIZE; j ++ )
			g_cgv.savedData[i].depot[j] = -1;
		
		for( j = 0; j < MAX_EQUIPMENT; j ++ )
			g_cgv.savedData[i].equipment[j] = -1;

		
		for( j = 0; j < MAX_PRECOCITY_SIZE; j ++ )
			g_cgv.savedData[i].precocityInventory[j] = -1;

		for( j = 0; j < MAX_BOOTH_INVENTORY_SIZE; j ++ )
			g_cgv.savedData[i].boothInventory[j].itemIdx = -1;

		for( j = 0; j < MAX_UPGRADE_SOURCE_ITEM_SIZE; j ++ )
			g_cgv.savedData[i].upgradeSourceInventory[j] = -1;

		for( j = 0; j < MAX_CRAFT_SOURCE_ITEM_SIZE; j ++ )
			g_cgv.savedData[i].craftSourceInventory[j] = -1;

		for( j = 0; j < MAX_TRADE_SIZE; j ++ )
			g_GonryunBattlePractic.m_imyTradetemp[j] = -1;
		
				
		g_cgv.savedData[i].mouseInventory = -1;

		
		for (j = 0; j < MAX_NUMBER_OF_OWNITEM; j++)
		{
			g_cgv.savedData[i].item[j].itemTableIdx = -1;
			for (k=0; k<MAX_NUMBER_OF_ITEM_OPTION; k++)
			{
				g_cgv.savedData[i].item[j].optionIdx[k] = -1;
			}
		}
		
		for( j = 0; j < MAX_TRADE_SIZE; j ++ )
		{
			g_cgv.savedData[i].myTrade[j] = -1;
			g_cgv.savedData[i].otherTrade[j] = -1;
			g_cgv.savedData[i].tradeItem[j].itemTableIdx = -1;
		    
			
			
			
			
			for (k=0; k<MAX_NUMBER_OF_ITEM_OPTION; k++)
			{
				g_cgv.savedData[i].tradeItem[j].optionIdx[k] = -1;
			}
		}
		
		
		
		memset( &g_cgv.savedData[i].organizerName, 0, sizeof( g_cgv.savedData[i].organizerName ) );
		g_cgv.savedData[i].organizeServer = -1;
		g_cgv.savedData[i].organizeTime = -1;
		g_cgv.savedData[i].partyIndex = -1;
		g_cgv.savedData[i].memberNumber = 0;
		g_cgv.savedData[i].itemDivideType = 0;
	    
		g_cgv.savedData[i].m_bischange = false;

		g_cgv.savedData[i].canJoin = true;
		g_cgv.savedData[i].isLeader = false;
		
		for( j = 0; j < MAX_NUMBER_OF_PARTY_MEMBER; j ++ )
		{
			memset( &g_cgv.savedData[i].member[j], 0, sizeof( partyMember_t ) );
			
			g_cgv.savedData[i].member[j].serverIndex = -1;
		}
		
		
		g_cgv.savedData[i].skillNumber = 0;
		for( j = 0; j < MAX_NUMBER_OF_SKILL; j ++ )
		{
			g_cgv.savedData[i].skill[j].idx = j;
			g_cgv.savedData[i].skill[j].tableIdx = -1;
			g_cgv.savedData[i].skill[j].iLevel = 0;
			g_cgv.savedData[i].skillCurCooltime[j] = 0.0f;
			g_cgv.savedData[i].skillMaxCooltime[j] = 0.0f;
		}
		g_cgv.savedData[i].skillCastingtime = 0.0f;
		g_cgv.savedData[i].skillCastingState = true;
		for (j=0; j<MAX_NUMBER_OF_SKILL_VARIABLE; j++)
			g_cgv.savedData[i].skillVariable[j] = -1;

		for( j = 0; j < MAX_AFFECT; j ++ )
		{
			memset( &g_cgv.savedData[i].affect[j], 0, sizeof( affect_t ) );
			g_cgv.savedData[i].affect[j].type = -1;
		}
		
		g_cgv.savedData[i].targetingType = TARGETING_NORMAL;
	}
}

void GTH_EnableMove()
{
	g_cgv.characterMoveFlag = true;
}

int GTH_CheckMoveable()
{

	return( 1 );
}

void GTH_DisableMove()
{
	g_cgv.characterMoveFlag = false;
#ifdef _GTH_ONLINE_VERSION
	g_pApp->m_myCharacter->targetIdx = -1;
	g_pApp->m_myCharacter->targetType = -1;
#endif
	g_cgv.pTargetChar = NULL;
}

int GTH_CanMove()
{
	
	if( g_ifMng->CheckConditionalWindow() ) return 0;
	
	
	if( g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_STAND || 
		g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_SIT ||
		g_pApp->m_myCharacter->event == GTH_EV_CHAR_SIT ||
		g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_SKILLCASTING ||
		g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_CASTINGIDLE ||
		
		g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_SOCIAL_SHOUT ||
		g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_SOCIAL_BOW ||
		
		g_pApp->m_myCharacter->ani_curSetType == ANIMTYPE_BYSKILL) return 0;

	if( g_input->GetMouseMode() == INPUT_MOUSE_INTERFACE_MODE ) return 0;

	return 1;
}


int GTH_SaveOptionToFile()
{
	FILE *file;

	char *buffer;
	int size, ret;
	
	file = fopen( "..\\optionconfig.cfg", "wb" );

	if( file == NULL )
	{

		return 0;
	}

	buffer = (char *)&g_ifMng->m_dlgWin->m_checkViewType[0][0];
	size = sizeof( g_ifMng->m_dlgWin->m_checkViewType );

	ret = fwrite( buffer, size, 1, file );

	g_ifMng->m_settingWin->GetOptionSet();

	buffer = (char *)&g_ifMng->m_settingWin->m_optionSetInfo;
	size = sizeof( g_ifMng->m_settingWin->m_optionSetInfo );

	ret = fwrite( buffer, size, 1, file );
	
	fclose( file );

	return 1;
}

int GTH_LoadOptionFromFile()
{
	FILE *file;

	char buffer[128];
	int size, ret;

	memset( buffer, 0, sizeof( buffer ) );

	file = fopen( "..\\optionconfig.cfg", "rb" );

	if( file == NULL )
	{

		return 0;
	}

	size = sizeof( g_ifMng->m_dlgWin->m_checkViewType );
	ret = fread( buffer, size, 1, file );

	if( ret == 1 )
	{
		memcpy( &g_ifMng->m_dlgWin->m_checkViewType[0][0], buffer, size );
	}

	size = sizeof( g_ifMng->m_settingWin->m_optionSetInfo );
	ret = fread( buffer, size, 1, file );

	if( ret == 1 )
	{
		memcpy( &g_ifMng->m_settingWin->m_optionSetInfo, buffer, size );
		g_ifMng->m_settingWin->SetOptionSet();
	}

	fclose( file );

	return 1;
}


int GTH_CanOpenBooth()
{
	if( g_ifMng->CheckConditionalWindow() ) return 0;
	
	
	if ( (g_pApp->m_myCharacter->ani_curSetID != ANIM_ITEM_SIT) &&
		(g_pApp->m_myCharacter->event != GTH_EV_CHAR_SIT) )
		return 0;

	return 1;
}