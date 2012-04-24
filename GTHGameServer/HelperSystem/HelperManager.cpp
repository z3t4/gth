


#include "..\global.h"

#include "..\Tools\CTools.h"
#include <assert.h>
extern CTools* gcpTools;
#include "..\GonryunPracticeBattle\CGonryunPracticeBattle.h"
#include "CHelperManager_Encoder.h"
extern CGonryunPracticeBattle	g_GonryunPracticeBattle;

#include "tagHelperPacket.h"




CHelperManager::CHelperManager()
{

}

CHelperManager::~CHelperManager()
{

}

BOOL CHelperManager::ProcessMessage_Helper_SS(const enum enumHelperPacket& SecoundPacket)
{
	switch ( SecoundPacket )
	{
	case SS_REMOVE_NOTIFY_toPlayer:
		ProcessMessage_SC_REMOVE_NOTIFY_toPlayer();
		return TRUE;
		
	case SS_SPAWN_Req_fromServer:
		ProcessMessage_ServerRequestSummons();		
		return TRUE;
		
	case SS_SPAWN_Res_fromServer:
		ProcessMessage_ServerReplySummons();
		return TRUE;
		
	case SS_REPLYMemberList_toServer:
		ProcessMessage_Reply_Helpers_List();
		return TRUE;
	case SS_REQUESTMemberList_toServer:
		ProcessMessage_MemberList_Request_List ();
		return TRUE;
	case SS_REPLY_UPDATE_MemberList_toServer:
		ProcessMessage_Reply_Helpers_Update ();
		return TRUE;
	case SS_REQUEST_UPDATE_MemberList_toServer:
		ProcessMessage_MemberList_Request_Update ();
		return TRUE;
	}

	return FALSE;
}


BOOL CHelperManager::ProcessMessage_Helper_CS(const enum enumHelperPacket& SecoundPacket)
{
	switch ( SecoundPacket )
	{
	case CS_INVITE_Req_fromHELPER:		
		ProcessMessage_Invite_REQ_fromHELPER(g_curPC);
		return TRUE;
		
	case CS_INVITE_Res_fromTAKER:
		ProcessMessage_Invite_RES_fromTAKER (g_curPC);
		return TRUE;
		
	case CS_REMOVE_Req_fromPlayer:
		ProcessMessage_CS_REMOVE_REQ_fromPlayer(g_curPC);
		return TRUE;
		
	case CS_GIVEPOINT_Req_fromHelper:
		ProcessMessage_Helper_GivePoint_Req_fromHelper(g_curPC);
		return TRUE;
		
	case CS_SPAWN_Req_fromHelper:
		ProcessMessage_Recall_Req_fromHelper(g_curPC);
		return TRUE;
		
	case CS_SPAWN_Res_fromTaker:
		ProcessMessage_Recall_Res_fromTaker(g_curPC);
		return TRUE;
		
	case CS_CONFIRM_MYPOINT_Req_fromPlayer:
		ProcessMessage_MyPointConfirm(g_curPC);
		return TRUE;
		
	case CS_CONFIRM_HELPPOINT_Req_fromPlayer:
		ProcessMessage_HelpPointConfirm(g_curPC);
		return TRUE;
		
	case CS_CONFIRM_WEPPOINT_Req_fromPlayer:
		ProcessMessage_Change_WepPoint(g_curPC);
		return TRUE;
		
	case CS_MemBerList_Req_fromPlayer:
		ProcessMessage_MemberList_Req_fromPlayer(g_curPC);
		return TRUE;
		
	}
	
	
	return FALSE;
}

void CHelperManager::ProcessMessage_Helper()
{
   	enum enumHelperPacket SecoundPacket;
	SecoundPacket = static_cast <enum enumHelperPacket>(MSG_ReadShort());

	if(TRUE == ProcessMessage_Helper_CS(SecoundPacket)) return;
	if(TRUE == ProcessMessage_Helper_SS(SecoundPacket)) return;


}

void CHelperManager::ProcessMessage_MemberList_Req_fromPlayer(playerCharacter_t* pPlayer)
{
	if(NULL == pPlayer) return;
	MemberList_Request_List(pPlayer);
}


