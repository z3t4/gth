#include "../global_def.h"






#include "..\GM(Game Master)\CGMCtrl.h"
extern CGMCtrl* gcpGMCtrl;

int GTH_StartUpGame(int _Type)
{
	
	GTH_InitClientGlobalVariable();
	
	
	if ( !GTH_Network_StartUp() )
		return FALSE;


	if(_Type != NONE_MASTERSERVER_CONNECT)
	{
			
			



			{		
			if ( !GTH_Network_EstablishMasterServerConnection() )
				return false;

			}
	}
	return true;
}

void GTH_ShutdownGame()
{
	
	if ( g_cgv.onMasterServer )
		GTH_SendMessage_DisconnectMasterServer(); 
	else if ( g_cgv.onGameServer )
		GTH_SendMessage_DisconnectGameServer(FALSE); 
}

int GTH_ParseGameConfig()
{
	int i;
	char *token;

#ifdef _GTH_CHINESE_VERSION
	
	
	if( g_script.Load( "server_chn.cfg" ) ) 
	{
		return( GTH_ParseGameConfig_MasterServerGroup() );
	}
#endif

	if( !g_script.Load( "server.cfg" ) )
		return false;

	
#ifdef _GTH_MALAYSIA_CHN_VERSION
	else
		return( GTH_ParseGameConfig_MasterServerGroup() );		
#endif

	g_script.BeginParse();

	g_cgv.isTestServer = false;
	g_cgv.languageType = GTH_LANGUAGE_TYPE_KOREAN;


	while ( 1 )
	{
		token = g_script.GetNextToken( true );
		if( !token[0] ) break;

		if( !stricmp( token, "MASTER_SERVER_NUMBER" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  return false;
			g_cgv.masterServerNumber = atoi( token );
		}
		else if( !stricmp( token, "MASTER_SERVER_IP" ) )
		{
			for (i=0; i<g_cgv.masterServerNumber; i++)
			{
				token = g_script.GetNextToken( true );
				if( !token )  return false;
				strcpy ( g_cgv.listMasterServerIP[i], token );
			}
		}
		else if( !stricmp( token, "MASTER_SERVER_PORT" ) )
		{
			for (i=0; i<g_cgv.masterServerNumber; i++)
			{
				token = g_script.GetNextToken( true );
				if( !token )  return false;
				g_cgv.listMasterServerPort[i] = atoi( token );
			}
		}
		





			










		else if( !stricmp( token, "SUB_VERSION" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  return false;
			strcpy ( g_cgv.subVersion, token );
			if( !stricmp( g_cgv.subVersion, "DEVELOPER" ) ) 
			{
				g_cgv.isDeveloper = true;
			}
			else 
			{
				g_cgv.isDeveloper = false;
			}
		}
		else if( !stricmp( token, "LANGUAGE_TYPE" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  return false;
			if( !stricmp( token, "KOERAN" ) ) g_cgv.languageType = GTH_LANGUAGE_TYPE_KOREAN;
			else if( !stricmp( token, "ENGLISH" ) ) g_cgv.languageType = GTH_LANGUAGE_TYPE_ENGLISH;
			else if( !stricmp( token, "JAPANESE" ) ) g_cgv.languageType = GTH_LANGUAGE_TYPE_JAPANESE;
			else if( !stricmp( token, "CHINESE" ) ) g_cgv.languageType = GTH_LANGUAGE_TYPE_SIMPLIFIED_CHINESE;
			else if( !stricmp( token, "TAIWAN" ) ) g_cgv.languageType = GTH_LANGUAGE_TYPE_TRADITIONAL_CHINESE; 
		}

	}

	return true;
}


int GTH_ParseGameConfig_MasterServerGroup()
{
	int i, groupIdx;
	char *token;

	g_script.BeginParse();

	g_cgv.isTestServer = false;
	g_cgv.languageType = GTH_LANGUAGE_TYPE_KOREAN;

	while ( 1 )
	{
		token = g_script.GetNextToken( true );
		if( !token[0] ) break;

		if( !stricmp( token, "MASTER_SERVER_GROUP_NUMBER" ) )
		{
			token = g_script.GetNextToken( true );
			g_cgv.masterServerGroupNumber = atoi( token );
		}
		else if( !stricmp( token, "MASTER_SERVER_GROUP_IDX" ) )
		{
			token = g_script.GetNextToken( true );
			groupIdx = atoi( token );
		}
		else if( !stricmp( token, "MASTER_SERVER_GROUP_NAME" ) )
		{
			token = g_script.GetNextToken( true );
			strcpy( g_cgv.masterServerGroupInfo[groupIdx].masterServerGroupName, token );
		}
		else if( !stricmp( token, "MASTER_SERVER_NUMBER" ) )
		{
			token = g_script.GetNextToken( true );
			g_cgv.masterServerGroupInfo[groupIdx].masterServerNumber = atoi( token );
		}
		else if( !stricmp( token, "MASTER_SERVER_IP" ) )
		{
			for (i=0; i<g_cgv.masterServerGroupInfo[groupIdx].masterServerNumber; i++)
			{
				token = g_script.GetNextToken( true );
				strcpy ( g_cgv.masterServerGroupInfo[groupIdx].listMasterServerIP[i], token );
			}
		}
		else if( !stricmp( token, "MASTER_SERVER_PORT" ) )
		{
			for (i=0; i<g_cgv.masterServerGroupInfo[groupIdx].masterServerNumber; i++)
			{
				token = g_script.GetNextToken( true );
				g_cgv.masterServerGroupInfo[groupIdx].listMasterServerPort[i] = atoi( token );
			}
		}
















		else if( !stricmp( token, "SUB_VERSION" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  return false;
			strcpy ( g_cgv.subVersion, token );
			if( !stricmp( g_cgv.subVersion, "DEVELOPER" ) ) 
			{
				g_cgv.isDeveloper = true;
			}
			else 
			{
				g_cgv.isDeveloper = false;
			}
		}
		else if( !stricmp( token, "LANGUAGE_TYPE" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  return false;
			if( !stricmp( token, "KOERAN" ) ) g_cgv.languageType = GTH_LANGUAGE_TYPE_KOREAN;
			else if( !stricmp( token, "ENGLISH" ) ) g_cgv.languageType = GTH_LANGUAGE_TYPE_ENGLISH;
			else if( !stricmp( token, "JAPANESE" ) ) g_cgv.languageType = GTH_LANGUAGE_TYPE_JAPANESE;
			else if( !stricmp( token, "CHINESE" ) ) g_cgv.languageType = GTH_LANGUAGE_TYPE_SIMPLIFIED_CHINESE;
			else if( !stricmp( token, "TAIWAN" ) ) g_cgv.languageType = GTH_LANGUAGE_TYPE_TRADITIONAL_CHINESE; 
		}
	}

	return true;
}

void GTH_SetMyCharacterEvent( Fx_CHARACTER_t *character )
{























































































































































































}

void GTH_SetMyCharacterMouseEvent( Fx_CHARACTER_t *character )
{
	vec3_t dir;
	vec3_t target;
	vec3_t angles;
	float dist, attackLength;


	
	if( !character->atk_changeEvent ) return;

	switch( g_cgv.clickEvent )
	{
		
	case GTH_CEV_CHAR_NONE :
		{
			if( g_cursor->IsPlayerTargetedMove() )
			{
				if( g_cursor->IsOverPointerTargetPos( character->position ) )
				{
					g_cursor->QuitPlayerTargetedMove();
					g_cgv.resendFlag = true;
					character->event = GTH_EV_CHAR_IDLE;
				}
			}
		}
		break;
	
		
	case GTH_CEV_CHAR_IDLE :
		{
			g_cgv.resendFlag = true;
			g_cgv.resendTargeting = false;
			g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
		}
		break;
	
		
	case GTH_CEV_CHAR_ATTACK :
		{
			dist = g_GameTarget.GTH_GetTargetDistance( character );
			if( dist < 0.00000f )
			{
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				character->event = GTH_EV_CHAR_IDLE;
				g_cgv.resendTargeting = false;
				g_cgv.saveEvent = -1;
				return;
			}

			if( character->isTransform )
			{
				attackLength = FindTransformAttackLength(character->transformEntityIdx, character->transformType);
				if( attackLength == 0.0f )
					attackLength = (float)g_cgv.myCharacterInfo->calAttackLength;
			}
			else
			{
				attackLength = (float)g_cgv.myCharacterInfo->calAttackLength;
			}

			attackLength = (float)attackLength * 0.8f;
			
			g_cgv.mouseClickEventDistance = attackLength;
			if( dist < attackLength )
			{
				g_cursor->QuitPlayerTargetedMove();
				
				g_GameTarget.GTH_SetAngleToTarget( character );

				

				if( character->ani_curGroup != FX_ANIM_GROUP_ATTACK )
				{



					character->event = GTH_EV_CHAR_ATTACK;
					character->atk_playAnim = false;
					g_cgv.resendTargeting = false;
					g_cgv.resendFlag = true;
					g_cgv.resendAttack = true;
				}
				else
				{
					
					
					g_cgv.saveEvent = GTH_EV_CHAR_ATTACK;
				}
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;	
				return;
			}
			else
			{
				g_cgv.resendTargeting = true;
			}
		}
		break;

		
	case GTH_CEV_CHAR_SKILL :
		{
			
			if( !g_SkillLogic.GTH_SkillClickEvent() ) 
			{
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				return;
			}
		}
		break;

		
	case GTH_CEV_CHAR_NPC_REQUEST :
		{
			dist = g_GameTarget.GTH_GetTargetDistance( character );
			if( dist < 0.00000f )
			{
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				character->event = GTH_EV_CHAR_IDLE;
				g_cgv.resendTargeting = false;
				g_cgv.saveEvent = -1;
				return;
			}
			g_cgv.mouseClickEventDistance = AVAILABLE_DISTANCE_ENTITYMENU * 0.8f;
			
			if( dist < AVAILABLE_DISTANCE_ENTITYMENU * 0.8f )
			{
				g_cursor->QuitPlayerTargetedMove();
				
				
				

				
				
				
				
		
		
						
						
							
													
						
						
							if(g_pApp->m_myCharacter->chaosPoint>29500)
							{
								if(g_cgv.pTargetChar->tableIdx == 13 || g_cgv.pTargetChar->tableIdx == 14 || 
									g_cgv.pTargetChar->tableIdx == 18 || g_cgv.pTargetChar->tableIdx == 104 || 
									g_cgv.pTargetChar->tableIdx == 105 || g_cgv.pTargetChar->tableIdx == 106 || 
									g_cgv.pTargetChar->tableIdx == 107||g_cgv.pTargetChar->tableIdx == 108||
									g_cgv.pTargetChar->tableIdx == 109 ||g_cgv.pTargetChar->tableIdx == 110 ||
									g_cgv.pTargetChar->tableIdx == 111 ||g_cgv.pTargetChar->tableIdx == 112||
									g_cgv.pTargetChar->tableIdx == 113 ||g_cgv.pTargetChar->tableIdx == 119||
									g_cgv.pTargetChar->tableIdx == 120 ||g_cgv.pTargetChar->tableIdx == 121||
									g_cgv.pTargetChar->tableIdx == 122 ||g_cgv.pTargetChar->tableIdx == 123||
									g_cgv.pTargetChar->tableIdx == 124)
								{
										g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 771 ) );
								}
		
								else 
								{
										GTH_SendMessage_RequestNpcChat( g_cgv.pTargetChar->idxOnServer );
								}
							
							}
						
							else
							{
								GTH_SendMessage_RequestNpcChat( g_cgv.pTargetChar->idxOnServer );
							}
							
						
						
		
			
			
			
		
				g_cursor->QuitPlayerTargetedMove();
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				character->event = GTH_EV_CHAR_IDLE;
				g_cgv.resendFlag = true;
				g_cgv.resendTargeting = false;
				return;
			}
			else
			{
				g_cgv.resendTargeting = true;
			}
		}
		break;
		
		
	case GTH_CEV_CHAR_PC_REQUEST :
		{
			dist = g_GameTarget.GTH_GetTargetDistance( character );
			if( dist < 0.00000f )
			{
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				character->event = GTH_EV_CHAR_IDLE;
				g_cgv.resendTargeting = false;
				g_cgv.saveEvent = -1;

				return;
			}
			g_cgv.mouseClickEventDistance = AVAILABLE_DISTANCE_ENTITYMENU * 0.8f;
			
			if( dist < AVAILABLE_DISTANCE_ENTITYMENU * 0.8f )
			{
				g_cursor->QuitPlayerTargetedMove();
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				character->event = GTH_EV_CHAR_IDLE;
				g_cgv.resendFlag = true;
				g_cgv.resendTargeting = false;

				
				
				if( g_cgv.pTargetChar->boothState == 2 )
				{
					
					
					if(  g_pApp->m_myCharacter->chaosPoint > 29500 )
					{
						
						g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 772 ) );
					}
					else
					{
						GTH_SendMessage_ItemSystem_BoothBuyOpen( g_cgv.pTargetChar->idxOnServer );
					}
				}
				return;
			}
			else
			{
				g_cgv.resendTargeting = true;
			}
		}
		break;
	case GTH_CEV_CHAR_FOLLOW :
		{
			dist = g_GameTarget.GTH_GetTargetDistance( character );

			
			if( g_ifMng->CheckConditionalWindow() ) 
			{
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				character->event = GTH_EV_CHAR_IDLE;
				g_cgv.resendTargeting = false;
				g_cgv.saveEvent = -1;
				return;
			}
			

			if( dist < 0.00000f )			 					
			{
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				character->event = GTH_EV_CHAR_IDLE;
				g_cgv.resendTargeting = false;
				g_cgv.saveEvent = -1;
				return;
			}
			g_cgv.mouseClickEventDistance = AVAILABLE_DISTANCE_ENTITYMENU * 0.8f;
			
			if( dist < AVAILABLE_DISTANCE_ENTITYMENU * 0.8f )
			{
				g_cursor->QuitPlayerTargetedMove();
				character->event = GTH_EV_CHAR_IDLE;
				return;
			}
			else
			{
				g_cgv.resendTargeting = true;
			}
		}
		break;

		
	case GTH_CEV_CHAR_PICKUP_ITEM :
		{
			if( !g_cursor->IsPlayerTargetedMove() ) break;
			
			g_cursor->GetTargetPos( target );
			dist = VectorDistance( target, character->position );
			g_cgv.mouseClickEventDistance = AVAILABLE_DISTANCE_PICKUPITEM * 0.5f;
			if( dist < AVAILABLE_DISTANCE_PICKUPITEM * 0.5f )
			{
				if( g_cgv.targetItemIdxOnServer >= 0 ) GTH_SendMessage_PickUpItem( g_cgv.targetItemIdxOnServer );
				g_cursor->QuitPlayerTargetedMove();
				g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
				character->event = GTH_EV_CHAR_IDLE;
				g_cgv.resendFlag = true;
				g_cgv.resendTargeting = false;
				return;
			}
		}
		break;
	}

	if( g_cursor->IsPlayerTargetedMove() )
	{
		if( !g_cursor->IsAdjustedPlayerAngle() )
		{
			g_cursor->GetTargetPos( target );
			VectorSubtract( dir , target , character->position );
			dir[2] = 0.0f;
			VectorNormalize( dir );
			VectorToAngles( dir , angles );
			
			VectorCopy(character->mousePos, target);
			
			character->mouseAngle = angles[ YAW ];	
			g_cursor->SetPlayerAngleAdjusted();
			
			character->event = GTH_EV_CHAR_MOUSERUN;
		}
	}

	
	if ( GTH_MyCharacter_CheckCollision() )
	{
		character->event = GTH_EV_CHAR_IDLE;
		g_cgv.resendFlag = true;
		g_cgv.resendTargeting = false;
		g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
	}
	
	if ( character->event == GTH_EV_CHAR_IDLE)
	{
		g_cgv.keyRun = false;
		g_cursor->QuitPlayerTargetedMove();
	}
}

void GTH_SendMyCharacterEvent()
{
	
	if(TRUE == g_pApp->m_myCharacter->m_bObserveMode) return;
	
	if ( g_pApp->m_myCharacter->event != g_pApp->m_myCharacter->oldEvent || g_cgv.resendFlag )
	{
		g_cgv.resendFlag = false;
		
		
		switch ( g_pApp->m_myCharacter->event )
		{
		case GTH_EV_CHAR_IDLE:
			GTH_SendEventMessage_Idle();
			GTH_UpdateSendedZPos();

		break;

		case GTH_EV_CHAR_WALK:
			GTH_SendEventMessage_Walk();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_WALKLEFT:
			GTH_SendEventMessage_WalkLeft();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_WALKRIGHT:
			GTH_SendEventMessage_WalkRight();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_BACK:
			GTH_SendEventMessage_Back();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_BACKLEFT:
			GTH_SendEventMessage_BackLeft();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_BACKRIGHT:
			GTH_SendEventMessage_BackRight();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_RUN:
			GTH_SendEventMessage_Run();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_RUNLEFT:
			GTH_SendEventMessage_RunLeft();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_RUNRIGHT:
			GTH_SendEventMessage_RunRight();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_TURNLEFT:
			GTH_SendEventMessage_TurnLeft();
			break;

		case GTH_EV_CHAR_TURNRIGHT:
			GTH_SendEventMessage_TurnRight();
			break;

		case GTH_EV_CHAR_MOVELEFT:
			GTH_SendEventMessage_MoveLeft();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_MOVERIGHT:
			GTH_SendEventMessage_MoveRight();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_ATTACK:
			if( g_cgv.resendAttack )
				GTH_SendEventMessage_Attack();

			g_cgv.resendAttack = false;
			break;

		case GTH_EV_CHAR_MOUSEWALK:
			GTH_SendEventMessage_MouseWalk();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_MOUSERUN:
			GTH_SendEventMessage_MouseRun();
			GTH_UpdateSendedZPos();
			break;

		case GTH_EV_CHAR_SIT:
			GTH_SendEventMessage_Sit();
			break;

		case GTH_EV_CHAR_STAND:
			GTH_SendEventMessage_Stand();
			break;

		case GTH_EV_CHAR_SKILL:			
			if( g_cgv.sendSkillFlag )
			{
				
				
				










				GTH_SendEventMessage_Skill();
				
				g_SkillLogic.UnSelectSkill();
	
			}
			break;
			
		case GTH_EV_CHAR_BEFORE_TELEPORT:
			if( g_cgv.sendTeleportFlag )
			{
				GTH_SendEventMessage_Stand();
				GTH_SendEventMessage_Before_Teleport();
			}
			break;

		case GTH_EV_CHAR_TELEPORT:
			if( g_cgv.sendSkillFlag )
			{
			}
			break;

		
		case GTH_EV_CHAR_SHOUT:
			GTH_SendEventMessage_SocialAnimation(GTH_EV_CHAR_SHOUT);
			break;
		case GTH_EV_CHAR_BOW:
			GTH_SendEventMessage_SocialAnimation(GTH_EV_CHAR_BOW);
			break;
		default:
			break;
		}
		g_pApp->m_myCharacter->oldEvent = g_pApp->m_myCharacter->event;

	}
}

void GTH_ExecuteCharacterEvent()
{
	int i;
	Fx_CHARACTER_t *character;
	int numCharacter;
	int direction;

	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i<numCharacter; i++, character++)
	{
		
		if(TRUE == character->m_bObserveMode)	continue;
		
		
		if( character->entityType == ENTITY_MONSTER || character->entityType == ENTITY_NPC )
		{
			g_ifMng->m_numCtl->ReadyDamage( character );
			if( character->summonValidTime > 0.0f )
			{
				character->summonValidTime -= g_timer.GetElapsedMilliTime();
			}
		}
		else if( character->entityType == ENTITY_PC )
		{






			 


		}

		GTH_CheckValidCharacterEvent( character );
		
		switch(character->event)
		{
		case GTH_EV_CHAR_IDLE:
			GTH_Character_Idle( character );
			break;

		case GTH_EV_CHAR_WALK:
			if ( character->entityType == ENTITY_PC )
			{
				GTH_Character_Walk( character );
			}
			else if (character->entityType == ENTITY_MONSTER || character->entityType == ENTITY_NPC )
			{
				direction = GTH_AdjustDirection( character->mouseAngle, character->angles[YAW] );
				if (direction == 1)
				{
					GTH_Character_MouseRight( character );
				}
				else if (direction == -1)
				{
					GTH_Character_MouseLeft( character );
				}
				GTH_Character_MonsterWalk( character );
			}
			break;

		case GTH_EV_CHAR_BACK:
			GTH_Character_Back( character );
			break;

		case GTH_EV_CHAR_TURNLEFT:
			GTH_Character_TurnLeft( character );
			break;

		case GTH_EV_CHAR_TURNRIGHT:
			GTH_Character_TurnRight( character );
			break;

		case GTH_EV_CHAR_WALKLEFT:
			GTH_Character_Walk( character );
			GTH_Character_TurnLeft( character );
			break;

		case GTH_EV_CHAR_WALKRIGHT:
			GTH_Character_Walk( character );
			GTH_Character_TurnRight( character );
			break;

		case GTH_EV_CHAR_BACKLEFT:
			GTH_Character_Back( character );
			GTH_Character_TurnLeft( character );
			break;

		case GTH_EV_CHAR_BACKRIGHT:
			GTH_Character_Back( character );
			GTH_Character_TurnRight( character );
			break;

		case GTH_EV_CHAR_RUN:
			if ( character->entityType == ENTITY_PC )
			{
				GTH_Character_Run( character );
			}
			else if (character->entityType == ENTITY_MONSTER || character->entityType == ENTITY_NPC )
			{
				direction = GTH_AdjustDirection( character->mouseAngle, character->angles[YAW] );
				if (direction == 1)
				{
					GTH_Character_MouseRight( character );
				}
				else if (direction == -1)
				{
					GTH_Character_MouseLeft( character );
				}
				GTH_Character_MonsterRun( character );
			}
			break;

		case GTH_EV_CHAR_RUNLEFT:
			GTH_Character_Run( character );
			GTH_Character_TurnLeft( character );
			break;

		case GTH_EV_CHAR_RUNRIGHT:
			GTH_Character_Run( character );
			GTH_Character_TurnRight( character );
			break;

		case GTH_EV_CHAR_MOVELEFT:
			GTH_Character_MoveLeft( character );
			break;

		case GTH_EV_CHAR_MOVERIGHT:
			GTH_Character_MoveRight( character );
			break;

		case GTH_EV_CHAR_ATTACK:
			GTH_Character_Attack( character );
			break;

		case GTH_EV_CHAR_DIE:
			GTH_Character_Die( character );
			break;

		case GTH_EV_CHAR_RESPAWN:
			GTH_Character_Respawn( character );
			break;

		case GTH_EV_CHAR_MOUSEWALK:
			direction = GTH_AdjustDirection( character->mouseAngle, character->angles[YAW] );
			if (direction == 1)
			{
				GTH_Character_MouseRight( character );
			}
			else if (direction == -1)
			{
				GTH_Character_MouseLeft( character );
				
			}
				
			GTH_Character_MouseWalk( character );
			break;

		case GTH_EV_CHAR_MOUSERUN:
			direction = GTH_AdjustDirection( character->mouseAngle, character->angles[YAW] );
			if (direction == 1)
			{
				GTH_Character_MouseRight( character );
			}
			else if (direction == -1)
			{
				GTH_Character_MouseLeft( character );
				
			}

			GTH_Character_MouseRun( character );
			break;
			
		case GTH_EV_CHAR_STUN :
			GTH_Character_Stun( character );
			break;

		
		case GTH_EV_CHAR_PC_STUN :
			GTH_Character_PcStun( character );
			break;
			
		case GTH_EV_CHAR_SIT:
			GTH_Character_Sit( character );
			break;
		case GTH_EV_CHAR_STAND:
			GTH_Character_Stand( character );
			break;
		case GTH_EV_CHAR_SKILL :
			g_SkillApp.GTH_Character_Skill( character );
			break;

		case GTH_EV_CHAR_SKILLCASTING :
			g_SkillApp.GTH_Character_SkillCasting( character );
			break;

		case GTH_EV_CHAR_PUSHED:

			break;

		case GTH_EV_CHAR_SHOUT:
			GTH_Character_Shout( character );
			break;
		case GTH_EV_CHAR_BOW:
			GTH_Character_Bow( character );
			break;

		default:
			break;
		}

		g_soundMng->Play( character );

		
		GTH_DeadReckoning( character );

	} 
}


