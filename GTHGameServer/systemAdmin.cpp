#include "global.h"

void GTH_SystemAdmin_ServerShutdown()
{
	if ( g_config.isManager )
	{
		
		GTH_SendMessage_DropServerGroup();

		
		GTH_SendMessage_SystemShutdownToMember();

		
		GTH_SystemAdmin_SetShutDownTimer();

		
		GTH_ProcessDanbattlePowerEnd();
	}
	else
	{
		
		GTH_SendMessage_SystemShutdownToManager();
	}
}

void GTH_SystemAdmin_SetShutDownTimer()
{
	g_serverShutdownTimer.active = true;
	g_serverShutdownTimer.setTime = g_globalTime;
	
	
	
	g_serverShutdownTimer.setSecond = 20 + (NewDanBattleSystem::POWE_END_WAIT_SEC/1000);
	g_serverShutdownTimer.countdown = g_serverShutdownTimer.setSecond * 1000 + 	SERVER_SHUTDOWN_DELAYTIME;

	g_serverShutdownTimer.isComplete = false; 
}

void GTH_Init_ServerShutdownTimer()
{
	g_serverShutdownTimer.active = false;
	g_serverShutdownTimer.setTime = 0;
	g_serverShutdownTimer.setSecond = 0;
	g_serverShutdownTimer.countdown = 0;
	g_serverShutdownTimer.isComplete = false; 
}

void GTH_Process_ServerShutdownTimer()
{
	int count;
	int i;
	playerCharacter_t *pc;

	if ( g_serverShutdownTimer.active )
	{
		
		if (g_globalTime - g_serverShutdownTimer.setTime > g_serverShutdownTimer.countdown )
			g_serverShutdownTimer.isComplete = true;

		if ( g_serverShutdownTimer.isComplete )
		{
			g_serverShutdownTimer.isComplete = false;
			PostMessage( g_hWnd, WM_COMMAND, IDM_EXIT, 0 );	
			return;
		}

		
		pc = &g_pc[0];
		for (i=0; i<MAX_PCS; i++, pc++)
		{
			if ( !pc->active ) continue;
			if ( !pc->ready ) continue;
			if ( pc->isSetServerShutdown ) continue;

			GTH_SendMessage_SetShutdownTimer( pc );
			pc->isSetServerShutdown = true;

			count ++;
			if ( count > 20 ) break;
		}
	}
}

void GTH_SystemAdmin_GoToPlayer(playerCharacter_t *pc)
{
	
	int  idx;
	char toName[NAMESTRING];
	int worldIdx, zoneIdx;
	vec3_t position;

	strcpy(toName, MSG_ReadString());

	idx = GTH_FindPCByName( toName );
	if (idx >= 0) 
	{
		worldIdx = g_pc[idx].worldIdx;
		vec_copy(g_pc[idx].position, position);

		if( worldIdx == pc->worldIdx ) 
		{
			vec_copy( position, pc->position );
			zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
			pc->zoneIdx = zoneIdx;

			GTH_SendPCEventMessage_Respawn( pc );
			GTH_SendMessage_SyncItemObject( pc );

			g_logSystem->Write("User (%s) Move to '%s' Character Pos. (World:%d, %.2f, %.2f, %.2f)", pc->userID, toName, pc->worldIdx, pc->position[0], pc->position[1], pc->position[2]);
		}
		else 
		{
			
			GTH_Zone_DeleteEntityList(pc->worldIdx, pc->zoneIdx, pc->entityType, pc->idx);

			pc->worldIdx = worldIdx;
			vec_copy( position, pc->position );

			pc->zoneIdx = GTH_Zone_GetZoneIdx( pc->position );
			
			GTH_Zone_AddEntityList(pc->worldIdx, pc->zoneIdx, pc->entityType, pc->idx);

			GTH_SendPCEventMessage_Respawn( pc );
			pc->ready = false;

			g_logSystem->Write("User (%s) Move to '%s' Character Pos. (World:%d, %.2f, %.2f, %.2f)", pc->userID, toName, pc->worldIdx, pc->position[0], pc->position[1], pc->position[2]);
		}
	}
	else	
	{
		if ( g_config.isManager )
		{			
			GTH_SendMessage_RequestFindPCToMember( g_config.gameServerNo, toName, pc->name );
		}
		else
		{
			GTH_SendMessage_RequestFindPCToManager( g_config.gameServerNo, toName, pc->name );
		}
	}
}






