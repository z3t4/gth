




#include "../global_def.h"
#include "../game/GolryunBattle/GolryunBattle.h"

CSkillApp g_SkillApp;





CSkillApp::CSkillApp()
{
	skillResendFlag=0;
}

CSkillApp::~CSkillApp()
{

}

int CSkillApp::Recv_ProcessEventMessage_Skill()
{
	int i;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;
	int targetIdx, targetType;
	float angles;
	int tableIdx, level, curRA, curSA;
	int skillIdx, coolTime, delayTime;
	int multiTargetNumber;
	int multiTargetType[8], multiTargetIdx[8];
	int resultHitRate;
	int isValid;
	vec3_t pos, delta;

	idx				= MSG_ReadShort();
	entityType		= MSG_ReadByte();

	MSG_ReadPosition( pos );
	angles			= MSG_ReadFloat();
	
	tableIdx		= MSG_ReadShort(); 

	level			= MSG_ReadByte();

	curRA			= MSG_ReadLong();
	curSA			= MSG_ReadShort();

	targetIdx		= MSG_ReadShort();
	targetType		= MSG_ReadByte();

	
	
	skillIdx =		 MSG_ReadChar();
	if( skillIdx < 0 )
	{
		
	}

	coolTime			= MSG_ReadLong();
	
	ShowLogInfo("SkillIdx : %d, Cooltime : %d", skillIdx, coolTime );
	delayTime			= MSG_ReadLong();
	isValid				= MSG_ReadByte();

	multiTargetNumber	= MSG_ReadByte();
	for( i = 0; i < multiTargetNumber; i ++ )
	{
		multiTargetType[i]	= MSG_ReadByte();
		multiTargetIdx[i]	= MSG_ReadShort();
	}

	resultHitRate			= MSG_ReadByte();

	character				= &g_charManager->m_Characters[0];
	numCharacter			= g_charManager->m_iNumCharacters;

	
	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idx == character->idxOnServer)
			break;
	}

	
	if (i >= numCharacter) return false; 

	
	
	
	int SkillTableIdx = -1;

	if( character->entityType == ENTITY_PC &&  character->idxOnServer == g_pApp->m_myCharacter->idxOnServer )	
	{
		g_SkillFile.DebugOutString("Recv_ProcessEventMessage_Skill 받은 스킬 인덱스 %d, 받은 스킬 타입 %d ",skillIdx,targetType);

		if(skillIdx >= 0 && skillIdx < MAX_NUMBER_OF_SKILL)
		{
			SkillTableIdx = g_cgv.myCharacterInfo->skill[skillIdx].tableIdx;

			CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(SkillTableIdx);
			if(pSkillTable!=NULL)
			{
				if( pSkillTable->m_nTargetType == SKILL_TARGET_MYSELF )
				{
					targetIdx = character->idxOnServer;
					targetType = ENTITY_PC;
				}
			}
		}
	}
	


	
	
	character->stat_attackSpeed	= MSG_ReadShort();
	character->angles[ YAW ]	= angles;
	character->stat_attackSpeed	= (float)delayTime;
	character->targetIdx		= targetIdx;
	character->targetType		= targetType;

	if( !VectorCompare( character->position, pos ) )
	{
		VectorClear(delta);
		g_move->Pmove( pos, delta, character->angles );
		VectorCopy( character->position, pos );
		g_charManager->FixCharacterPosition();
	}
	character->event = GTH_EV_CHAR_SKILL;
	character->curRA = curRA;
	character->curSA = curSA;
	
	g_SkillLogic.GTH_SkillExecute( character, tableIdx, level, multiTargetNumber, multiTargetType, multiTargetIdx ,targetType);

	
	if( character->entityType == ENTITY_PC && character->idxOnServer == g_cgv.myPCIdxOnServer )
	{
		g_cgv.myCharacterInfo->skillCastingtime = 0.0f;
		g_cgv.myCharacterInfo->skillCastingState = true;

		
		
		

		
		if(skillIdx >= 0 && skillIdx < MAX_NUMBER_OF_SKILL)
		{
			
			
			g_cgv.myCharacterInfo->skillCurCooltime[skillIdx] = (float)coolTime	+ g_timer.GetAbsoluteTime() * 1000.0f;
			g_cgv.myCharacterInfo->skillMaxCooltime[skillIdx] = (float)coolTime;
		}
		g_cgv.myCharacterInfo->resultHitRate = resultHitRate;

		g_cgv.saveEvent = GTH_EV_CHAR_SKILL;
		if( !isValid )
		{
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,180), D3DCOLOR_ARGB(255, 255, 55, 55) );
		}

		
		
		
		
		if( SkillTableIdx == 1)	g_cgv.Skill.iRecvSkillCastingCount=3;
		else					g_cgv.Skill.iRecvSkillCastingCount=1;

		g_SkillFile.DebugOutString("Recv_ProcessEventMessage_Skill 설정 %d \n",g_cgv.Skill.iRecvSkillCastingCount);
	}

	return true;
}