void CHelperManager::ProcessMessage_Invite_REQ_fromHELPER(playerCharacter_t* pHelper)
{
	
	if(TRUE == IsBadWritePtr(pHelper,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pHelper,sizeof(playerCharacter_t)))
	{
	
	
		
		return;
	}


	
	if( tagHelper::tagMemberList::HELPER != pHelper->tHelper.List.mode)
	{
		SendMessage_Invite_Respond_toHelper(
			pHelper,tagHelperPacket_SC_INVITE_RES_toHELPER::Fail_RegistrationRequest_invalidMode );
			g_logSystem->Write("Fail_RegistrationRequest_invalidMode" );
		return ;
	}



	char cz_tempTakerName[NAMESTRING+1];
	strncpy(cz_tempTakerName,MSG_ReadString(),NAMESTRING);
	cz_tempTakerName[NAMESTRING]=NULL;

	int idx = GTH_FindPCByName(cz_tempTakerName);
	
	playerCharacter_t * pTaker=gcpTools->GetPlayerRecordPointer(idx);

	if(NULL == pTaker){
		SendMessage_Invite_Respond_toHelper(
			pHelper,tagHelperPacket_SC_INVITE_RES_toHELPER::Fail_RegistrationRequest_invalidUser);
			g_logSystem->Write("Fail_RegistrationRequest_invalidUser" );
		return;
	}
	
	
	if( tagHelper::tagMemberList::TAKER != pTaker->tHelper.List.mode)
	{
		SendMessage_Invite_Respond_toHelper(
			pHelper,tagHelperPacket_SC_INVITE_RES_toHELPER::Fail_RegistrationRequest_invalidTakerMode );
			g_logSystem->Write("Fail_RegistrationRequest_invalidMode" );
		return ;
	}

	
	if(pTaker->idx == pHelper->idx)
	{
		SendMessage_Invite_Respond_toHelper( 
			pHelper,tagHelperPacket_SC_INVITE_RES_toHELPER::Fail_RegistrationRequest_invalidUser);
		g_logSystem->Write("Fail_RegistrationRequest_invalidUser" );
		return;
	}


	
	if ( pHelper->tHelper.List.count > tagHelper::MAX_MEMBER)
	{
		SendMessage_Invite_Respond_toHelper( 
			pHelper,tagHelperPacket_SC_INVITE_RES_toHELPER::Fail_RegistrationRequest_invalidTakersCount);
			g_logSystem->Write("Fail_RegistrationRequest_invalidTakersCount" );
		return;
	}

	
	if(pTaker->tHelper.List.count >= tagHelper::MAX_Accept_HELPERS)
	{
		SendMessage_Invite_Respond_toHelper( 
			pHelper,tagHelperPacket_SC_INVITE_RES_toHELPER::Fail_RegistrationRequest_invalidHelperCount);
		g_logSystem->Write("Fail_RegistrationRequest_invalidHelperCount" );
		return;
	}

	if ( pHelper->worldIdx != pTaker->worldIdx )
	{
		SendMessage_Invite_Respond_toHelper( 
			pHelper,tagHelperPacket_SC_INVITE_RES_toHELPER::Fail_RegistrationRequest_invalidWorldIndex);
			g_logSystem->Write("Fail_RegistrationRequest_invalidWorldIndex" );
		return ;
	}

	if(TRUE == isMyChild(pHelper,pTaker->name))
	{
		SendMessage_Invite_Respond_toHelper(
			pHelper,tagHelperPacket_SC_INVITE_RES_toHELPER::Fail_RegistrationRequest_invaliChild);
			g_logSystem->Write("Fail_RegistrationRequest_invaliChild" );
		return ;
	}



	strncpy(pTaker->HelperAdd.RequestPlayer.Name,pHelper->name,NAMESTRING);
	pTaker->HelperAdd.RequestPlayer.Name[NAMESTRING]=NULL;
	pTaker->HelperAdd.RequestPlayer.pcIdx=pHelper->idx;


	SendMessage_Invite_Respond_toHelper( 
		pHelper,tagHelperPacket_SC_INVITE_RES_toHELPER::Ready_Registration);
	
	ShowLogInfo("Helper_Invite_Suggest: name:%s,level:%d ",pHelper->name,pHelper->level);
	
	SendMessage_Invite_Request_toTaker( pTaker,pHelper->name);
	ShowLogInfo("Helper_Invite_Suggest: name:%s,level:%d ",pTaker->name,pTaker->level);
}


void CHelperManager::ProcessMessage_Invite_RES_fromTAKER (playerCharacter_t * pTaker)
{

	if(TRUE == IsBadWritePtr(pTaker,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pTaker,sizeof(playerCharacter_t)))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		ShowLogInfo("ProcessMessage_Invite_RES_fromTAKER (playerCharacter_t * pTaker)");
		return;
	}
	
	int respond = MSG_ReadShort();
	
	
	
	if(RESPOND_NO == respond)
	{
		SendMessage_Invite_Respond_toHelper(
			pTaker,tagHelperPacket_SC_INVITE_RES_toHELPER::enumCode::Fail_RegistrationReject);
		
		return;
	}
	
	else if(RESPOND_YES == respond)
	{
		

		
		 
		
		playerCharacter_t* pHelper = gcpTools->GetPlayerRecordPointer(pTaker->HelperAdd.RequestPlayer.pcIdx);		
		if((NULL == pHelper) || (0 != strncmp(pTaker->HelperAdd.RequestPlayer.Name,pHelper->name,NAMESTRING))) 
		{
		
		
			g_logSystem->Write("Fail_RegistrationRequest_invalidUser" );
			return;
		}



		if ( pTaker->worldIdx != pHelper->worldIdx )
		{
			SendMessage_Invite_Respond_toHelper(
				pHelper,tagHelperPacket_SC_INVITE_RES_toHELPER::enumCode::Fail_RegistrationRequest_invalidWorldIndex);
			
		
		
			g_logSystem->Write("Fail_RegistrationRequest_invalidWorldIndex" );
			return ;
		}

		SendMessage_Invite_Respond_toHelper( 
			pHelper,tagHelperPacket_SC_INVITE_RES_toHELPER::enumCode::Success_RegistrationRequest);
		
		pHelper->tHelper.List.mode =tagHelper::tagMemberList::enumMode::HELPER;
		pTaker->tHelper.List.mode  =tagHelper::tagMemberList::enumMode::TAKER;



		if((pHelper->tHelper.List.count < 0) ||
		  (pHelper->tHelper.List.count >= tagHelper::MAX_MEMBER) ||
		  (pTaker->tHelper.List.count !=0 )){

			SendMessage_Invite_Respond_toHelper(pHelper,tagHelperPacket_SC_INVITE_RES_toHELPER::enumCode::Fail_RegistrationRequest_invalidUser);
			SendMessage_Invite_Respond_toTaker(pTaker,tagHelperPacket_SC_INVITE_RES_toHELPER::enumCode::Fail_RegistrationRequest_invalidUser);
			return;

		}
			
		Add_toMemberList( pHelper, pTaker );
		
		SendMessage_Invite_Respond_toTaker(
			pTaker,tagHelperPacket_SC_INVITE_RES_toHELPER::enumCode::Success_RegistrationRequest);
		
		Add_toMemberList( pTaker, pHelper );
		pTaker->HelperAdd.RequestPlayer.pcIdx=-1;
	}

}