void GTH_DeadReckoning(Fx_CHARACTER_t *character)
{
	if( character->event == GTH_EV_CHAR_MOUSERUN || character->event == GTH_EV_CHAR_MOUSEWALK )
		return;


	
	
	
	if( character->event == GTH_EV_CHAR_PUSHED )
	{
		character->useInterpolation = false;
		return;
	}

	if ( character->useInterpolation ) 
	{
		GTH_CharacterInterpolation( character );
	}
}

void GTH_CharacterInterpolation(Fx_CHARACTER_t *character)
{
	float distance, speed;
	vec3_t delta, position;

	distance = GTH_GetDistance2D(character->syncPosition, character->position);
	speed = INTERPOLATION_SPEED * character->moveSpeed * g_timer.GetElapsedTime();
	if (distance > speed)
	{
		VectorSubtract(delta, character->syncPosition, character->position);
		delta[2] = 0;
		VectorNormalize( delta );
		VectorScale( delta , delta , speed );
		VectorCopy( position , character->position );
		g_move->Pmove( position , delta , character->angles );
		VectorCopy( character->position , position );
	}
	else
	{
		VectorCopy(character->position, character->syncPosition);
		character->useInterpolation = false;
	}
}


void GTH_Character_Idle(Fx_CHARACTER_t *character)
{
	GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_IDLE);
}

void GTH_Character_Walk(Fx_CHARACTER_t *character)
{
	vec3_t delta, position;
	float speed;

	character->SetupVectors();
	character->GetFwdVector( delta );
	delta[2] = 0;
	VectorNormalize( delta );
	speed = SPEED_CONSTANT * character->moveSpeed * g_timer.GetElapsedTime();
	VectorScale( delta , delta , speed );

	VectorCopy( position , character->position );
	g_move->Pmove( position , delta , character->angles );
	VectorCopy( character->position , position );

	
	VectorCopy( position , character->syncPosition );
	g_move->Pmove( position , delta , character->angles );
	VectorCopy( character->syncPosition , position );

	GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_WALK);
}

void GTH_Character_MonsterWalk(Fx_CHARACTER_t *character)
{
	vec3_t delta, position;
	float speed;
	vec3_t dir;



	VectorClear( dir );
	dir[YAW] = character->mouseAngle;

	AngleToVector( dir , delta );

	speed = SPEED_CONSTANT * character->moveSpeed * g_timer.GetElapsedTime();
	VectorScale( delta , delta , speed );

	VectorCopy( position , character->position );
	g_move->Pmove( position , delta , dir );
	VectorCopy( character->position , position );

	
	VectorCopy( position , character->syncPosition );
	g_move->Pmove( position , delta , dir );
	VectorCopy( character->syncPosition , position );

	GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_WALK);
}


void GTH_Character_MonsterRun(Fx_CHARACTER_t *character)
{
	vec3_t delta, position;
	float speed;
	vec3_t dir;

	VectorClear( dir );
	dir[YAW] = character->mouseAngle;

	AngleToVector( dir , delta );

	
	speed = MONSTER_RUN_SCALE * SPEED_CONSTANT * character->moveSpeed * g_timer.GetElapsedTime();

	VectorScale( delta , delta , speed );

	VectorCopy( position , character->position );
	g_move->Pmove( position , delta , dir );
	VectorCopy( character->position , position );

	
	VectorCopy( position , character->syncPosition );
	g_move->Pmove( position , delta , dir );
	VectorCopy( character->syncPosition , position );

	GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_RUN );
}

void GTH_Character_Back(Fx_CHARACTER_t *character)
{
	vec3_t delta, position;
	float speed;

	character->SetupVectors();
	character->GetFwdVector( delta );
	VectorNegate( delta , delta );
	delta[2] = 0;
	VectorNormalize( delta );
	speed = BACK_SCALE * SPEED_CONSTANT * character->moveSpeed * g_timer.GetElapsedTime();
	VectorScale( delta , delta , speed );

	VectorCopy( position , character->position );
	g_move->Pmove( position , delta , character->angles );
	VectorCopy( character->position , position );

	
	VectorCopy( position , character->syncPosition );
	g_move->Pmove( position , delta , character->angles );
	VectorCopy( character->syncPosition , position );

	GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_WALK_BACK);
}


