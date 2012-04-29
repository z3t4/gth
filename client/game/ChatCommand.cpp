#include "../global_def.h"
#include "../game/GolryunBattle/GolryunBattle.h"
#include "../game/GolryunBattle/GolryunBattle_Command.h"

int g_saveGomonIndex = -1;




#define MAX_OF_CHEAT_LEVEL_ITEM			20
#define MAX_OF_CHEAT_LEVEL_SKILL		10
#define MAX_NUMBER_OF_CHEAT_LEVEL		1400



#include "..\GM(Game Master)\CGMCtrl.h"
extern CGMCtrl* gcpGMCtrl;

class CGameDirectorCheatCode
{
public:

	CGameDirectorCheatCode();
	virtual ~CGameDirectorCheatCode();

	struct cheatItemStruct
	{
		int			itemTableIdx;
		int			optionIdx[MAX_NUMBER_OF_ITEM_OPTION];
		int			optionValue[MAX_NUMBER_OF_ITEM_OPTION];
		int			optionNumber;
	};

	
	struct cheatSkillStruct
	{
		int			skillIdx;
		int			skillLevel;
	};

	struct cheatMonsterStruct
	{
		int			monsterTableIdx;
		int			AIType;
		int			level;
		int			maxRA;
		int			maxSA;
		int			recoveryRARate;
		int			recoverySARate;
		int			atkPhyMinDmg;
		int			atkPhyMaxDmg;
		int			atkMagMinDmg;
		int			atkMagMaxDmg;
		int			atkHitPower;
		int			atkDelay;
		int			atkLength;
		int			atkRange;
		float		atkCriticalPro;
		int			atkCriticalDmg;
		int			defPower;
		int			defAvoidPower;
		int			defPositiveRes;
		int			defNegativeRes;
		int			velocity;
		int			stunTime;
		int			exp;
		int			genExp;
		int			lootSE;
		int			pathFindingSize;
	};

	
	struct cheatLevelStruct
	{
		int					cheatLevelCode;
		cheatItemStruct		cheatItem[MAX_OF_CHEAT_LEVEL_ITEM];
		int					itemNumber;
		cheatSkillStruct	cheatSkill[MAX_OF_CHEAT_LEVEL_SKILL];
		int					skillNumber;
		int					cheatExp;
		int					cheatGenExp;
		int					cheatSe;
		int					awaken;
	};

	cheatItemStruct		item[256];
	cheatMonsterStruct	monster[256];

	
	cheatLevelStruct	level[MAX_NUMBER_OF_CHEAT_LEVEL];

	bool		LoadItemCheatData();
	bool		LoadMonsterCheatData();
	
	bool		LoadLevelCheatData();
	
	void		SendMessageItemCheat( int idx );
	void		SendMessageMonsterCheat( int idx );
	
	void		SendMessageLevelCheat( int idx );
	void		SendMessageLevelItemCheat( int idx );
	void		SendMessageLevelSkillCheat( int idx );

	void		SendMessageAtkDelayCheat( int delay );
	
	void		SendMessageItemDropTest( int monsterIdx, long killNumber );

	
	void		SendMessageMonsterSpawnCheat(int monsteridx, int AI_IDX);
	
	void		SendMessageMonsterKill(int range);
	
	void		SendMessageAddQuestItem(int tableidx);

	
	void		SendMessageOutputGuildinfo(void);	

	
	void		SendMessageGetMemberShipType(void);	

	
	void		SendMessageHideName(int show);

	void		SendMessageGetDanBattlePcPoint();

	

	
	

};

CGameDirectorCheatCode::CGameDirectorCheatCode()
{
	for( int i = 0; i < 256; i ++ )
	{
		ZeroMemory( &item[i], sizeof( cheatItemStruct ) );
		ZeroMemory( &monster[i], sizeof( cheatMonsterStruct ) );
		item[i].itemTableIdx = -1;
		monster[i].monsterTableIdx = -1;
	}

	
	for( i=0; i<MAX_NUMBER_OF_CHEAT_LEVEL; i++ )
	{
		ZeroMemory( &level[i], sizeof( cheatLevelStruct ) );

		level[i].cheatLevelCode = -1;
		level[i].itemNumber = 0;
		level[i].skillNumber = 0;
		level[i].awaken = -1;

		for( int j=0; j<MAX_OF_CHEAT_LEVEL_ITEM; j++ )
		{
			level[i].cheatItem[j].itemTableIdx = -1;
		}
	}
}

CGameDirectorCheatCode::~CGameDirectorCheatCode()
{
}

bool CGameDirectorCheatCode::LoadItemCheatData()
{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return false;
	//end
	if( !BeginTokenScript( "../gdcitem.txt" ) ) return false;

	char *token;
	cheatItemStruct *pItem;

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;
		if( !stricmp( token, "cheat_item" ) )
		{
			pItem = &item[atoi( NextToken() )];
			
			pItem->optionIdx[0] = -1;
			pItem->optionIdx[1] = -1;
			pItem->optionIdx[2] = -1;
			pItem->optionIdx[3] = -1;
			pItem->optionIdx[4] = -1;
			pItem->optionIdx[5] = -1;
		}
		else if( token[0] == '{' )
		{
			int optionIdx = 0;
			while( 1 )
			{
				token = NextToken();
				if( token == NULL ) break;
				if( token[0] == '}' ) break;
				else if( !stricmp( token, "tableIdx" ) )
				{
					pItem->itemTableIdx = atoi( NextToken() );
				}
				else if( !stricmp( token, "optionIdx" ) && optionIdx < MAX_NUMBER_OF_ITEM_OPTION )
				{
					pItem->optionIdx[optionIdx] = atoi( NextToken() );
					pItem->optionValue[optionIdx] = atoi( NextToken() );
					optionIdx ++;
					pItem->optionNumber = optionIdx;
				}

			}
		}
	}

	EndTokenScript();
	
	return true;
}

bool CGameDirectorCheatCode::LoadMonsterCheatData()
{
		//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return false;
	//end
	if( !BeginTokenScript( "../gdcmonster.txt" ) ) return false;

	char *token;
	cheatMonsterStruct *pMon;

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;
		if( !stricmp( token, "cheat_monster" ) )
		{
			pMon = &monster[atoi( NextToken() )];
		}
		else if( token[0] == '{' )
		{
			int optionIdx = 0;
			while( 1 )
			{
				token = NextToken();
				if( token == NULL ) break;
				if( token[0] == '}' ) break;
				else if( !stricmp( token, "tableIdx" ) )
				{
					pMon->monsterTableIdx = atoi( NextToken() );
				}
				else if( !stricmp( token, "AIType" ) )
				{
					pMon->AIType = atoi( NextToken() );
				}
				else if( !stricmp( token, "level" ) )
				{
					pMon->level = atoi( NextToken() );
				}
				else if( !stricmp( token, "maxRA" ) )
				{
					pMon->maxRA = atoi( NextToken() );
				}
				else if( !stricmp( token, "maxSA" ) )
				{
					pMon->maxSA = atoi( NextToken() );
				}
				else if( !stricmp( token, "recoveryRARate" ) )
				{
					pMon->recoveryRARate = atoi( NextToken() );
				}
				else if( !stricmp( token, "recoverySARate" ) )
				{
					pMon->recoverySARate = atoi( NextToken() );
				}
				else if( !stricmp( token, "atkPhyMinDmg" ) )
				{
					pMon->atkPhyMinDmg = atoi( NextToken() );
				}
				else if( !stricmp( token, "atkPhyMaxDmg" ) )
				{
					pMon->atkPhyMaxDmg = atoi( NextToken() );
				}
				else if( !stricmp( token, "atkMagMinDmg" ) )
				{
					pMon->atkMagMinDmg = atoi( NextToken() );
				}
				else if( !stricmp( token, "atkMagMaxDmg" ) )
				{
					pMon->atkMagMaxDmg = atoi( NextToken() );
				}
				else if( !stricmp( token, "atkHitPower" ) )
				{
					pMon->atkHitPower = atoi( NextToken() );
				}
				else if( !stricmp( token, "atkDelay" ) )
				{
					pMon->atkDelay = atoi( NextToken() );
				}
				else if( !stricmp( token, "atkLength" ) )
				{
					pMon->atkLength = atoi( NextToken() );
				}
				else if( !stricmp( token, "atkRange" ) )
				{
					pMon->atkRange = atoi( NextToken() );
				}
				else if( !stricmp( token, "atkCriticalPro" ) )
				{
					pMon->atkCriticalPro = atof( NextToken() );
				}
				else if( !stricmp( token, "atkCriticalDmg" ) )
				{
					pMon->atkCriticalDmg = atoi( NextToken() );
				}
				else if( !stricmp( token, "defPower" ) )
				{
					pMon->defPower = atoi( NextToken() );
				}
				else if( !stricmp( token, "defAvoidPower" ) )
				{
					pMon->defAvoidPower = atoi( NextToken() );
				}
				else if( !stricmp( token, "defPositiveRes" ) )
				{
					pMon->defPositiveRes = atoi( NextToken() );
				}
				else if( !stricmp( token, "defNegativeRes" ) )
				{
					pMon->defNegativeRes = atoi( NextToken() );
				}
				else if( !stricmp( token, "velocity" ) )
				{
					pMon->velocity = atoi( NextToken() );
				}
				else if( !stricmp( token, "stunTime" ) )
				{
					pMon->stunTime = atoi( NextToken() );
				}
				else if( !stricmp( token, "exp" ) )
				{
					pMon->exp = atoi( NextToken() );
				}
				else if( !stricmp( token, "genExp" ) )
				{
					pMon->genExp = atoi( NextToken() );
				}
				else if( !stricmp( token, "lootSE" ) )
				{
					pMon->lootSE = atoi( NextToken() );
				}
				else if( !stricmp( token, "pathFindingSize" ) )
				{
					pMon->pathFindingSize = atoi( NextToken() );
				}
			}
		}
	}

	EndTokenScript();
	
	return true;
}