void GTH_SystemAdmin_Notice()
{
	char notice[256];

	strcpy(notice, MSG_ReadString());

	if ( g_config.isManager )
	{

		
		GTH_SendMessage_NoticeToMember( notice );

		
		GTH_SystemAdmin_SetNoticeInfo( notice );
	}
	else
	{
		
		GTH_SendMessage_NoticeToManager( notice );
	}
}



void GTH_SystemAdmin_SetNoticeInfo( char *notice )
{
	g_noticeInfo.active = true;
	g_noticeInfo.setTime = g_globalTime;
	
	g_noticeInfo.setRepeatCount = 3;			
	g_noticeInfo.setTerm = 10000;				
	g_noticeInfo.nextTime = g_globalTime;
	g_noticeInfo.curRepeatCount = 0;
	g_noticeInfo.curPC = 0;
	strcpy(g_noticeInfo.notice, notice);
}

void GTH_Init_ServerNoticeInfo()
{
	g_noticeInfo.active = false;
	g_noticeInfo.setTime = 0;
	g_noticeInfo.setRepeatCount = 0;		
	g_noticeInfo.setTerm = 0;				
	g_noticeInfo.nextTime = 0;
	g_noticeInfo.curRepeatCount = 0;
	g_noticeInfo.curPC = 0;
	strcpy(g_noticeInfo.notice, "");
}

void GTH_Process_NoticeInfo()
{
	int count;
	int i;
	playerCharacter_t *pc;
	int start;

	if ( g_noticeInfo.active )
	{
		if ( g_globalTime >= g_noticeInfo.nextTime) 
		{

			
			start = g_noticeInfo.curPC;
			pc = &g_pc[start];
			for (i=start; i<MAX_PCS; i++, pc++)
			{
				if ( !pc->active ) continue;
				if ( !pc->ready ) continue;

				GTH_SendMessage_NoticeMessage( pc, g_noticeInfo.notice );

				count ++;
				if ( count >= 20 ) 				
				{
					i++;
					break;
				}
			}

			if (i >= MAX_PCS)
			{
				g_noticeInfo.curPC = 0;
				g_noticeInfo.nextTime = g_globalTime + g_noticeInfo.setTerm;
				g_noticeInfo.curRepeatCount ++;
				if (g_noticeInfo.curRepeatCount >= g_noticeInfo.setRepeatCount)
					g_noticeInfo.active = false;
			}
			else
			{
				g_noticeInfo.curPC = i;
			}
		}
	}
}

int	GTH_CheckValidation_GameMaster( playerCharacter_t *pc )
{
	if ( pc->gameMaster < 1) return 0;

	return 1;
}

void GTH_SystemAdmin_RequestFindPC()
{
	int idx;
	char toName[NAMESTRING], fromName[NAMESTRING];
	int worldIdx, reqMemberIdx;
	vec3_t position;

	reqMemberIdx = MSG_ReadByte();
	strcpy(toName, MSG_ReadString());
	strcpy(fromName, MSG_ReadString());

	idx = GTH_FindPCByName( toName );
	if (idx >= 0) 
	{
		worldIdx = g_pc[idx].worldIdx;
		vec_copy(g_pc[idx].position, position);

		if ( g_config.isManager )
		{
			GTH_SendMessage_ReplyFindPCToMember( reqMemberIdx, worldIdx, position, fromName );
		}
		else
		{
			GTH_SendMessage_ReplyFindPCToManager( reqMemberIdx, worldIdx, position, fromName );
		}
	}
	else	
	{
		if ( g_config.isManager )
		{			
			GTH_SendMessage_RequestFindPCToMember( reqMemberIdx, toName, fromName );
		}
	}
}