int CSkillApp::Recv_ProcessEventMessage_SkillCasting()
{
	int i;
	int idxOnServer, entityType, curSA, castingTime, generalspeed;
	int numCharacter;
	Fx_CHARACTER_t *character;


	idxOnServer		= MSG_ReadShort();
	entityType		= MSG_ReadByte();
	curSA			= MSG_ReadShort();
	castingTime		= MSG_ReadLong();
	
	
	int Index		= MSG_ReadShort();
	int recvSkillTargetType	= MSG_ReadShort();
	
	

	
	generalspeed	= MSG_ReadLong();
	
	
	int UseType		= MSG_ReadByte();

	character		= &g_charManager->m_Characters[0];
	numCharacter	= g_charManager->m_iNumCharacters;

	
	for (i=0; i < numCharacter; i++, character++) 
	{
		if (entityType == character->entityType && idxOnServer == character->idxOnServer)	break;
	}

	
	if (i >= numCharacter) return false; 


	if( character->entityType == ENTITY_PC && character->idxOnServer == g_cgv.myPCIdxOnServer )
	{	
		g_cgv.Skill.iSelectSkillUseType = UseType;

		int SkillTableIdx =-1;
		int recvSkillIndex = -1;
		if(UseType == globalVariable_t::SELECT_ITEM)
		{
			g_cgv.Skill.iSelectSkillTableIdx	= g_SkillItem.GetSkillItemTableIndex(Index);
			g_cgv.Skill.iUseInvenPos			= Index;

			SkillTableIdx = g_cgv.Skill.iSelectSkillTableIdx;

		
			g_cgv.Skill.iSkillExecuteIndex       = g_cgv.Skill.iUseInvenPos;			
		}
		else
		{
			recvSkillIndex = Index;		
			SkillTableIdx = g_cgv.myCharacterInfo->skill[recvSkillIndex].tableIdx;

			
			g_cgv.Skill.iSkillExecuteIndex       = recvSkillIndex;			
		}

		g_cgv.Skill.iSkillExecuteUseType	 = UseType;
		g_cgv.Skill.iSkillExecuteTargetType  = recvSkillTargetType;

		g_SkillFile.DebugOutString("스킬 캐스팅 받은 스킬 인덱스 %d, 받은 스킬 타입 %d ",recvSkillIndex,recvSkillTargetType);

		
		
	
		
		

		character->curSA							= curSA;
		g_cgv.myCharacterInfo->skillCastingtime		= (float)castingTime;
		g_cgv.myCharacterInfo->skillCastingState	= false;
		g_cgv.myCharacterInfo->skillMaxCastingtime	= (float)castingTime;
		g_cgv.myCharacterInfo->curSA				= curSA;		

		
		g_cgv.syncCharacter[0].generalSpeed			= generalspeed;
		g_charManager->SetCharacterSpeed( &g_charManager->m_Characters[0], g_cgv.syncCharacter[0].generalSpeed );

		
		g_pApp->m_myCharacter->event = GTH_EV_CHAR_SKILLCASTING;
		

		
		g_cgv.Skill.iRecvSkillCastingCount=1;
		g_SkillFile.DebugOutString("설정 %d \n",g_cgv.Skill.iRecvSkillCastingCount);

	}
	character->event = GTH_EV_CHAR_SKILLCASTING;
	
	
	
	g_SkillLogic.GTH_AddSkillCastingEffect( character, castingTime );
	return true;
}