bool CGameDirectorCheatCode::LoadLevelCheatData()
{
		//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return false;
	//end
	if( !BeginTokenScript( "../gdclevel.txt" ) ) return false;

	int levelIdx = 0;
	char *token;
	cheatLevelStruct *pLevel;

	while( 1 )
	{
		token = NextToken();
		if( token == NULL ) break;
		if( !stricmp( token, "cheat_level" ) )
		{
			token = NextToken();

			pLevel = &level[levelIdx];
			pLevel->cheatLevelCode = atoi( token );
		}
		else if( token[0] == '{' )
		{
			int optionIdx, skillIdx;
			int itemIdx = -1;		

			while( 1 )
			{
				token = NextToken();

				if( token == NULL ) return false;
				if( token[0] == '}' ) break;

				else if( !stricmp( token, "tableIdx" ) )
				{
					itemIdx++;
					optionIdx = 0;
					skillIdx = 0;
					pLevel->cheatItem[itemIdx].itemTableIdx = atoi( NextToken() );
					pLevel->itemNumber = itemIdx + 1;

					pLevel->cheatItem[itemIdx].optionIdx[0] = -1;
					pLevel->cheatItem[itemIdx].optionIdx[1] = -1;
					pLevel->cheatItem[itemIdx].optionIdx[2] = -1;
					pLevel->cheatItem[itemIdx].optionIdx[3] = -1;
					pLevel->cheatItem[itemIdx].optionIdx[4] = -1;
					pLevel->cheatItem[itemIdx].optionIdx[5] = -1;

				}
				else if( !stricmp( token, "optionIdx" ) && optionIdx < MAX_NUMBER_OF_ITEM_OPTION )
				{
					pLevel->cheatItem[itemIdx].optionIdx[optionIdx] = atoi( NextToken() );
					pLevel->cheatItem[itemIdx].optionValue[optionIdx] = atoi( NextToken() );
					optionIdx ++;
					pLevel->cheatItem[itemIdx].optionNumber = optionIdx;
				}
				else if( !stricmp( token, "skillIdx" ) )
				{
					pLevel->cheatSkill[skillIdx].skillIdx = atoi( NextToken() );
					pLevel->cheatSkill[skillIdx].skillLevel = atoi( NextToken() );
					pLevel->skillNumber = skillIdx + 1;

					skillIdx++;
				}
				else if( !stricmp( token, "exp" ) )
				{
					pLevel->cheatExp = atoi( NextToken() );
				}
				else if( !stricmp( token, "genexp" ) )
				{
					pLevel->cheatGenExp = atoi( NextToken() );
				}
				else if( !stricmp( token, "se" ) )
				{
					pLevel->cheatSe = atoi( NextToken() );
				}
				else if( !stricmp( token, "awaken" ) )
				{
					pLevel->awaken = atoi( NextToken() );
				}
			}
			levelIdx++;
		}
	}

	EndTokenScript();
	
	return true;
}

void CGameDirectorCheatCode::SendMessageItemCheat( int idx )
{
	cheatItemStruct *pItem = &item[idx];

	if( pItem->itemTableIdx < 0 ) return;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE );
		MSG_WriteByte(&netMessage, CHEATCODE_ITEM );
		
		MSG_WriteShort( &netMessage, pItem->itemTableIdx );
		MSG_WriteByte( &netMessage, pItem->optionNumber );
		
		for( int i = 0; i < pItem->optionNumber; i ++ )
		{
			MSG_WriteShort( &netMessage, pItem->optionIdx[i] );
			MSG_WriteShort( &netMessage, pItem->optionValue[i] );
		}
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGameDirectorCheatCode::SendMessageMonsterCheat( int idx )
{
	cheatMonsterStruct *pMon = &monster[idx];
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
	if( pMon->monsterTableIdx < 0 ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE );
		MSG_WriteByte(&netMessage, CHEATCODE_MONSTER );
		
		MSG_WriteShort( &netMessage, pMon->monsterTableIdx );
		MSG_WriteShort( &netMessage, pMon->AIType );
		MSG_WriteShort( &netMessage, pMon->level );
		MSG_WriteShort( &netMessage, pMon->maxRA );
		MSG_WriteShort( &netMessage, pMon->maxSA );
		MSG_WriteShort( &netMessage, pMon->recoveryRARate );
		MSG_WriteShort( &netMessage, pMon->recoverySARate );
		MSG_WriteShort( &netMessage, pMon->atkPhyMinDmg );
		MSG_WriteShort( &netMessage, pMon->atkPhyMaxDmg );
		MSG_WriteShort( &netMessage, pMon->atkMagMinDmg );
		MSG_WriteShort( &netMessage, pMon->atkMagMaxDmg );
		MSG_WriteShort( &netMessage, pMon->atkHitPower );
		MSG_WriteShort( &netMessage, pMon->atkDelay );
		MSG_WriteShort( &netMessage, pMon->atkLength );
		MSG_WriteShort( &netMessage, pMon->atkRange );
		MSG_WriteFloat( &netMessage, pMon->atkCriticalPro );
		MSG_WriteShort( &netMessage, pMon->atkCriticalDmg );
		MSG_WriteShort( &netMessage, pMon->defPower );
		MSG_WriteShort( &netMessage, pMon->defAvoidPower );
		MSG_WriteShort( &netMessage, pMon->defPositiveRes );
		MSG_WriteShort( &netMessage, pMon->defNegativeRes );
		MSG_WriteShort( &netMessage, pMon->velocity );
		MSG_WriteShort( &netMessage, pMon->stunTime );
		MSG_WriteShort( &netMessage, pMon->exp );
		MSG_WriteShort( &netMessage, pMon->genExp );
		MSG_WriteShort( &netMessage, pMon->lootSE );
		MSG_WriteShort( &netMessage, pMon->pathFindingSize );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void CGameDirectorCheatCode::SendMessageAtkDelayCheat( int delay )
{
		//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE );
		MSG_WriteByte(&netMessage, CHEATCODE_ATKDELAY );
		MSG_WriteShort( &netMessage, delay );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGameDirectorCheatCode::SendMessageLevelCheat( int idx )
{
		//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
	cheatLevelStruct *pLevel;

	for( int i=0; i<MAX_NUMBER_OF_CHEAT_LEVEL; i++ )
	{
		if ( level[i].cheatLevelCode == idx )
		{
			pLevel = &level[i];
			break;
		}
	}

	if ( i == MAX_NUMBER_OF_CHEAT_LEVEL )
	{
		g_ifMng->AddSysMessage( "Cheat level idx is invalid!" );
		return;
	}

	
	for( i=0; i<MAX_NUMBER_OF_SKILL; i++ )
	{
		g_cgv.myCharacterInfo->skill[i].idx = -1;
		g_cgv.myCharacterInfo->skill[i].tableIdx = -1;
		g_cgv.myCharacterInfo->skill[i].iLevel = 0;
	}
	g_cgv.myCharacterInfo->skillNumber = 0;
	
	for( i=0; i<MAX_NUMBER_OF_HOTKEY; i++ )
	{
		g_cgv.myCharacterInfo->hotkeyIdx[i] = -1;
	}

	
	if( pLevel->awaken<0 )
		g_cgv.myCharacterInfo->pcJob = g_cgv.myCharacterInfo->pcClass;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE );
		MSG_WriteByte(&netMessage, CHEATCODE_LEVEL);
		
		MSG_WriteLong( &netMessage, pLevel->cheatExp );
		MSG_WriteLong( &netMessage, pLevel->cheatGenExp );
		MSG_WriteLong( &netMessage, pLevel->cheatSe );
		
		
		MSG_WriteShort( &netMessage, pLevel->awaken );
		MSG_WriteShort( &netMessage, g_cgv.myCharacterInfo->pcJob );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

}

void CGameDirectorCheatCode::SendMessageLevelSkillCheat( int idx )	
{
		//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
	cheatLevelStruct *pLevel;
	cheatSkillStruct *pSkill;

	for( int i=0; i<MAX_NUMBER_OF_CHEAT_LEVEL; i++ )
	{
		if ( level[i].cheatLevelCode == idx )
		{
			pLevel = &level[i];
			break;
		}
	}

	if ( i == MAX_NUMBER_OF_CHEAT_LEVEL )
	{
		return;
	}

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE);
		MSG_WriteByte(&netMessage, CHEATCODE_SKILL);
		
		MSG_WriteShort(&netMessage, pLevel->skillNumber);
		
		for( i = 0; i<pLevel->skillNumber; i++ )
		{
			pSkill = &pLevel->cheatSkill[i];
			
			MSG_WriteShort(&netMessage, pSkill->skillIdx);
			MSG_WriteShort(&netMessage, pSkill->skillLevel);
		}
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGameDirectorCheatCode::SendMessageLevelItemCheat( int idx )
{
		//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
	cheatLevelStruct *pLevel;
	cheatItemStruct  *pItem;

	for( int i=0; i<MAX_NUMBER_OF_CHEAT_LEVEL; i++ )
	{
		if ( level[i].cheatLevelCode == idx )
		{
			pLevel = &level[i];
			break;
		}
	}

	if ( i == MAX_NUMBER_OF_CHEAT_LEVEL )	return;

	
	for( i = 0; i<pLevel->itemNumber; i++ )
	{
		pItem = &pLevel->cheatItem[i];

		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE );
			MSG_WriteByte(&netMessage, CHEATCODE_ITEM );
			
			MSG_WriteShort( &netMessage, pItem->itemTableIdx );
			MSG_WriteByte( &netMessage, pItem->optionNumber );
			
			for( int j = 0; j < pItem->optionNumber; j ++ )
			{
				MSG_WriteShort( &netMessage, pItem->optionIdx[j] );
				MSG_WriteShort( &netMessage, pItem->optionValue[j] );
			}
			
			NET_SendMessage(&gsSocket, &netMessage);
		}
		MSG_EndWriting( &netMessage );
	}
}