void CHelperManager::ProcessMessage_SC_REMOVE_NOTIFY_toPlayer()
{
	int  idx=-1;
	char removeName[NAMESTRING+1], playerName[NAMESTRING+1];

	sstrncpy( removeName, MSG_ReadString(), NAMESTRING);
	removeName[NAMESTRING]=NULL;
	sstrncpy( playerName, MSG_ReadString(), NAMESTRING);
	playerName[NAMESTRING]=NULL;

	idx = GTH_FindPCByName(removeName);
	if (0 <= idx ) 
	{
		playerCharacter_t* pRemover=gcpTools->GetPlayerRecordPointer(idx);
		if(NULL == pRemover) return;
		
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			
			MSG_WriteByte(&netMessage,	 EXTEND_SECOND);
			MSG_WriteShort( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort( &netMessage, SC_REMOVE_NOTIFY_toPlayer );
			MSG_WriteString(&netMessage, playerName);								
		
			NET_SendUnreliableMessage(&pRemover->sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);

		
		GTH_SendMessage_AckMessage(playerName);
	}
	
	else	
	{
		if (TRUE == g_config.isManager )
		{			
			int fromMemberIdx = MSG_ReadByte();											

			MSG_BeginWriting(&netMessage);
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte(&netMessage, EXTEND_SECOND);
				MSG_WriteShort( &netMessage, HELPER_SYSTEM );
				MSG_WriteShort( &netMessage, SS_REMOVE_NOTIFY_toPlayer );
				MSG_WriteString(&netMessage, removeName);							
				MSG_WriteString(&netMessage, playerName);							
				
				
				for (int gameseridx=1; gameseridx < MAX_MEMBER_SERVER; gameseridx++)	
				{
					if ( !g_memberServer[gameseridx].active ) continue;
					if ( gameseridx == fromMemberIdx ) continue;					
					
					NET_SendUnreliableMessage(&g_memberServer[gameseridx].sock, &netMessage);
				}
			}
			MSG_EndWriting(&netMessage);
		}
	}
}

void CHelperManager::ProcessMessage_CS_REMOVE_REQ_fromPlayer(playerCharacter_t* pPlayer)
{
	if(TRUE == IsBadWritePtr(pPlayer,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pPlayer,sizeof(playerCharacter_t)))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		ShowLogInfo("ProcessMessage_CS_REMOVE_REQ_fromPlayer (playerCharacter_t * pHelper)");
		return;
	}

	char removeName[NAMESTRING+1] ;

	strncpy ( removeName, MSG_ReadString (),NAMESTRING );
	removeName[NAMESTRING]=NULL;

	
	Remove_toMemberList( pPlayer, removeName );
}



void CHelperManager::ProcessMessage_Helper_GivePoint_Req_fromHelper(playerCharacter_t* pTaker)
{
	if(TRUE == IsBadWritePtr(pTaker,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pTaker,sizeof(playerCharacter_t)))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		ShowLogInfo("ProcessMessage_Helper_GivePointtoHelperUser(playerCharacter_t * pTaker)");
		return;
	}
    
	int point=MSG_ReadLong();
	char cHelperName[NAMESTRING+1];
	strncpy(cHelperName,MSG_ReadString(),NAMESTRING);
	cHelperName[NAMESTRING]=NULL;
	
	
	int idx = GTH_FindPCByName( cHelperName );
	
	playerCharacter_t* pHelper=gcpTools->GetPlayerRecordPointer(idx);
	if(NULL == pHelper) return;
	
	if(FALSE == isMyChild(pTaker,pHelper->name)) return;

	if(HELPER_STANDARD_LEVEL > pHelper->level) return;
	
	if(0 > point) return;

	
	if( point > pTaker->tHelper.tPoint.iMyPoint )
	{
		
		return;
	}
	
	if( HELPER_STANDARD_LEVEL <= pTaker->level ) 
	{
		
		return;
	}

	
	pTaker->tHelper.tPoint.iMyPoint=pTaker->tHelper.tPoint.iMyPoint-point;
	pHelper->tHelper.tPoint.iHelpPoint=pHelper->tHelper.tPoint.iHelpPoint+point;
	pHelper->tHelper.tPoint.iTotalHelpPoint=pHelper->tHelper.tPoint.iTotalHelpPoint+point;

	SendMessage_Mypoint_toTaker(pTaker,pTaker->name,pHelper->name,point);

	ShowLogInfo(
		"pTaker MYPOINT: pTaker->NAME: %s, POINT: %d",pTaker->name,pTaker->tHelper.tPoint.iMyPoint);
	
	SendMessage_Helppoint_toHelper(pHelper,pTaker->name,point);

	ShowLogInfo("TARGETpPlayer HELPPOINT: TARGETpPlayer->NAME: %s, HELPPOINT: %d"
		,pHelper->name,pHelper->tHelper.tPoint.iHelpPoint);

}



void CHelperManager::ProcessMessage_Recall_Req_fromHelper(playerCharacter_t * pHelper)
{
	if(TRUE == IsBadWritePtr(pHelper,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pHelper,sizeof(playerCharacter_t)))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		ShowLogInfo("ProcessMessage_Recall_Req_fromHelper(playerCharacter_t * pHelper)");
		return;
	}

	char cTakerName[NAMESTRING+1];
	strncpy(cTakerName,MSG_ReadString(),NAMESTRING);
	cTakerName[NAMESTRING]=NULL;
	
	if(FALSE == isMyChild(pHelper,cTakerName)) return;

	for(int idx = 0; idx < pHelper->tHelper.List.count; idx++)
	{
		if( NULL == strcmp(cTakerName, pHelper->tHelper.List.Member[idx].name)) 
		{
			if( TRUE == pHelper->tHelper.List.Member[idx].bIsMatchHELPERandTACKER )
			{
				return;
			}
		}
	}

	SendMessage_RequestSummons(pHelper,cTakerName);
    	
}