void GTH_Character_Run(Fx_CHARACTER_t *character)
{
	vec3_t delta, position;
	float speed;

	character->SetupVectors();
	character->GetFwdVector( delta );
	delta[2] = 0;
	VectorNormalize( delta );
	speed = RUN_SCALE * SPEED_CONSTANT * character->moveSpeed * g_timer.GetElapsedTime();

	VectorScale( delta , delta , speed );

	VectorCopy( position , character->position );
	g_move->Pmove( position , delta , character->angles );
	VectorCopy( character->position , position );

	
	VectorCopy( position , character->syncPosition );
	g_move->Pmove( position , delta , character->angles );
	VectorCopy( character->syncPosition , position );

	GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_RUN );

}

void GTH_Character_TurnLeft(Fx_CHARACTER_t *character)
{
	vec3_t rotate;

	VectorClear( rotate );

	rotate[ YAW ] = (float) TURN_SCALE * g_timer.GetElapsedTime();
	VectorAdd( character->angles , character->angles , rotate );
	if ( character->angles[ YAW ] >= 360 ) character->angles[ YAW ] -= 360.0f;

	character->SetupVectors();
}

void GTH_Character_TurnRight(Fx_CHARACTER_t *character)
{
	vec3_t rotate;

	VectorClear( rotate );

	rotate[ YAW ] = -(float) TURN_SCALE * g_timer.GetElapsedTime();
	VectorAdd( character->angles , character->angles , rotate );
	if ( character->angles[ YAW ] < 0 ) character->angles[ YAW ] += 360.0f;

	character->SetupVectors();
}

void GTH_Character_MoveLeft(Fx_CHARACTER_t *character)
{
	vec3_t delta, position;
	float speed;

	character->SetupVectors();
	character->GetRightVector( delta );
	VectorNegate( delta , delta );
	delta[2] = 0;
	VectorNormalize( delta );
	speed = SPEED_CONSTANT * character->moveSpeed * g_timer.GetElapsedTime();
	VectorScale( delta , delta , speed );

	VectorCopy( position , character->position );
	g_move->Pmove( position , delta , character->angles );
	VectorCopy( character->position , position );

	
	VectorCopy( position , character->syncPosition );
	g_move->Pmove( position , delta , character->angles );
	VectorCopy( character->syncPosition , position );

	GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_WALK);
}

void GTH_Character_MoveRight(Fx_CHARACTER_t *character)
{
	vec3_t delta, position;
	float speed;

	character->SetupVectors();
	character->GetRightVector( delta );
	delta[2] = 0;
	VectorNormalize( delta );
	speed = SPEED_CONSTANT * character->moveSpeed * g_timer.GetElapsedTime();
	VectorScale( delta , delta , speed );

	VectorCopy( position , character->position );
	g_move->Pmove( position , delta , character->angles );
	VectorCopy( character->position , position );

	
	VectorCopy( position , character->syncPosition );
	g_move->Pmove( position , delta , character->angles );
	VectorCopy( character->syncPosition , position );

	GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_WALK);
}
 
void GTH_Character_Attack(Fx_CHARACTER_t *character)
{
	int itemIdx = -1;
	int tableIdx, attackID;
	float attackSpeed;

	Fx_CHARACTER_t* defender = NULL;

	if( character->curRA <= 0 || character->event == GTH_EV_CHAR_DIE ) return;

	
	if( character->entityType == ENTITY_PC )
	{
		if( character->atk_playAnim )
		{
			
			if( character->isTransform )
			{
				attackID = ANIM_ITEM_ATTACK_0;
			}
			else if( character->attackComboType == ATTACK_COMBO_TYPE_RANGE || character->attackComboType == ATTACK_COMBO_TYPE_NEAR_RANGE ) 
			{
				attackID = ANIM_ITEM_ATTACK_3;
			}
			else if( character->attackComboType == ATTACK_COMBO_TYPE_NOT )
			{
				attackID = ANIM_ITEM_ATTACK_3;
			}
			else if( character->attackComboType == ATTACK_COMBO_TYPE_0 )
			{
				attackID = ANIM_ITEM_ATTACK_0;
			}
			else if( character->attackComboType == ATTACK_COMBO_TYPE_1 )
			{
				attackID = ANIM_ITEM_ATTACK_1;
			}
			else if( character->attackComboType == ATTACK_COMBO_TYPE_2 )
			{
				attackID = ANIM_ITEM_ATTACK_2;
			}
			else attackID = ANIM_ITEM_ATTACK_3;

			attackSpeed = character->stat_attackSpeed;

			GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, attackID, true, &attackSpeed );			
		} 
		else
		{
			GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_PRE_ATTACK );

			
			ShowLogInfo("프리어텍셋팅");
			
			
		}
	}
	else if( character->entityType == ENTITY_MONSTER || character->entityType == ENTITY_NPC )
	{
		if( character->ani_curSetID == ANIM_ITEM_ATTACK_0 )
		{
			GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_IDLE );
		}
		else if( character->atk_playAnim )
		{
			character->atk_playAnim = false;
			attackSpeed = character->stat_attackSpeed;
			
			
			if( attackSpeed > 1200.0f ) attackSpeed = 1200.0f;
			GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_ATTACK_0, true, &attackSpeed );

			tableIdx = character->tableIdx;
	
		} 
		else GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_IDLE );
	}
}

void GTH_ChangeAnimation(Fx_CHARACTER_t *character, int	aniSetType,	int aniSetID, bool in_forceUpdate, float* attackSpeed,
						bool in_forceChange	)
{
	int mainClass, itemIdx, monSetIdx;
	int tableIdx;
	
	if( character->entityType == ENTITY_PC )
	{
		if( !character->isTransform )
		{
			tableIdx = character->tableIdx;
			if( (! in_forceUpdate) && character->ani_curSetType == aniSetType && character->ani_curSetID == aniSetID ) 
			{
				return;
			}
			if( !( character->ani_curSetType == aniSetType && character->ani_curSetID == aniSetID ) ) 
			{
				if( g_charManager->m_pAnimManager->IsComboAttack( character->ani_curIdx ) && character->entityType == ENTITY_PC ) 
				{
					if( aniSetID != ANIM_ITEM_PRE_ATTACK ) in_forceChange = true;
				}
			}
			
			if (aniSetType == ANIMTYPE_BYITEM)
			{
				if ( character->equipNumber > 0 )
				{
					itemIdx = character->equipment[0];
					mainClass = g_itemTable[itemIdx].classIdx;
					
					if ( g_itemClassTable[mainClass].attackAtt )
					{
						g_charManager->ChangeAnimation(character->ID, aniSetType, g_itemTable[itemIdx].attackAni[tableIdx], aniSetID,	attackSpeed, in_forceChange	);
					}
					else
					{
						g_charManager->ChangeAnimation(character->ID, aniSetType, 0, aniSetID,	attackSpeed, in_forceChange );
					}
				}
				else
				{
					g_charManager->ChangeAnimation( character->ID, aniSetType, 0, aniSetID,	attackSpeed, in_forceChange );
				}
			} else if( aniSetType == ANIMTYPE_BYSKILL )
			{
				g_charManager->ChangeAnimation(character->ID, aniSetType, 0, aniSetID,	attackSpeed, in_forceChange );
			}

		}
		else
		{
			if( character->ani_curSetID == aniSetID ) return;
			
			monSetIdx = g_monsterTable[character->transformEntityIdx].aniSetType;
			if (aniSetType == ANIMTYPE_BYITEM)
			{
				g_charManager->ChangeAnimation( character->ID, monSetIdx, 0, aniSetID,	attackSpeed, in_forceChange );
			} else if( aniSetType == ANIMTYPE_BYSKILL )
			{
				g_charManager->ChangeAnimation( character->ID, monSetIdx, 0, ANIM_ITEM_ATTACK_0, attackSpeed, in_forceChange );
			}
		}
	}
	else if( character->entityType == ENTITY_MONSTER || character->entityType == ENTITY_NPC )
	{
		if( character->ani_curSetID == aniSetID ) return;
		
		monSetIdx = g_monsterTable[character->tableIdx].aniSetType;				
		g_charManager->ChangeAnimation( character->ID, monSetIdx, 0, aniSetID,	attackSpeed, in_forceChange );										

	}
}

void GTH_Character_Die(Fx_CHARACTER_t *character)
{
	character->isAlive = false;
	character->curRA = 0;
	
	if( character->isNewChar )
	{
		GTH_ChangeAnimation( character, ANIMTYPE_BYITEM, ANIM_ITEM_DEAD_0, true, NULL, true );
		return;
	}

	if( character->idxOnServer == g_cgv.myPCIdxOnServer &&
		character->entityType == ENTITY_PC )
	{
		g_cgv.resendTargeting = false;

		g_cursor->QuitPlayerTargetedMove();
		g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
		if( g_musicMng->IsPlaying( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_PC_HEARTBEAT_0 ) ) ) g_musicMng->Stop( GTH_SOUND_ID_PC_HEARTBEAT_0 );
	}

	if (character->ani_curSetID == ANIM_ITEM_DYING_0 || character->ani_curSetID == ANIM_ITEM_DEAD_0)
		GTH_ChangeAnimation( character, ANIMTYPE_BYITEM, ANIM_ITEM_DEAD_0);
	else
	if (character->ani_curSetID != ANIM_ITEM_DYING_0 )
	{
		GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_DYING_0, NULL, NULL, true);
	}
}

void GTH_Character_Respawn(Fx_CHARACTER_t *character)
{
	vec3_t delta;

	VectorClear(delta);
	g_move->Pmove( character->position , delta , character->angles );

	GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_IDLE, NULL, NULL, true );

	
	if(	!GTH_IsGameMaster ( character )	)
	{
		 
		g_effectMng ->Add ( GTH_EFFECT_CLASS_MODEL , 7 , character->position , NULL, character ->angles , 0.0f, character ->entityType, character ->idxOnServer );
	}
	
	character->event = GTH_EV_CHAR_IDLE;

	
	if( character->idxOnServer == g_cgv.myPCIdxOnServer && character->entityType == ENTITY_PC ) 
	{
		


		g_cursor->QuitPlayerTargetedMove();
		g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
		GTH_EnableMove();
	}
}

void GTH_Character_MouseLeft(Fx_CHARACTER_t *character)
{
	vec3_t rotate;

	VectorClear( rotate );
	rotate[ YAW ] = (float) TURN_SCALE * 3 * g_timer.GetElapsedTime();

	if ( rotate[YAW] > abs(character->mouseAngle - character->angles[YAW]) )
		character->angles[YAW] = character->mouseAngle;
	else
		VectorAdd( character->angles , character->angles , rotate );

	if ( character->angles[ YAW ] >= 360 ) character->angles[ YAW ] -= 360.0f;

	character->SetupVectors();
}

void GTH_Character_MouseRight(Fx_CHARACTER_t *character)
{
	vec3_t rotate;

	VectorClear( rotate );
	rotate[ YAW ] = (float) TURN_SCALE * 3 * g_timer.GetElapsedTime();

	if ( rotate[YAW] > abs(character->mouseAngle - character->angles[YAW]) )
		character->angles[YAW] = character->mouseAngle;
	else
	{
		rotate[ YAW ] = - rotate[ YAW ];
		VectorAdd( character->angles , character->angles , rotate );
	}

	if ( character->angles[ YAW ] < 0 ) character->angles[ YAW ] += 360.0f;

	character->SetupVectors();
}

void GTH_Character_MouseWalk(Fx_CHARACTER_t *character)
{
	vec3_t delta, position;
	float speed;
	vec3_t dir;

	float distance;

	VectorClear( dir );
	dir[YAW] = character->mouseAngle;

	AngleToVector( dir , delta );

	speed = SPEED_CONSTANT * character->moveSpeed * g_timer.GetElapsedTime();
	distance = GTH_GetDistance2D(character->mousePos, character->position);
	if ( speed + 2.0f >= distance) 
	{
		speed = 0;
		VectorScale( delta , delta , speed );
		character->position[0] = character->mousePos[0];
		character->position[1] = character->mousePos[1];

		VectorCopy( position , character->position );
		g_move->Pmove( position , delta , dir );
		VectorCopy( character->position , position );

		
		VectorCopy( character->syncPosition , character->position );

		if( !( g_cgv.clickEvent == GTH_CEV_CHAR_IDLE || g_cgv.clickEvent == GTH_CEV_CHAR_NONE ) ) return;

		character->oldEvent = character->event;
		character->event = GTH_EV_CHAR_IDLE;
	}
	else
	{
		VectorScale( delta , delta , speed );
		VectorCopy( position , character->position );
		g_move->Pmove( position , delta , dir );
		VectorCopy( character->position , position );
	
		
		VectorCopy( position , character->syncPosition );
		g_move->Pmove( position , delta , dir);
		VectorCopy( character->syncPosition , position );
	}

	GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_WALK);
}

void GTH_Character_MouseRun(Fx_CHARACTER_t *character)
{
	vec3_t delta, position;
	float speed;
	vec3_t dir;

	float distance;

	VectorClear( dir );
	dir[YAW] = character->mouseAngle;

	AngleToVector( dir , delta );

	speed = RUN_SCALE * SPEED_CONSTANT * character->moveSpeed * g_timer.GetElapsedTime();
	distance = GTH_GetDistance2D(character->mousePos, character->position);
	if (speed + 4.0f >= distance) 
	{
		speed = 0;
		VectorScale( delta , delta , speed );
		character->position[0] = character->mousePos[0];
		character->position[1] = character->mousePos[1];

		VectorCopy( position , character->position );
		g_move->Pmove( position , delta , dir );
		VectorCopy( character->position , position );

		
		VectorCopy( character->syncPosition , character->position );

		if( !( g_cgv.clickEvent == GTH_CEV_CHAR_IDLE || g_cgv.clickEvent == GTH_CEV_CHAR_NONE ) ) return;

		character->oldEvent = character->event;
		character->event = GTH_EV_CHAR_IDLE;
	}
	else
	{
		VectorScale( delta , delta , speed );
		VectorCopy( position , character->position );
		g_move->Pmove( position , delta , dir );
		VectorCopy( character->position , position );

		
		VectorCopy( position , character->syncPosition );
		g_move->Pmove( position , delta , dir );
		VectorCopy( character->syncPosition , position );
	}

	GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_RUN);
}