void CGameDirectorCheatCode::SendMessageItemDropTest( int monsterIdx, long killNumber )
{
		//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE );
		MSG_WriteByte(&netMessage, CHEATCODE_ITEMDROPTEST );
		
		MSG_WriteShort(&netMessage, monsterIdx);
		MSG_WriteLong(&netMessage, killNumber);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	
	char szTemp[128];
	sprintf( szTemp, "%s(Index:%d) : %d", g_monsterTable[monsterIdx].name, monsterIdx, killNumber );
	g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
}


CGameDirectorCheatCode		GAMEDirectorCheat;








void GTH_Cheat_ForceQuitByGM(const char* pName)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE );
		MSG_WriteByte(&netMessage, CHEATCODE_FORCEQUIT_BY_GM);		
		MSG_WriteString(&netMessage,(char*)pName);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}




int GTH_CheckChatCommandState()
{
	if( !g_pApp->m_myCharacter->isAlive ) return( false );
	if( g_pApp->m_myCharacter->event == GTH_EV_CHAR_DIE ) return( false );

	return( true );
}

void GTH_RunGMChatCommand( char message[], char command[], chatMessage param[] )
{
	int intparam;
	char szTemp[256];



	if(TRUE == gcpGMCtrl->RunGMChatCommand(message,command,param)) return;
	



	
	if( !stricmp( command, "danbattleend"))
	{				
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GTH_SendMessage_ChangeStatus(
			NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_END, 
			param[0]);		
	}

	
	if ( !stricmp( command, g_LPACK.GetMassage( 0, 433 ) ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GTH_SendMessage_SystemAdimin_ShutdownServer();
	}
	
	if ( !stricmp( command, g_LPACK.GetMassage( 0, 434 ) ) || !stricmp( command, "goto" )) 
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		if( param[0][0] == 0 ) return;
		GTH_SendMessage_SystemAdimin_GoToPlayer( param[0] );
	}

	
	if ( !stricmp( command, g_LPACK.GetMassage( 0, 435 )) ) 
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		if( param[0][0] == 0 ) return;
		GTH_SendMessage_SystemAdimin_Notice( param[0] );
	}
	
	
	
	if ( !stricmp( command, g_LPACK.GetMassage( 0, 436 ) ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		if( param[0][0] == 0 ) return;
		GTH_SendMessage_RequestSummons( param[0], 0 );
	}
	
	if ( !stricmp( command, g_LPACK.GetMassage( 0, 437 ) ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		if( param[0][0] == 0 ) return;
		GTH_SendMessage_RequestSummons( param[0], 1 );
	}
	if( !stricmp( command, "monsterkill" ) )
	{		
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		GAMEDirectorCheat.SendMessageMonsterKill( atoi(param[0]) );
	}
	
	if( !stricmp( command, "MonsterSpawn" ) )
	{
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;		
		GAMEDirectorCheat.SendMessageMonsterSpawnCheat( atoi(param[0]), atoi (param[1]) );
	}

	if( !stricmp( command, "ChristMasEvent" ) )
	{
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		g_ChristMasEvent.GTH_SendMessageEventSeting(atoi(param[0]));
	}

	if( !stricmp( command, "ForceQuit" ) )
	{
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;

		GTH_Cheat_ForceQuitByGM(param[0]);
	}
	
	if( stricmp( command, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 450) ) == 0 ||
		stricmp( command, "eventstart" ) == 0)	
	{		
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		g_ExpRateMng.SendMessage_SetEventActive( 1 );
	}	
	if( stricmp( command, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 451) ) == 0 ||
		stricmp( command, "eventend" ) == 0)	
	{		
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		g_ExpRateMng.SendMessage_SetEventActive(0);
	}	
	
	if( stricmp( command,  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 452)) == 0 ||
		stricmp( command, "expevent" ) == 0)
	{		
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		g_ExpRateMng.SendMessage_SetEvent( g_ExpRateMng.EXP_RATE, atof( param[0] ));
	}
	if( stricmp( command, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 453) ) == 0 ||
		stricmp( command, "nakevent" ) == 0)
	{
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		g_ExpRateMng.SendMessage_SetEvent( g_ExpRateMng.NAK_DROP_RATE, atof( param[0] ));
	}
	if( stricmp( command, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 454) ) == 0 ||
		stricmp( command, "itemevent" ) == 0)	
	{
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		g_ExpRateMng.SendMessage_SetEvent( g_ExpRateMng.ITEM_DROP_RATE, atof( param[0] ));
	}
	
	if( stricmp( command, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 455)) == 0 ||
		stricmp( command, "gencapevent" ) == 0)
	{
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		g_ExpRateMng.SendMessage_SetEvent( g_ExpRateMng.GENCAPABLITY_RATE, atof( param[0] ));
	}
	
	if( stricmp( command, "allreadlogic" ) == 0)
	{
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		g_ExpRateMng.SendMessage_SetEvent( g_ExpRateMng.ALL_READ_LOGIC, 1.0f );
	}

	if( stricmp( command, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 456) ) == 0 ||
		stricmp( command, "eventcheck" ) == 0)
	{
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		g_ExpRateMng.SendMessage_RequestEventStatus();
	}	


	if( !stricmp( command, "겜블시스템시작") ||
		!stricmp( command, "gamblesystemstart"))													
	{
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		GTH_SendMessage_GambleSystem_Active(1);
	}

	if( !stricmp( command, "겜블시스템중지") ||
		!stricmp( command, "gamblesystemend"))													
	{
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		GTH_SendMessage_GambleSystem_Active(0);
	}


	if( !stricmp( command, "banbatting"))
	{
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		g_GonryunBattlePractic.SendBanBatting();
	}
	
	if( !stricmp( command, "cancelbatting"))
	{
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
		g_GonryunBattlePractic.SendCancelBatting();
	}
	//end
	if ( !stricmp( command, "gomon" ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end

		intparam = atoi( param[0] );
		if ( intparam < 0 ) return;
		g_saveGomonIndex = intparam;

		GTH_SendMessage_SystemAdimin_GoToMonster( intparam );
		sprintf( szTemp, "go to %d monster", g_saveGomonIndex );
		g_ifMng->AddSysMessage( szTemp );
	}
	if ( !stricmp( command, "nextmon" ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_saveGomonIndex ++;
		if( g_saveGomonIndex < 0 ) return;

		sprintf( szTemp, "go to %d monster", g_saveGomonIndex );
		g_ifMng->AddSysMessage( szTemp );
		GTH_SendMessage_SystemAdimin_GoToMonster( g_saveGomonIndex );
	}
	if ( !stricmp( command, "prevmon" ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_saveGomonIndex --;
		if( g_saveGomonIndex < 0 ) return;

		GTH_SendMessage_SystemAdimin_GoToMonster( g_saveGomonIndex );
		sprintf( szTemp, "go to %d monster", g_saveGomonIndex );
		g_ifMng->AddSysMessage( szTemp );
	}


	
	
	
	if( !stricmp( command, g_LPACK.GetMassage(LPACK_TYPE_NORMAL, 438 ) ) || !stricmp( command, "warpmap" ))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		
		if(g_SkillLogic.CancelTransform()) return ;

		
		
		if(true == g_ifMng->m_Item_ScrollWin->IfWorldChangeing())
		{	
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 588 ) );
			return;
		}

		intparam = atoi( param[0] );
		if( intparam < 0 || intparam >= g_cgv.worldTableNumber ) return;
		GTH_RequestChangeWorld( intparam, 0 );
		return;
	}

	
	if(!stricmp( command, "setra" ))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE);
			MSG_WriteByte(&netMessage, CHEATCODE_KILLME);		

			int ra = atoi(param[0]);
			MSG_WriteLong(&netMessage, ra);		
			NET_SendMessage(&gsSocket, &netMessage);
		}
		MSG_EndWriting( &netMessage );

		return;
	}
	
	if( !stricmp( command, "itemlist") )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		int start = atoi(param[0]);
		int end   = atoi(param[1]);

		if(end <= 0) end =start + 100;
		if(end >= MAX_NUMBER_OF_ITEM_TABLE) end = MAX_NUMBER_OF_ITEM_TABLE;

		g_ifMng->AddSysMessage( "===== itemlist ====",D3DCOLOR_ARGB( 255,255,0,255 ));

		for(int i=start;i<end;i++)
		{
			if(g_itemTable[i].idx < 0) continue;

			char szTemp[256];
			wsprintf(szTemp,"%d : %s",g_itemTable[i].idx, g_itemTable[i].name);
			g_ifMng->AddSysMessage( szTemp,D3DCOLOR_ARGB( 255,255,255,255 ));
		}
	} 

	
	if( !stricmp( command, "ssitem") )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		
		g_ifMng->AddSysMessage( "===== FindingItem ====",D3DCOLOR_ARGB( 255,255,0,255 ));

		for(int i=0;i<MAX_NUMBER_OF_ITEM_TABLE;i++)
		{
			if(g_itemTable[i].idx < 0) continue;
			char* fineitemname = NULL;
			fineitemname = strstr(g_itemTable[i].name, param[0]);
			if ( fineitemname == NULL)
				continue;

			char szTemp[256];
			wsprintf(szTemp,"%d : %s",g_itemTable[i].idx, g_itemTable[i].name);
			g_ifMng->AddSysMessage( szTemp,D3DCOLOR_ARGB( 255,255,255,255 ));
		}
	}
	
	
	
	
	if( !stricmp( command, "monsterlist") )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		int start = atoi(param[0]);
		int end   = atoi(param[1]);

		if(end <= 0) end =start + 100;
		if(end >= MAX_NUMBER_OF_MONSTER_TABLE) end = MAX_NUMBER_OF_MONSTER_TABLE;

		g_ifMng->AddSysMessage( "===== monsterlist ====",D3DCOLOR_ARGB( 255,255,0,255 ));

		for(int i=start;i<end;i++)
		{
			if(g_monsterTable[i].idx < 0) continue;

			char szTemp[256];
			wsprintf(szTemp,"%d : %s",g_monsterTable[i].idx, g_monsterTable[i].name);
			g_ifMng->AddSysMessage( szTemp,D3DCOLOR_ARGB( 255,0,255,255 ));
		}
	}

	
	if( !stricmp( command, "maplist") )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		char szTemp[256];

		g_ifMng->AddSysMessage( "===== maplist ====",D3DCOLOR_ARGB( 255,255,0,255 ));

		for(int i=0;i<MAX_WORLD_NUMBER;i++)
		{
			if(strlen(g_cgv.worldTable[i].hanName)  <= 0) break;
			wsprintf(szTemp,"%d : %s",g_cgv.worldTable[i].idx, g_cgv.worldTable[i].hanName );
			g_ifMng->AddSysMessage( szTemp,D3DCOLOR_ARGB( 255,255,255,255 ));
		}
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( 0, 439 ) ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		if( param[0][0] == 0 ) return;
		g_ifMng->m_gmWin->AddCaller( param[0], g_LPACK.GetMassage( 0, 571 ) );
	}
	
	if( !stricmp( command, g_LPACK.GetMassage( 0, 440 ) ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
	}	
	if( !stricmp( command, "busy" ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_cgv.isGmBusy = ( g_cgv.isGmBusy + 1 ) % 2;
		GTH_SendMessage_SystemAdimin_Busy();
	}
	if( !stricmp( command, "hide" ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_cgv.isGmHide = ( g_cgv.isGmHide + 1 ) % 2;
		GTH_SendMessage_SystemAdimin_Hide();
	}
	
	if( !stricmp( command, g_LPACK.GetMassage( 0, 441 ) ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		if( param[0][0] == 0 ) return;
		GTH_SendChatMessage_Whisper( param[0], g_LPACK.GetMassage( 0, 441 ) );
	}
	
	
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 199)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_GonryunBattlePractic.SendGonryunBan();
	}
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 198)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_GonryunBattlePractic.SendGonryunStart(atoi(param[0]),atoi(param[1]));

	}



	
	
	
	
	

	g_GolryunBattle_Command.ProcessCommandToGM(message, command, param);

	
	if ( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 376)) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		if(NULL == param[0][0]) return;
		g_GolryunBattle.SendNoticeBattle(param[0]);	
	}
	
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 201)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		int intparam = atoi( param[0] );

		if(intparam<0 || intparam>4)
		{
			
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 379));
			return;
		}

		char szTemp[255];
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,205));
		g_GolryunBattle.SendRequestStartBattle(intparam);
		GTH_SendMessage_SystemAdimin_Notice( szTemp );

		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 206)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		int intparam = atoi( param[0] );

		switch(intparam)
		{
		case 30:
			g_GolryunBattle.SetTournamentIdx(0);
			break;
		case 40:
			g_GolryunBattle.SetTournamentIdx(1);
			break;
		case 50:
			g_GolryunBattle.SetTournamentIdx(2);
			break;
		case 60:
			g_GolryunBattle.SetTournamentIdx(3);
			break;
		case 70:
			g_GolryunBattle.SetTournamentIdx(4);
			break;
		case 80:
			g_GolryunBattle.SetTournamentIdx(5);
			break;
		default:
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 379) );
			return;
		}

		char szTemp[255];
		int cnt = g_GolryunBattle.GetTournamentIdx();
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,143), g_golryunManagerTable[cnt].levelMin);
		g_GolryunBattle.SendRequestStartJoin();
		g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_START_JOIN);
		GTH_SendMessage_SystemAdimin_Notice( szTemp );

		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 463)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		int intparam = atoi( param[0] );

		switch(intparam)
		{
		case 30:
			g_GolryunBattle.SetTournamentIdx(0);
			break;
		case 40:
			g_GolryunBattle.SetTournamentIdx(1);
			break;
		case 50:
			g_GolryunBattle.SetTournamentIdx(2);
			break;
		case 60:
			g_GolryunBattle.SetTournamentIdx(3);
			break;
		case 70:
			g_GolryunBattle.SetTournamentIdx(4);
			break;
		case 80:
			g_GolryunBattle.SetTournamentIdx(5);
			break;
		default:
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 379) );
			return;
		}

		char szTemp[255];
		int cnt = g_GolryunBattle.GetTournamentIdx();
		
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,464), g_golryunManagerTable[cnt].levelMin);
		g_GolryunBattle.SendRequestStartReCallFighter();
		GTH_SendMessage_SystemAdimin_Notice( szTemp );

		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 422)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle.SendRequestCloseJoin();
		g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_CLOSE_JOIN);
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 202)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle.SendRequestStartBetting();
		g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_START_BETTING);
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 203)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle.SendRequestReadyTournament();
		g_GolryunBattle.SetStatus(tagGolryun_Packet_Status::GB_REQUEST_READY_TOURNAMENT);
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 204)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle.SendRequestStartCount();

	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 207)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle.SendRequestPauseMatch();
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 467)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle.SendRequestReStartMatch();
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 208)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		char szTemp[255];
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,169));
		g_GolryunBattle.SendRequestCloseBattle();
		GTH_SendMessage_SystemAdimin_Notice( szTemp );
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 176)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
	
		if(g_pApp->m_myCharacter->worldIdx == CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX )
			g_GolryunBattle.SendTransWatchingRoom();
		else
			GTH_SendMessage_TransportWorld( CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX, 0 );
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 377)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
	
		if(g_pApp->m_myCharacter->worldIdx == CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX )
			g_GolryunBattle.SendTransWaitingRoom();
		else
			GTH_SendMessage_TransportWorld( CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX, 1 );
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 378)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
	
		if(g_pApp->m_myCharacter->worldIdx == CGolryunBattle::GOLRYUN_BATTLE_MAP_INDEX )
			g_GolryunBattle.SendTransBattleStadium();
		else
			return;
	
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 381)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		int intparam = atoi( param[0] );

		if(intparam<0 || intparam > CGolryunBattle::MAX_NUM_FIGHTER_PER_TOURNAMENT)
		{
			
			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 379) );
			return;
		}

		g_GolryunBattle.SendWalkOverMatch(intparam);
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 382)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle.SendCheckJoinerNum();
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 383)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		char szTemp[255];
		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,172));
		g_GolryunBattle.SendRequestStartPrize();
		GTH_SendMessage_SystemAdimin_Notice( szTemp );
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 384)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle.SendRequestCancelTournament();
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 421)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle.SendTwoGyunTotalNak();
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 465)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		if( param[0][0] == 0 ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle.SendInsertJoinList(param[0]);
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 466)))
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		if( param[0][0] == 0 ) return;
		g_ifMng->AddChatMessage( message, GTHCOLOR_SYSMESSAGE, 
			g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_SYSTEM );
		g_GolryunBattle.SendAssignFighterNumber( param[0], atoi(param[1]) );
	}






	
	


	if( !stricmp( command, "exp" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		intparam = atoi( param[0] );
		GTH_SendMessage_CheatCode( CHEATCODE_GETEXP, intparam );
	}
	if( !stricmp( command, "genexp" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		intparam = atoi( param[0] );
		GTH_SendMessage_CheatCode( CHEATCODE_GETGENEXP, intparam );
	}
	if( !stricmp( command, "skillpoint" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GTH_SendMessage_CheatCode( CHEATCODE_SKILLPOINT, 0 );
	}
	if( !stricmp( command, "getskill" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		intparam = atoi( param[0] );
		GTH_SendMessage_CheatCode( CHEATCODE_GETSKILL, intparam);
	}
	if( !stricmp( command, "maxse" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		intparam = atoi( param[0] );
		GTH_SendMessage_CheatCode( CHEATCODE_GETMAXSE, intparam);
	}
	if( !stricmp( command, "clearquest" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		intparam = atoi( param[0] );
		GTH_SendMessage_CheatCode( CHEATCODE_CLEARQUEST, intparam );
	}
	if( !stricmp( command, "additem" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		intparam = atoi( param[0] );
		if( intparam == QUESTSCROLL_ITEM_INDEX || intparam == SUMMONCARD_ITEM_INDEX)
			GTH_SendMessage_CheatCode( CHEATCODE_ADDITEM, intparam, atoi( param[1] ) );
		else
			GTH_SendMessage_CheatCode( CHEATCODE_ADDITEM, intparam );
	}
	if( !stricmp( command, "gencap" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		intparam = atoi( param[0] );
		GTH_SendMessage_CheatCode( CHEATCODE_GETGENCAPABILITY, intparam );
	}
	if( !stricmp( command, "rasa" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GTH_SendMessage_CheatCode( CHEATCODE_MAXRASA, intparam );
	}
	if( !stricmp( command, "awaken" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		intparam = atoi( param[0] );
		GTH_SendMessage_CheatCode( CHEATCODE_AWAKEN, intparam );
	}
	if ( !stricmp( command, "gom" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		intparam = atoi( param[0] );
		if ( intparam < 0 ) return;
		g_saveGomonIndex = intparam;

		GTH_SendMessage_CheatCode( CHEATCODE_GOMONSTER, intparam );


	}
	if ( !stricmp( command, "nextm" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_saveGomonIndex ++;
		if( g_saveGomonIndex < 0 ) return;

		sprintf( szTemp, "go to %d monster", g_saveGomonIndex );
		g_ifMng->AddSysMessage( szTemp );
		GTH_SendMessage_CheatCode( CHEATCODE_GOMONSTER, g_saveGomonIndex );
	}
	if ( !stricmp( command, "prevm" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_saveGomonIndex --;
		if( g_saveGomonIndex < 0 ) return;

		GTH_SendMessage_CheatCode( CHEATCODE_GOMONSTER, g_saveGomonIndex );
		sprintf( szTemp, "go to %d monster", g_saveGomonIndex );
		g_ifMng->AddSysMessage( szTemp );
	}
	if ( !stricmp( command, "readyw" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GTH_SendMessage_CheatCode( CHEATCODE_READYAWAKEN, 0 );
	}
	

	if ( !stricmp( command, "loadgdc" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		if( !GAMEDirectorCheat.LoadItemCheatData() ) g_ifMng->AddSysMessage( "gdcitem.txt file not found!" );
		else g_ifMng->AddSysMessage( "gdcitem.txt load complete!" );
		if( !GAMEDirectorCheat.LoadMonsterCheatData() ) g_ifMng->AddSysMessage( "gdcmonster.txt file not found!" );
		else g_ifMng->AddSysMessage( "gdcmonster.txt load complete!" );
		if( !GAMEDirectorCheat.LoadLevelCheatData() ) g_ifMng->AddSysMessage( "gdclevel.txt file not found!" );
		else g_ifMng->AddSysMessage( "gdclevel.txt load complete!" );
		
		
		
		if ( atoi( param[0] ) <= 0 ) return;
		GAMEDirectorCheat.SendMessageLevelCheat( atoi( param[0] ) );
		GAMEDirectorCheat.SendMessageLevelItemCheat( atoi( param[0] ) );
		GAMEDirectorCheat.SendMessageLevelSkillCheat( atoi( param[0] ) );
		
		
	}
	if ( !stricmp( command, "gdcitem" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GAMEDirectorCheat.SendMessageItemCheat( atoi( param[0] ) );
	}
	if ( !stricmp( command, "gdcmon" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GAMEDirectorCheat.SendMessageMonsterCheat( atoi( param[0] ) );
	}
	
	if( !stricmp( command, "gdcatkdelay" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GAMEDirectorCheat.SendMessageAtkDelayCheat( atoi( param[0] ) );
		if( atoi( param[0] ) < 0 ) g_ifMng->AddSysMessage( "atk delay forced setting off" );
		else g_ifMng->AddSysMessage( "atk delay forced setting on" );
	}

	
	if( !stricmp( command, "gdclevel" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GAMEDirectorCheat.SendMessageLevelCheat( atoi( param[0] ) );
	}
	if( !stricmp( command, "gdclevelitem" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GAMEDirectorCheat.SendMessageLevelItemCheat( atoi( param[0] ) );
	}
	if( !stricmp( command, "gdclevelskill" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GAMEDirectorCheat.SendMessageLevelSkillCheat( atoi( param[0] ) );
	}

	
	if( !stricmp( command, "itemdroptest" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GAMEDirectorCheat.SendMessageItemDropTest( atoi(param[0]), atol(param[1]) );
	}

	





	
	if( !stricmp( command, "cooltime" ) )
	{	
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		for (int i = 0; i < MAX_NUMBER_OF_SKILL; i++)
		g_cgv.myCharacterInfo->skillCurCooltime[i] = 0.0f;				
	}
	










	
	
	if( !stricmp( command, "addquestitem" ) )
	{	
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GAMEDirectorCheat.SendMessageAddQuestItem( atoi(param[0]) );
	}
	

	
	if( !stricmp( command, "outputguildinfo" ) )
	{	
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GAMEDirectorCheat.SendMessageOutputGuildinfo();
	}
	

	
	if( !stricmp( command, "getmembershiptype" ) )
	{	
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		GAMEDirectorCheat.SendMessageGetMemberShipType();
	}
	
	

	
	if( !stricmp( command, "setweathertype" ) )
	{
			//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end	
		int weatheridx = atoi(param[0]);
		if ( weatheridx <0 || weatheridx >= TIME_TYPE_NUM)
			weatheridx = 0;

		float limittime = atof(param[1]) * 1000;
		if ( limittime < 0 )
			limittime = CWeatherSystem::enumProgressLimitTime::DEFAULT_LIMIT_TIME;		

		g_WeatherSystem.GTH_SendMessage_Request_WeatherSetting((TIMESTATE)weatheridx, limittime);
	}	
	



	if( !stricmp( command, "showdebuginfo" ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end	
		g_cgv.displayDebugMessage = !g_cgv.displayDebugMessage;
	}


	if ( !stricmp( command, "단전맵이벤트시작" ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_ifMng->AddSysMessage("단전맵이 이벤트 모드로 셋팅되었습니다. 이벤트기간동안 단전은 불가능합니다.");
		g_DanBattle.SendSetStatus(7);
	}

	
	if ( !stricmp( command, "단전맵이벤트종료" ) )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		g_ifMng->AddSysMessage("단전맵이 원상복귀 되었습니다.");
		g_DanBattle.SendSetStatus(1);
	}
}

void GTH_RunChatCommand( char command[], chatMessage param[] )
{

	
	if(!stricmp(command, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,309)))
		g_HelperSystem.GTH_SendMessage_HelpSystem_DisplayMyPoint();
		
	
	if(!stricmp(command, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,310)))
		g_HelperSystem.GTH_SendMessage_HelpSystem_DisplayHelpPoint();



	
	if (!stricmp( command, "danbattlePcpoint") || !stricmp( command, "단전개인점수"))
	{
		GAMEDirectorCheat.SendMessageGetDanBattlePcPoint();
	}

	
	if ( !stricmp( command, g_LPACK.GetMassage( 0, 443 ) ) )
	{
		if( param[0][0] == 0 )
		{
			g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 570 ) );
			return;
		}
		if ( g_timer.GetAppMilliTime() >= g_cgv.callGMTimer )
		{
			g_cgv.callGMTimer = (unsigned int) g_timer.GetAppMilliTime() + 60000;
			GTH_SendMessage_CallGameMaster( param[0] );
		}
		else
		{
			g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 444 ) );
		}
	}

	
	if( !stricmp( command, g_LPACK.GetMassage( 0, 445 ) ) )
	{
		GTH_RecoverySafePosition();
	}


	
	if( !stricmp( command, g_LPACK.GetMassage( 0, 446 ) ) ) 
	{
		GTH_SendMessage_Request_SavePortalPosition();
	}

	

	
	if(g_ifMng->m_EventCraftWin->GetTableSize() > 0)
	{
		if( !stricmp( command, g_LPACK.GetMassage(9,103)) || !stricmp( command, g_LPACK.GetMassage(9,104)))
		{
			for(int i=0;i<g_ifMng->m_EventCraftWin->GetTableSize();i++)
			{
				itemCraftTable_t *pData = g_ifMng->m_EventCraftWin->Get(i);
				if(pData == NULL) 
				{
					break;
				}

				if(!stricmp(pData->m_chCheatName, param[0]))
				{
					GTH_SendMessage_ItemSystem_ChatCommand(pData->m_lEventIndex);
					return;		
				}
			}
			return;
		}
	}

	
	
	
	if( !stricmp( command, "effect") )
	{
	//lucky 2012 NEW GM system
	if (g_pApp->m_myCharacter->gameMaster <1 ) return;
	//end
		
		
		static int CheatEffectNum=0;
		static int CheatEffectState=0;
		static int CheatEffectPart=0;

		mdleffectclass_t * mdlEffectClass = &g_effectMng->m_MDLMng->m_MDLEffectClasses[CheatEffectNum];

		
		if(!stricmp("help", param[0]))
		{
			char strtemp[256];
			sprintf(strtemp,"effect set 이펙트번호 %d",CheatEffectNum);
			g_ifMng->AddSysMessage( strtemp, D3DCOLOR_ARGB(255, 255, 255, 55) );

			sprintf(strtemp,"effect state 스테이트번호 armed, fire, hitted, missed %d",CheatEffectState);
			g_ifMng->AddSysMessage( strtemp, D3DCOLOR_ARGB(255, 255, 255, 55) );

			sprintf(strtemp,"effect part 파트번호 %d ",CheatEffectPart);
			g_ifMng->AddSysMessage( strtemp, D3DCOLOR_ARGB(255, 255, 255, 55) );

			sprintf(strtemp,"effect height  높이 %.3f",mdlEffectClass->states[CheatEffectState].bobHeight );
			g_ifMng->AddSysMessage( strtemp, D3DCOLOR_ARGB(255, 255, 255, 55) );
			
			sprintf(strtemp,"effect start  스테이트 시작 시간 %.3f",mdlEffectClass->states[CheatEffectState].startTime);
			g_ifMng->AddSysMessage( strtemp, D3DCOLOR_ARGB(255, 255, 255, 55) );

			sprintf(strtemp,"effect startTime 파트 시작 시간 %.3f",mdlEffectClass->states[CheatEffectState].parts[CheatEffectPart].startTime);
			g_ifMng->AddSysMessage( strtemp, D3DCOLOR_ARGB(255, 255, 255, 55) );

			sprintf(strtemp,"effect endTime  파트 끝나는 시간 %.3f",mdlEffectClass->states[CheatEffectState].parts[CheatEffectPart].endTime);
			g_ifMng->AddSysMessage( strtemp, D3DCOLOR_ARGB(255, 255, 255, 55) );

			sprintf(strtemp,"effect scale  스케일 조절 %.3f",mdlEffectClass->states[CheatEffectState].parts[CheatEffectPart].scale);
			g_ifMng->AddSysMessage( strtemp, D3DCOLOR_ARGB(255, 255, 255, 55) );

			sprintf(strtemp,"effect billboard  빌보드 %d",mdlEffectClass->states[CheatEffectState].parts[CheatEffectPart].billboard);
			g_ifMng->AddSysMessage( strtemp, D3DCOLOR_ARGB(255, 255, 255, 55) );

			sprintf(strtemp,"effect offsetx 위치조절 %.3f",mdlEffectClass->states[CheatEffectState].parts[CheatEffectPart].offset[0]);
			g_ifMng->AddSysMessage( strtemp, D3DCOLOR_ARGB(255, 255, 255, 55) );

			sprintf(strtemp,"effect offsety 위치조절 %.3f",mdlEffectClass->states[CheatEffectState].parts[CheatEffectPart].offset[1]);
			g_ifMng->AddSysMessage( strtemp, D3DCOLOR_ARGB(255, 255, 255, 55) );

			sprintf(strtemp,"effect offsetz 위치조절 %.3f",mdlEffectClass->states[CheatEffectState].parts[CheatEffectPart].offset[2]);
			g_ifMng->AddSysMessage( strtemp, D3DCOLOR_ARGB(255, 255, 255, 55) );

			return;
		}

		
		if(!stricmp("set", param[0]))
		{
			CheatEffectNum=atoi(param[1]);
			return;
		}
		
		
		if(!stricmp("state", param[0]))
		{
			CheatEffectState=atoi(param[1]);
			return;
		}

		
		if(!stricmp("part", param[0]))
		{
			CheatEffectPart=atoi(param[1]);
			return;
		}

		
		if(!stricmp("height", param[0]))	
		{
			mdlEffectClass->states[CheatEffectState].bobHeight= atof(param[1]);
			return;
		}

		if(!stricmp("start", param[0]))	
		{
			mdlEffectClass->states[CheatEffectState].startTime= atof(param[1]);
			return;
		}

		
		if(!stricmp("startTime", param[0]))	
		{
			mdlEffectClass->states[CheatEffectState].parts[CheatEffectPart].startTime= atof(param[1]);
			return;
		}

		
		if(!stricmp("endTime", param[0]))	
		{
			mdlEffectClass->states[CheatEffectState].parts[CheatEffectPart].endTime= atof(param[1]);
			return;
		}

		
		if(!stricmp("scale", param[0]))	
		{
			for(int i=0;i<mdlEffectClass->states[CheatEffectState].numParts;i++)
				mdlEffectClass->states[CheatEffectState].parts[i].scale= atof(param[1]);
			return;
		}

		
		if(!stricmp("billboard", param[0]))	
		{
			for(int i=0;i<mdlEffectClass->states[CheatEffectState].numParts;i++)
				mdlEffectClass->states[CheatEffectState].parts[i].billboard= atoi(param[1]);
			return;
		}

		
		if(!stricmp("offsetx", param[0]))	
		{
			for(int i=0;i<mdlEffectClass->states[CheatEffectState].numParts;i++)
				mdlEffectClass->states[CheatEffectState].parts[i].offset[0]= atof(param[1]);
			return;
		}

		if(!stricmp("offsety", param[0]))	
		{
			for(int i=0;i<mdlEffectClass->states[CheatEffectState].numParts;i++)
				mdlEffectClass->states[CheatEffectState].parts[i].offset[1]= atof(param[1]);
			return;
		}

		if(!stricmp("offsetz", param[0]))	
		{
			for(int i=0;i<mdlEffectClass->states[CheatEffectState].numParts;i++)
				mdlEffectClass->states[CheatEffectState].parts[i].offset[2]= atof(param[1]);
			return;
		}
	}
	
	
	if( !stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 92 ) ) ) 
	{
		GTH_SendMessage_Request_ChaosValue();
	}
}



void GetWord(int index, const char *source, char* token)
{
	int i=0;
	int j=0;

	while( source[i] != '\0' ) 
	{
		if( source[i] == ' ' )
		{
			token[j] = '\0';

			if( index == 0 )
				return;
			else
			{
				i++;
				j=0;
				index--;

				continue;
			}
		}
		else
		{
			token[j] = source[i];
		}

		i++;
		j++;
	}

	if( index == 0 )
		token[j] = '\0';
	else
		token[0] = '\0';
}



int GTH_CheckValidName( char *name )
{
	char substr[2];

	if( name[0] == 0 )
	{
		
		
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(0,863) );
		return 0;
	}

	if ( strstr( name, g_LPACK.GetMassage(0,96)) != NULL )
	{
		
		
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(0,864) );
		return 0;
	}
	
	if ( strstr( name, g_LPACK.GetMassage(0,97)) != NULL )
	{
		
		
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(0,865) );
		return 0;
	}
	
	if ( strstr( name, g_LPACK.GetMassage(0,99)) != NULL )
	{
		
		
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(0,865) );
		return 0;
	}
	
	if ( strstr( name, "gM") != NULL )
	{
		
		
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(0,865) );
		return 0;
	}
	
	if ( strstr( name, "Gm") != NULL )
	{
		
		
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(0,865) );
		return 0;
	}
	
	if ( !g_chatManager->IsVaildString( name ) )
	{
		
		
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(0,866) );
		return 0;
	}
	
	for ( int i = 0; i < 128; i++)
	{
		if ( i == 0 ) continue;				
		if (i > 47 && i < 58) continue;		
		if (i > 64 && i < 91) continue;		
		if (i > 96 && i < 123) continue;	
		
		sprintf(substr, "%c", i);
		if ( strstr( name, substr) != NULL )
		{
			
			
			g_ifMng->AddSysMessage( g_LPACK.GetMessage(0,867) );
			return 0;
		}
	}

	
	if ( strlen( name ) > GUILDNAMESTRING - 2 )
	{
		
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(0,868) );
		return 0;
	}
	
	return 1;
}

void GTH_ChatCommand( char* message )
{
	
	
	char chatmsg[CHATSTRING];
	char *tempMsg;
	char command[CHATSTRING];
	chatMessage param[4];
	char szTemp[CHATSTRING];
	int l = 0, numParam = 0;

	
	sstrncpy( chatmsg, message, CHATSTRING );
	
	tempMsg = chatmsg;

	g_ifMng->m_dlgWin->SetSendString();

	


	
	if( chatmsg[0] == 0 )
	{
		return;
	}

	
	
	
	g_input->m_oldDiks[DIK_RETURN] |= 0x80;
	g_input->m_oldDiks[DIK_ESCAPE] |= 0x80;

	if(FALSE == g_cgv.GM.bChatAllow){
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), 
							 g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,490), 1, IF_MESSAGE_NONE );
		(gcpGMCtrl->m_cpChatBlockCtrl)->TransmitePacket_GMsystem_ChatBan_Req_ReaminTime();


		return;

	}
	
	
	
	
	if( *tempMsg == '/' )
	{
		tempMsg ++;
		memset( command, 0, sizeof( command ) );
		l = 0;

		while( *tempMsg != ' ' && *tempMsg != NULL )
		{
			command[l] = *tempMsg;
			tempMsg ++;
			l ++;
		}
		
		l = 0;
		numParam = 0;
		if( *tempMsg != NULL ) tempMsg ++;

		memset( param[0], 0, sizeof( chatMessage ) );
		memset( param[1], 0, sizeof( chatMessage ) );
		memset( param[2], 0, sizeof( chatMessage ) );
		memset( param[3], 0, sizeof( chatMessage ) );

		if( !stricmp( command, g_LPACK.GetMassage( 0, 435 ) ) || 
			!stricmp( command, g_LPACK.GetMassage( 0, 443 ) ) || 
			!stricmp( command, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 376)) ) 
		{
			while( *tempMsg != NULL )
			{	
				param[numParam][l] = *tempMsg;
				tempMsg ++;
				l ++;
			}
			numParam ++;
		}
		else
		{
			while( *tempMsg != NULL )
			{	
				while( *tempMsg != NULL && *tempMsg != ' ' )
				{
					param[numParam][l] = *tempMsg;
					tempMsg ++;
					l ++;
				}
				numParam ++;
				l = 0;
				if( numParam == 4 || *tempMsg == NULL )
				{
					if( numParam < 4 )
						strcpy( param[numParam], "-1" );
					break;
				}
				tempMsg++;
			}
		}

		
#ifdef _GTH_ONLINE_VERSION
		g_ifMng->AddChatMessage( message, GTHCOLOR_CHATMESSAGE, NULL );
#else
		g_ifMng->AddChatMessage( message, GTHCOLOR_CHATMESSAGE, "user" );
#endif
		
		GTH_RunGMChatCommand( chatmsg, command, param );
		GTH_RunChatCommand( command, param );

		return;
	} 

	
	
	
	
	else if( tempMsg[0] == '@' )
	{
		memset( param[0], 0, sizeof( chatMessage ) );
		GetWord( 0, tempMsg, param[0] );

		

		
		
		if( stricmp( param[0], "@눼쉔무삔" ) == 0 || 
			!stricmp( param[0], "@guildcreate" )	)
		{
			memset( param[1], 0, sizeof( chatMessage ) );
			GetWord( 1, tempMsg, param[1] );

			if( param[1][0] != '\0' )
			{
				if( GTH_CheckValidName( param[1] ) )
				{
					char mark[GUILDMARKSIZE];

					memset( param[2], 0, sizeof( chatMessage ) );
					GetWord( 2, tempMsg, param[2] );

					if ( param[2][0] == '\0' )
						strcpy( param[2], "..\\markupload\\mark.bmp" );
					
					memset( mark, 0, GUILDMARKSIZE );

					if( CheckGuildMarkFile( param[2], mark ) )
					{
						GTH_SendMessage_Guild_Organize_Request( param[1], mark );
					}
				}
			}
			else
			{
				strcpy( szTemp, g_LPACK.GetMessage(0,863) ); 
				
				g_ifMng->AddSysMessage( szTemp, GTHCOLOR_ERRORMESSAGE );
			}
			
			return;
		}
		
		
		else if( stricmp( param[0], "@무삔男헝") == 0 )
		{
			memset( param[1], 0, sizeof( chatMessage ) );
			GetWord( 1, tempMsg, param[1] );

			if( param[1][0] != '\0' )
			{
				GTH_SendMessage_Guild_Invite_Request( param[1] );
			}
			else
			{
				strcpy( szTemp, g_LPACK.GetMessage(0,886) );
				g_ifMng->AddSysMessage( szTemp, GTHCOLOR_ERRORMESSAGE );
			}
			
			return;
		}

		
		else if( stricmp( param[0], g_LPACK.GetMessage(0,857) ) == 0 )
		{
			
			
			
			g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322), g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 662)
				, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_DISORGANIZE1, 10000.0f );

			return;
		}

		
		else if( stricmp( param[0], g_LPACK.GetMessage(0,858) ) == 0 )
		{
			
			
			
			g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322), g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 660)
				, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_SECEDE, 10000.0f );

			return;
		}

		
		else if( stricmp( param[0], g_LPACK.GetMessage(0,859) ) == 0 )
		{
			memset( param[1], 0, sizeof( chatMessage ) );
			GetWord( 1, tempMsg, param[1] );

			if( param[1][0] != '\0' )
			{
				if( g_cgv.myCharacterInfo->guildIdx != -1 )
				{
					strcpy( g_ifMng->m_tempString, param[1] );

					
					sprintf( szTemp, g_LPACK.GetMessage(0,862), param[1] );
					g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_BANISH, 10000.0f );
				}
			}
			else
			{
				strcpy( szTemp, g_LPACK.GetMessage(0,887) );
				g_ifMng->AddSysMessage( szTemp, GTHCOLOR_ERRORMESSAGE );
			}

			return;
		}

		
		else if( stricmp( param[0], g_LPACK.GetMessage(0,901) ) == 0 )
		{
			memset( param[1], 0, sizeof( chatMessage ) );
			GetWord( 1, tempMsg, param[1] );

			if( param[1][0] != '\0' )
			{
				GTH_SendMessage_Guild_Subscribe_Request( param[1] );
			}
			else
			{
				strcpy( szTemp, g_LPACK.GetMessage(0,905) );
				g_ifMng->AddSysMessage( szTemp, GTHCOLOR_ERRORMESSAGE );
			}

			return;
		}
		
		
		else if( stricmp( param[0], g_LPACK.GetMessage(0,902) ) == 0 )
		{
			memset( param[1], 0, sizeof( chatMessage ) );
			GetWord( 1, tempMsg, param[1] );

			if( param[1][0] != '\0' )
			{
				if( g_cgv.myCharacterInfo->guildIdx != -1 )
				{
					strcpy( g_ifMng->m_tempString, param[1] );

					
					sprintf( szTemp, g_LPACK.GetMessage(0,916), param[1] );
					g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_ADD_SUBMASTER, 10000.0f );
				}
			}
			else
			{
				strcpy( szTemp, g_LPACK.GetMessage(0,906) );
				g_ifMng->AddSysMessage( szTemp, GTHCOLOR_ERRORMESSAGE );
			}

			return;
		}

		
		else if( stricmp( param[0], g_LPACK.GetMessage(0,903) ) == 0 )
		{
			memset( param[1], 0, sizeof( chatMessage ) );
			GetWord( 1, tempMsg, param[1] );

			if( param[1][0] != '\0' )
			{
				if( g_cgv.myCharacterInfo->guildIdx != -1 )
				{
					strcpy( g_ifMng->m_tempString, param[1] );
					
					sprintf( szTemp, g_LPACK.GetMessage(0,917), param[1] );
					g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_ADD_FACTIONMASTER, 10000.0f );
				}
			}
			else
			{
				strcpy( szTemp, g_LPACK.GetMessage(0,907) );
				g_ifMng->AddSysMessage( szTemp, GTHCOLOR_ERRORMESSAGE );
			}

			return;
		}

		
		else if( stricmp( param[0], g_LPACK.GetMessage(0,904) ) == 0 )
		{
			memset( param[1], 0, sizeof( chatMessage ) );
			GetWord( 1, tempMsg, param[1] );

			if( param[1][0] != '\0' )
			{
				if( g_cgv.myCharacterInfo->guildIdx != -1 )
				{
					strcpy( g_ifMng->m_tempString, param[1] );

					
					sprintf( szTemp, g_LPACK.GetMessage(0,918), param[1] );
					g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_DELETEMASTER, 10000.0f );
				}
			}
			else
			{
				strcpy( szTemp, g_LPACK.GetMessage(0,908) );
				g_ifMng->AddSysMessage( szTemp, GTHCOLOR_ERRORMESSAGE );
			}

			return;
		}

		
		
		else if(0 == stricmp(param[0],g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,138-1)))
		{

			if(-1 == g_cgv.myCharacterInfo->guildIdx){
				g_ifMng->AddSysMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL,889), GTHCOLOR_ERRORMESSAGE );
				return;
			}



			if(0 != strcmp(g_cgv.myCharacterInfo->guild.userName[0],g_cgv.myCharacterInfo->name)){			
				g_ifMng->AddSysMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,139-1), GTHCOLOR_ERRORMESSAGE );
				return;
			}

			
			char szMarkFileFullPath[_MAX_PATH];
			unsigned char MarkImage[GUILDMARKSIZE];


			{
				char szMarkFileName[_MAX_PATH];
				GetWord( 1, tempMsg, szMarkFileName);
				if(NULL == szMarkFileName[0]) strcpy(szMarkFileName,"mark.bmp");
				wsprintf(szMarkFileFullPath,"..\\markupload\\%s",szMarkFileName);
			}			

			if(false == CheckGuildMarkFile(szMarkFileFullPath,(char*)MarkImage)){
				
				g_ifMng->AddSysMessage(szMarkFileFullPath, GTHCOLOR_ERRORMESSAGE );
				return;
			}
			g_cpGuildCtrl->TransmitePacket_MarkChange_Req(g_cgv.myCharacterInfo->guildIdx,MarkImage);
			return;

		}
		
	}



	 
	 

	if	(	g_chatMessageCount	<=	0	)
	{
		
		g_chatMessageCount = 0;

		memset ( g_chatMessageBuffer, 0, sizeof(g_chatMessageBuffer) );
		memset ( g_chatMessageTimeClamp, 0, sizeof(g_chatMessageTimeClamp) );

		strcpy ( g_chatMessageBuffer [g_chatMessageCount], chatmsg );
		g_chatMessageTimeClamp [g_chatMessageCount]	=	g_timer .GetAppMilliTime ();
		g_chatMessageCount	=	1;
	}
	else if	(	(	g_chatMessageCount	>	0	)	&&
				(	g_chatMessageCount	<	MAX_CHAT_MESSAGE_COUNT	)	)
	{
		int		originalLength	=	strlen ( g_chatMessageBuffer [g_chatMessageCount - 1] );
		int		newLength		=	strlen ( chatmsg );
		int		shortLength		=	( originalLength < newLength ) ? originalLength : newLength;

		if	(	! strnicmp ( g_chatMessageBuffer [g_chatMessageCount - 1], chatmsg, shortLength )	)
		{
			 
			strcpy ( g_chatMessageBuffer [g_chatMessageCount], chatmsg );
			g_chatMessageTimeClamp [g_chatMessageCount]	=	g_timer .GetAppMilliTime ();
			++g_chatMessageCount;
		}
		else
		{
			 
			memset ( g_chatMessageBuffer, 0, sizeof(g_chatMessageBuffer) );
			memset ( g_chatMessageTimeClamp, 0, sizeof(g_chatMessageTimeClamp) );

			strcpy ( g_chatMessageBuffer [g_chatMessageCount], chatmsg );
			g_chatMessageTimeClamp [g_chatMessageCount]	=	g_timer .GetAppMilliTime ();
			g_chatMessageCount	=	1;
		}
	}
	else if	(	g_chatMessageCount	==	MAX_CHAT_MESSAGE_COUNT	)
	{
		 
		memcpy ( g_chatMessageBuffer [0], g_chatMessageBuffer [1], sizeof(chatMessage) * ( MAX_CHAT_MESSAGE_COUNT - 1 ) );
		memcpy ( &g_chatMessageTimeClamp [0], &g_chatMessageTimeClamp [1], sizeof(float) * ( MAX_CHAT_MESSAGE_COUNT - 1 ) );
		strcpy ( g_chatMessageBuffer [MAX_CHAT_MESSAGE_COUNT - 1], chatmsg );
		g_chatMessageTimeClamp [MAX_CHAT_MESSAGE_COUNT - 1]	=	g_timer .GetAppMilliTime ();

		float	elapsedTime	=	g_chatMessageTimeClamp [MAX_CHAT_MESSAGE_COUNT - 1] - g_chatMessageTimeClamp [0];
		if	(	elapsedTime	<	MAX_ALLOWED_CHAT_TIME	)
		{
			int		originalLength	=	strlen ( g_chatMessageBuffer [MAX_CHAT_MESSAGE_COUNT - 1] );
			int		newLength;
			int		shortLength;
			for	(	int	index	=	0;	\
						index	<	( MAX_CHAT_MESSAGE_COUNT - 1 );	\
						++index		)
			{
				newLength		=	strlen ( g_chatMessageBuffer [index] );
				shortLength		=	( originalLength < newLength ) ? originalLength : newLength;

				if	(	strnicmp ( g_chatMessageBuffer [index], g_chatMessageBuffer [MAX_CHAT_MESSAGE_COUNT - 1], shortLength )	)
				{
					break;
				}

				
				
				g_ifMng->AddSysMessage( g_LPACK.GetMessage(0,894) );
				
				return;
			}
		}
	}
	 


	
	
	
	
	if( *tempMsg == 34 )
	{
		if( g_cgv.myCharacterInfo->organizeServer < 0 )
		{
			g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 447 ), GTHCOLOR_ERRORMESSAGE );
			return;
		}

		tempMsg ++;

		
		while( *tempMsg == ' ' || ( (BYTE)*tempMsg == 0x81 && (BYTE)*(tempMsg+1) == 0x40 ) )
		{
			if( ( (BYTE)*tempMsg == 0x81 && (BYTE)*(tempMsg+1) == 0x40 ) )
				tempMsg += 2;
			else
				tempMsg ++;
		}

		memset( command, 0, sizeof( command ) );
		l = 0;

		while( *tempMsg != NULL )
		{
			command[l] = *tempMsg;
			tempMsg ++;
			l ++;
		}
		
		if( command[0] == 0 ) return;

		GTH_SendMessage_Party_ChatMessage( command );

		sprintf( szTemp, "%s(to Party)", g_cgv.myCharacterInfo->name );
		g_ifMng->AddChatMessage( command, GTHCOLOR_PARTYMESSAGE, szTemp, IF_CHATMSG_TYPE_PARTY );
	}

	
	
	
	else if( *tempMsg == 35 )
	{
		if( g_cgv.myCharacterInfo->guildIdx < 0 )
		{
			g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 889 ), GTHCOLOR_ERRORMESSAGE );
			return;
		}

		tempMsg ++;

		
		while( *tempMsg == ' ' || ( (BYTE)*tempMsg == 0x81 && (BYTE)*(tempMsg+1) == 0x40 ) )
		{
			if( ( (BYTE)*tempMsg == 0x81 && (BYTE)*(tempMsg+1) == 0x40 ) )
				tempMsg += 2;
			else
				tempMsg ++;
		}

		memset( command, 0, sizeof( command ) );
		l = 0;

		while( *tempMsg != NULL )
		{
			command[l] = *tempMsg;
			tempMsg ++;
			l ++;
		}
		
		if( command[0] == 0 ) return;


		GTH_SendChatMessage( command, IF_CHATMSG_TYPE_DAN );



	}

	
	
	
	else if( *tempMsg == 38 )
	{
		tempMsg ++;

		memset( command, 0, sizeof( command ) );
		l = 0;

		while( *tempMsg != NULL )
		{
			command[l] = *tempMsg;
			tempMsg ++;
			l ++;
		}
		
		if( command[0] == 0 ) return;

		GTH_SendChatMessage( command, IF_CHATMSG_TYPE_TRADE );

		g_ifMng->AddChatMessage( command, GTHCOLOR_TRADEMESSAGE, g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_TRADE );

	}

	
	
	
	else if( *tempMsg == 39 ) 
	{
		tempMsg ++;
		
		if( *tempMsg == ' ' || ( (BYTE)*tempMsg == 0x81 && (BYTE)*(tempMsg+1) == 0x40 ) )
		{
			return;
		}

		memset( command, 0, sizeof( command ) );
		l = 0;
		
		while( *tempMsg != ' ' && *tempMsg != NULL && !( (BYTE)*tempMsg == 0x81 && (BYTE)*(tempMsg+1) == 0x40 ) )
		{
			command[l] = *tempMsg;
			tempMsg ++;
			l ++;
		}

		memset( param[0], 0, sizeof( chatMessage ) );

		l = 0;
		
		if( *tempMsg != NULL ) 
		{
			if ( *tempMsg == ' ' )
				tempMsg ++;
			else
				tempMsg += 2;
		}

		while( *tempMsg != NULL )
		{
			param[0][l] = *tempMsg;
			tempMsg ++;
			l ++;
		}

		if( strlen( command ) > NAMESTRING - 2 ) return;

		strcpy( g_cgv.whisperName, command );

		if( param[0][0] == 0 ) return;


		
		
		{
			char tempWhisperName[NAMESTRING+1];
			strncpy(tempWhisperName,g_cgv.whisperName,NAMESTRING);
			tempWhisperName[5-1]=NULL;	


			
			if(0 == stricmp(tempWhisperName,"[gm]")){
				g_ifMng->AddSysMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,471), GTHCOLOR_SYSMESSAGE );
				g_ifMng->AddSysMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,472),GTHCOLOR_SYSMESSAGE);
				g_ifMng->AddSysMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,473), GTHCOLOR_SYSMESSAGE);
				return;
			}

			
		}

		GTH_SendChatMessage_Whisper( g_cgv.whisperName, param[0] );

		sprintf( szTemp, "(to %s)", g_cgv.whisperName );

		g_ifMng->AddChatMessage( param[0], GTHCOLOR_WHISPERMESSAGE, szTemp, IF_CHATMSG_TYPE_WHISPER );
		g_ifMng->m_dlgWin->SetWhisper( g_cgv.whisperName );
	}

	
	
	
	else
	{ 
		
		if( chatmsg[0] == NULL ) return;

			if(	g_cgv.myCharacterInfo->m_bChatBan_Flag==false)
			{
				GTH_SendChatBan_Flag();
			}

		
		
		g_ifMng->AddChatMessage( chatmsg, GTHCOLOR_CHATMESSAGE, g_pApp->m_myCharacter->name, IF_CHATMSG_TYPE_NORMAL );

		GTH_SendChatMessage( chatmsg, IF_CHATMSG_TYPE_NORMAL );
	}

	if( g_cgv.isDeveloper )
	{
		
		
		if( !stricmp( chatmsg, g_LPACK.GetMassage(0,849) ) ) g_cgv.myCharacterInfo->targetingType = TARGETING_NORMAL;
		
		else if( !stricmp( chatmsg, g_LPACK.GetMassage(0,850) ) ) g_cgv.myCharacterInfo->targetingType = TARGETING_SAFE;
		
		else if( !stricmp( chatmsg, g_LPACK.GetMassage(0,851) ) ) g_cgv.myCharacterInfo->targetingType = TARGETING_EVERY;
		
		else if( !stricmp( chatmsg, g_LPACK.GetMassage(0,852) ) ) g_cgv.myCharacterInfo->targetingType = TARGETING_ONLY_MONSTER;
	}
}