int CSkillApp::GTH_SkillCastingProcess()
{
	


	
	g_cgv.myCharacterInfo->skillCastingtime -= g_timer.GetElapsedMilliTime();
	if( g_cgv.myCharacterInfo->skillCastingtime < 0.0f )
	{
		g_cgv.myCharacterInfo->skillCastingtime = 0.0f;
		g_cgv.myCharacterInfo->skillCastingState = true;
	}
	if( !g_cgv.myCharacterInfo->skillCastingState )
	{
		return( false );
	}
	return( true );
}


void CSkillApp::GTH_Character_SkillCasting( Fx_CHARACTER_t* character )
{	
	if( character->entityType == ENTITY_PC )
	{
		if( character->ani_curSetID == ANIM_ITEM_SKILLCASTING )
		{
			GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_CASTINGIDLE );
		}
		else
		{
			if( character->ani_curSetID != ANIM_ITEM_CASTINGIDLE )
				GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_SKILLCASTING, true, NULL );
		}
	}
	else if( character->entityType == ENTITY_MONSTER || character->entityType == ENTITY_NPC )
	{
		GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_IDLE );
	}
}

void CSkillApp::GTH_Character_Skill( Fx_CHARACTER_t *character )
{

	 

	
		GTH_ChangeAnimation( character, ANIMTYPE_BYITEM, ANIM_ITEM_PRE_ATTACK );
	
	
}

int CSkillApp::CheckActionTarget( int entityType, int checkDistanceFlag )
{
	float dist;
	
	if( g_pApp->m_myCharacter->targetType != entityType || g_pApp->m_myCharacter->targetIdx < 0 ) return 0;
	dist = g_GameTarget.GTH_GetTargetDistance( g_pApp->m_myCharacter );
	
	switch( entityType )
	{
	case ENTITY_PC :
		if( g_pApp->m_myCharacter->targetType != ENTITY_PC ) return 0;
		if( checkDistanceFlag ) 
		{
			if( dist < 0.00000f || dist > AVAILABLE_DISTANCE_ENTITYMENU ) return 0;
		}
		break;
	case ENTITY_MONSTER :
		if( g_pApp->m_myCharacter->targetType != ENTITY_MONSTER ) return 0;
		break;
	}

	return 1;
}