void CHelperManager::ProcessMessage_ServerRequestSummons()
{
	int idx =-1;
	char toName[NAMESTRING+1], summoner[NAMESTRING+1];
	vec3_t position;

	strncpy(toName, MSG_ReadString(),NAMESTRING);
	toName[NAMESTRING]=NULL;
	strncpy(summoner, MSG_ReadString(),NAMESTRING);
	summoner[NAMESTRING]=NULL;

	int worldIdx = MSG_ReadByte();
	MSG_ReadPosition( position );
	

	idx = GTH_FindPCByName(toName);
	if (0 <= idx) 
	{
		playerCharacter_t* pTaker=gcpTools->GetPlayerRecordPointer(idx);
		if(NULL == pTaker) return;
		
		
	
		if( (playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER
			== pTaker->GonryunBattlePractice.MemberShip) ||
			(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT
			== pTaker->GonryunBattlePractice.MemberShip)) return;
		
		if( (pTaker->worldIdx == tagGolryunBattle::Golryun_Battle_Map_Index) ||
			(pTaker->worldIdx == DAN_BATTLEMAP_NO)) return;
		
		if(BUSY_STATE_NONE != pTaker->busyState) return;
		
		PC_SetSummonsInfo(pTaker, summoner,worldIdx, position);

		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort ( &netMessage, HELPER_SYSTEM);
			MSG_WriteShort(&netMessage, SC_SPAWN_Req_toTaker);
			MSG_WriteString(&netMessage, summoner);	
			NET_SendMessage(&pTaker->sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	else	
	{
		if (TRUE == g_config.isManager )
		{
			int fromMemberIdx = MSG_ReadByte(); 

			MSG_BeginWriting(&netMessage);
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte(&netMessage, EXTEND_SECOND);
				MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
				MSG_WriteShort(&netMessage, SS_SPAWN_Req_fromServer);
				MSG_WriteString(&netMessage, toName); 
				MSG_WriteString(&netMessage, summoner);	
				MSG_WriteByte(&netMessage, worldIdx);	
				MSG_WritePosition(&netMessage, position); 
							
				
				for (int serveridx =1; serveridx < MAX_MEMBER_SERVER; serveridx++)	
				{
					if ( !g_memberServer[serveridx].active ) continue;
					if ( serveridx == fromMemberIdx ) continue;	
					
					NET_SendUnreliableMessage(&g_memberServer[serveridx].sock, &netMessage);
				}
			}
			MSG_EndWriting(&netMessage);
		}
	}
}


void CHelperManager::ProcessMessage_ServerReplySummons(void)
{
	int idx=-1;
	char toName[NAMESTRING+1], fromName[NAMESTRING+1];
	
	int reply = MSG_ReadByte();
	
	strncpy(toName, MSG_ReadString(),NAMESTRING);
	toName[NAMESTRING]=NULL;
	strncpy(fromName, MSG_ReadString(),NAMESTRING);
	fromName[NAMESTRING]=NULL;

	idx = GTH_FindPCByName(toName);
	if (0 <= idx) 
	{
		playerCharacter_t * pTaker=gcpTools->GetPlayerRecordPointer(idx);
		if(NULL==pTaker) return;
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_REPLYSUMMONS);
			MSG_WriteByte(&netMessage, reply);	
			MSG_WriteString(&netMessage, fromName);	
			NET_SendMessage(&pTaker->sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	else	
	{
		if (TRUE == g_config.isManager )
		{	
			int fromMemberIdx = MSG_ReadByte(); 
			
			MSG_BeginWriting(&netMessage);
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte(&netMessage, GSC_REPLYSUMMONS);
				MSG_WriteByte(&netMessage, reply);	
				MSG_WriteString(&netMessage, toName);	
				MSG_WriteString(&netMessage, fromName);	
				
				for (int severidx=1; severidx < MAX_MEMBER_SERVER; severidx++)	
				{
					if ( !g_memberServer[severidx].active ) continue;
					if ( severidx == fromMemberIdx ) continue;	
					
					NET_SendUnreliableMessage(&g_memberServer[severidx].sock, &netMessage);
				}
			}
			MSG_EndWriting(&netMessage);
		}
	}
}

void CHelperManager::ProcessMessage_Recall_Res_fromTaker(playerCharacter_t* pTaker)
{
	if(TRUE == IsBadWritePtr(pTaker,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pTaker,sizeof(playerCharacter_t)))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		ShowLogInfo("ProcessMessage_Recall_Res_tsTaker(playerCharacter_t *pTaker)");
		return;
	}

	int worldIdx =-1;
	vec3_t position;
	vec3_t savePosition;

	int reply = MSG_ReadByte();

	worldIdx = pTaker->summonsInfo.worldIdx;
	vec_copy( pTaker->summonsInfo.position, position );

	if( pTaker->isTransform ) GTH_DeleteTransform( pTaker );

	if (RESPOND_YES == reply) 
	{
		if ( GTH_IsLocalServiceWorld(worldIdx) )
		{
			if( worldIdx == pTaker->worldIdx ) 
			{
				vec_copy( position, pTaker->position );

				
				pTaker->reliableWorldIdx = pTaker->worldIdx;
				vec_copy( pTaker->position, pTaker->reliablePosition );

				int zoneIdx = GTH_Zone_UpdateCurrentZone(
					ENTITY_PC, pTaker->idx, pTaker->worldIdx, pTaker->zoneIdx, pTaker->position);
				pTaker->zoneIdx = zoneIdx;

				GTH_SendPCEventMessage_Respawn( pTaker );
				GTH_SendMessage_SyncItemObject( pTaker );
			}
			else 
			{
				
				GTH_Zone_DeleteEntityList(
					pTaker->worldIdx, pTaker->zoneIdx, pTaker->entityType, pTaker->idx);

				pTaker->worldIdx = worldIdx;
				vec_copy( position, pTaker->position );

				
				pTaker->reliableWorldIdx = pTaker->worldIdx;
				vec_copy( pTaker->position, pTaker->reliablePosition );

				pTaker->zoneIdx = GTH_Zone_GetZoneIdx( pTaker->position );
				
				GTH_Zone_AddEntityList(
					pTaker->worldIdx, pTaker->zoneIdx, pTaker->entityType, pTaker->idx);

				GTH_SendPCEventMessage_Respawn( pTaker );
				pTaker->ready = FALSE;

				
				g_logSystem->Write(
					"User [%s] Move To Summonning Pos. Pos(%.2f, %.2f, %.2f), World : %d", 
					pTaker->userID, pTaker->position[0], pTaker->position[1], pTaker->position[2],
					pTaker->worldIdx);
			}
		}
		else 
		{
			int memberIdx = GTH_FindServerWithinGroup( worldIdx );
			if (0 > memberIdx) 
			{
				g_logSystem->Write(
					"User [%s] Summon Error(worldIdx:%d)", pTaker->name, pTaker->worldIdx);
				return;
			}
			else
			{
				
				int saveWorldIdx = pTaker->worldIdx;
				vec_copy(pTaker->position, savePosition);

				
				pTaker->worldIdx = worldIdx;
				vec_copy(position, pTaker->position);

				pTaker->flagChangeServer = 1; 
				pTaker->changeServerWorldIdx = worldIdx;
				DB_SaveGameData( pTaker, SAVEGAMEDATA_QUIT);

				
				pTaker->worldIdx = saveWorldIdx;
				vec_copy(savePosition, pTaker->position);
			}
		}
	}

	
	SendMessage_ReplySummons(reply, pTaker->summonsInfo.summoner, pTaker->name);

	
	PC_InitSummonsInfo( pTaker );
}