void CGameDirectorCheatCode::SendMessageMonsterSpawnCheat(int monsteridx, int AI_IDX)
{
	monsterBasicTable_t* pmonster = NULL;
	if ( monsteridx < 0 ) return;	
	if ( AI_IDX < 0)
		AI_IDX = g_monsterTable[monsteridx].AIScriptIdx;	

	int ret = 0;
	for (int i =0; i < g_pApp ->m_mapInfos[0].monsterKind; i ++)
	{
		if ( g_pApp ->m_mapInfos[0].monsters[i] == monsteridx )
		{
			ret = 1;	
			break;
		}
	}
	
	if ( ret == 0 )
		return;	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE);
		MSG_WriteByte(&netMessage, CHEATCODE_MONSTERSPAWN);
		MSG_WriteShort(&netMessage, monsteridx);
		MSG_WriteShort(&netMessage, AI_IDX);
		
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGameDirectorCheatCode::SendMessageMonsterKill(int range)
{
	if ( range <= 0)
		range = 10;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE);
		MSG_WriteByte(&netMessage, CHEATCODE_MONSTERKILL);
		MSG_WriteShort(&netMessage, range);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}



void CGameDirectorCheatCode::SendMessageAddQuestItem(int tableidx)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE);
		MSG_WriteByte(&netMessage, CHEATCODE_ADDQUESTITEM);
		MSG_WriteShort(&netMessage, tableidx);				
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}


void CGameDirectorCheatCode::SendMessageOutputGuildinfo(void)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE);
		MSG_WriteByte(&netMessage, CHEATCODE_OUTPUTGUILDINFO);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}







void CGameDirectorCheatCode::SendMessageGetMemberShipType(void)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE);
		MSG_WriteByte(&netMessage, CHEATCODE_GET_MEMBERSHIP_TYPE);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}




void CGameDirectorCheatCode::SendMessageGetDanBattlePcPoint(void)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE);
		MSG_WriteByte(&netMessage, CHEATCODE_DANBATTLE_PCPOINT);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}




void CGameDirectorCheatCode::SendMessageHideName(int show)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHEATCODE);
		MSG_WriteByte(&netMessage, CHEATCODE_HIDENAME);		
		MSG_WriteByte(&netMessage, show);		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}