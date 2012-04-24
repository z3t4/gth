



#include "..\global.h"
#include "CHelperManager_Encoder.h"
#include "tagHelperPacket.h"
#include "..\Tools\CTools.h"
#include <assert.h>
extern CTools* gcpTools;
#include "..\GonryunPracticeBattle\CGonryunPracticeBattle.h"
extern CGonryunPracticeBattle	g_GonryunPracticeBattle;




CHelperManager_Encoder::CHelperManager_Encoder()
{

}

CHelperManager_Encoder::~CHelperManager_Encoder()
{

}




void CHelperManager_Encoder::SendMessage_Invite_Request_toTaker ( 
	playerCharacter_t* pTaker,
	 char* pHelperName
	)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage, SC_INVITE_Req_toTAKER);
		MSG_WriteString(&netMessage, pHelperName);
		NET_SendMessage(&pTaker->sock, &netMessage);
	
	}
	MSG_EndWriting(&netMessage);

}


void CHelperManager_Encoder::SendMessage_Invite_Respond_toHelper ( 
	playerCharacter_t* pHelper, 
	int errorCode 
	 )
{
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage, SC_INVITE_Res_toHELPER);
		MSG_WriteShort(&netMessage, errorCode);
		NET_SendMessage(&pHelper->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	
}

void CHelperManager_Encoder::SendMessage_Invite_Respond_toTaker( 
	playerCharacter_t* pTaker,int errorCode)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage, SC_INVITE_Res_toTAKER);
		MSG_WriteShort(&netMessage, errorCode);
		NET_SendMessage(&pTaker->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}



void CHelperManager_Encoder::SendMessage_Remove_Respond_toPlayer(
	playerCharacter_t* pPlayer, char* removeName,int errorcode )

{
	MSG_BeginWriting ( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage, SC_REMOVE_Res_toPlayer);
		MSG_WriteShort(&netMessage, errorcode);
		MSG_WriteString ( &netMessage, removeName);
		
		if ( pPlayer->ready )
		{
			NET_SendMessage ( &pPlayer->sock, &netMessage );
		}
	}
	MSG_EndWriting(&netMessage);

	ShowLogInfo("RemoveUser :pPlayer->name:%s, removeName:%s",pPlayer->name,removeName);
}



void CHelperManager_Encoder::SendMessage_Added_toPlayer ( 
	playerCharacter_t* pHelper, char* name, int level, int job, int worldIdx )
{
	
	MSG_BeginWriting ( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort( &netMessage, HELPER_SYSTEM );
		MSG_WriteShort( &netMessage, SC_SEND_ADD_toPlayer );
		
		MSG_WriteByte ( &netMessage, level );
		MSG_WriteByte ( &netMessage, job );
		MSG_WriteByte ( &netMessage, worldIdx );
		MSG_WriteString ( &netMessage, name );
		
		if ( pHelper->ready )
		{
			NET_SendMessage ( &pHelper->sock, &netMessage );
		}
	}
	MSG_EndWriting(&netMessage);
}

void CHelperManager_Encoder::SendMessage_Remove_Notify_toRemovePlayer(
	playerCharacter_t* pPlayer,char* premoveName)
{



	int idx=GTH_FindPCByName(premoveName);
	 
	if (0 <= idx )
	{
		playerCharacter_t* pRemoveUser=gcpTools->GetPlayerRecordPointer(idx);
		if(NULL == pRemoveUser)return;
	
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort( &netMessage, SC_REMOVE_NOTIFY_toPlayer );
			MSG_WriteString(&netMessage, pPlayer->name);	
			NET_SendUnreliableMessage(&pRemoveUser->sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
		ShowLogInfo("RemoveUser :pRemoveUser->name:%s, pRemoveUser->name:%s",
			pRemoveUser->name,premoveName);
	}
	
	else
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage,	 EXTEND_SECOND);
			MSG_WriteShort( &netMessage, HELPER_SYSTEM);
			MSG_WriteShort( &netMessage, SS_REMOVE_NOTIFY_toPlayer );
			MSG_WriteString(&netMessage, premoveName);
			MSG_WriteString(&netMessage, pPlayer->name);	
			if ( TRUE == g_config.isManager )
			{
				
				for (int serveridx=1; serveridx < MAX_MEMBER_SERVER; serveridx++)	
				{
					if ( !g_memberServer[serveridx].active ) continue;
					
					NET_SendUnreliableMessage(&g_memberServer[serveridx].sock, &netMessage);
				}
			}
			else
			{
				
				MSG_WriteByte(&netMessage, g_config.gameServerNo);
				
				NET_SendUnreliableMessage(&localSocket, &netMessage);
			}
		}
		MSG_EndWriting(&netMessage);

		

	}

}

void CHelperManager_Encoder::SendMessage_UpdatePointInfo_toPlayer(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting ( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage,EXTEND_SECOND);
		MSG_WriteShort( &netMessage, HELPER_SYSTEM );
		MSG_WriteShort( &netMessage, SC_UPDATE_POINTINFO_SEND_toPlayer );
		MSG_WriteLong( &netMessage,pPlayer->tHelper.tPoint.iMyPoint);
		MSG_WriteLong( &netMessage,pPlayer->tHelper.tPoint.iHelpPoint);
		MSG_WriteLong( &netMessage,pPlayer->tHelper.tPoint.iTotalHelpPoint);
		NET_SendMessage ( &pPlayer->sock, &netMessage );
		
	}
	MSG_EndWriting(&netMessage);

}