void GTH_Character_PcStun( Fx_CHARACTER_t *character )
{
	if (character->ani_curSetID == ANIM_ITEM_ATTACKED_0 ) 
	{
		GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_IDLE );
		return;
	}

	if( character->flag_replayAnim == 1 )
	{
		character->flag_replayAnim = 0;

		float delay = (float)character->stat_recoveryTime;
		GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_ATTACKED_0, true, &delay, true );
		
		
		if( character->idxOnServer == g_cgv.myPCIdxOnServer && character->entityType == ENTITY_PC ) 
		{
			g_cursor->QuitPlayerTargetedMove();
			g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
		}
	}
}

void GTH_Character_Stun( Fx_CHARACTER_t *character )
{
	if (character->ani_curSetID == ANIM_ITEM_ATTACKED_0 ) 
	{
		GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_IDLE );
		return;
	}

	if( character->flag_replayAnim == 1 )
	{
		character->flag_replayAnim = 0;


		
		
		GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_ATTACKED_0, true, NULL, true );
		
		
		if( character->idxOnServer == g_cgv.myPCIdxOnServer && character->entityType == ENTITY_PC ) 
		{
			g_cursor->QuitPlayerTargetedMove();
			g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
		}
	}
}
void GTH_Character_Sit( Fx_CHARACTER_t* character ,float fSpeed ,bool bNewFlag)
{
	if( character->ani_curSetID == ANIM_ITEM_SIT_IDLE ) return;

	if( character->isNewChar && bNewFlag)
	{
		GTH_ChangeAnimation( character, ANIMTYPE_BYITEM, ANIM_ITEM_SIT_IDLE, true, NULL, true );
		return;
	}
	if( character->ani_curSetID == ANIM_ITEM_SIT )
	{
		GTH_ChangeAnimation( character, ANIMTYPE_BYITEM, ANIM_ITEM_SIT_IDLE );
		return;
	}

	if( character->idxOnServer == g_cgv.myPCIdxOnServer &&
		character->entityType == ENTITY_PC )
	{
		g_cgv.resendTargeting = false;

		g_cursor->QuitPlayerTargetedMove();
		g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
	}
	float attackSpeed = fSpeed;

	GTH_ChangeAnimation( character, ANIMTYPE_BYITEM, ANIM_ITEM_SIT, true, &attackSpeed, true );
	g_charManager->GenerateOtherCharacter( character->ID );
}
void GTH_Character_Stand( Fx_CHARACTER_t* character )
{
	if( character->ani_curSetID == ANIM_ITEM_STAND ) 
	{
		GTH_ChangeAnimation( character, ANIMTYPE_BYITEM, ANIM_ITEM_IDLE );
		return;
	}
	if( character->ani_curSetID == ANIM_ITEM_IDLE )
	{
		if( character->idxOnServer == g_cgv.myPCIdxOnServer &&
			character->entityType == ENTITY_PC )
		{
			g_cgv.resendTargeting = false;

			g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
			character->event = GTH_EV_CHAR_IDLE;
			GTH_EnableMove();
		}
		return;
	}
	float attackSpeed = 2000.0f;
	if( character->ani_curSetID == ANIM_ITEM_SIT_IDLE )
	{
		GTH_ChangeAnimation( character, ANIMTYPE_BYITEM, ANIM_ITEM_STAND, false, &attackSpeed );
		g_charManager->GenerateOtherCharacter( character->ID );
	}
}





void GTH_Character_Shout( Fx_CHARACTER_t* character )
{
	if( character->ani_curSetID == ANIM_ITEM_SOCIAL_SHOUT )
	{
		GTH_ChangeAnimation( character, ANIMTYPE_BYITEM, ANIM_ITEM_IDLE );
		GTH_EnableMove();
		return;
	}

	float attackSpeed = 2000.0f;
	if( character->ani_curSetID == ANIM_ITEM_IDLE )
	{
		GTH_ChangeAnimation( character, ANIMTYPE_BYITEM, ANIM_ITEM_SOCIAL_SHOUT, false, &attackSpeed );
		g_charManager->GenerateOtherCharacter( character->ID );

		g_cgv.resendTargeting = false;
		g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
		character->event = GTH_EV_CHAR_IDLE;
		
		
		
	}
}


void GTH_Character_Bow( Fx_CHARACTER_t* character )
{
	if( character->ani_curSetID == ANIM_ITEM_SOCIAL_BOW ) 
	{
		GTH_ChangeAnimation( character, ANIMTYPE_BYITEM, ANIM_ITEM_IDLE );
		GTH_EnableMove();
		return;
	}

	float attackSpeed = 2000.0f;
	if( character->ani_curSetID == ANIM_ITEM_IDLE )
	{
		GTH_ChangeAnimation( character, ANIMTYPE_BYITEM, ANIM_ITEM_SOCIAL_BOW, false, &attackSpeed );
		g_charManager->GenerateOtherCharacter( character->ID );

		
		g_cgv.resendTargeting = false;		
		
		g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
		character->event = GTH_EV_CHAR_IDLE;
		
		
		
	}
}



float GTH_GetDistance(vec3_t s, vec3_t t)
{
	return sqrt( (t[0]-s[0])*(t[0]-s[0]) + (t[1]-s[1])*(t[1]-s[1]) + (t[2]-s[2])*(t[2]-s[2]) );
}

float GTH_GetDistance2D(vec3_t s, vec3_t t)
{
	return sqrt( (t[0]-s[0])*(t[0]-s[0]) + (t[1]-s[1])*(t[1]-s[1]) );
}

void GTH_SetWorldInfo()
{
	int worldIdx;
#ifdef _GTH_ONLINE_VERSION
	worldIdx = g_cgv.syncCharacter[0].worldIdx;
		
	if ( worldIdx >= g_cgv.worldTableNumber )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,46), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,160), 1, IF_MESSAGE_EXIT_CLIENT );
		return;
	}
	strcpy ( g_cgv.worldInfo, g_cgv.worldTable[worldIdx].cfgFile );
	g_cgv.currentWorldIdx = worldIdx;

#else
	if( !g_cgv.offLineStartFlag  )
	{
		memcpy( &g_cgv.syncCharacter[0], &g_cgv.syncCharacter[g_cgv.selectedGameDataIdx], sizeof( syncCharacter_t ) );
		g_cgv.offLineStartFlag = true;
		VectorCopy( g_cgv.syncCharacter[0].position, g_cgv.worldTable[g_cgv.syncCharacter[0].worldIdx].startPos );
	}
	else
	{
		
		g_cgv.syncCharacter[0].equipNumber = 0;
		for( int i = 0; i < EQUIP_RING; i ++ )
		{
			if( g_cgv.myCharacterInfo->equipment[i]	== -1 ) continue;
			g_cgv.syncCharacter[0].equipment[g_cgv.syncCharacter[0].equipNumber] = 
				g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->equipment[i]].itemTableIdx;
				
			g_cgv.syncCharacter[0].equipNumber ++;
		}
	}

	worldIdx = g_cgv.syncCharacter[0].worldIdx;
	if ( worldIdx >= g_cgv.worldTableNumber )
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,46), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,114), 1, IF_MESSAGE_EXIT_CLIENT );
		return;
	}
	strcpy ( g_cgv.worldInfo, g_cgv.worldTable[worldIdx].cfgFile );
#endif
	
	g_ifMng->m_changeStateFlag = true;
}

void GTH_RequestChangeWorld( int targetWorldIdx, int targetGateIdx )
{
	if ( g_pApp->m_myCharacter->worldIdx == targetWorldIdx )
	{
		
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,115), D3DCOLOR_ARGB(255, 255, 55, 55) );
		return;
	}



	
#ifdef _GTH_ONLINE_VERSION

	struct tagTransportWorldMarking{
		int targetWorldIdx;
		int targetGateIdx;	
		vec3_t position;
		int    timer;
	};
	static struct tagTransportWorldMarking TrWorldMark={-1,-1,0,};

	if((TrWorldMark.targetWorldIdx == targetWorldIdx) &&
		(TrWorldMark.targetGateIdx == targetGateIdx)){

		if(1 == VectorCompare(g_cgv.syncCharacter[0].position,TrWorldMark.position)) return;
		if((timeGetTime() - TrWorldMark.timer) < 10 * 1000) return;
	}		

	TrWorldMark.targetWorldIdx = targetWorldIdx;
	TrWorldMark.targetGateIdx = targetGateIdx;	
	TrWorldMark.timer = timeGetTime();
	VectorCopy(TrWorldMark.position,g_cgv.syncCharacter[0].position);
	

	GTH_SendMessage_TransportWorld( targetWorldIdx, targetGateIdx ); 

#else
	g_cgv.syncCharacter[0].worldIdx = targetWorldIdx;
	VectorCopy(g_cgv.syncCharacter[0].position, g_cgv.worldTable[targetWorldIdx].spawnPos[targetGateIdx]);
	VectorSet(g_cgv.syncCharacter[0].angles, 0, g_cgv.worldTable[targetWorldIdx].spawnYaw[targetGateIdx], 0);
	
	GTH_PrepareChangeWorld();
#endif
}

void GTH_PrepareChangeWorld()
{
	g_cgv.changeServerFlag = 0;
	
	g_ifMng->m_changeStateFlag = true;
	g_cgv.restartState = RESTART_NONE;

	
	g_ifMng->m_itemWin->ClearLockItems();

	
	g_NewDanBattleSystem.InitDanBattleInfo();
	g_ifMng->m_DanWarBattleWin->Enable(FALSE);
}