void CHelperManager::ProcessMessage_MyPointConfirm(playerCharacter_t* pHelper)
{
	if(TRUE == IsBadWritePtr(pHelper,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pHelper,sizeof(playerCharacter_t)))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		ShowLogInfo("ProcessMessage_MyPointConfirm(playerCharacter_t * pHelper)");
		return;
	}
	
	SendMessage_Helper_Mypoint(pHelper);
}

void CHelperManager::ProcessMessage_HelpPointConfirm(playerCharacter_t* pHelper)
{
	if(TRUE == IsBadWritePtr(pHelper,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pHelper,sizeof(playerCharacter_t)))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		ShowLogInfo("ProcessMessage_HelpPointConfirm(playerCharacter_t * pHelper)");
		return;
	}

	SendMessage_Helper_Helppoint(pHelper);
}

void CHelperManager::ProcessMessage_Change_WepPoint(playerCharacter_t* pHelper)
{
	if(TRUE == IsBadWritePtr(pHelper,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pHelper,sizeof(playerCharacter_t)))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		ShowLogInfo("ProcessMessage_Change_WepPoint(playerCharacter_t * pHelper)");
		return;
	}
    
	int wepPoint=MSG_ReadLong();

	if(0 >= wepPoint) return;
	

	
	SendMessage_UpdatePointInfo_toPlayer(pHelper);

}

void CHelperManager::MemberList_Send_toPlayer( entityPCList_t* pl )
{
	
	
	struct tagHelper *helperlist=GetHelperPtr(pl->idx);
	if(NULL == helperlist)return;
	
	MSG_BeginWriting ( &netMessage );
	MSG_Clear( &netMessage );
	{
		if ( pl->serverIdx == 0 )
		{
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort(&netMessage, HELPER_SYSTEM);
			MSG_WriteShort(&netMessage, SC_SEND_MEMBERLIST_toAllPlayer);
		}
		
		else		
		{		
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort(&netMessage, HELPER_SYSTEM);
			MSG_WriteShort( &netMessage, SS_REPLYMemberList_toServer);
			MSG_WriteByte ( &netMessage, pl->serverIdx );
			MSG_WriteString(&netMessage, pl->name);
			MSG_WriteShort ( &netMessage, pl->idxOnServer );
		}
		
		MSG_WriteByte(&netMessage, helperlist->List.count );
		
		MSG_WriteShort(&netMessage, helperlist->List.mode);
		 
		
		for ( int listcount = 0; listcount < helperlist->List.count; listcount++ )
		{
			entityPCList_t target = { 0, };
			int idx = g_entityListMng.FindPCList ( helperlist->List.Member[listcount].name, &target);
		
			
			MSG_WriteByte ( &netMessage, target.level );
			MSG_WriteByte ( &netMessage, target.job );
			MSG_WriteByte ( &netMessage, idx < 0 ? -1 : target.worldIdx );
			MSG_WriteString ( &netMessage, helperlist->List.Member[listcount].name );
			helperlist->List.Member[listcount].bIsMatchHELPERandTACKER = FALSE;	
			
			struct tagHelper* Memperlist = GetHelperPtr(idx);
			if(NULL != Memperlist)
			{
				
				int memberidx=0;
				for(memberidx=0; memberidx<Memperlist->List.count; memberidx++)
				{
					
					if(NULL == strcmp(pl->name,Memperlist->List.Member[memberidx].name))
					{
						helperlist->List.Member[listcount].bIsMatchHELPERandTACKER = FALSE;	
						break;
					}						
				}
				
				if(memberidx>=Memperlist->List.count)
				{
					helperlist->List.Member[listcount].bIsMatchHELPERandTACKER = TRUE;	
				}
				
			}
			
			MSG_WriteByte( &netMessage,helperlist->List.Member[listcount].bIsMatchHELPERandTACKER);
		}
		
		if ( pl->serverIdx == 0 )
		{
			
			playerCharacter_t* pPlayer = gcpTools->GetPlayerRecordPointer(pl->idxOnServer);
			if(NULL == pPlayer)
			{
				MSG_EndWriting(&netMessage);
				return;
			}
			if (TRUE == pPlayer->ready )
			{
			
				NET_SendMessage ( &pPlayer->sock, &netMessage );
			}
		}
		
		else
		{
			NET_SendUnreliableMessage ( &g_memberServer[pl->serverIdx].sock, &netMessage );
		}
	
	}
	MSG_EndWriting(&netMessage);
}