void CHelperManager_Encoder::SendMessage_RequestSummons(playerCharacter_t* pHelper,char* toName)
{
	int idx=GTH_FindPCByName(toName);

	if (0 <= idx) 
	{
		playerCharacter_t* pTaker=gcpTools->GetPlayerRecordPointer(idx);
		if(NULL == pTaker) return;

		
		
		if( (playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER
			== pTaker->GonryunBattlePractice.MemberShip) ||
			(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT
			== pTaker->GonryunBattlePractice.MemberShip)) return;
		
		if((pTaker->worldIdx == tagGolryunBattle::Golryun_Battle_Map_Index) ||
			(pTaker->worldIdx == DAN_BATTLEMAP_NO)) return;


		if(BUSY_STATE_NONE != pTaker->busyState) return;

		PC_SetSummonsInfo(pTaker, pHelper->name, pHelper->worldIdx, pHelper->position);

		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort(&netMessage, SC_SPAWN_Req_toTaker);
			MSG_WriteString(&netMessage, pHelper->name);	
			NET_SendMessage(&pTaker->sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	else	
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort(&netMessage, SS_SPAWN_Req_fromServer);
			MSG_WriteString(&netMessage, toName); 
			MSG_WriteString(&netMessage, pHelper->name);	
			MSG_WriteByte(&netMessage, pHelper->worldIdx);	
			MSG_WritePosition(&netMessage, pHelper->position); 
				
			if ( TRUE == g_config.isManager )
			{
				
				for (int serveridx=1; serveridx < MAX_MEMBER_SERVER; serveridx++)	
				{
					if ( !g_memberServer[serveridx].active ) continue;
					
					NET_SendUnreliableMessage(&g_memberServer[serveridx].sock, &netMessage);
				}
			}
			else
			{
				
				MSG_WriteByte(&netMessage, g_config.gameServerNo);
				
				NET_SendUnreliableMessage(&localSocket, &netMessage);
			}
		}
		MSG_EndWriting(&netMessage);
	}
}


void CHelperManager_Encoder::SendMessage_ReplySummons(int reply, char* toName, char* fromName)
{
	

	int idx = GTH_FindPCByName(toName);
	if ( 0 <= idx) 
	{
		playerCharacter_t* pTaker=gcpTools->GetPlayerRecordPointer(idx);
		if(NULL == pTaker) return;
		
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort(&netMessage, SC_SPAWN_Res_toTaker);
			MSG_WriteByte(&netMessage, reply);	
			MSG_WriteString(&netMessage, fromName);	
			NET_SendMessage(&pTaker->sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
	}
	else	
	{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort(&netMessage, SS_SPAWN_Res_fromServer);
			MSG_WriteByte(&netMessage, reply);	
			MSG_WriteString(&netMessage, toName);	
			MSG_WriteString(&netMessage, fromName);	
			
			if ( TRUE == g_config.isManager )
			{
				
				for (int serveridx=1; serveridx < MAX_MEMBER_SERVER; serveridx++)	
				{
					if ( !g_memberServer[serveridx].active ) continue;
					
					NET_SendUnreliableMessage(&g_memberServer[serveridx].sock, &netMessage);
				}
			}
			else
			{
				
				MSG_WriteByte(&netMessage, g_config.gameServerNo);
				
				NET_SendUnreliableMessage(&localSocket, &netMessage);
			}
		}
		MSG_EndWriting(&netMessage);
	}
}



void CHelperManager_Encoder::SendMessage_Helper_Mypoint(playerCharacter_t* pHelper)
{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort(&netMessage, SC_CONFIRM_MYPOINT_Res_toPlayer);
			MSG_WriteLong(&netMessage, pHelper->tHelper.tPoint.iMyPoint);	
			NET_SendMessage(&pHelper->sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
}

void CHelperManager_Encoder::SendMessage_Helper_Helppoint(playerCharacter_t* pHelper)
{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort(&netMessage, SC_CONFIRM_HELPPOINT_Res_toPlayer);
			MSG_WriteLong(&netMessage, pHelper->tHelper.tPoint.iHelpPoint);	
			NET_SendMessage(&pHelper->sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
}


void CHelperManager_Encoder::SendMessage_Helper_StackOpen(playerCharacter_t* pHelper)
{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort(&netMessage, SC_STACK_OPEN_Req_toPlayer);
			NET_SendMessage(&pHelper->sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
}


void CHelperManager_Encoder::PC_InitSummonsInfo(playerCharacter_t* pHelper)
{
	memset(&pHelper->summonsInfo, 0, sizeof(summons_t));
}


void CHelperManager_Encoder::PC_SetSummonsInfo(
	playerCharacter_t *pHelper, char *name, int worldIdx, vec3_t position)
{
	strcpy(pHelper->summonsInfo.summoner, name);
	pHelper->summonsInfo.worldIdx = worldIdx;
	vec_copy(position, pHelper->summonsInfo.position);
	
}

void CHelperManager_Encoder::SendMessage_Mypoint_toTaker(
	playerCharacter_t* pPlayer, char* pTakerName,char* pHelperName, int MyPoint)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort( &netMessage, HELPER_SYSTEM );
		MSG_WriteShort(&netMessage, SC_SENDMYPOINT_toAllPlayer);
		MSG_WriteString(&netMessage,pTakerName);
		MSG_WriteString(&netMessage,pHelperName);
		MSG_WriteLong(&netMessage,MyPoint);
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	
	MSG_EndWriting(&netMessage);
	
}

void CHelperManager_Encoder::SendMessage_Helppoint_toHelper(
	playerCharacter_t* pc, char* pGiverName, int HelpPoint)
{
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, EXTEND_SECOND);
			MSG_WriteShort ( &netMessage, HELPER_SYSTEM );
			MSG_WriteShort(&netMessage, SC_SENDHELPPOINT_toHelper);
			MSG_WriteString(&netMessage,pGiverName);
			MSG_WriteLong(&netMessage,HelpPoint);
			NET_SendMessage(&pc->sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
}