void GTH_GenerateMyCharacter()
{
	int i, j;
	int itemIdx;


	memset( g_cgv.syncCharacter, 0, sizeof( g_cgv.syncCharacter ) );

#ifndef	_GTH_ONLINE_VERSION
	g_script.Load( "offline.cfg" );
	g_script.BeginParse();

	token = g_script.GetNextToken( true );
	if( !stricmp( token, "CHARACTER_NUMBER" ) )
	{
		token = g_script.GetNextToken( true );
		g_cgv.savedDataNumber = atoi( token );
	}
	curIdx = -1;
	
	GTH_InitSavedData();

	while( 1 )
	{
		token = g_script.GetNextToken( true );
		if( !token[0] ) break;
		if( !stricmp( token, "CLASS" ) )
		{
			curIdx ++;
			token = g_script.GetNextToken( true );
			g_cgv.savedData[curIdx].pcClass = atoi( token );
			g_cgv.savedData[curIdx].characterID = curIdx;
		} else if( !stricmp( token, "SKINPART" ) )
		{
			token = g_script.GetNextToken( true );
			g_cgv.savedData[curIdx].skinPart[0] = atoi( token );
			token = g_script.GetNextToken( true );
			g_cgv.savedData[curIdx].skinPart[1] = atoi( token );
			token = g_script.GetNextToken( true );
			g_cgv.savedData[curIdx].skinPart[2] = atoi( token );
			token = g_script.GetNextToken( true );
			g_cgv.savedData[curIdx].skinPart[3] = atoi( token );
		} else if( !stricmp( token, "ITEMNUMBER" ) )
		{
			token = g_script.GetNextToken( true );
			g_cgv.savedData[curIdx].itemNumber = atoi( token );

			
			token = g_script.GetNextToken( true );
			for( i = 0; i < g_cgv.savedData[curIdx].itemNumber; i ++ )
			{
				token = g_script.GetNextToken( true );
				g_cgv.savedData[curIdx].item[i].itemTableIdx = atoi( token );
				g_cgv.savedData[curIdx].item[i].durability = 100;
			}
		} else if( !stricmp( token, "INVENTORY" ) )
		{
			token = g_script.GetNextToken( true );
			itemPos = atoi( token );
			token = g_script.GetNextToken( true );
			itemIdx = atoi( token );
			g_cgv.savedData[curIdx].inventory[itemPos] = itemIdx;
		} else if( !stricmp( token, "NAME" ) )
		{
			token = g_script.GetNextToken( true );
			strcpy( g_cgv.savedData[curIdx].name, token );
		} else if( !stricmp( token, "WORLDIDX" ) )
		{
			token = g_script.GetNextToken( true );
			g_cgv.savedData[curIdx].worldIdx = atoi( token );
		}
		g_cgv.savedData[curIdx].selectedGen = 0;
		g_cgv.savedData[curIdx].genLevel = 1;
		g_cgv.savedData[curIdx].genExp = 100;
		g_cgv.savedData[curIdx].exp = 100;
		g_cgv.savedData[curIdx].level = 22;
		g_cgv.savedData[curIdx].curRA = 100;
		g_cgv.savedData[curIdx].force = 50;
		g_cgv.savedData[curIdx].soulpower = 50;
		g_cgv.savedData[curIdx].agility = 50;
		g_cgv.savedData[curIdx].vitality = 50;

		for (j=0; j<MAX_NUMBER_OF_HOTKEY; j++)
		{
			g_cgv.savedData[curIdx].hotkeyType[j] = 0;
			g_cgv.savedData[curIdx].hotkeyIdx[j] = -1;
		}
		g_cgv.savedData[curIdx].questPoint = 0;
		g_cgv.savedData[curIdx].questNumber = 0;
		
		for (j=0; j<MAX_COUNT_QUEST_VARIABLE; j++)
			g_cgv.savedData[curIdx].questVar[j].iVar = 65280;
		
		for (j=0; j<MAX_COUNT_QUEST_INVENTORY; j++)
			g_cgv.savedData[curIdx].questInventory[j].iItem = 65535;
		
		for( j = 0; j < MAX_COUNT_QUEST_STATUS; j ++ )
		{
			memset( &g_cgv.savedData[curIdx].questStatus[j], 0, sizeof( questStatus_t ) );
			g_cgv.savedData[curIdx].questStatus[j].questIdx = -1;
		}
	}

#endif

	
	
	
	













































































































































































































	
	

	int SaveDataIdx = 0;
	if (g_cgv.characterSlotMode == globalVariable_t::CharacterSlotMode::BACK)
		SaveDataIdx = 3;
	
	g_cgv.syncCharacterNumber = 3;

	for( int SyncCharacterIdx= 0; SyncCharacterIdx < g_cgv.syncCharacterNumber; SyncCharacterIdx ++ , SaveDataIdx++)
	{
		if( g_cgv.savedData[SaveDataIdx].characterID == -1 )
		{
			g_cgv.syncCharacter[SyncCharacterIdx].entityType = ENTITY_NPC;
			
			g_cgv.syncCharacter[SyncCharacterIdx].tableIdx = 11;
			g_cgv.syncCharacter[SyncCharacterIdx].aliveState = MON_ALIVE_STATE_NORMAL;
		}
		else
		{
			g_cgv.syncCharacter[SyncCharacterIdx].entityType = ENTITY_PC;
			g_cgv.syncCharacter[SyncCharacterIdx].tableIdx = g_cgv.savedData[SaveDataIdx].pcClass;
			g_cgv.syncCharacter[SyncCharacterIdx].skinPart[0] = g_cgv.savedData[SaveDataIdx].skinPart[0];
			g_cgv.syncCharacter[SyncCharacterIdx].skinPart[1] = g_cgv.savedData[SaveDataIdx].skinPart[1];
			g_cgv.syncCharacter[SyncCharacterIdx].skinPart[2] = g_cgv.savedData[SaveDataIdx].skinPart[2];
			g_cgv.syncCharacter[SyncCharacterIdx].skinPart[3] = g_cgv.savedData[SaveDataIdx].skinPart[3];
			
			g_cgv.syncCharacter[SyncCharacterIdx].curRA = g_cgv.savedData[SaveDataIdx].curRA;
			g_cgv.syncCharacter[SyncCharacterIdx].curSA = g_cgv.savedData[SaveDataIdx].curSA;
			g_cgv.syncCharacter[SyncCharacterIdx].speed = 3;
			g_cgv.syncCharacter[SyncCharacterIdx].event = GTH_EV_CHAR_IDLE;
			
			strcpy( g_cgv.syncCharacter[SyncCharacterIdx].name, g_cgv.savedData[SaveDataIdx].name );

			
			g_cgv.savedData[SaveDataIdx].calForce = 512;
			g_cgv.savedData[SaveDataIdx].calSoulpower = 512;
			g_cgv.savedData[SaveDataIdx].calAgility = 512;
			g_cgv.savedData[SaveDataIdx].calVitality = 512;

			
			g_cgv.syncCharacter[SyncCharacterIdx].equipNumber = 0;
			for( j = 0; j < EQUIP_RING; j ++ )
			{
				if( g_cgv.savedData[SaveDataIdx].equipment[j]	== -1 ) continue;
				if( j == EQUIP_WEAPON_1 && g_cgv.savedData[SaveDataIdx].curWeapon == 1 ) continue;
				if( j == EQUIP_WEAPON_2 && g_cgv.savedData[SaveDataIdx].curWeapon == 0 ) continue;
				itemIdx = g_cgv.savedData[SaveDataIdx].equipment[j];
				if( !GTH_CheckRequire( &g_cgv.savedData[SaveDataIdx], 0, &g_cgv.savedData[SaveDataIdx].item[itemIdx] ) ) continue;

				g_cgv.syncCharacter[SyncCharacterIdx].equipment[g_cgv.syncCharacter[SyncCharacterIdx].equipNumber] = 
					g_cgv.savedData[SaveDataIdx].item[g_cgv.savedData[SaveDataIdx].equipment[j]].itemTableIdx;
				
				g_cgv.syncCharacter[SyncCharacterIdx].equipNumber ++;
			}
			
			
			g_cgv.syncCharacter[SyncCharacterIdx].calMaxRA = 100;
			g_cgv.syncCharacter[SyncCharacterIdx].calMaxSA = 100;
		}
				
		g_cgv.syncCharacter[SyncCharacterIdx].worldIdx = g_cgv.savedData[SaveDataIdx].worldIdx;
		g_cgv.syncCharacter[SyncCharacterIdx].idxOnServer = SyncCharacterIdx;
		
		if( SyncCharacterIdx == 0 )
		{
			g_cgv.syncCharacter[SyncCharacterIdx].position[0]	= -85.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].position[1]	= -174.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].position[2]	= -904.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].angles [0]	= 0.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].angles [1]	= 285.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].angles [2]	= 0.0f;
		}
		if( SyncCharacterIdx == 1 )
		{
			g_cgv.syncCharacter[SyncCharacterIdx].position[0]	= 0.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].position[1]	= -100.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].position[2]	= -904.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].angles [0]	= 0.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].angles [1]	= 270.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].angles [2]	= 0.0f;
		}
		if( SyncCharacterIdx == 2 )
		{
			g_cgv.syncCharacter[SyncCharacterIdx].position[0]	= 85.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].position[1]	= -174.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].position[2]	= -904.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].angles [0]	= 0.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].angles [1]	= 255.0f;
			g_cgv.syncCharacter[SyncCharacterIdx].angles [2]	= 0.0f;
		}
		g_cgv.syncCharacter[SyncCharacterIdx].generalSpeed = 100;
	}





	
	
	
	int numGen = 4;
	for( int SelectCharacterIdx = 3; SelectCharacterIdx < numGen + 3; SelectCharacterIdx ++ )
	{
		g_cgv.syncCharacter[SelectCharacterIdx].entityType = ENTITY_PC;
		
		g_cgv.syncCharacter[SelectCharacterIdx].skinPart[0] = 0;
		g_cgv.syncCharacter[SelectCharacterIdx].skinPart[1] = 0;
		g_cgv.syncCharacter[SelectCharacterIdx].skinPart[2] = 0;
		g_cgv.syncCharacter[SelectCharacterIdx].skinPart[3] = 0;
		
		g_cgv.syncCharacter[SelectCharacterIdx].curRA		= 0;
		g_cgv.syncCharacter[SelectCharacterIdx].curSA		= 0;
		g_cgv.syncCharacter[SelectCharacterIdx].calMaxRA		= 0;
		g_cgv.syncCharacter[SelectCharacterIdx].calMaxSA		= 0;
		
		
		g_cgv.syncCharacter[SelectCharacterIdx].speed = 3;
		g_cgv.syncCharacter[SelectCharacterIdx].event = GTH_EV_CHAR_IDLE;
		
		strcpy( g_cgv.syncCharacter[SelectCharacterIdx].name, "" );
		
		g_cgv.syncCharacter[SelectCharacterIdx].equipNumber = 0;	
		g_cgv.syncCharacter[SelectCharacterIdx].worldIdx = 3;



		
		if( SelectCharacterIdx == 3 )
		{
			g_cgv.syncCharacter[SelectCharacterIdx].tableIdx = 0;

			g_cgv.syncCharacter[SelectCharacterIdx].position[0] = 140.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].position[1] = 788.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].position[2] = -850.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].angles [0] = -4.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].angles [1] = 180.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].angles [2] = 0.0f;










		}
		
		if( SelectCharacterIdx == 4 )
		{
			g_cgv.syncCharacter[SelectCharacterIdx].tableIdx = 1;

			g_cgv.syncCharacter[SelectCharacterIdx].position[0] = -134.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].position[1] = 788.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].position[2] = -850.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].angles [0] = -4.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].angles [1] = 0.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].angles [2] = 0.0f;









		}
		
		if( SelectCharacterIdx == 5 )
		{
			g_cgv.syncCharacter[SelectCharacterIdx].tableIdx = 2;

			g_cgv.syncCharacter[SelectCharacterIdx].position[0] = 140.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].position[1] = 588.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].position[2] = -850.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].angles [0] = -4.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].angles [1] = 180.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].angles [2] = 0.0f;










		}
		
		if( SelectCharacterIdx == 6 )
		{
			g_cgv.syncCharacter[SelectCharacterIdx].tableIdx = 3;

			g_cgv.syncCharacter[SelectCharacterIdx].position[0] = -134.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].position[1] = 588.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].position[2] = -850.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].angles [0] = -4.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].angles [1] = 0.0f;
			g_cgv.syncCharacter[SelectCharacterIdx].angles [2] = 0.0f;










		}

		g_cgv.syncCharacter[SelectCharacterIdx].idxOnServer = i;
		g_cgv.syncCharacter[SelectCharacterIdx].generalSpeed = 100;

		g_cgv.syncCharacterNumber ++;
	}


}

int GTH_Temp_GetTargetMonster()
{
	float distance;
	int i, number;
	Fx_CHARACTER_t	 *character;

	character = &g_charManager->m_Characters[0];
	number = g_charManager->m_iNumCharacters;

	for (i=0; i<number; i++, character++)
	{
		if ( !character->isAlive ) continue;
		if (character->entityType != ENTITY_MONSTER) continue;

		distance = GTH_GetDistance(g_pApp->m_myCharacter->position, character->position);
		if (distance < 200.0f)
			return character->idxOnServer;
	}

	return -1;
}



int GTH_AdjustNextAngle( float target, float angle )
{
	float td[2];					
	
	if( target != angle )
	{
		td[0] = target - angle;
		if( td[0] < 0 ) td[0] += 360.0f;
		td[1] = 360.0f - td[0];

		
		if( td[0] > td[1] ) 
		{
			
			if( td[1] == 0 ) 
			{
				return( 0 );	
			}
			return( 1 ); 
		}
		else 
		{

			if( td[0] == 0 )
			{
				return( 0 );	
			}
			else return( -1 ); 
		}
				
	}
	else 
	{
		return( 0 );			
	}
}

int GTH_AdjustDirection( float target, float angle )
{
	float td[2];			
	
	if( target != angle )
	{
		td[0] = target - angle;
		if( td[0] < 0 ) td[0] += 360.0f;
		td[1] = 360.0f - td[0];

		
		if( td[0] > td[1] ) 
		{
			if( td[1] == 0 ) 
			{
				return( 0 );	
			}
			return( 1 ); 
		}
		else 
		{
			if( td[0] == 0 )
			{
				return( 0 );	
			}
			else return( -1 ); 
		}
				
	}
	else 
	{
		return( 0 );			
	}
}












void GTH_InitItem( item_t *item )
{

	memset(item, 0, sizeof(item_t));

	item->itemTableIdx = -1;
	for (int i=0; i<MAX_NUMBER_OF_ITEM_OPTION; i++)
	{
		item->optionIdx[i] = -1;
	}
	
	SetItemUseFlag(item, item_t::sItemExtendInfo::enumItemUseFlag::ITEM_USE_FLAG_UN_CHECK);
	SetItemSpawnType(item, ITEM_SPAWN_INFO::enumItemSpawnType::ITEM_SPAWN_TYPE_UNKNOW);
	
	
}


void GTH_DeleteMyItem( item_t *item )
{
	int i;

	memset(item, 0, sizeof(item_t));

	item->itemTableIdx = -1;
	for (i=0; i<MAX_NUMBER_OF_ITEM_OPTION; i++)
	{
		item->optionIdx[i] = -1;
	}
	g_cgv.myCharacterInfo->itemNumber --;

}


int GTH_AddMyItem( int itemIdx, item_t *item )
{
	if (itemIdx >= MAX_NUMBER_OF_OWNITEM)
	{		
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,46), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,157), 1, IF_MESSAGE_NONE );
		return 0;
	}

	if ( g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx != -1)
	{
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,46), g_LPACK.GetMassage(LPACK_TYPE_NORMAL,158), 1, IF_MESSAGE_NONE );
		return 0;
	}

	memcpy(&g_cgv.myCharacterInfo->item[itemIdx], item, sizeof(item_t));
	g_cgv.myCharacterInfo->itemNumber ++;
	
	return 1;
}


int GTH_AddMyItem( item_t *item )
{
	int i;

	for (i=0; i<MAX_NUMBER_OF_OWNITEM; i++)
	{
		if ( g_cgv.myCharacterInfo->item[i].itemTableIdx == -1 ) break;
	}
	
	if (i >= MAX_NUMBER_OF_OWNITEM)
		return -1;

	memcpy(&g_cgv.myCharacterInfo->item[i], item, sizeof(item_t));

	g_cgv.myCharacterInfo->itemNumber++;

	return i;
}

int GTH_StackMyItem( item_t *item )
{
	int i;
	int itemIdx;

	
	if( GTH_IsStackItem( item ) )
	{
		for( i = 0; i < MAX_INVENTORY_SIZE; i ++ )
		{
			if( g_cgv.myCharacterInfo->inventory[i] < 0 ) continue;
			itemIdx = g_cgv.myCharacterInfo->inventory[i];

			if( GTH_CheckStackItem( item, &g_cgv.myCharacterInfo->item[itemIdx] ) )
			{
				return( itemIdx );
			}
		}
	}
	return( -1 );
}

int GTH_FindEmptyInventoryPos()
{
	int i;

	for( i = 0; i < MAX_INVENTORY_SIZE; i ++ )
	{
		if( g_cgv.myCharacterInfo->inventory[i] == -1 )
		{
			return i;
		}
	}
	return -1;
}


int GTH_StackMyItemForDepot( item_t *item )
{
	int i;
	int itemIdx;

	
	if( GTH_IsStackItem( item ) )
	{
		for( i = 0; i < MAX_DEPOT_SIZE; i ++ )
		{
			if( g_cgv.myCharacterInfo->depot[i] < 0 ) continue;
			itemIdx = g_cgv.myCharacterInfo->depot[i];

			if( GTH_CheckStackItem( item, &g_cgv.myCharacterInfo->item[itemIdx] ) )
			{
				return( itemIdx );
			}
		}
	}
	return( -1 );
}


int GTH_FindEmptyDepotPos()
{
	int i;

	for( i = 0; i < MAX_DEPOT_SIZE; i ++ )
	{
		if( g_cgv.myCharacterInfo->depot[i] == -1 )
		{
			return i;
		}
	}
	return -1;
}