void CHelperManager::MemberListInfo_Send_toAllPlayer(entityPCList_t* pl, int fstate )
{
	
	struct tagHelper *helperlist = GetHelperPtr(pl->idx);
	if(NULL == helperlist) return;

	entityPCList_t target = { 0, };
	for (int idx = 0; idx < helperlist->List.count; idx++ )
	{	
		
		if ( g_entityListMng.FindPCList ( helperlist->List.Member[idx].name, &target ) < 0) continue;
		if( TRUE ==  helperlist->List.Member[idx].bIsMatchHELPERandTACKER) continue;
		
		if ( target.serverIdx == 0 )
		{
			MSG_BeginWriting ( &netMessage );
			MSG_Clear( &netMessage );

			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort ( &netMessage, SC_LISTINFO_toAllPlayer );
			MSG_WriteByte ( &netMessage, pl->level );
			MSG_WriteByte ( &netMessage, pl->job );
			MSG_WriteByte ( &netMessage, fstate == HelpersLogout ? -1 : pl->worldIdx );				
			MSG_WriteString ( &netMessage, pl->name );				    
			MSG_WriteShort ( &netMessage, helperlist->List.mode);

			playerCharacter_t* pPlayer=gcpTools->GetPlayerRecordPointer(target.idxOnServer);
			if(NULL == pPlayer) {
				MSG_EndWriting(&netMessage);
				return;
			}
		
			NET_SendMessage ( &pPlayer->sock, &netMessage );
			MSG_EndWriting(&netMessage);
			ShowLogInfo("SC_LISTINFO_toAllPlayer: PlayName= %s, level= %d, job= %d, fstate=%d,mode=%d,",
				pl->name,pl->level, pl->job,fstate,helperlist->List.mode );
			
		}else{	

			MSG_BeginWriting ( &netMessage );
			MSG_Clear( &netMessage );
		
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort ( &netMessage, SS_REPLY_UPDATE_MemberList_toServer);				
			MSG_WriteByte ( &netMessage, target.serverIdx );
			MSG_WriteShort ( &netMessage, target.idxOnServer );
			
			MSG_WriteByte ( &netMessage, pl->level );
			MSG_WriteByte ( &netMessage, pl->job );
			MSG_WriteByte ( &netMessage, fstate == HelpersLogout ? -1 : pl->worldIdx );				
			MSG_WriteString ( &netMessage, pl->name );					
			MSG_WriteShort ( &netMessage, helperlist->List.mode);
			
			NET_SendUnreliableMessage ( &g_memberServer[target.serverIdx].sock, &netMessage );
			MSG_EndWriting(&netMessage);
		}		
	}
}



void CHelperManager::MemberList_Request_List( playerCharacter_t* pPlayer )
{
	
	if (TRUE ==  g_config.isManager )
	{
		entityPCList_t pl;

		int idx = g_entityListMng.FindPCList ( pPlayer->name, &pl );
		if ( 0 > idx )
		{
			g_logSystem->Write ("Error: pPlayerList에 없는 유저가 추가요청. userID: %s, name: %s",
				pPlayer->userID, pPlayer->name );
			return;
		}
		else
		{
			
			memcpy (&g_HelperManager.m_tHelpersList[idx], &pPlayer->tHelper, sizeof (struct tagHelper) );

			
 			MemberList_Send_toPlayer( &pl );

			
			
			MemberListInfo_Send_toAllPlayer( &pl, HelpersLogin );
			SendMessage_UpdatePointInfo_toPlayer(pPlayer);
		}
	}

	else 
	{
		
		MemberList_Request_List_OtherServer ( pPlayer );
	}
}


void CHelperManager::MemberList_Request_Update( playerCharacter_t* pPlayer, int fstate )
{
	if (  TRUE == g_config.isManager )
	{
		entityPCList_t pl ;
		if ( g_entityListMng.FindPCList ( pPlayer->name, &pl ) < 0 )
		{
			g_logSystem->Write ("Error: g_entityListMng에서 자신을 찾는데 실패."
				"userID: %s, "
				"name: %s",
				pPlayer->userID,
				pPlayer->name );
			return;
		}

		MemberListInfo_Send_toAllPlayer(&pl, fstate );
		SendMessage_UpdatePointInfo_toPlayer(pPlayer);
	}
	else
	{
		MemberList_Request_Update_OtherServer (pPlayer, fstate );
	}
}


void CHelperManager::MemberList_Request_List_OtherServer ( playerCharacter_t* pHelper )
{
	MSG_BeginWriting ( &netMessage );
	MSG_Clear( &netMessage );
	{

		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort( &netMessage, SS_REQUESTMemberList_toServer);
		MSG_WriteString ( &netMessage, pHelper->name );
		MSG_WriteByte ( &netMessage, pHelper->tHelper.List.count );
		
		MSG_WriteShort(&netMessage, pHelper->tHelper.List.mode);
		for (int idx = 0; idx < pHelper->tHelper.List.count; idx++ )
		{
			MSG_WriteString ( &netMessage, pHelper->tHelper.List.Member[idx].id );
			MSG_WriteString ( &netMessage, pHelper->tHelper.List.Member[idx].name );
			
			MSG_WriteByte(&netMessage, pHelper->tHelper.List.Member[idx].bIsMatchHELPERandTACKER);
		}
		
		NET_SendUnreliableMessage ( &localSocket, &netMessage );
	}
	MSG_EndWriting(&netMessage);
}




void CHelperManager::MemberList_Request_Update_OtherServer ( playerCharacter_t* pHelper, int fstate )
{
	MSG_BeginWriting ( &netMessage );
	MSG_Clear( &netMessage );
	{

		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
		MSG_WriteShort ( &netMessage, SS_REQUEST_UPDATE_MemberList_toServer);
		MSG_WriteString ( &netMessage, pHelper->name );
		MSG_WriteByte ( &netMessage, fstate );
		NET_SendUnreliableMessage ( &localSocket, &netMessage );
	}
	MSG_EndWriting(&netMessage);
}