void CSkillApp::ActionProcess( int id )
{
	
	switch( id )
	{
	case ACTION_ATTACK :
		{
			
			if(g_ifMng->m_autoQuestWin->m_iautoflag==AUTOQUESTSCROLL_START) return;
			vec3_t touched;

			
			
			

			
			if( g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_SKILLCASTING ||
				g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_CASTINGIDLE ||
				g_pApp->m_myCharacter->ani_curSetType == ANIMTYPE_BYSKILL )				
				return;			
			
			

			g_cgv.pTargetChar = g_GameTarget.GTH_GetTargetPointer( g_pApp->m_myCharacter );
			if ( g_cgv.pTargetChar == NULL )	return;

			switch( g_pApp->m_myCharacter->targetType )
			{
			case ENTITY_MONSTER :
				{
					if( g_cgv.pTargetChar->stat_alive > MON_ALIVE_STATE_NORMAL ) 
						return;
					break;
				}
			case ENTITY_PC :
				{
					
					if ( g_cgv.pTargetChar->idxOnServer == g_pApp->m_myCharacter->idxOnServer ) return;
					if( !g_pApp->m_myCharacter->pvpMode )
					{

						
						if( !g_AttackProc.IsSaftTargeting() ) return;
					}
					break;
				}
			default :
				return;
			}

			VectorClear( g_cgv.oldTargetPosition );	
			VectorCopy( touched, g_cgv.pTargetChar->position );
			g_cursor->SetPointerTargetPos( touched, g_pApp->m_myCharacter->position );
			g_cgv.clickEvent = GTH_CEV_CHAR_ATTACK;

		}
		break;
	case ACTION_ASSIST : 
		{
		}
		break;
	case ACTION_FOLLOW :
		{
			vec3_t touched;
			if( g_pApp->m_myCharacter->targetType < 0 || g_pApp->m_myCharacter->targetIdx < 0 ) return;
			if( g_cgv.pTargetChar == NULL ) return;
			
			
			if( g_ifMng->CheckConditionalWindow() ) return;

			VectorClear( g_cgv.oldTargetPosition );	
			VectorCopy( touched, g_cgv.pTargetChar->position );

			g_cursor->SetPointerTargetPos( touched, g_pApp->m_myCharacter->position );
			g_cgv.clickEvent = GTH_CEV_CHAR_FOLLOW;			
		}
		break;
	case ACTION_PICKUP_ITEM :
		{
			
			itementity_t *pItemObject;
			pItemObject = g_itemMng->SearchShortestEntity( g_pApp->m_myCharacter->position );
			if( pItemObject == NULL ) return;
			if( pItemObject->idxOnServer == g_cgv.targetItemIdxOnServer && g_cgv.clickEvent == GTH_CEV_CHAR_PICKUP_ITEM ) return;
			
			g_cgv.clickEvent = GTH_CEV_CHAR_PICKUP_ITEM;
			g_cgv.targetItemIdxOnServer = pItemObject->idxOnServer;			
			
			VectorClear( g_cgv.oldTargetPosition );
			g_cursor->SetPointerTargetPos( pItemObject->origin , g_pApp->m_myCharacter->position );
			g_cursor->SetPlayerTargetedMove();
			g_cgv.saveEvent = -1;
		}
		break;	
		
	case ACTION_EPACK_RA :
		{
			
			if( !g_pApp->m_myCharacter->isAlive ) return;
			if( g_cgv.myCharacterInfo->curRA >= g_cgv.myCharacterInfo->calMaxRA ) return;
			if( g_cgv.myCharacterInfo->curChargeRA <= 0 ) return;
		
		
			GTH_SendMessage_RequestEpackUse( 0 );
		}
		break;
		
	case ACTION_EPACK_SA :
		{
			if( !g_pApp->m_myCharacter->isAlive ) return;
			if( g_cgv.myCharacterInfo->curSA >= g_cgv.myCharacterInfo->calMaxSA ) return;
			if( g_cgv.myCharacterInfo->curChargeSA <= 0 ) return;
		
		
			GTH_SendMessage_RequestEpackUse( 1 );
		}
		break;
	case ACTION_SIT :
		{
			if( g_pApp->m_myCharacter->isTransform )
			{
				
				g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 534 ) );
				return;
			}
			
			
			if( g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_STAND || 
				g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_SIT ) return;
			
			if( g_pApp->m_myCharacter->event == GTH_EV_CHAR_IDLE )
			{
				g_pApp->m_myCharacter->event = GTH_EV_CHAR_SIT;
			}
			else if( g_pApp->m_myCharacter->event == GTH_EV_CHAR_SIT )  
			{
				g_pApp->m_myCharacter->event = GTH_EV_CHAR_STAND;
			}
		}
		break;
	case ACTION_TRADE :
		{
			if( g_pApp->m_myCharacter->event != GTH_EV_CHAR_IDLE ) return;

			
			if ( g_cgv.clickEvent == GTH_CEV_CHAR_FOLLOW  || 
				g_pApp->m_myCharacter->event ==GTH_CEV_CHAR_FOLLOW )
				return;
			
			

			if( g_cgv.saveMenuTime > 0.0f )
			{
				g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 199 ) );
				return;
			}

			if( !CheckActionTarget( ENTITY_PC, true ) ) return;
			if( g_pApp->m_myCharacter->targetIdx == g_cgv.myPCIdxOnServer ) return;


			if(NULL == g_cgv.pTargetChar) return;
			if(TRUE == g_cgv.syncCharacter[(g_cgv.pTargetChar)->ID].hide){
				g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 199 ) );
				return;
			}
			
			
			GTH_SendMessage_PCTrade_Request( g_pApp->m_myCharacter->targetIdx );
		}
		break;
	case ACTION_PARTY_INVITE :
		{
			if( g_pApp->m_myCharacter->event != GTH_EV_CHAR_IDLE ) return;
			if( g_cgv.saveMenuTime > 0.0f )
			{
				g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 199 ) );
				return;
			}
			if( !CheckActionTarget( ENTITY_PC, true ) ) return;
			if( g_pApp->m_myCharacter->targetIdx == g_cgv.myPCIdxOnServer ) return;



			if(NULL == g_cgv.pTargetChar) return;
			if(TRUE == g_cgv.syncCharacter[(g_cgv.pTargetChar)->ID].hide){
				g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 199 ) );
				return;
			}


			
			if( g_cgv.myCharacterInfo->organizeServer == -1 ) 
			{
				GTH_SendMessage_Party_Request_Organize( g_pApp->m_myCharacter->targetIdx );
			}
			else
			{
				if( g_cgv.myCharacterInfo->isLeader ) GTH_SendMessage_Party_Request_Join( g_pApp->m_myCharacter->targetIdx );
			}
		}
		break;
	case ACTION_PARTY_OUT :
		{
			
			if( g_cgv.myCharacterInfo->organizeServer == -1 ) return;
			
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,123), g_LPACK.GetMassage(0,124), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_PARTY_OUT );
		}
		break;
	case ACTION_PARTY_FORCEDOUT :
		{
			
			if( g_cgv.myCharacterInfo->organizeServer == -1 ) 
			{
				return;
			}
			else
			{
				if( g_cgv.myCharacterInfo->isLeader ) 
				{
					char szTemp[256];
					
					sprintf( szTemp, g_LPACK.GetMassage(0,130), g_ifMng->m_partyWin->GetPartyMemberName() );
					g_ifMng->SetMessage( g_LPACK.GetMassage(0,131), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_PARTY_FORCEDOUT );
				}
			}
		}
		break;
	case ACTION_PARTY_DISORGANIZE :
		{
			
			if( g_cgv.myCharacterInfo->organizeServer == -1 ) return;
			
			if( g_cgv.myCharacterInfo->isLeader ) 
			{
				
				g_ifMng->SetMessage( g_LPACK.GetMassage(0,132), g_LPACK.GetMassage(0,133), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_PARTY_DISORGANIZE );
			}
		}
		break;
	case ACTION_MAP :
		{
			g_ifMng->m_mapWin->ToggleState();
		}
		break;
	case ACTION_BOOTH :
		
		{
			
			

			
			if ( GTH_CanOpenBooth() == FALSE)
			{
				g_ifMng->SetMessage(g_LPACK.GetMassage(0,322),
					g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 594 ) );
				return;			
			}
			
			if ( g_cgv.clickEvent == GTH_CEV_CHAR_FOLLOW  || 
				g_pApp->m_myCharacter->event ==GTH_CEV_CHAR_FOLLOW )
				return;
			
			

			
			if( g_pApp->m_myCharacter->chaosPoint >29500 )
			{
				
				g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 772 ) );
				return;
			}
			else
			{
				GTH_SendMessage_ItemSystem_BoothSellOpen();
			}
		}
		break;
	
	
	case ACTION_FRIEND_MAKE : 
		{
			if( g_pApp->m_myCharacter->event != GTH_EV_CHAR_IDLE ) return;
			if( g_cgv.saveMenuTime > 0.0f )
			{
				g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 199 ) );
				return;
			}
			if( !CheckActionTarget( ENTITY_PC, true ) ) return;
			if( g_pApp->m_myCharacter->targetIdx == g_cgv.myPCIdxOnServer ) return;

			if(NULL == g_cgv.pTargetChar) return;
			if(TRUE == g_cgv.syncCharacter[(g_cgv.pTargetChar)->ID].hide){
				g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 199 ) );
				return;
			}
			

			GTH_SendMessage_Friends_Invite( g_pApp->m_myCharacter->targetIdx );
		}
		break;
	case ACTION_FRIEND_BYE : 
		{
			char szTemp[128];

			if( g_ifMng->m_friendWin->m_selectedFriendName[0] != 0 )
			{
				sprintf( szTemp, g_LPACK.GetMessage(0, 733), g_ifMng->m_friendWin->GetSelectedFriendName() );
				g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_FRIENDS_REQUEST_REMOVE, 10000.0f );
			}
			
			
		}
		break;
	
	case ACTION_SHOUT :
		{
			if( g_pApp->m_myCharacter->isTransform )
			{
				
				g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 910 ) );
				return;
			}
			
			
			if( g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_STAND || 
				g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_SIT ||
				g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_SOCIAL_SHOUT ||
				g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_SOCIAL_BOW	) return;

			if( g_pApp->m_myCharacter->event == GTH_EV_CHAR_IDLE )
			{
				
				

				
				GTH_SendEventMessage_SocialAnimation(GTH_EV_CHAR_SHOUT);
			}
		}
		break;
	case ACTION_BOW:
		{
			if( g_pApp->m_myCharacter->isTransform )
			{
				g_ifMng->AddSysMessage( g_LPACK.GetMessage( 0, 910 ) );
				return;
			}

			if( g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_STAND ||
				g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_SIT ||
				g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_SOCIAL_BOW ||
				g_pApp->m_myCharacter->ani_curSetID == ANIM_ITEM_SOCIAL_SHOUT )	return;

			if( g_pApp->m_myCharacter->event == GTH_EV_CHAR_IDLE )
			{
				
				
				GTH_SendEventMessage_SocialAnimation(GTH_EV_CHAR_BOW);
			}
		}
		break;
	case ACTION_CHATTING :
		{
			if(TRUE == g_pApp->m_myCharacter->m_bWaitingRoom)
			{
				g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 392 ));
				return;
			}

			
			if(gGameConfigFlag.m_bWorld_Chat_Limit && g_cgv.myCharacterInfo->m_nMemberShipType <= 0)
			{
				GTH_SendMessage_WorldChatting_Count();
			}
			

			
			g_ifMng->m_worldChatWin->Enable( true );
			g_ifMng->SetFocus( g_ifMng->m_worldChatWin );
			
			g_chatManager->InitInputStr( INPUT_INDEX_WORLDCHAT );
			g_chatManager->SetInputState( INPUT_INDEX_WORLDCHAT, 1 );
			g_chatManager->ChangeInputMessage( INPUT_INDEX_WORLDCHAT );

		}
		break;

	case ACTION_REGIST_HELPER:
		{
			if(g_cgv.myCharacterInfo->targetType == -1 || 
			   g_cgv.myCharacterInfo->targetType != ENTITY_PC )
				return;
		
		
		
		
		
			g_HelperSystem.GTH_SendMessage_HelpSystem_DoYouWant();
			
			break;
		}
	case ACTION_GIVE_POINT:
		{
			
			
			if(g_cgv.myCharacterInfo->targetType == -1 || 
			   g_cgv.myCharacterInfo->targetType != ENTITY_PC)
			   		return;
			
			char szTarketName[NAMESTRING+1] = "";
			g_GameTarget.GTH_GetTargetName(g_cgv.myCharacterInfo->targetType,g_cgv.myCharacterInfo->targetIdx,szTarketName);
			if( FALSE == g_HelperSystem.IsChild(szTarketName)) return;
			
				g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 313), 
									MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_HELPERSYSTEM_GIVEPOINT, 10000.0f);
			break;
		}

		


		
	}
}