int GTH_MyCharacter_CheckCollision()
{
	vec3_t position, delta, dir;
	float speed;
	int flag;

	switch ( g_pApp->m_myCharacter->event )
	{
	case GTH_EV_CHAR_WALK : 	
	case GTH_EV_CHAR_MOUSEWALK :
	case GTH_EV_CHAR_WALKRIGHT :
	case GTH_EV_CHAR_WALKLEFT :
		speed = SPEED_CONSTANT * g_pApp->m_myCharacter->moveSpeed * g_timer.GetElapsedTime();
		break;

	case GTH_EV_CHAR_BACK :
	case GTH_EV_CHAR_BACKRIGHT :
	case GTH_EV_CHAR_BACKLEFT :
		speed = -1 * BACK_SCALE * SPEED_CONSTANT * g_pApp->m_myCharacter->moveSpeed * g_timer.GetElapsedTime();
		break;

	case GTH_EV_CHAR_RUN :
	case GTH_EV_CHAR_MOUSERUN :
	case GTH_EV_CHAR_RUNLEFT :
	case GTH_EV_CHAR_RUNRIGHT :
		speed = RUN_SCALE * SPEED_CONSTANT * g_pApp->m_myCharacter->moveSpeed * g_timer.GetElapsedTime();
		break;


	case GTH_EV_CHAR_MOVERIGHT :
		speed = SPEED_CONSTANT * g_pApp->m_myCharacter->moveSpeed * g_timer.GetElapsedTime();
		break;

	case GTH_EV_CHAR_MOVELEFT :
		speed = -1 * SPEED_CONSTANT * g_pApp->m_myCharacter->moveSpeed * g_timer.GetElapsedTime();
		break;

	case GTH_EV_CHAR_ATTACK :
		speed = 50;	
		break;

	default: 
		return 0;
	}

	if (g_pApp->m_myCharacter->event == GTH_EV_CHAR_MOUSEWALK || g_pApp->m_myCharacter->event == GTH_EV_CHAR_MOUSERUN)
	{
		VectorClear( dir );
		dir[YAW] = g_pApp->m_myCharacter->mouseAngle;
		AngleToVector( dir , delta );
	}
	else
	{
		g_pApp->m_myCharacter->SetupVectors();
		g_pApp->m_myCharacter->GetFwdVector( delta );
		delta[2] = 0;
		VectorNormalize( delta );
	}

	VectorScale( delta , delta , speed );
	VectorCopy( position , g_pApp->m_myCharacter->position );
	flag = g_move->Pmove( position , delta , g_pApp->m_myCharacter->angles );

	









	if ( flag != COLLIDE_STARTSOLID )	
		return 0;

	return 1;
}

void GTH_UpdateSendedZPos()
{
	g_cgv.sendedZPos = g_pApp->m_myCharacter->position[2];
}

int GTH_MyCharacter_UpdatePosition()
{
	
	if(g_pApp->m_myCharacter->m_bObserveMode) return 1;
	
		
	float height;

	height = g_pApp->m_myCharacter->position[2] - g_cgv.sendedZPos;

	if ( abs( height ) < UPDATE_ZPOS_HEIGHT )
		return 0;

	if( abs( height ) > UPDATE_ZPOS_HEIGHT )
	{
		GTH_SendMessage_UpdatePosition();
	}
	
	GTH_UpdateSendedZPos();
	return 1;
}

int GTH_WorldToIndex( char *world )
{
	int i;

	for (i=0; i<g_cgv.worldTableNumber; i++)
	{
		if( stricmp( world , g_cgv.worldTable[i].name ) == 0 )
			return g_cgv.worldTable[i].idx;
	}
	return -1;
}