void CHelperManager::ProcessMessage_MemberList_Request_List ()
{
	if ( FALSE == g_config.isManager )
	{
		g_logSystem->Write (
			"Error: 매니저가 아닌곳에서 ProcessMessage_MemberList_Request_List 호출됨");
		return;
	}

	char targetName[NAMESTRING+1];
	strncpy (targetName, MSG_ReadString (),NAMESTRING);
	targetName[NAMESTRING]=NULL;

	struct tagHelper helperList = { 0, };
	helperList.List.count = MSG_ReadByte () ;
	
	helperList.List.mode = static_cast<tagHelper::tagMemberList::enumMode>(MSG_ReadShort());
	
	for ( int listmember = 0 ; listmember < helperList.List.count; listmember++ )
	{
		strncpy (helperList.List.Member[listmember].id, MSG_ReadString(),IDSTRING );
			helperList.List.Member[listmember].id[IDSTRING]=NULL;
		
		strncpy (helperList.List.Member[listmember].name, MSG_ReadString(),NAMESTRING );
		helperList.List.Member[listmember].name[NAMESTRING]=NULL;
		helperList.List.Member[listmember].bIsMatchHELPERandTACKER = MSG_ReadByte();
	}

	entityPCList_t pl;
	int idx = g_entityListMng.FindPCList ( targetName, &pl );
	
	if (0 > idx  )
	{
		g_logSystem->Write ("Error: pPlayerList에 없는 유저가 추가요청. name: %s", targetName );
		return;
	}
	else
	{
		
		memcpy (&g_HelperManager.m_tHelpersList[idx], &helperList, sizeof (struct tagHelper) );

		
		MemberList_Send_toPlayer( &pl );

		
		MemberListInfo_Send_toAllPlayer( &pl, HelpersLogin );
	}
}


void CHelperManager::ProcessMessage_MemberList_Request_Update()
{
	char targetName[NAMESTRING+1];
	strncpy ( targetName, MSG_ReadString (),NAMESTRING );
	targetName[NAMESTRING]=NULL;

	int fstate = (int)MSG_ReadByte ();

	entityPCList_t pl;
	if ( 0 > g_entityListMng.FindPCList ( targetName, &pl )  )
	{
		g_logSystem->Write (
			"Error: pPlayerLIST_HELPERS_UPDATE를 받았지만 pPlayerLIST에 없음: name: %s",
			targetName );
		return;
	}
	else
	{
		MemberListInfo_Send_toAllPlayer( &pl, fstate );
	
	}
}


void CHelperManager::ProcessMessage_Reply_Helpers_List ()
{
	if ( g_config.gameServerNo != MSG_ReadByte () ) return;

	char czTempName[NAMESTRING+1];

	strncpy(czTempName,MSG_ReadString(),NAMESTRING);

	int idx = MSG_ReadShort ();
	int count = MSG_ReadByte ();
	int enumMode = static_cast<tagHelper::tagMemberList::enumMode>(MSG_ReadShort());
	
	playerCharacter_t* pTaker=gcpTools->GetPlayerRecordPointer(idx);
	
	if(NULL == pTaker) return;
	if(NULL != strcmp(pTaker->name,czTempName)) return;
	
	if(count < USERCOUNT || count > tagHelper::MAX_MEMBER) return;

	HelperEntry HelperEntries[tagHelper::MAX_MEMBER];


	for (int listmember = 0; listmember < count; listmember++ )
	{
		HelperEntries[listmember].level = MSG_ReadByte ();
		HelperEntries[listmember].job = MSG_ReadByte ();
		HelperEntries[listmember].worldIdx = MSG_ReadByte ();
		strncpy (HelperEntries[listmember].name, MSG_ReadString (),NAMESTRING);
		HelperEntries[listmember].name[NAMESTRING]=NULL;
		HelperEntries[listmember].bsync= MSG_ReadByte ();
	}

	MSG_BeginWriting ( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort( &netMessage, HELPER_SYSTEM );
		MSG_WriteShort( &netMessage, SC_LIST_Res_toServer );
		MSG_WriteByte( &netMessage, count);
		
		MSG_WriteShort ( &netMessage, enumMode);
		for ( int listmember = 0; listmember < count; listmember++)
		{
			MSG_WriteByte ( &netMessage, HelperEntries[listmember].level );
			MSG_WriteByte ( &netMessage, HelperEntries[listmember].job );
			MSG_WriteByte ( &netMessage, HelperEntries[listmember].worldIdx );
			MSG_WriteString ( &netMessage, HelperEntries[listmember].name );
			MSG_WriteByte(&netMessage, HelperEntries[listmember].bsync);
		}
	
		NET_SendMessage ( &pTaker->sock, &netMessage );
		
	}
	MSG_EndWriting(&netMessage);

}



void CHelperManager::ProcessMessage_Reply_Helpers_Update ()
{
	if ( g_config.gameServerNo != MSG_ReadByte () ) return;

		int idx = MSG_ReadShort ();
		
		playerCharacter_t* pTaker=gcpTools->GetPlayerRecordPointer(idx);
		if(NULL==pTaker) return;
		
		HelperEntry HelperEntries;
		HelperEntries.level = MSG_ReadByte ();
		HelperEntries.job = MSG_ReadByte ();
		HelperEntries.worldIdx = MSG_ReadByte ();
		strncpy ( HelperEntries.name, MSG_ReadString(),NAMESTRING);
		HelperEntries.name[NAMESTRING]=NULL;
		MSG_BeginWriting( &netMessage );
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort ( &netMessage, SC_LISTINFO_toAllPlayer);
			
			MSG_WriteByte ( &netMessage, HelperEntries.level );
			MSG_WriteByte ( &netMessage, HelperEntries.job );
			MSG_WriteByte ( &netMessage, HelperEntries.worldIdx );
			MSG_WriteString ( &netMessage, HelperEntries.name );
			
			NET_SendMessage ( &pTaker->sock, &netMessage );
			
		}
		MSG_EndWriting(&netMessage);
	
}