void GTH_SystemAdmin_ReplyFindPC()
{
	
	int idx;
	int worldIdx, reqMemberIdx, memberIdx;
	char fromName[NAMESTRING];
	vec3_t position;
	playerCharacter_t *pc;
	vec3_t savePosition;
	int saveWorldIdx;

	reqMemberIdx = MSG_ReadByte();
	worldIdx = MSG_ReadByte();
	MSG_ReadPosition(position);
	strcpy(fromName, MSG_ReadString());

	if ( reqMemberIdx == g_config.gameServerNo ) 
	{
		idx = GTH_FindPCByName( fromName );
		if ( idx >= 0 )
		{
			pc = &g_pc[idx];

			memberIdx = GTH_FindServerWithinGroup( worldIdx );
			if (memberIdx < 0) 
			{
				g_logSystem->Write("Move Error!! in GTH_SystemAdmin_ReplyFindPC(reqMemberIdx:%d, worldIdx:%d, fromName:%s", reqMemberIdx, worldIdx, fromName);
			}
			else
			{
				
				saveWorldIdx = pc->worldIdx;
				vec_copy(pc->position, savePosition);

				
				pc->worldIdx = worldIdx;
				vec_copy(position, pc->position);

				pc->flagChangeServer = 1; 
				pc->changeServerWorldIdx = worldIdx;
				
				DB_SaveGameData( pc, SAVEGAMEDATA_QUIT);

				
				pc->worldIdx = saveWorldIdx;
				vec_copy(savePosition, pc->position);
			}
		}
	}
	else
	{
		if ( g_config.isManager )
		{
			GTH_SendMessage_ReplyFindPCToMember( reqMemberIdx, worldIdx, position, fromName );
		}
	}
}

void GTH_SystemAdmin_GoToMonster(playerCharacter_t *pc)
{
	if( !pc->gameMaster ) return;
	

	int zoneIdx;
	int monsterIdx;

	monsterIdx = MSG_ReadShort();

	if (monsterIdx < 0 || monsterIdx >= MAX_MONSTERS)
		return;

	if( !g_monster[monsterIdx].active ) return;

	if ( pc->worldIdx == g_monster[monsterIdx].worldIdx )
	{
		vec_copy( g_monster[monsterIdx].position, pc->position );
		zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
		pc->zoneIdx = zoneIdx;

		GTH_SendPCEventMessage_Respawn( pc );
		GTH_SendMessage_SyncItemObject( pc );

		g_logSystem->Write("User (%s) Move to Monster(%d) Pos. (World:%d, %.2f, %.2f, %.2f)", pc->userID, monsterIdx, pc->worldIdx, pc->position[0], pc->position[1], pc->position[2]);
	}
}

void GTH_SystemAdmin_GMBusyOnOff(playerCharacter_t *pc)
{
	if( !pc->gameMaster ) return;
	pc->gmBusy = !pc->gmBusy;
}

void GTH_SystemAdmin_GMHideOnOff(playerCharacter_t *pc)
{
	if( !pc->gameMaster ) return;
	pc->gmHide = !pc->gmHide;
}

void GTH_SystemAdmin_SummonMonster( playerCharacter_t *pc )
{
	int worldIdx, monsterTableIdx, aiType, entityType;
	int summonerType, summonerIdx;
	int num;
	float validTime;
	vec3_t position;
	int reply = 1, error = 0;

	if( !pc->gameMaster ) return;

	summonerIdx = MSG_ReadShort();
	summonerType = MSG_ReadByte();
	
	monsterTableIdx = MSG_ReadShort();
	aiType = MSG_ReadShort();

	worldIdx = MSG_ReadByte();
	MSG_ReadPosition( position );
	validTime = (float)MSG_ReadShort();
	validTime = validTime * 1000.0f;
	num = MSG_ReadShort();

	if( num > 5 ) num = 5;

	entityType = ENTITY_MONSTER;

	for( int i = 0; i < num; i ++ )
	{
		reply = MONSTER_Summon( monsterTableIdx, worldIdx, position, aiType, ENTITY_MONSTER, summonerIdx, summonerIdx, validTime );
	}


}



extern i3sizebuf_t netMessage; 