int GTH_LoadWorldTable()
{
	char *token;
	int idx, i;

	if( !g_script.Load( "worldtable.cfg" ) )
		return false;

	g_script.BeginParse();

	g_cgv.worldTableNumber = 0;
	idx = 0;

	while ( 1 )
	{
		token = g_script.GetNextToken( true );
		if( !token[0] ) break;

		if( !stricmp( token, "WORLD_IDX" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  return false;
			idx = atoi( token );
			g_cgv.worldTable[idx].idx = idx;
			g_cgv.worldTableNumber ++;
		}
		else if( !stricmp( token, "HAN_NAME" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  return false;
			strcpy( g_cgv.worldTable[idx].hanName, token );
		}
		else if( !stricmp( token, "NAME" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  return false;
			strcpy( g_cgv.worldTable[idx].name, token );
		}
		else if( !stricmp( token, "CFG_FILE_NAME" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  return false;
			strcpy( g_cgv.worldTable[idx].cfgFile, token );
		}
		else if( !stricmp( token, "START_POSITION" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  return false;
			g_cgv.worldTable[idx].startPos[0] = atof( token );

			token = g_script.GetNextToken( true );
			if( !token )  return false;
			g_cgv.worldTable[idx].startPos[1] = atof( token );

			token = g_script.GetNextToken( true );
			if( !token )  return false;
			g_cgv.worldTable[idx].startPos[2] = atof( token );
		}
		else if( !stricmp( token, "START_YAW" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  return false;
			g_cgv.worldTable[idx].startYaw = atof( token );
		}
		else if( !stricmp( token, "GATE_NUMBER" ) )
		{
			token = g_script.GetNextToken( true );
			if( !token )  return false;
			g_cgv.worldTable[idx].gateNumber = atoi( token );
		}
		else if( !stricmp( token, "GATE_IDX" ) )
		{
			for (i=0; i<g_cgv.worldTable[idx].gateNumber; i++)
			{
				token = g_script.GetNextToken( true );
				if( !token )  return false;
				g_cgv.worldTable[idx].gateIdx[i] = atoi( token );
			}
		}
		else if( !stricmp( token, "SPAWN_POSITION" ) )
		{
			for (i=0; i<g_cgv.worldTable[idx].gateNumber; i++)
			{
				token = g_script.GetNextToken( true );
				if( !token )  return false;
				g_cgv.worldTable[idx].spawnPos[i][0] = atof( token );
				token = g_script.GetNextToken( true );
				if( !token )  return false;
				g_cgv.worldTable[idx].spawnPos[i][1] = atof( token );
				token = g_script.GetNextToken( true );
				if( !token )  return false;
				g_cgv.worldTable[idx].spawnPos[i][2] = atof( token );
			}
		}
		else if( !stricmp( token, "SPAWN_YAW" ) )
		{
			for (i=0; i<g_cgv.worldTable[idx].gateNumber; i++)
			{
				token = g_script.GetNextToken( true );
				if( !token )  return false;
				g_cgv.worldTable[idx].spawnYaw[i] = atof( token );
			}
		}
		else if( !stricmp( token, "LINKED_WORLD_IDX" ) )
		{
			for (i=0; i<g_cgv.worldTable[idx].gateNumber; i++)
			{
				token = g_script.GetNextToken( true );
				if( !token )  return false;
				g_cgv.worldTable[idx].linkedWorldIdx[i] = atoi( token );
			}
		}
		else if( !stricmp( token, "LINKED_GATE_IDX" ) )
		{
			for (i=0; i<g_cgv.worldTable[idx].gateNumber; i++)
			{
				token = g_script.GetNextToken( true );
				if( !token )  return false;
				g_cgv.worldTable[idx].linkedGateIdx[i] = atoi( token );
			}
		}
	}

	return true;
}

void GTH_EpackProcess()
{
	
	g_cgv.epackRADelayTime -= g_timer.GetElapsedMilliTime();
	if( g_cgv.epackRADelayTime <= 0.0f ) 
	{
		g_cgv.epackRADelayTime = 0.0f;
	}
	g_cgv.epackSADelayTime -= g_timer.GetElapsedMilliTime();

	if( g_cgv.epackSADelayTime <= 0.0f ) 
	{
		g_cgv.epackSADelayTime = 0.0f;
	}
}




void GTH_SetItemObjectModel( int objectType, int itemTableIdx, int &classType, int &mdlID )
{
	

	classType = 0;
	mdlID = 0;
	switch( objectType )
	{
	case OBJECT_TYPE_ITEM :
		classType = (GTH_ITEM_CLASS_TYPE)g_itemTable[itemTableIdx].mdlItemType;
		mdlID = g_itemTable[itemTableIdx].mdlItemID;
		break;
	case OBJECT_TYPE_PORTAL :
		classType = GTH_ITEM_CLASS_ACCESSORY;
		
		mdlID = 3;
		break;
	}
	
	
	if( mdlID < 0 || classType < 0 )
	{
		classType = 8;
		mdlID = 2;
	}
}


void GTH_RecoverySafePosition()
{
	int worldIdx;
	vec3_t svrPosition, delta;

	
	VectorClear( delta );
	g_move->Pmove( g_pApp->m_myCharacter->position , delta , g_pApp->m_myCharacter->angles );



	svrPosition[2] = g_pApp->m_myCharacter->position[2] - g_pApp->m_worldBSPs[0]->m_models->bbox[2];

	if (svrPosition[2] < 100.0f) 
	{
		worldIdx = g_cgv.syncCharacter[0].worldIdx;
		VectorCopy(g_pApp->m_myCharacter->position, g_cgv.worldTable[worldIdx].startPos);
		g_pApp->m_myCharacter->angles[1] = g_cgv.worldTable[worldIdx].startYaw;

		GTH_SendMessage_UpdatePosition();
		GTH_UpdateSendedZPos();
		
	}
	else
	{
		
		g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 448 ), GTHCOLOR_ERRORMESSAGE );
	}
}

void GTH_InitPartyInfo()
{
	int i;
	memset( g_cgv.myCharacterInfo->organizerName, 0, sizeof( g_cgv.myCharacterInfo->organizerName ) );
	g_cgv.myCharacterInfo->organizeServer = -1;
	g_cgv.myCharacterInfo->organizeTime = 0;
	g_cgv.myCharacterInfo->isLeader = false;
	g_cgv.myCharacterInfo->memberNumber = 0;
	
	g_cgv.myCharacterInfo->m_bischange  = false;
	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		memset( &g_cgv.myCharacterInfo->member[i], 0, sizeof( partyMember_t ) );
		g_cgv.myCharacterInfo->member[i].serverIndex = -1;
	}
	
	g_cgv.myCharacterInfo->memberNumber = 0;
}

int GTH_CheckJob( myCharacterInfo_t *pc, int j0, int j1, int j2, int j3 )
{
	int reqBeforeAT, req1st, req2nd, req3rd;

	reqBeforeAT = 0;
	req1st = 0;
	req2nd = 0;
	req3rd = 0;
	
	switch( pc->pcJob )
	{
	case 0 :
		reqBeforeAT |= AT_MENTALITY;
		break;
	case 1 :
		reqBeforeAT |= AT_WIND;
		break;
	case 2 :
		reqBeforeAT |= AT_WATER;
		break;
	case 3:
		reqBeforeAT |= AT_EARTH;
		break;
	case 4 :
		reqBeforeAT |= AT_MENTALITY;
		req1st |= AT_DEVOTIONAL_KNIGHT;
		break;
	case 5 :
		reqBeforeAT |= AT_MENTALITY;
		req1st |= AT_ROMING_KNIGHT;
		break;
	case 6 :
		reqBeforeAT |= AT_WIND;
		req1st |= AT_PIECING_EYES;
		break;
	case 7 :
		reqBeforeAT |= AT_WIND;
		req1st |= AT_WINDY_ROMER;
		break;
	case 8 :
		reqBeforeAT |= AT_WATER;
		req1st |= AT_DESTROYER;
		break;
	case 9 :
		reqBeforeAT |= AT_WATER;
		req1st |= AT_GUARDIAN;
		break;
	case 10 :
		reqBeforeAT |= AT_WATER;
		req1st |= AT_PROPHECIER;
		break;
	case 11 :
		reqBeforeAT |= AT_EARTH;
		req1st |= AT_TRANSFORMER;
		break;
	case 12 :
		reqBeforeAT |= AT_EARTH;
		req1st |= AT_DEFENDER;
		break;
	case 13 :
		reqBeforeAT |= AT_EARTH;
		req1st |= AT_STRANGER;
		break;
	}
	if( j0 != 0 )
	{
		if( j0 & reqBeforeAT ) return 1;
	}
	if( j1 != 0 )
	{
		if( j1 & req1st ) return 1;
	}
	if( j2 != 0 )
	{
		if( j2 & req2nd ) return 1;
	}
	if( j3 != 0 )
	{
		if( j3 & req3rd ) return 1;
	}
	
	return 0;
}

int GTH_CheckJob( int job, int reqJob )
{
	
	if( reqJob == J_MENTALITY )
	{
		if( job == J_MENTALITY || job == J_DEVOTIONAL_KNIGHT || 
			job == J_ROMING_KNIGHT ) return 1;
	}
	if( reqJob == J_WIND )
	{
		if( job == J_WIND || job == J_PIECING_EYES ||
			job == J_WINDY_ROMER ) return 1;
	}
	if( reqJob == J_WATER )
	{
		if( job == J_WATER || job == J_DESTROYER || 
			job == J_GUARDIAN || job == J_PROPHECIER ) return 1;
	}
	if( reqJob == J_EARTH )
	{
		if( job == J_EARTH || job == J_TRANSFORMER || 
			job == J_DEFENDER || job == J_STRANGER ) return 1;
	}
	
	if( reqJob == job ) return 1;

	return 0;
}


int GTH_CheckRequire( myCharacterInfo_t *pc, int requireType, void* pReq )
{
	int level, genLevel, force, soul, agi, vit;
	int notReq;

	level = pc->level;
	genLevel = pc->genLevel;
	force = pc->calForce;
	soul = pc->calSoulpower;
	agi = pc->calAgility;
	vit = pc->calVitality;

	switch( requireType )
	{
		
	case REQUIRE_TYPE_ITEM :
		{
			item_t *item = static_cast<item_t*>(pReq);
			itemTable_t *pItem;
			pItem = &g_itemTable[item->itemTableIdx];
			if( pItem->wearPosition == ITEM_WEAR_POS_NONE ) return 1;

			
			notReq = false;

			
			if( !GTH_CheckJob( pc, pItem->reqBeforeAT, pItem->req1stAT, pItem->req2ndAT, pItem->req3rdAT) ) return 0;
			if( GTH_IsRepairItem( item ) && item->durability <= 0 ) return 0;

			if( !notReq )
			{
				if( pItem->reqForce > force ) return 0;
				if( pItem->reqSoulpower > soul ) return 0;
				if( pItem->reqAgility > agi ) return 0;
				if( pItem->reqVitality > vit ) return 0;
				if( item->reqLevel > level ) return 0;
			}

			return 1;
		}
	case REQUIRE_TYPE_SKILL :
		{
			
			int skillTableIdx = (* static_cast<int *>(pReq) );

			
			CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
			if(pSkillTable!=NULL) 
			{
				
				if( !GTH_CheckJob( pc->pcJob, pSkillTable->m_nJobType) ) return 0;
				if( genLevel < pSkillTable->GetSkillRequire()->m_nGenLevel ) return 0;
			}

			
			
			
			

			return 1;
		}
		break;
	}
	return 0;
}

int GTH_GetGenTypeFromJob( int pcJob )
{	
	if( pcJob == J_MENTALITY || pcJob == J_DEVOTIONAL_KNIGHT || 
		pcJob == J_ROMING_KNIGHT ) return( 0 );
	
	if( pcJob == J_WIND || pcJob == J_PIECING_EYES || 
		pcJob == J_WINDY_ROMER ) return( 1 );

	if( pcJob == J_WATER || pcJob == J_DESTROYER || 
		pcJob == J_GUARDIAN || pcJob == J_PROPHECIER ) return( 2 );

	if( pcJob == J_EARTH || pcJob == J_TRANSFORMER || 
		pcJob == J_DEFENDER || pcJob == J_STRANGER ) return( 3 );

	return( -1 );
}

int GTH_GetEquipPos( item_t *item, int equipPos )
{
	int itemTableIdx;
	int tableEquipPos;

	itemTableIdx = item->itemTableIdx;
	tableEquipPos = g_itemTable[itemTableIdx].wearPosition;

	if( tableEquipPos == ITEM_WEAR_POS_WEAPON )
	{
		if( equipPos == EQUIP_WEAPON_1 || equipPos == EQUIP_WEAPON_2 ) return( equipPos );
		return( EQUIP_WEAPON_1 );
	}
	else if( tableEquipPos >= ITEM_WEAR_POS_UPPER && ITEM_WEAR_POS_UPPER <= ITEM_WEAR_POS_EPACK )
	{
		return( tableEquipPos + 1 );
	}
	else return( -1 );
}

int GTH_IsRepairItem( item_t *item )
{
	int itemTableIdx = item->itemTableIdx;
	if( !g_itemTable[itemTableIdx].durYn ) return 0;
	if( !g_itemTable[itemTableIdx].repairYn ) return 0;
	if( g_itemTable[itemTableIdx].durability == 0 ) return 0;
	if( item->durability == g_itemTable[itemTableIdx].durability ) return 0;

	return 1;
}

int GTH_GetAllRepairCost( myCharacterInfo_t* pc, int curSE )
{
	int i, itemIdx, dur, cost = 0;
	int itemTableIdx; 
	int wearPosition;
	item_t *item;

	for( i = EQUIP_WEAPON_1; i < EQUIP_EPACK; i ++ )
	{
		itemIdx = pc->equipment[i];
		if( itemIdx < 0 ) continue;
		item = &pc->item[itemIdx];
		itemTableIdx = item->itemTableIdx;

		
		wearPosition = g_itemTable[itemTableIdx].wearPosition;
		
		if( !( wearPosition == ITEM_WEAR_POS_WEAPON ||
			wearPosition == ITEM_WEAR_POS_UPPER || wearPosition == ITEM_WEAR_POS_LOWER || 
			wearPosition == ITEM_WEAR_POS_AIR_PROTECTOR || wearPosition == ITEM_WEAR_POS_BODY_PROTECTOR ||
			wearPosition == ITEM_WEAR_POS_GLOVES || wearPosition == ITEM_WEAR_POS_BOOTS ) ) continue;

		if( item->durability >= g_itemTable[itemTableIdx].durability ) continue;
		if( g_itemTable[itemTableIdx].durability <= 0 ) continue;
		
		if( !( g_itemClassTable[g_itemTable[itemTableIdx].classIdx].attackAtt || 
			g_itemClassTable[g_itemTable[itemTableIdx].classIdx].defenseAtt ) ) continue;
		
		cost = GTH_GetRepairCost( item, curSE, dur );
		
		if( curSE < cost || dur == 0 ) continue;
		curSE -= cost;
	}
	return( pc->curChargeSE - curSE );
}

int GTH_GetRepairCost( item_t* item, int curSE, int &checkDur )
{
	int itemTableIdx;
	int cost, dur, decDur, totalPrice;
	float a;
	itemTableIdx = item->itemTableIdx;
	cost = g_itemTable[itemTableIdx].sellCost;
	cost = GTH_GetBuyPrice( item );
	cost = (int)( (float)cost * 0.1f );

	dur = g_itemTable[itemTableIdx].durability;

	if( item->durability > dur ) item->durability = dur;

	decDur = dur - item->durability;

	
	a = (float)cost / (float)( dur * dur );

	
	totalPrice = (int)( (float)( decDur * decDur ) * a );

	if( totalPrice == 0 ) totalPrice = 1;

	if( curSE < 0 ) return( totalPrice );

	
	if( curSE < totalPrice )
	{
		
		checkDur = (int)sqrtf( (float)curSE / a );
		totalPrice = curSE;
	}
	else checkDur = decDur;

	return( totalPrice );
}

int GTH_GetBuyPrice( item_t* item )
{
	int cost, realCost;
 
	if( GTH_IsStackItem( item ) )
	{
		cost = g_itemTable[item->itemTableIdx].sellCost;
		cost *= ( item->durability + 1 );
		return( cost );
	}

	
	switch( item->itemTableIdx )
	{
	case QUESTSCROLL_ITEM_INDEX:
		realCost = (int)( 2.5f * (float) g_questscrollTable[item->optionIdx[0]].data.price );
		break;
	default:
		{
			realCost = cost = g_itemTable[item->itemTableIdx].sellCost;
			for( int i = 0; i < item->optionNumber; i ++ )
			{
				if( item->optionIdx[i] < 0 ) continue;
				realCost += (int)( (float)cost * 0.1f );
			}
		}
		break;
	}
	if( realCost < 0 ) realCost = 0;
	return( realCost );
}

int GTH_GetSellPrice( item_t* item )
{
	float a, cost;
	int dur, sellcost;
	dur = g_itemTable[item->itemTableIdx].durability;
	cost = (float)GTH_GetBuyPrice( item );
	if( dur <= 0 ) a = cost;
	else a = cost - (float)GTH_GetRepairCost( item, -1, dur );
	a *= 0.3f;
	sellcost = (int)a;
	if( sellcost <= 0 ) sellcost = 0;
	return( sellcost );
}


int GTH_GetDepotPrice( item_t *item )
{
	
	return( 300 );
}

int GTH_IsConsumptiveItem( item_t* item )
{
	int itemTableIdx, classIdx;
	itemTableIdx = item->itemTableIdx;
	classIdx = g_itemTable[itemTableIdx].classIdx;

	if( !g_itemClassTable[classIdx].etcAtt ) return 0;
	if( g_itemTable[itemTableIdx].useFlag ) return 1;

	return 0;
}

int GTH_IsStackItem( item_t* item )
{
	int itemTableIdx;
	itemTableIdx = item->itemTableIdx;

	if( g_itemTable[itemTableIdx].stackFlag ) return 1;

	return 0;
}

int GTH_CheckStackItem( item_t* item1, item_t* item2 )
{
	if( !GTH_IsStackItem( item1 ) ) return( 0 );
	if( !GTH_IsStackItem( item2 ) ) return( 0 );
	if( item1->itemTableIdx == item2->itemTableIdx ) return( 1 );
	return( 0 );
}

int GTH_GetAttackAnimationIndex()
{
	int itemIdx, itemTableIdx, weaponType, classIdx, comboLevel;
	int attackComboType, aniSetIndex, attackID, animIndex;

	if( g_cgv.myCharacterInfo->calAttackLength < DEFAULT_RANGE_ATTACK_LEGNTH )
	{
		itemIdx = g_cgv.myCharacterInfo->equipment[EQUIP_WEAPON_1 + g_cgv.myCharacterInfo->curWeapon];

		attackComboType = ATTACK_COMBO_TYPE_NOT;

		if( itemIdx < 0 ) 
		{
			attackComboType = ATTACK_COMBO_TYPE_NOT;
			aniSetIndex = 0;
		}
		else
		{
			if( g_cgv.myCharacterInfo->level > 0 && g_cgv.myCharacterInfo->level <= 10 ) comboLevel = 0;
			else if( g_cgv.myCharacterInfo->level >= 11 && g_cgv.myCharacterInfo->level <= 30 ) comboLevel = 1;
			else comboLevel = 2;

			itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
			classIdx = g_itemTable[itemTableIdx].classIdx;
			weaponType = g_itemClassTable[classIdx].weaponType;

			aniSetIndex = g_itemTable[itemTableIdx].attackAni[g_cgv.myCharacterInfo->pcClass];

			switch( weaponType )
			{
				
			case -1 :
				attackComboType = ATTACK_COMBO_TYPE_NOT;
				break;
				
			case WEAPON_TYPE_DANGGER :
				attackComboType = ATTACK_COMBO_TYPE_NOT;
				break;
				
			case WEAPON_TYPE_ONEHAND_SWORD :
				attackComboType = ATTACK_COMBO_TYPE_0 + comboLevel;
				break;
				
			case WEAPON_TYPE_TWOHAND_SWORD :
				attackComboType = ATTACK_COMBO_TYPE_0 + comboLevel;
				break;
				
			case WEAPON_TYPE_MAGIC_BOW :
				attackComboType = ATTACK_COMBO_TYPE_NOT;
				break;
				
			case WEAPON_TYPE_STAFF :
				attackComboType = ATTACK_COMBO_TYPE_NOT;
				break;
				
			case WEAPON_TYPE_SPIRITOR :
				attackComboType = ATTACK_COMBO_TYPE_0 + comboLevel;
				break;
				
			case WEAPON_TYPE_CLAW :
				attackComboType = ATTACK_COMBO_TYPE_NOT;
				break;
			}
		}
	}
	else
	{
		itemIdx = g_cgv.myCharacterInfo->equipment[EQUIP_WEAPON_1 + g_cgv.myCharacterInfo->curWeapon];

		if( itemIdx < 0 ) 
		{
			attackComboType = ATTACK_COMBO_TYPE_NOT;
			aniSetIndex = 0;
		}
		else
		{
			itemTableIdx = g_cgv.myCharacterInfo->item[itemIdx].itemTableIdx;
			
			aniSetIndex = g_itemTable[itemTableIdx].attackAni[g_cgv.myCharacterInfo->pcClass];
			
			attackComboType = ATTACK_COMBO_TYPE_RANGE;
		}
	}
	
	if( attackComboType == ATTACK_COMBO_TYPE_RANGE || attackComboType == ATTACK_COMBO_TYPE_NEAR_RANGE ) 
	{
		attackID = ANIM_ITEM_ATTACK_3;
	}
	else if( attackComboType == ATTACK_COMBO_TYPE_NOT )
	{
		attackID = ANIM_ITEM_ATTACK_3;
	}
	else if( attackComboType == ATTACK_COMBO_TYPE_0 )
	{
		attackID = ANIM_ITEM_ATTACK_0;
	}
	else if( attackComboType == ATTACK_COMBO_TYPE_1 )
	{
		attackID = ANIM_ITEM_ATTACK_1;
	}
	else if( attackComboType == ATTACK_COMBO_TYPE_2 )
	{
		attackID = ANIM_ITEM_ATTACK_2;
	}
	else attackID = ANIM_ITEM_ATTACK_3;

	animIndex =	g_charManager->m_pCharLookupMng->GetAnimIDFromTable( ENTITY_PC, g_cgv.myCharacterInfo->pcClass, 
		ANIMTYPE_BYITEM, aniSetIndex, attackID, 0 );
	
	return( animIndex );
}

int GTH_GetItemType( int itemTableIdx )
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



void AddTargetList( char *name )
{
	
	
	if( g_pApp->m_myCharacter->pvpMode ) return;

	for( int i = 0; i < MAX_TARGET_PCLIST; i ++ )
	{
		if( !stricmp( name, g_cgv.myCharacterInfo->targetPCList[i] ) ) break;
	}
	
	if( i == MAX_TARGET_PCLIST )
	{
		for( i = 0; i < MAX_TARGET_PCLIST; i ++ )
		{
			if( g_cgv.myCharacterInfo->targetPCList[i][0] == 0 )
			{
				strcpy( g_cgv.myCharacterInfo->targetPCList[i], name );
				g_cgv.myCharacterInfo->targetPCListTime[i] = (int)g_timer.GetAppMilliTime();
				break;
			}
			else if( g_timer.GetAppMilliTime() - g_cgv.myCharacterInfo->targetPCListTime[i] > 6000 )
			{
				strcpy( g_cgv.myCharacterInfo->targetPCList[i], name );
				g_cgv.myCharacterInfo->targetPCListTime[i] = g_timer.GetAppMilliTime();
				break;
			}
		}
	}
	
	else
	{
		g_cgv.myCharacterInfo->targetPCListTime[i] = g_timer.GetAppMilliTime();
	}
}

int GetChaosClass( Fx_CHARACTER_t *character )
{
	if( character->chaosPoint == 0 ) return( CHAOS_PURE );
	
	else if( 0 < character->chaosPoint && character->chaosPoint <= 10 ) return( CHAOS_GENERAL );
	else if( character->chaosPoint > 10 && character->chaosPoint <=10000) return( CHAOS_DISORDER );
	else if( character->chaosPoint > 10000 && character->chaosPoint <=29500 ) return( CHAOS_MONSTER );
	return( CHAOS_ATROCITY );
}


int IsPKDisableWorld( int worldIdx )
{	
	
	
	if ( g_pApp->m_myCharacter->worldIdx == DAN_BATTLEMAP_NUM )
		return FALSE;




	int worldIdxList[] = {0,1,3,4,9,10,11,15};
	int num = sizeof( worldIdxList ) / 4;

	for (int i=0; i<num; i++)
	{
		if ( worldIdx == worldIdxList[i] )
			return 1;
	}
	return 0;
}



guildMarkInfo_t		g_guildMarkInfo[MAX_GUILDS];



bool GetGuildMarkInformationByFileName( const char *fname, int &serverGroupId, short &guildIdx, int &updateTime )
{
	int year, month, day;
	char buffer[32];

	
	

	
	

	
	if( fname[2] != '-' || fname[6] != '-' ) return false;

	memset( buffer, 0, sizeof(buffer) );


	buffer[0] = fname[0];
	buffer[1] = fname[1];

	serverGroupId = atoi( buffer );
	memset( buffer, 0, sizeof(buffer) );

	
	if( serverGroupId != g_cgv.selectedGroupIdx ) return false;


	buffer[0] = fname[3];
	buffer[1] = fname[4];
	buffer[2] = fname[5];

	guildIdx = atoi( buffer );
	memset( buffer, 0, sizeof(buffer) );

	
	if( guildIdx < 0 || guildIdx >= MAX_GUILDS ) return false;


	buffer[0] = fname[7];
	buffer[1] = fname[8];
	buffer[2] = fname[9];
	buffer[3] = fname[10];
	buffer[4] = fname[11];
	buffer[5] = fname[12];

	updateTime = atoi( buffer );
	memset( buffer, 0, sizeof(buffer) );

	buffer[0] = fname[7];
	buffer[1] = fname[8];

	
	year = atoi( buffer );
	memset( buffer, 0, sizeof(buffer) );

	buffer[0] = fname[9];
	buffer[1] = fname[10];

	
	month = atoi( buffer );
	memset( buffer, 0, sizeof(buffer) );

	buffer[0] = fname[11];
	buffer[1] = fname[12];

	
	day = atoi( buffer );
	memset( buffer, 0, sizeof(buffer) );

	
	
	

	return true;
}



void InitGuildMarkInfo()
{
	int i;

	for( i=0; i<MAX_GUILDS; i++)
	{
		g_guildMarkInfo[i].serverGroupId = -1;
		g_guildMarkInfo[i].guildIdx = -1;
		g_guildMarkInfo[i].updateTime = 0;
		memset( g_guildMarkInfo[i].fname, 0, 256 );
		g_guildMarkInfo[i].bLoaded = false;
	}
}


void MakeGuildMarkInfo()
{
	
	WIN32_FIND_DATA		FindFileData;
	HANDLE hFind;
	guildMarkInfo_t		guildMarkInfo;

	hFind = FindFirstFile( "..\\danmark\\?????????????.bmp", &FindFileData );
	
	if( hFind == INVALID_HANDLE_VALUE )
	{
		return;
	}

	while( 1 )
	{
		
		if( GetGuildMarkInformationByFileName( FindFileData.cFileName, guildMarkInfo.serverGroupId, guildMarkInfo.guildIdx, guildMarkInfo.updateTime ) )
		{
			if( strcmp( g_guildMarkInfo[guildMarkInfo.guildIdx].fname, FindFileData.cFileName ) != 0 )
			{
				g_guildMarkInfo[guildMarkInfo.guildIdx].bLoaded = false;
			}

			g_guildMarkInfo[guildMarkInfo.guildIdx].serverGroupId = guildMarkInfo.serverGroupId;
			g_guildMarkInfo[guildMarkInfo.guildIdx].guildIdx = guildMarkInfo.guildIdx;
			g_guildMarkInfo[guildMarkInfo.guildIdx].updateTime = guildMarkInfo.updateTime;
			strcpy( g_guildMarkInfo[guildMarkInfo.guildIdx].fname, FindFileData.cFileName );
		}

		if( !FindNextFile( hFind, &FindFileData ) )
			break;
	}
	FindClose( hFind );
}


void LoadGuildMark()
{
	Sleep(0);
	
	
	
}

#include <direct.h> 


bool MakeGuildMarkFile( short guildIdx, int updateTime, char *buffer )
{
	HANDLE file;
	char fname[256];

	
	
	mkdir( "../danmark/" );
	
	memset( fname, 0, 256 );
	sprintf( fname, "..\\danmark\\%02d-%03d-%06d.bmp", g_cgv.selectedGroupIdx, guildIdx, updateTime );

	file = CreateFile( (LPCSTR)fname, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( file == INVALID_HANDLE_VALUE )
		return	false;

	BITMAPFILEHEADER    bmpfh;
    BITMAPINFOHEADER    bmih;

	
	
	bmpfh.bfType = *((WORD*) "BM");
	bmpfh.bfSize = 822;
	bmpfh.bfReserved1 = 0;
	bmpfh.bfReserved2 = 0;
	bmpfh.bfOffBits = 54;

	
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = 16;
	bmih.biHeight = 16; 
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression		= BI_RGB; 
	bmih.biSizeImage		= 0;
    bmih.biXPelsPerMeter	= 0; 
	bmih.biYPelsPerMeter	= 0; 
	bmih.biClrUsed			= 0;
	bmih.biClrImportant		= 0;

	DWORD written;

	if( !WriteFile( file, &bmpfh, sizeof(BITMAPFILEHEADER), &written, NULL ) )
	{
		CloseHandle( file );
		return false;
	}

	if( !WriteFile( file, &bmih, sizeof(BITMAPINFOHEADER), &written, NULL ) )
	{
		CloseHandle( file );
		return false;
	}

	if( !WriteFile( file, buffer, 768, &written, NULL ) )
	{
		CloseHandle( file );
		return false;
	}

	CloseHandle( file );
	return true;
}


bool CheckGuildMarkFile( char *fname, char *buffer )
{
	HANDLE	file;
	DWORD	fileSize;
	char msgBuffer[256];

	file = CreateFile( (LPCSTR)fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL );
	if( file == INVALID_HANDLE_VALUE )
	{
		strcpy( msgBuffer, g_LPACK.GetMessage(0,938) );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), msgBuffer );

		return false;
	}

	BITMAPFILEHEADER    bmpfh;
    BITMAPINFOHEADER    bmih;

	fileSize = GetFileSize( file, NULL );
	if( fileSize == INVALID_FILE_SIZE )
	{
		strcpy( msgBuffer, g_LPACK.GetMessage(0,939) );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), msgBuffer );

		CloseHandle( file );
		return false;
	}

	DWORD	byteRead;

	if( !ReadFile( file, &bmpfh, sizeof(BITMAPFILEHEADER), &byteRead, NULL ) )
	{
		strcpy( msgBuffer, g_LPACK.GetMessage(0,939) );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), msgBuffer );

		CloseHandle( file );
		return false;
	}

	if( !ReadFile( file, &bmih, sizeof(BITMAPINFOHEADER), &byteRead, NULL ) )
	{
		strcpy( msgBuffer, g_LPACK.GetMessage(0,939) );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), msgBuffer );

		CloseHandle( file );
		return false;
	}

	









	
	if( bmih.biWidth != 16 || bmih.biHeight != 16 || bmih.biBitCount != 24 )
	{
		strcpy( msgBuffer, g_LPACK.GetMessage(0,939) );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), msgBuffer );

		CloseHandle( file );
		return false;
	}


	if( !ReadFile( file, buffer, 768, &byteRead, NULL ) )

	{
		strcpy( msgBuffer, g_LPACK.GetMessage(0,939) );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), msgBuffer );

		CloseHandle( file );
		return false;
	}

	CloseHandle( file );

	return true;
}