void CHelperManager::Add_toMemberList( playerCharacter_t* pPlayer, playerCharacter_t* pTater )
{
	



	struct tagHelper::tagMemberList::tagMember* pMember=
		&(pPlayer->tHelper.List.Member[pPlayer->tHelper.List.count]);
		
	strncpy(pMember->name, pTater->name, NAMESTRING );
	pMember->id[IDSTRING]=NULL;

	strncpy(pMember->id,pTater->userID,IDSTRING);
	pMember->name[NAMESTRING]=NULL;

	pMember->bIsMatchHELPERandTACKER=FALSE;
	pPlayer->tHelper.List.count++;
	
	
	g_DBGameServer->SaveHelpList( pPlayer );
	SendMessage_Added_toPlayer(
		pPlayer, pTater->name, pTater->level, pTater->pcJob, pTater->worldIdx );
	
	ShowLogInfo("ADDUSER :pPlayer->name:%s,add name:%s,pTater->level: %d, pPlayer of count:%d"
	            ,pPlayer->name,pTater->name,pTater->level,pPlayer->tHelper.List.count);
	
	MemberList_Request_List( pPlayer );

}



void CHelperManager::Remove_toMemberList( playerCharacter_t* pPlayer, char* removeName )
{
	
	int index=-1;
	BOOL bIsMissMatch = FALSE;
	for (int idx = 0 ; idx < pPlayer->tHelper.List.count ; idx++)
	{
		if ( NULL == stricmp ( pPlayer->tHelper.List.Member[idx].name, removeName) )
		{
			if(TRUE == pPlayer->tHelper.List.Member[idx].bIsMatchHELPERandTACKER)
				bIsMissMatch = TRUE;
			index = idx;
			break ;
		}
	}
	

	
	if( -1 == index ) 
	{
		SendMessage_Remove_Respond_toPlayer( 
			pPlayer, removeName,tagHelperPacket_SC_INVITE_RES_toHELPER::enumCode::Fail_RemoveRequest);
		return;
	}

	
	if(pPlayer->tHelper.List.mode == tagHelper::tagMemberList::enumMode::TAKER)
	{
		
		strncpy ( pPlayer->tHelper.List.Member[0].name, "",NAMESTRING);
		strncpy ( pPlayer->tHelper.List.Member[0].id, "",IDSTRING);
		
		if(HELPER_STANDARD_LEVEL <= pPlayer->level)
		{
			pPlayer->tHelper.List.mode=pPlayer->tHelper.List.mode 
				=tagHelper::tagMemberList::enumMode::HELPER;
		}
		
	}
	
	
    else
	{
		index++;
		
		int memberIndex=0;
		for (memberIndex=index ; memberIndex < pPlayer->tHelper.List.count ; memberIndex++ )
		{
			strncpy ( pPlayer->tHelper.List.Member[memberIndex - 1].name, 
				pPlayer->tHelper.List.Member[memberIndex].name,NAMESTRING);
			strncpy ( pPlayer->tHelper.List.Member[memberIndex - 1].id, 
				pPlayer->tHelper.List.Member[memberIndex].id,IDSTRING);
			pPlayer->tHelper.List.Member[memberIndex - 1].name[NAMESTRING]=NULL;
		}
		
		strncpy ( pPlayer->tHelper.List.Member[memberIndex - 1].name, "",NAMESTRING);
		strncpy ( pPlayer->tHelper.List.Member[memberIndex - 1].id, 
			pPlayer->tHelper.List.Member[memberIndex].id,IDSTRING);
		pPlayer->tHelper.List.Member[memberIndex - 1].name[NAMESTRING]=NULL;
	}
	
	
	
	pPlayer->tHelper.List.count--;
	if (USERCOUNT > pPlayer->tHelper.List.count  )
	{
		pPlayer->tHelper.List.count = 0 ;
	}

	g_DBGameServer->SaveHelpList( pPlayer );

	SendMessage_Remove_Respond_toPlayer( 
		pPlayer, removeName,tagHelperPacket_SC_INVITE_RES_toHELPER::enumCode::Success_RemoveRequest);

	if(FALSE == bIsMissMatch)
	SendMessage_Remove_Notify_toRemovePlayer(pPlayer,removeName);

	
	MemberList_Request_List ( pPlayer );
}



void CHelperManager::GiveHelpPointToUser(playerCharacter_t* pTaker)
{
	if( tagHelper::tagMemberList::HELPER == pTaker->tHelper.List.mode ) return;
	
	switch(pTaker->level) {
	case LEVEL_FOUR:
		pTaker->tHelper.tPoint.iMyPoint=pTaker->tHelper.tPoint.iMyPoint+g_logic.HelperLevelPoint.iLevel4;
		break;
	case LEVEL_TEN:
		pTaker->tHelper.tPoint.iMyPoint=pTaker->tHelper.tPoint.iMyPoint+g_logic.HelperLevelPoint.iLevel10;
		break;
	case LEVEL_TWENTY:
		pTaker->tHelper.tPoint.iMyPoint=pTaker->tHelper.tPoint.iMyPoint+g_logic.HelperLevelPoint.iLevel20;
		break;
	case LEVEL_THIRTY:
		pTaker->tHelper.tPoint.iMyPoint=pTaker->tHelper.tPoint.iMyPoint+g_logic.HelperLevelPoint.iLevel30;
		break;
	}

	
	if(HELPER_STANDARD_LEVEL > pTaker->level
		|| tagHelper::tagMemberList::TAKER != pTaker->tHelper.List.mode) return;
	
	
	if(USERCOUNT != pTaker->tHelper.List.count) return;

	
	pTaker->tHelper.List.mode = tagHelper::tagMemberList::HELPER;
}

struct tagHelper* CHelperManager::GetHelperPtr(const int idx)
{
	BOOL bretval=isvalidIndex(idx);
	if(TRUE == bretval) return &m_tHelpersList[idx];

	return NULL;
}

BOOL CHelperManager::isMyChild(const playerCharacter_t* pHelper,const char* pName) const
{	
	for(int idx = 0; idx < pHelper->tHelper.List.count; idx++)
	{
		if( NULL == strcmp(pName, pHelper->tHelper.List.Member[idx].name))  return TRUE;
	}

	return FALSE;
}