void GTH_Character_Pushed(Fx_CHARACTER_t *character)
{
	vec3_t delta, position;
	float speed;

	if( character->flag_replayAnim == 1 )
	{
		
		if( character->idxOnServer == g_cgv.myPCIdxOnServer && character->entityType == ENTITY_PC ) 
		{
			g_cursor->QuitPlayerTargetedMove();
			g_cgv.clickEvent = GTH_CEV_CHAR_NONE;
		}
	}

	character->SetupVectors();
	character->GetFwdVector( delta );
	VectorNegate( delta , delta );
	delta[2] = 0;
	VectorNormalize( delta );
	
	speed = 2.0f * SPEED_CONSTANT * character->moveSpeed * g_timer.GetElapsedTime();
	VectorScale( delta , delta , speed );

	VectorCopy( position , character->position );
	g_move->Pmove( position , delta , character->angles );
	VectorCopy( character->position , position );

	
	VectorCopy( position , character->syncPosition );
	g_move->Pmove( position , delta , character->angles );
	VectorCopy( character->syncPosition , position );

	GTH_ChangeAnimation(character, ANIMTYPE_BYITEM, ANIM_ITEM_ATTACKED_0 );

	


}

int GTH_GetUsedMonster_AnimationList( int list[] )
{
	int					monsterIdx;
	int					aniSetType;
	int					animIndex;
	int					numUsedAni	=	0;
	int					i, j, k;

	FX_CCharLookupMng	*pLookup		=	g_charManager->m_pCharLookupMng;
	FX_CAnimManager		*pAnim			=	g_charManager->m_pAnimManager;


	
	for( i = 0; i < g_pApp->m_mapInfos[0].monsterKind; i ++ )
	{
		monsterIdx		=	g_pApp->m_mapInfos[0].monsters[i];
		aniSetType		=	g_monsterTable[monsterIdx].aniSetType;

		if( aniSetType < 0 ) continue;

		for( j = 0; j < MAX_ANIMCOUNT_TYPE; j ++ )
		{
			animIndex				=	pLookup->GetAnimIDFromTable( ENTITY_MONSTER,	
																monsterIdx,				
																aniSetType,				
																0,						
																j,						
																0 );					
			if( animIndex < 0 ) continue;
			
			
			for( k = 0; k < numUsedAni; k ++ )
			{
				if( list[k] == animIndex ) break;
			}
			if( k != numUsedAni ) continue;

			list[numUsedAni ++]	=	animIndex;
		}
	}

	return( numUsedAni );
}



float	FindTransformAttackLength(int transformEntityIdx, int transformType)
{
	if( transformEntityIdx >= MAX_NUMBER_OF_MONSTER_TABLE )	return 0.0f;



		return (float)g_monsterTable[transformEntityIdx].atkLength;


	return 0.0f;
} 




float	GetEpackRechargePrice_RA( int onceRecovery, float cost, int level )
{
	
	float tempcost=(float)onceRecovery * cost * powf( (float)level, 0.67f ) * 1.5f / 8.0f;
	switch(GetChaosClass(g_pApp->m_myCharacter)) {
	case CHAOS_DISORDER:
		tempcost=tempcost*3;
		
		break;
	case CHAOS_MONSTER:
		tempcost=tempcost*5;
		break;
		
	}

	return tempcost;

} 

float	GetEpackRechargePrice_SA( int onceRecovery, float cost, int level )
{
	
	float tempcost= (float)onceRecovery * cost * powf( (float)level, 0.67f ) * 1.7f / 8.0f;
	switch(GetChaosClass(g_pApp->m_myCharacter)) {
	case CHAOS_DISORDER:
		tempcost=tempcost*3;
		break;
	case CHAOS_MONSTER:
		tempcost=tempcost*5;
		break;
		
	}

	return tempcost;

}




bool CheckStateWeaponChange(Fx_CHARACTER_t* character)
{

	
	if ( character->isTransform || !character->isAlive )
		return 0;

	
	switch(character->event)
	{
	case GTH_EV_CHAR_ATTACK:
	case GTH_EV_CHAR_SKILL:
	case GTH_EV_CHAR_SKILLCASTING:
	case GTH_EV_CHAR_BEFORE_TELEPORT:
	case GTH_EV_CHAR_TELEPORT:
	case GTH_EV_CHAR_FREEZE:
	case GTH_EV_CHAR_PUSHED:
	case GTH_EV_CHAR_HIDE:
		{
			g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,232) );
			return 0;
		}
	}	
	return 1;
}


bool CheckAvailableGenCapability( myCharacterInfo_t *pc, int genType )
{
	if( genType > J_EARTH ) return false;

	
	if( pc->genCapability[genType] < 0 ) return false;
	
	if( pc->pcClass == genType ) return true;
	
	if( pc->pcJob <= J_EARTH ) return false;
	
	return true;
}









int GTH_GetMatchMyItemIdx_fromInventory(const int itemtableIdx)
{
	for (int InvenToryIdx = 0; InvenToryIdx < MAX_INVENTORY_SIZE; InvenToryIdx++)
	{		
		int ItemIdx =  g_cgv.myCharacterInfo->inventory[InvenToryIdx];
		
		if( ItemIdx < 0 || ItemIdx >= MAX_NUMBER_OF_OWNITEM)
			continue;
		
		if ( g_cgv.myCharacterInfo->item[ItemIdx].itemTableIdx == itemtableIdx)
			return ItemIdx;
	}	
	return -1;
}







int GTH_GetInvenPosMatchMyItemTableIdx(const int itemtableIdx)
{
	for (int InvenToryIdx = 0; InvenToryIdx < MAX_INVENTORY_SIZE; InvenToryIdx++)
	{		
		int ItemIdx =  g_cgv.myCharacterInfo->inventory[InvenToryIdx];
		
		if( ItemIdx < 0 || ItemIdx >= MAX_NUMBER_OF_OWNITEM)
			continue;
		
		if ( g_cgv.myCharacterInfo->item[ItemIdx].itemTableIdx == itemtableIdx)
			return InvenToryIdx;
	}	
	return -1;
}







BOOL	DeleteItemFromMyInventoryByInvenPos(const int invenPos, const int StackConut)
{		
	if ( invenPos < 0 || invenPos >= MAX_INVENTORY_SIZE)
		return FALSE;

	int itemIdx = g_cgv.myCharacterInfo->inventory[invenPos];

	if ( itemIdx < 0 || itemIdx >= MAX_NUMBER_OF_OWNITEM)
		return FALSE;	

	item_t *pItem = NULL;
	pItem = &g_cgv.myCharacterInfo->item[itemIdx];	

	
	if (GTH_IsStackItem (pItem) == FALSE)
	{
		GTH_DeleteMyItem( pItem );
		g_cgv.myCharacterInfo->inventory[invenPos] = -1;
	}
	else
	{		
	if( pItem->durability - StackConut >= 0 )
	{
		pItem->durability -= StackConut;
	}
	else
	{
		GTH_DeleteMyItem( pItem );
		g_cgv.myCharacterInfo->inventory[invenPos] = -1;
	}
	}
	











	return TRUE;
}



