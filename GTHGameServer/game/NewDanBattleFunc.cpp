




#include "../Global.h"
#include "../Tools/CTools.h"

extern CTools* gcpTools;

#include "..\GolryunBattle\CServerLpackCtrl.h"
extern CServerLpackCtrl* gcpServerLpackCtrl;






void GTH_DecodeDanBattleSystem()
{
	enumNewDanBattlePacket Packet = static_cast<enum enumNewDanBattlePacket>(MSG_ReadShort());	
	switch(Packet)
	{		
		
	case CC_ACTIVE_SYSTEM:
		GTH_ProcessMessage_DanBattleActiveSystem();
		break;		

		
	case CC_DAN_BATTLE_WIN_OPEN:
		GTH_ProcessMessage_NewDanBattleWinOpen();
		break;

		
	case CC_REQUEST_DANBATTLE:		
		GTH_ProcessMessage_RequestDanBattle();
		break;

	case CC_SUGGEST_DANBATTLE:
		GTH_ProcessMessage_SuggestDanBattleReply();
		break;




































		
	case CS_DAN_BATTLE_CHANGE_STATUS:
		GTH_ProcessMessage_DanBattleChangeStatus();
		break;

		
	case GSC_DAN_BATTLE_POWER_END:
		GTH_ProcessDanbattlePowerEnd();
		break;
		
	}
}






void GTH_ProcessMessage_DanBattleActiveSystem()
{

}

void GTH_ProcessMessage_DanBattleChangeStatus()
{
	NewDanBattleSystem::DanBattleInfo::enumBattleStatus status = 
		(NewDanBattleSystem::DanBattleInfo::enumBattleStatus)MSG_ReadLong();

	char DanName[GUILDNAMESTRING+1];
	strncpy(DanName, MSG_ReadString(), GUILDNAMESTRING);
	DanName[GUILDNAMESTRING] = NULL;
	
	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = NULL;
	pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo(DanName);

	if ( pDanBattleInfo == NULL)
		return;

	

	if (status == NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_END)
	{
		pDanBattleInfo->endType = NewDanBattleSystem::DanBattleInfo::enumEndType::SYSTEM;
		g_NewDanBattleSystem.DecsionDanBattleBestMember(pDanBattleInfo);
		g_NewDanBattleSystem.DecsionDanBattleWinTO_TimeOut(pDanBattleInfo);
		
		g_logSystem->WriteToNewDanBattleLog("%s, %d, %d, %s, %d, %d, %d",
			"LOG_CLASS_NEWDANBATTLE_RESULT",
			pDanBattleInfo->nDanBattleIdx,	
			pDanBattleInfo->AcceptDanInfo.wincode,
			pDanBattleInfo->AcceptDanInfo.szGuildName,
			pDanBattleInfo->BattleMode,				
			pDanBattleInfo->AcceptDanInfo.nRemainPoint,
			pDanBattleInfo->nBattleMapIdx);
		
		g_logSystem->WriteToNewDanBattleLog("%s, %d, %d, %s, %d, %d, %d",
			"LOG_CLASS_NEWDANBATTLE_RESULT",
			pDanBattleInfo->nDanBattleIdx,				
			pDanBattleInfo->RequestDanInfo.wincode,
			pDanBattleInfo->RequestDanInfo.szGuildName,
			pDanBattleInfo->BattleMode,				
			pDanBattleInfo->RequestDanInfo.nRemainPoint,
			pDanBattleInfo->nBattleMapIdx);

	}

	g_NewDanBattleSystem.SetDanBatttleStatus(pDanBattleInfo, status);	
	
	
}







void GTH_ProcessMessage_RequestDanBattle()
{
	char szDanName[GUILDNAMESTRING+1];
	int DanWarType			= 0;	
	int DanWarTImeType		= 0;
	int GuildMemberCount	= 0;	
	
	BOOL bPlunderItem		= FALSE;

	_snprintf(szDanName, GUILDNAMESTRING, MSG_ReadString());
	szDanName[GUILDNAMESTRING]= NULL;
	
	DanWarType		= MSG_ReadByte();
	DanWarTImeType	= MSG_ReadByte();

	
	bPlunderItem	= MSG_ReadLong();


		
	
	if ( g_curPC->guildIdx < 0)	
	{
		GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::not_join_guild);	
		return;
	}		

	
	if ( (GTH_GetGuildMemberIndex(g_curPC->guildIdx, GUILD_MASTER, g_curPC->name) == -1) &&
		(GTH_GetGuildMemberIndex(g_curPC->guildIdx,	 GUILD_SUBMASTER, g_curPC->name)) == -1)
	{
		GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::not_capacity);
		return;
	}	

	int LiveGuildNumber =GTH_Get_OnlineGuildMemeberCount(g_curPC->worldIdx, g_curPC->guildIdx); 

#ifndef _DEV_TEST
	
	if ( LiveGuildNumber <	NewDanBattleSystem::MIN_GUILD_MEMBER_NUMBER_FOR_DAN_BATTLE)
	{
		GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::not_enough_my_member);
		return;
	}
#endif
	
	int TargetGuildIdx = GTH_GetGuildIndexByName(szDanName);
	if ( TargetGuildIdx == -1 )
	{
		GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::nonexstent_guild);
		return;
	}

	
	if ( TargetGuildIdx == g_curPC->guildIdx)
	{
		GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::myself_guild);
		return;
	}

	
	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = NULL;
	pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo(TargetGuildIdx);
	if ( pDanBattleInfo != NULL)
	{
		GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::target_can_not_reciveMessage);
		return;
	}

	
	int LiveTargetGuildPcNum = 1;
	
	LiveTargetGuildPcNum = GTH_Get_OnlineGuildMemeberCount(g_curPC->worldIdx, TargetGuildIdx);

#ifndef _DEV_TEST
	if ( LiveTargetGuildPcNum <	NewDanBattleSystem::MIN_GUILD_MEMBER_NUMBER_FOR_DAN_BATTLE)
	{
		GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::not_enough_target_member);
		return;
	}
#endif
	


	
	playerCharacter_t *pTargetGuildPC = GTH_GetGuildNoneBusyMemberPtr(TargetGuildIdx, g_curPC->worldIdx, MAX_GUILD_MASTERS);
	if ( pTargetGuildPC == NULL)
	{
		
		pTargetGuildPC = GTH_GetGuildNoneBusyMemberPtr(TargetGuildIdx, g_curPC->worldIdx, MAX_GUILD_SUBMASTERS);
		if ( pTargetGuildPC == NULL)
		{
			
			GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::target_can_not_reciveMessage);
			return;
		}
	}

	float BattleTime;
	if ( DanWarTImeType == 0)
		BattleTime = g_NewDanBattleSystem.m_nBattleTimeForMin1;
	else
		BattleTime = g_NewDanBattleSystem.m_nBattleTimeForMin2;


	
	NewDanBattleSystem::DanBattleInfo* pRequestDanBattleInfo = &pTargetGuildPC->DanBattleInfo;
	NewDanBattleSystem::DanBaseInfo*   pRequestDanBaseInfo	 = &pTargetGuildPC->DanBattleInfo.RequestDanInfo;	
	NewDanBattleSystem::DanBaseInfo*   pAcceptDanBaseInfo	 = &pTargetGuildPC->DanBattleInfo.AcceptDanInfo;	

	
	
	g_NewDanBattleSystem.InitDanBatlleInfo(&(pTargetGuildPC->DanBattleInfo));
	
	pRequestDanBattleInfo->fBattleSec			= BattleTime;
	pRequestDanBattleInfo->nBattleMapIdx		= g_curPC->worldIdx;
	pRequestDanBattleInfo->BattleMode			= (NewDanBattleSystem::DanBattleInfo::enumBattleMode)DanWarType;
	pRequestDanBattleInfo->BattleStatus			= NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_NONE;
	
	pRequestDanBattleInfo->bPlunderPremiumItem	= bPlunderItem;

	pRequestDanBaseInfo->nRequest_AcceptPcIdx	= g_curPC->idx;
	pRequestDanBaseInfo->nDanIdx				= g_curPC->guildIdx;
	pRequestDanBaseInfo->nRemainPoint			= LiveGuildNumber;
	

	





	_snprintf(pRequestDanBaseInfo->szGuildName, GUILDNAMESTRING, g_guild[g_curPC->guildIdx].name);
	pRequestDanBaseInfo->szGuildName[GUILDNAMESTRING] = NULL;

	
	GTH_AddNoticePC( g_curPC,		 ENTITY_PC, pTargetGuildPC->idx, BUSY_STATE_DAN_BATTLE );
	GTH_AddNoticePC( pTargetGuildPC, ENTITY_PC, g_curPC->idx, BUSY_STATE_DAN_BATTLE );

	
	GTH_SendMessage_RequestDanBattleReply(g_curPC,
		tagGCPacket_NewDanBattle::enumCode::success,
		0);
	
	
	GTH_SendMessage_SuggestDanBattle(pTargetGuildPC, &pTargetGuildPC->DanBattleInfo);
}








void GTH_SendMessage_RequestDanBattleReply(playerCharacter_t* pPlayer, BOOL Reply, int ErrorIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);		
		MSG_WriteShort(&netMessage, GSC_REQUEST_DANBATTLE);		
		MSG_WriteLong(&netMessage,	Reply);
		MSG_WriteByte(&netMessage,	ErrorIdx);
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_SendMessage_SuggestDanBattle(playerCharacter_t* pPlayer, NewDanBattleSystem::DanBattleInfo* pInfo)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);		
		MSG_WriteShort(&netMessage, GSC_SUGGEST_DANBATTLE);		
		MSG_WriteByte(&netMessage,  pInfo->BattleMode);
		MSG_WriteFloat(&netMessage, pInfo->fBattleSec);
		MSG_WriteLong(&netMessage,  pInfo->RequestDanInfo.nDanIdx);
		MSG_WriteByte(&netMessage,  pInfo->RequestDanInfo.nRemainPoint);
		MSG_WriteByte(&netMessage,  pInfo->nBattleMapIdx);
		MSG_WriteString(&netMessage,  pInfo->RequestDanInfo.szGuildName);
		
		MSG_WriteLong(&netMessage,  pInfo->bPlunderPremiumItem);		
		
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_ProcessMessage_SuggestDanBattleReply()
{
	tagGCPacket_NewDanBattle::enumCode code = tagGCPacket_NewDanBattle::enumCode::fail;
	code = (tagGCPacket_NewDanBattle::enumCode)MSG_ReadByte();


	
	int RequestDanIdx = g_curPC->DanBattleInfo.RequestDanInfo.nDanIdx;
	int RequestPcIdx  = g_curPC->DanBattleInfo.RequestDanInfo.nRequest_AcceptPcIdx;

	playerCharacter_t* pRequestPlayer = NULL;
	pRequestPlayer = gcpTools->GetPlayerRecordPointer(RequestPcIdx);	

	GTH_UndoPcDanBattle(g_curPC);	

	if ( pRequestPlayer != NULL )
		GTH_UndoPcDanBattle(pRequestPlayer);


	
	if ( code == tagGCPacket_NewDanBattle::enumCode::fail)
	{						
		if ( pRequestPlayer == NULL)
			return;	

		if ( pRequestPlayer->guildIdx != g_curPC->DanBattleInfo.RequestDanInfo.nDanIdx)
			return;









		
		GTH_SendMessage_RequestDanBattleReply(pRequestPlayer, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::Not_Accept_danBattle);		

		return;
	}



	
	
	if ( pRequestPlayer == NULL)
	{		
		GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::target_can_not_reciveMessage);
		return;
	}	
	if ( pRequestPlayer->guildIdx != RequestDanIdx)
	{
	
		GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::target_can_not_reciveMessage);
		return;
	}

	
	
	
	
	vector<NewDanBattleSystem::DanBaseInfo::sDanBattleMember> vRequestGuildMember;
	vector<NewDanBattleSystem::DanBaseInfo::sDanBattleMember> vAcceptGuildMember;
	int LiveGuildNumber	 =	GTH_Get_OnlineGuildMemeberCount(
		g_curPC->worldIdx, g_curPC->guildIdx, &vAcceptGuildMember); 

	int LiveTargetGuildPcNum = GTH_Get_OnlineGuildMemeberCount(
		g_curPC->worldIdx, RequestDanIdx, &vRequestGuildMember);


	
#ifndef _DEV_TEST
	if ( LiveTargetGuildPcNum < g_NewDanBattleSystem.m_nMinPcCount ||
		LiveTargetGuildPcNum < g_NewDanBattleSystem.m_nMinPcCount ||
		g_NewDanBattleSystem.m_bActive == FALSE)
	{
		
		GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::target_can_not_reciveMessage);		
		
		GTH_SendMessage_RequestDanBattleReply(pRequestPlayer, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::target_can_not_reciveMessage);
		return;
	}
#endif
	

	for (int nIdx =0; nIdx < vRequestGuildMember.size(); nIdx++)
		ShowLogInfo("RequestGuildMember = %s",vRequestGuildMember[nIdx].name );

	for (nIdx =0; nIdx < vRequestGuildMember.size(); nIdx++)
		ShowLogInfo("RequestGuildMember = %s",vAcceptGuildMember[nIdx].name );

	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = NULL;
	
	pDanBattleInfo = g_NewDanBattleSystem.InsertDanBattle(
		RequestDanIdx,
		RequestPcIdx,
		vRequestGuildMember,
		g_curPC->guildIdx,
		g_curPC->idx,
		vAcceptGuildMember,
		g_curPC->DanBattleInfo.BattleMode,
		g_curPC->DanBattleInfo.nBattleMapIdx, 
		g_curPC->DanBattleInfo.fBattleSec,
		pRequestPlayer->exp,
		g_curPC->exp,
		g_curPC->DanBattleInfo.bPlunderPremiumItem);

	
	
	
	if ( pDanBattleInfo == NULL){				
		GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::target_can_not_reciveMessage);		
		
		if ( pRequestPlayer != NULL){			
			GTH_SendMessage_RequestDanBattleReply(pRequestPlayer, 
			tagGCPacket_NewDanBattle::enumCode::fail,
			tagGCPacket_NewDanBattle::enumError::target_can_not_reciveMessage);
		}
		return;
	}

	
	if ( g_NewDanBattleSystem.m_bActive == FALSE){
		GTH_SendMessage_RequestDanBattleReply(g_curPC, 
			tagGCPacket_NewDanBattle::enumCode::fail, 
			tagGCPacket_NewDanBattle::enumError::attleInsert_Error);		
	}

	GTH_SendMessage_RequestDanBattleReply(pRequestPlayer,
		tagGCPacket_NewDanBattle::enumCode::success,
		1);
	
	g_DBGameServer->Insert_Danbattle(pDanBattleInfo->nDanBattleIdx, RequestDanIdx, g_curPC->guildIdx);	



	
}







void GTH_SendMessage_DanBattleInfo(playerCharacter_t* pPlayer, NewDanBattleSystem::DanBattleInfo* pDanBattleInfo)
{
	if ( pDanBattleInfo == NULL || pPlayer == NULL)
		return;

	NewDanBattleSystem::DanBaseInfo* pMyDanBaseInfo		= NULL;
	NewDanBattleSystem::DanBaseInfo* pEnemyDanBaseInfo	= NULL;

	pMyDanBaseInfo	  = g_NewDanBattleSystem.GetDanBaseInfo(pDanBattleInfo, pPlayer->guildIdx);
	pEnemyDanBaseInfo = g_NewDanBattleSystem.GetEnemyDanBaseInfo(pDanBattleInfo->nDanBattleIdx, pPlayer->guildIdx);

	if ( pDanBattleInfo == NULL || pEnemyDanBaseInfo == NULL)
		return;

	NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pMyMemberInfo = NULL;
	pMyMemberInfo = g_NewDanBattleSystem.GetDanBattleMember(pDanBattleInfo, pPlayer->guildIdx, pPlayer->name);
	if ( pMyMemberInfo == NULL)
		return;	

	if ( pMyDanBaseInfo->nDanIdx <0 || pMyDanBaseInfo->nDanIdx >= MAX_GUILDS)
		return;

	if ( pEnemyDanBaseInfo->nDanIdx <0 || pEnemyDanBaseInfo->nDanIdx >= MAX_GUILDS)
		return;

	int RemainSec = pDanBattleInfo->fRemainTime - g_timer->GetAppMilliTime();
	if ( RemainSec < 0 )
		RemainSec = 0;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);
		MSG_WriteShort(&netMessage, GSC_DAN_BATTLE_INFO);
		MSG_WriteByte(&netMessage,  pDanBattleInfo->BattleMode);
		MSG_WriteByte(&netMessage,  pDanBattleInfo->BattleStatus);

		
		MSG_WriteLong(&netMessage, pDanBattleInfo->nDanBattleIdx);

		
		MSG_WriteFloat(&netMessage, RemainSec);

		
		MSG_WriteByte(&netMessage, pDanBattleInfo->nBattleMapIdx);

		
		MSG_WriteLong(&netMessage,  pMyDanBaseInfo->nDanIdx);		

		
		MSG_WriteByte(&netMessage,  pMyDanBaseInfo->nRemainPoint);

		
		MSG_WriteLong(&netMessage,  pMyDanBaseInfo->nKillPoint);

		
		MSG_WriteLong(&netMessage,  g_guild[pMyDanBaseInfo->nDanIdx].nKillPoint);

		
		MSG_WriteLong(&netMessage,  pMyMemberInfo->killPoint);				

		
		MSG_WriteLong(&netMessage,  pPlayer->nAccumulationBattleKillPoint);

		
		MSG_WriteLong(&netMessage,  pEnemyDanBaseInfo->nKillPoint);

		
		MSG_WriteByte(&netMessage,  pEnemyDanBaseInfo->nRemainPoint);

		
		MSG_WriteLong(&netMessage,  g_guild[pEnemyDanBaseInfo->nDanIdx].nKillPoint);
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	

	

}







void GTH_SendMessage_DanBattleResult(playerCharacter_t* pPlayer, tagGCPacket_NewDanBattle::enumwincode code, char* enemyGuildName, char* bestid, int wincount, int losecount, int mypoint, NewDanBattleSystem::DanBattleInfo::enumEndType type)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);		
		MSG_WriteShort(&netMessage, GSC_DAN_BATTLE_RESULT);
		MSG_WriteByte(&netMessage,	code);
		MSG_WriteLong(&netMessage,  type);		
		MSG_WriteString(&netMessage, enemyGuildName);
		MSG_WriteString(&netMessage,bestid);	
		MSG_WriteLong(&netMessage,  wincount);	
		MSG_WriteLong(&netMessage,  losecount);	
		MSG_WriteLong(&netMessage,  mypoint);		
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_ProcessMessage_NewDanBattleWinOpen()
{
	BOOL Reply = tagGCPacket_NewDanBattle::enumCode::success;

	if( g_curPC->busyState != BUSY_STATE_NONE || !g_curPC->alive  || !g_curPC->ready ||
		g_NewDanBattleSystem.m_bActive == FALSE)		
	{
		Reply = tagGCPacket_NewDanBattle::enumCode::fail;	
		GTH_SendMessage_NewDanBattleWinOpenReply(g_curPC, Reply);
		return;
	}

	NewDanBattleSystem::DanBattleInfo* pDanBatttle  = NULL;
	pDanBatttle = g_NewDanBattleSystem.GetDanBattleInfo(g_curPC->guildIdx);

	if ( pDanBatttle != NULL)
	{
		Reply = tagGCPacket_NewDanBattle::enumCode::fail;	
		GTH_SendMessage_NewDanBattleWinOpenReply(g_curPC, Reply);
		return;
	}


	if ( g_curPC->worldIdx ==0 || g_curPC->worldIdx == 9)
	{
		Reply = tagGCPacket_NewDanBattle::enumCode::fail;	
		GTH_SendMessage_NewDanBattleWinOpenReply(g_curPC, Reply);
		return;
	}
		
	GTH_SendMessage_NewDanBattleWinOpenReply(g_curPC, Reply);

}






void GTH_SendMessage_NewDanBattleWinOpenReply(playerCharacter_t* pPlayer, BOOL Reply)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);		
		MSG_WriteShort(&netMessage, GSC_DAN_BATTLE_WIN_OPEN);				
		MSG_WriteLong(&netMessage,	Reply);			
		MSG_WriteByte(&netMessage,	g_NewDanBattleSystem.m_nMinPcCount);		
		MSG_WriteLong(&netMessage,	g_NewDanBattleSystem.m_nBattleTimeForMin1);		
		MSG_WriteLong(&netMessage,	g_NewDanBattleSystem.m_nBattleTimeForMin2);		
		MSG_WriteLong(&netMessage,	pPlayer->worldIdx);				

		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_ProcessMessage_NewDanBattleReady(int nGuildIdx, int EnemyGuildIdx, int nWorldIdx, int RemainTime, int mode)
{
	
	if ( EnemyGuildIdx < 0 || EnemyGuildIdx >= MAX_GUILDS)
		return;	

	if ( nGuildIdx< 0 || nGuildIdx >= MAX_GUILDS)
		return;	
	
	guild_t* pGuild = NULL;
	guild_t* pEnemyGuild = NULL;

	pGuild = &g_guild[nGuildIdx];
	pEnemyGuild = &g_guild[EnemyGuildIdx];
	
	playerCharacter_t* pPC  = NULL;	
	
	
	pPC = gcpTools->GetPlayerRecordPointer(pGuild->masterName);	
	if ( pPC != NULL ){		
		if ( pPC->worldIdx == nWorldIdx)
		{		
			GTH_SendMessage_NewDanBattleReady(pPC, EnemyGuildIdx, g_guild[EnemyGuildIdx].name, nWorldIdx, RemainTime, 
				(NewDanBattleSystem::DanBattleInfo::enumBattleMode)mode);
		}
	}

	
	for(int SubMasterIdx=0; SubMasterIdx < MAX_GUILD_SUBMASTERS; SubMasterIdx++)
	{
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->subMasterName[SubMasterIdx]);
		if ( pPC != NULL ){
			if ( pPC->worldIdx == nWorldIdx)
			{		
				GTH_SendMessage_NewDanBattleReady(pPC, EnemyGuildIdx, g_guild[EnemyGuildIdx].name, nWorldIdx, RemainTime, 
					(NewDanBattleSystem::DanBattleInfo::enumBattleMode)mode);		
			}
		}
	}
	
	
	for(int factionMasterIdx=0; factionMasterIdx < MAX_GUILD_FACTIONMASTERS; factionMasterIdx++)
	{
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->factionMasterName[factionMasterIdx]);
		if ( pPC != NULL ){
			if ( pPC->worldIdx == nWorldIdx)
			{		
					GTH_SendMessage_NewDanBattleReady(pPC, EnemyGuildIdx, g_guild[EnemyGuildIdx].name, nWorldIdx,  RemainTime, 
					(NewDanBattleSystem::DanBattleInfo::enumBattleMode)mode);		
			}
		}
	}	

	for(int mateIdx=0; mateIdx < MAX_GUILD_MATES; mateIdx++)
	{
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->mateName[mateIdx]);
		if ( pPC != NULL ){
			if ( pPC->worldIdx == nWorldIdx)
			{		
				GTH_SendMessage_NewDanBattleReady(pPC, EnemyGuildIdx, g_guild[EnemyGuildIdx].name, nWorldIdx, RemainTime, 
					(NewDanBattleSystem::DanBattleInfo::enumBattleMode)mode);	
			}
		}
	}
	

	
}






void GTH_ProcessMessage_NewDanBattleStart(int nGuildIdx, int EnemyGuildIdx, int nWorldIdx, int RemainTime)
{
if ( EnemyGuildIdx < 0 || EnemyGuildIdx >= MAX_GUILDS)
		return;	
	
	guild_t* pGuild = NULL;

	pGuild = &g_guild[nGuildIdx];
	playerCharacter_t* pPC  = NULL;	
	
	
	pPC = gcpTools->GetPlayerRecordPointer(pGuild->masterName);	
	if ( pPC != NULL ){		
		if ( pPC->worldIdx == nWorldIdx)
		{		
			GTH_SendMessage_NewDanBattleStart(pPC, EnemyGuildIdx, nWorldIdx, RemainTime);
		}
	}

	
	for(int SubMasterIdx=0; SubMasterIdx < MAX_GUILD_SUBMASTERS; SubMasterIdx++)
	{
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->subMasterName[SubMasterIdx]);
		if ( pPC != NULL ){
			if ( pPC->worldIdx == nWorldIdx)
			{		
				GTH_SendMessage_NewDanBattleStart(pPC, EnemyGuildIdx, nWorldIdx, RemainTime);		
			}
		}
	}
	
	
	for(int factionMasterIdx=0; factionMasterIdx < MAX_GUILD_FACTIONMASTERS; factionMasterIdx++)
	{
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->factionMasterName[factionMasterIdx]);
		if ( pPC != NULL ){
			if ( pPC->worldIdx == nWorldIdx)
			{		
				GTH_SendMessage_NewDanBattleStart(pPC, EnemyGuildIdx,nWorldIdx,  RemainTime);
			}
		}
	}	

	for(int mateIdx=0; mateIdx < MAX_GUILD_MATES; mateIdx++)
	{
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->mateName[mateIdx]);
		if ( pPC != NULL ){
			if ( pPC->worldIdx == nWorldIdx)
			{		
				GTH_SendMessage_NewDanBattleStart(pPC, EnemyGuildIdx, nWorldIdx, RemainTime);
			}
		}
	}
	
}







void GTH_ProcessMessage_NewDanBattleEnd(int nGuildIdx, int EnemyGuildIdx, int nWorldIdx, int RemainTime)
{
	if ( EnemyGuildIdx < 0 || EnemyGuildIdx >= MAX_GUILDS)
		return;	
	
	guild_t* pGuild = NULL;

	pGuild = &g_guild[nGuildIdx];
	playerCharacter_t* pPC  = NULL;	
	
	
	pPC = gcpTools->GetPlayerRecordPointer(pGuild->masterName);	
	if ( pPC != NULL ){				
		if ( pPC->worldIdx == nWorldIdx)
		{		
			GTH_SendMessage_NewDanBattleEnd(pPC, EnemyGuildIdx, nWorldIdx, RemainTime);
		}
	}

	
	for(int SubMasterIdx=0; SubMasterIdx < MAX_GUILD_SUBMASTERS; SubMasterIdx++)
	{
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->subMasterName[SubMasterIdx]);
		if ( pPC != NULL ){
			if ( pPC->worldIdx == nWorldIdx)
			{		
				GTH_SendMessage_NewDanBattleEnd(pPC, EnemyGuildIdx, nWorldIdx, RemainTime);		
			}
		}
	}
	
	
	for(int factionMasterIdx=0; factionMasterIdx < MAX_GUILD_FACTIONMASTERS; factionMasterIdx++)
	{
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->factionMasterName[factionMasterIdx]);
		if ( pPC != NULL ){
			if ( pPC->worldIdx == nWorldIdx)
			{		
				GTH_SendMessage_NewDanBattleEnd(pPC, EnemyGuildIdx,nWorldIdx,  RemainTime);
			}
		}
	}	

	for(int mateIdx=0; mateIdx < MAX_GUILD_MATES; mateIdx++)
	{
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->mateName[mateIdx]);
		if ( pPC != NULL ){
			if ( pPC->worldIdx == nWorldIdx)
			{		
				GTH_SendMessage_NewDanBattleEnd(pPC, EnemyGuildIdx, nWorldIdx, RemainTime);
			}
		}
	}
	

}






void GTH_SendMessage_NewDanBattleReady(playerCharacter_t* pPlayer, int EneumGuildIdx, char* szEnemyGuildName, int nWorldIdx, int RemainTime, NewDanBattleSystem::DanBattleInfo::enumBattleMode mode)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);		
		MSG_WriteShort(&netMessage, GSC_DAN_BATTLE_READY);		
		MSG_WriteLong(&netMessage,  EneumGuildIdx);
		MSG_WriteString(&netMessage,szEnemyGuildName);
		MSG_WriteByte(&netMessage,  nWorldIdx);
		MSG_WriteLong(&netMessage,  RemainTime);
		MSG_WriteByte(&netMessage,  mode);
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_SendMessage_NewDanBattleStart(playerCharacter_t* pPlayer, int EneumGuildIdx, int nWorldIdx, int RemainTime)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);		
		MSG_WriteShort(&netMessage, GSC_DAN_BATTLE_START);
		MSG_WriteLong(&netMessage, EneumGuildIdx);
		MSG_WriteLong(&netMessage, nWorldIdx);
		MSG_WriteLong(&netMessage, RemainTime);
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}








void GTH_SendMessage_NewDanBattleEnd(playerCharacter_t* pPlayer, int EneumGuildIdx, int nWorldIdx, int RemainTime)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);		
		MSG_WriteShort(&netMessage, GSC_DAN_BATTLE_END);
		MSG_WriteLong(&netMessage, EneumGuildIdx);
		MSG_WriteLong(&netMessage, nWorldIdx);
		MSG_WriteLong(&netMessage, RemainTime);
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}






void GTH_ProcessMessage_NewDanBattlePlunder()
{

}






void GTH_SendMessage_NewDanBattlePlunder(playerCharacter_t* pPlayer, tagGCPacket_NewDanBattle::PlunderPcCode plunderpccode
										 ,tagGCPacket_NewDanBattle::PlunderItemCode PlunderItemCode, int DeleteInvenPos, item_t* pItem )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);
		MSG_WriteShort(&netMessage, GSC_DAN_BATTLE_PLUNDER);
		MSG_WriteByte(&netMessage,  plunderpccode);
		MSG_WriteByte(&netMessage,  PlunderItemCode);		

		if ( plunderpccode == tagGCPacket_NewDanBattle::PlunderPcCode::Dier)
				MSG_WriteLong(&netMessage,  DeleteInvenPos);
		else
		{
			if ( PlunderItemCode == tagGCPacket_NewDanBattle::PlunderItemCode::itemplunderSuccecs)
				MSG_WriteItem(&netMessage,  pItem);	
		}

		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







int	 GTH_Get_OnlineGuildMemeberCount(int WorldIdx, int GuildIdx, 
									 vector<NewDanBattleSystem::DanBaseInfo::sDanBattleMember> *pv_LiveGuiledmembers)
{
	if ( WorldIdx < 0 || WorldIdx >= MAX_GUILDS)
		return 0;
	
	int OnlineGuildNumber = 0;	
	guild_t* pGuild = NULL;
	pGuild = &g_guild[GuildIdx];
	playerCharacter_t* pPC  = NULL;
	


	if ( pv_LiveGuiledmembers != NULL)
		pv_LiveGuiledmembers->clear();		

	NewDanBattleSystem::DanBaseInfo::sDanBattleMember DanMember;
	
	pPC = gcpTools->GetPlayerRecordPointer(pGuild->masterName);	
	if ( pPC != NULL )
	{
		if ( pPC->worldIdx  == WorldIdx )
		{
			OnlineGuildNumber++;
			if ( pv_LiveGuiledmembers != NULL)
			{
				strncpy(DanMember.name, pPC->name, NAMESTRING);
				DanMember.name[NAMESTRING] = NULL;
				DanMember.PcIdx = pPC->idx;
				DanMember.killPoint = 0;
				DanMember.nAccumulationBattleKillPoint = pPC->nAccumulationBattleKillPoint;
				DanMember.exp = pPC->exp;
				pv_LiveGuiledmembers->push_back(DanMember);	
			}
		}
	}

	
	for(int SubMasterIdx=0; SubMasterIdx < MAX_GUILD_SUBMASTERS; SubMasterIdx++)
	{
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->subMasterName[SubMasterIdx]);
		if ( pPC != NULL )
		{
			if ( pPC->worldIdx  == WorldIdx )
			{
				OnlineGuildNumber++;
				if ( pv_LiveGuiledmembers != NULL)
				{
					strncpy(DanMember.name, pPC->name, NAMESTRING);
					DanMember.name[NAMESTRING] = NULL;
					DanMember.PcIdx = pPC->idx;
					DanMember.killPoint = 0;
					DanMember.nAccumulationBattleKillPoint = pPC->nAccumulationBattleKillPoint;
					DanMember.exp = pPC->exp;
					pv_LiveGuiledmembers->push_back(DanMember);
				}
			}
		}
	}
		
	
	for(int factionMasterIdx=0; factionMasterIdx < MAX_GUILD_FACTIONMASTERS; factionMasterIdx++)
	{
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->factionMasterName[factionMasterIdx]);
		if ( pPC != NULL ){
			if ( pPC->worldIdx  == WorldIdx )
			{
				OnlineGuildNumber++;
				if ( pv_LiveGuiledmembers != NULL)
				{
					strncpy(DanMember.name, pPC->name, NAMESTRING);
					DanMember.name[NAMESTRING] = NULL;
					DanMember.PcIdx = pPC->idx;
					DanMember.killPoint = 0;
					DanMember.nAccumulationBattleKillPoint = pPC->nAccumulationBattleKillPoint;
					DanMember.exp = pPC->exp;
					pv_LiveGuiledmembers->push_back(DanMember);
				}
			}
		}
	}	

	for(int mateIdx=0; mateIdx < MAX_GUILD_MATES; mateIdx++)
	{
		pPC = gcpTools->GetPlayerRecordPointer(pGuild->mateName[mateIdx]);
		if ( pPC != NULL ){
			if ( pPC->worldIdx  == WorldIdx )
			{
				OnlineGuildNumber++;
				if ( pv_LiveGuiledmembers != NULL)
				{
					strncpy(DanMember.name, pPC->name, NAMESTRING);
					DanMember.name[NAMESTRING] = NULL;
					DanMember.PcIdx = pPC->idx;
					DanMember.killPoint = 0;
					DanMember.nAccumulationBattleKillPoint = pPC->nAccumulationBattleKillPoint;
					DanMember.exp = pPC->exp;
					pv_LiveGuiledmembers->push_back(DanMember);
				}
			}
		}
	}

	return OnlineGuildNumber;
}






void	GTH_UndoPcDanBattle(playerCharacter_t* pPc)
{
	if ( pPc != NULL)
	{
		
		GTH_DeleteNoticePC(pPc);
	}
}








void	GTH_SendMessage_AcceptDanBattle(playerCharacter_t* pPlayer, 
										NewDanBattleSystem::DanBattleInfo* pDanBattleInfo)
{		
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);
		MSG_WriteShort(&netMessage, GSC_ACCEPT_DANBATTLE);
		MSG_WriteByte(&netMessage,  pDanBattleInfo->BattleMode);

		MSG_WriteFloat(&netMessage, pDanBattleInfo->fBattleSec);
		MSG_WriteByte(&netMessage,  pDanBattleInfo->nBattleMapIdx);
		MSG_WriteFloat(&netMessage, pDanBattleInfo->fWaitTimeforReady);
		MSG_WriteFloat(&netMessage, pDanBattleInfo->fWaitTimeforEnd);

		MSG_WriteLong(&netMessage,  pDanBattleInfo->RequestDanInfo.nDanIdx);
		MSG_WriteByte(&netMessage,  pDanBattleInfo->RequestDanInfo.nRemainPoint);
		

		MSG_WriteLong(&netMessage,  pDanBattleInfo->AcceptDanInfo.nDanIdx);
		MSG_WriteByte(&netMessage,  pDanBattleInfo->AcceptDanInfo.nRemainPoint);
		

		MSG_WriteFloat(&netMessage, pDanBattleInfo->fWaitTimeforReady);
		MSG_WriteFloat(&netMessage, pDanBattleInfo->nDanBattleIdx);
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}









void	GTH_ProcessInsertDanBattle(NewDanBattleSystem::DanBattleInfo* pDanBattleInfo, tagGCPacket_NewDanBattle::enumDanBattleInsertcode code)
{	
	if ( pDanBattleInfo == NULL)
		return;
	
	NewDanBattleSystem::DanBaseInfo* pRequestDanInfo = NULL;
	NewDanBattleSystem::DanBaseInfo* pAcceptDanInfo  = NULL;

	pRequestDanInfo = &pDanBattleInfo->RequestDanInfo;
	pAcceptDanInfo  = &pDanBattleInfo->AcceptDanInfo;

	playerCharacter_t* pRequestPc = NULL;
	playerCharacter_t* pAcceptPc = NULL;

	pRequestPc = gcpTools->GetPlayerRecordPointer(pRequestDanInfo->nRequest_AcceptPcIdx);
	pAcceptPc  = gcpTools->GetPlayerRecordPointer(pAcceptDanInfo->nRequest_AcceptPcIdx);

	GTH_UndoPcDanBattle(pRequestPc);
	GTH_UndoPcDanBattle(pAcceptPc);


	
	
	if ( code != tagGCPacket_NewDanBattle::enumDanBattleInsertcode::insert_success)
	{	
		tagGCPacket_NewDanBattle::enumError  ErrorCode;
	switch (code)
	{
	case tagGCPacket_NewDanBattle::enumDanBattleInsertcode::insert_fail:
	case tagGCPacket_NewDanBattle::enumDanBattleInsertcode::already_active_acceptdan:
	case tagGCPacket_NewDanBattle::enumDanBattleInsertcode::already_active_requestdan:
			ErrorCode = tagGCPacket_NewDanBattle::enumError::attleInsert_Error;
			break;
			
		case tagGCPacket_NewDanBattle::enumDanBattleInsertcode::full_danbattleCount:
			ErrorCode = tagGCPacket_NewDanBattle::enumError::Error_Full_danbattleCount;
			break;			
		}
		
		if ( pRequestPc != NULL){			
			g_NewDanBattleSystem.InitDanBatlleInfo(&pRequestPc->DanBattleInfo);
			GTH_SendMessage_RequestDanBattleReply(pRequestPc, 			
			tagGCPacket_NewDanBattle::enumCode::fail, 
				ErrorCode);
		}
		
		if ( pAcceptPc != NULL){			
			g_NewDanBattleSystem.InitDanBatlleInfo(&pAcceptPc->DanBattleInfo);
			GTH_SendMessage_RequestDanBattleReply(pAcceptPc,
			tagGCPacket_NewDanBattle::enumCode::fail,
				ErrorCode);
		}

		
		g_NewDanBattleSystem.InitDanBatlleInfo(pDanBattleInfo);
		return;
	}


	
		

	

















	
	GTH_ProcessMessage_NewDanBattleReady(
		pRequestDanInfo->nDanIdx,
		pAcceptDanInfo->nDanIdx,
		pDanBattleInfo->nBattleMapIdx,
		pDanBattleInfo->fWaitTimeforReady,
		pDanBattleInfo->BattleMode);

	GTH_ProcessMessage_NewDanBattleReady(
		pAcceptDanInfo->nDanIdx,
		pRequestDanInfo->nDanIdx,
		pDanBattleInfo->nBattleMapIdx,
		pDanBattleInfo->fWaitTimeforReady,
		pDanBattleInfo->BattleMode);




	
	pDanBattleInfo->fRemainTime = pDanBattleInfo->fWaitTimeforReady + g_timer->GetAppMilliTime();

	
	g_NewDanBattleSystem.SetDanBatttleStatus(pDanBattleInfo,
		NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_READY);	


	g_DBGameServer->Load_danbattle_guild_Info(pRequestDanInfo->nDanIdx);
	g_DBGameServer->Load_danbattle_guild_Info(pAcceptDanInfo->nDanIdx);
	
	
	
	
	for (int nDanMemberIdx = 0; nDanMemberIdx < pRequestDanInfo->v_GuildMembers.size(); nDanMemberIdx++)
	{			
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pDanBattleMember = NULL;
		pDanBattleMember = &pRequestDanInfo->v_GuildMembers[nDanMemberIdx];		
		playerCharacter_t *pPC = gcpTools->GetPlayerRecordPointer(pDanBattleMember->name);
		if ( pPC == NULL)
			continue;

		if ( pPC->worldIdx != pDanBattleInfo->nBattleMapIdx)
			continue;
		
		pPC->DanBattleInfo.nDanBattleIdx	= pDanBattleInfo->nDanBattleIdx;		
		pPC->DanBattleInfo.BattleMode		= pDanBattleInfo->BattleMode;	
		pPC->DanBattleInfo.nBattleMapIdx	= pDanBattleInfo->nBattleMapIdx;
		
	}
	
	
	for (nDanMemberIdx = 0; nDanMemberIdx < pAcceptDanInfo->v_GuildMembers.size(); nDanMemberIdx++)
	{
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pDanBattleMember = NULL;
		
		pDanBattleMember = &pAcceptDanInfo->v_GuildMembers[nDanMemberIdx];		
		playerCharacter_t *pPC = gcpTools->GetPlayerRecordPointer(pDanBattleMember->name);
		if ( pPC == NULL)
			continue;
		
		if ( pPC->worldIdx != pDanBattleInfo->nBattleMapIdx)
			continue;
		
		pPC->DanBattleInfo.nDanBattleIdx	= pDanBattleInfo->nDanBattleIdx;	
		pPC->DanBattleInfo.BattleMode		= pDanBattleInfo->BattleMode;	
		pPC->DanBattleInfo.nBattleMapIdx	= pDanBattleInfo->nBattleMapIdx;
		
	}
}









void GTH_CalcPointNewDanBattle(playerCharacter_t *pPC)
{
	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo =NULL;
	pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo(pPC->DanBattleInfo.nDanBattleIdx, pPC->guildIdx);

	
	if ( pDanBattleInfo == NULL)
		return;






	
	NewDanBattleSystem::DanBaseInfo* pAttackerDanBaseInfo  = NULL;		
	NewDanBattleSystem::DanBaseInfo* pDierDanBaseInfo      = NULL;      
	
	pAttackerDanBaseInfo = 
		g_NewDanBattleSystem.GetDanBaseInfo(pDanBattleInfo, pPC->guildIdx);

	if ( NULL == pAttackerDanBaseInfo )
		return;	
	
	
	pAttackerDanBaseInfo->nKillPoint += 1;

	
	
	
	






	
	
	if ( pDanBattleInfo->BattleMode == 
		NewDanBattleSystem::DanBattleInfo::enumBattleMode::BATTLEMODE_GENERAL)
	{			
		
		if ( g_guild[pPC->guildIdx].nKillPoint < 2147483647 )
		{		
			g_guild[pPC->guildIdx].nKillPoint += 3;
			g_DBGameServer->Save_danbattle_guild_AddAccpoint(pAttackerDanBaseInfo->nDanIdx, 3);		
		}
	}
	else
	{	
		if (g_guild[pPC->guildIdx].nKillPoint < 2147483647 )
		{		
			g_guild[pPC->guildIdx].nKillPoint += 1;
			g_DBGameServer->Save_danbattle_guild_AddAccpoint(pAttackerDanBaseInfo->nDanIdx, 1);		
		}
	}
	



	NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pMemberInfo = NULL;
	pMemberInfo = g_NewDanBattleSystem.GetDanBattleMember(pDanBattleInfo, pPC->guildIdx, pPC->name);

	if (NULL == pMemberInfo)
		return;

	

	
	
	pMemberInfo->killPoint += 1;
	
	if ( pMemberInfo->killPoint %10 == 0)
	{
		char Msg[256];
		
		_snprintf(Msg, 256, gcpServerLpackCtrl->GetLpack(6), pAttackerDanBaseInfo->szGuildName, pPC->name, pMemberInfo->killPoint );
		Msg[255] = NULL;
		GTH_SendMessage_DanBattleNoticeMsgToDanMember(pDanBattleInfo, 
			NewDanBattleSystem::noticetype::killpoint_notice, Msg);
	}

	
	if ( pDanBattleInfo->BattleMode == 
		NewDanBattleSystem::DanBattleInfo::enumBattleMode::BATTLEMODE_GENERAL)
	{	
		if ( pPC->nAccumulationBattleKillPoint < 2147483647)
		{		
			pPC->nAccumulationBattleKillPoint += 1;
			g_DBGameServer->Save_danbattle_pcpoint(pPC, 1);
		}
	}
}









BOOL	GTH_ProcessPlunder(playerCharacter_t* pAttackPC, playerCharacter_t* pDiePC)
{
	if ( pAttackPC== NULL || pDiePC == NULL )
		return FALSE;		

	
	if ( pAttackPC->DanBattleInfo.nDanBattleIdx != pDiePC ->DanBattleInfo.nDanBattleIdx)
		return FALSE;

	
	GTH_CalcPointNewDanBattle(pAttackPC);
	
	
	
	
	
	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo =NULL;
	pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo(pAttackPC->DanBattleInfo.nDanBattleIdx, pAttackPC->guildIdx);


	if ( pDanBattleInfo != NULL)
		GTH_SendMessage_SuncMessageDanBattleInfo(pDanBattleInfo);


	if ( pDiePC->rankInGuild == GUILD_MASTER)
	{
		char Msg[256];
		
		_snprintf(Msg, 256, gcpServerLpackCtrl->GetLpack(7), 		
			pAttackPC->guildName,
			pAttackPC->name, 
			pDiePC->guildName);

		Msg[255] = NULL;
		GTH_SendMessage_DanBattleNoticeMsgToDanMember(pDanBattleInfo, 
			NewDanBattleSystem::noticetype::plunder_notice, Msg);
	}

	
	if ( pAttackPC ->DanBattleInfo.BattleMode !=
		NewDanBattleSystem::DanBattleInfo::enumBattleMode::BATTLEMODE_GENERAL)
		return FALSE;

	if ( pDiePC ->DanBattleInfo.BattleMode != 
		NewDanBattleSystem::DanBattleInfo::enumBattleMode::BATTLEMODE_GENERAL)
		return FALSE;

	
	
	

	
	
	if ( TRUE == GTH_ProcessPlunderExp(pAttackPC, pDiePC) )
		return TRUE;
	
	
	
	
	GTH_ProcessPlunderItem(pAttackPC, pDiePC, pDanBattleInfo->bPlunderPremiumItem);	

	return TRUE;	
}










BOOL	GTH_ProcessPlunderExp(playerCharacter_t* pAttackPC, playerCharacter_t* pDiePC)
{	
	
	int		PlunderExp		= 0;
	
	float	PlunderRatio	= 0.0f;
	
	int		prevTotalExp	= 0;	



































	
	PlunderExp				= GTH_GetReduceEXP(pDiePC, PlunderRatio, prevTotalExp);	

	




	if( PlunderExp <= 0)
		return FALSE;
	
	
	













	


		
		int DierCurrentLevelExp	= pDiePC->exp - prevTotalExp;

		pDiePC->exp -= PlunderExp;

		if ( pDiePC->exp < 0 )
			pDiePC->exp = 0;

		
		if( pDiePC->exp <= prevTotalExp ) 
		{
			
			pDiePC->exp			= prevTotalExp;
			pDiePC->reduceExp	= DierCurrentLevelExp;
			

			
			GTH_PC_GetLoot_ForDanBattle( pAttackPC->idx, DierCurrentLevelExp , 0, 0, 0, 0 );
			return FALSE;
		}
		
		else{			
			pDiePC->reduceExp = PlunderExp;
			GTH_PC_GetLoot_ForDanBattle( pAttackPC->idx, PlunderExp, 0, 0, 0, 0 );
		}		
	
	return TRUE;
}








BOOL ISAllowPlunderItem(int nItemIdx, BOOL bPrimiumItem)
{

	
	if ( bPrimiumItem )
	{
		if ( nItemIdx == 576 || nItemIdx == 606 ||	nItemIdx == 575 || nItemIdx == 605 ||		 
			 nItemIdx == 45 || nItemIdx == 662 || nItemIdx == 350 || nItemIdx == 351 ||		 
			 nItemIdx == 352 || nItemIdx == 598 ||nItemIdx == 571 || nItemIdx == 450 ||		 
			 nItemIdx == 456 || nItemIdx == 457 ||nItemIdx == 514 || nItemIdx == 531 ||nItemIdx == 551)
			return FALSE;
		else
			return TRUE;
			
	}	
	
	
	
	if ( nItemIdx == 607 || nItemIdx == 604 ||
		 nItemIdx == 632 || nItemIdx == 613 ||		 
		 nItemIdx == 576 || nItemIdx == 606 ||		 
		 nItemIdx == 575 || nItemIdx == 605 ||		 
		 nItemIdx == 45 || nItemIdx == 662 ||		 
		 nItemIdx == 350 || nItemIdx == 351 ||		 
		 nItemIdx == 352 || nItemIdx == 598 ||		 
		 nItemIdx == 571 || nItemIdx == 450 ||		 
		 nItemIdx == 456 || nItemIdx == 457 ||		 
		 nItemIdx == 514 || nItemIdx == 531 ||		 
		 nItemIdx == 551
		)
		return FALSE;

	return TRUE;
}








BOOL	GTH_ProcessPlunderItem(playerCharacter_t* pAttackPC, playerCharacter_t* pDiePC, BOOL bPrimiumItem )
{
	vector<int> vCollectItem;
	item_t *pPlunderItem = NULL;	
	

	
	for (int nInvenItemIdx = 0; nInvenItemIdx < MAX_INVENTORY_SIZE;  nInvenItemIdx++)
	{
		pPlunderItem	= NULL;
		int ItemIdx		= pDiePC->inventory[nInvenItemIdx];
		pPlunderItem	= gcpTools->GetMyInventoryItem(pDiePC, nInvenItemIdx);

		if ( pPlunderItem == NULL)	continue;

		
		if (ISAllowPlunderItem(pPlunderItem->itemTableIdx, bPrimiumItem))		
			vCollectItem.push_back(nInvenItemIdx);	
	}
	
	
	if ( vCollectItem.size() <= 0)
	{
		
		GTH_SendMessage_NewDanBattlePlunder(pAttackPC,
		tagGCPacket_NewDanBattle::PlunderPcCode::Attacker,
		tagGCPacket_NewDanBattle::PlunderItemCode::Nomoreitemplunder, 0);
		return FALSE;
	}

	int CollectIdx			 = rand() % vCollectItem.size();	
	int SelectCollectInveIdx = vCollectItem[CollectIdx];
	pPlunderItem			 = NULL;
	pPlunderItem			 = gcpTools->GetMyInventoryItem(pDiePC, SelectCollectInveIdx);	

	
	if ( pPlunderItem == NULL)	
	{
		
		GTH_SendMessage_NewDanBattlePlunder(pAttackPC,
		tagGCPacket_NewDanBattle::PlunderPcCode::Attacker,
		tagGCPacket_NewDanBattle::PlunderItemCode::Nomoreitemplunder, SelectCollectInveIdx);
		return FALSE;		
	}


	item_t SelectPlunderItem;
	memcpy(&SelectPlunderItem, pPlunderItem, sizeof(item_t));

	
	if ( GTH_IsStackItem(&SelectPlunderItem) )
		SelectPlunderItem.durability = 0;
	
	
	
	if (FALSE == gcpTools->DeleteItemFromMyInventory(pDiePC, SelectCollectInveIdx, pPlunderItem->itemTableIdx))
	{
		
		GTH_SendMessage_NewDanBattlePlunder(pAttackPC,
		tagGCPacket_NewDanBattle::PlunderPcCode::Attacker,
		tagGCPacket_NewDanBattle::PlunderItemCode::itemplunderFail,
		SelectCollectInveIdx);
		return FALSE;

		
	}		
	else
	{
		
		GTH_SendMessage_NewDanBattlePlunder(pDiePC,
		tagGCPacket_NewDanBattle::PlunderPcCode::Dier,
		tagGCPacket_NewDanBattle::PlunderItemCode::itemDeleteSuccecs, 
		SelectCollectInveIdx);		
		
	}


	
	if (FALSE == gcpTools->AddItem(pAttackPC, SelectPlunderItem))
	{
		
		GTH_SendMessage_NewDanBattlePlunder(pAttackPC,
		tagGCPacket_NewDanBattle::PlunderPcCode::Attacker,
		tagGCPacket_NewDanBattle::PlunderItemCode::itemplunderFail, 
		SelectCollectInveIdx);
		return FALSE;
	}
	else
	{
		
		GTH_SendMessage_NewDanBattlePlunder(pAttackPC,
		tagGCPacket_NewDanBattle::PlunderPcCode::Attacker,
		tagGCPacket_NewDanBattle::PlunderItemCode::itemplunderSuccecs, 
		SelectCollectInveIdx,
		&SelectPlunderItem);
	}

	return TRUE;	
}







int		GTH_GetReduceEXP(playerCharacter_t* pPC, float percent, int& prevTotalExp_OUT)
{	
	
	__int64 NeedExpForNextLevel	= 0;
	__int64 prevTotalExp	= 0;
	float	reduceExp		= 0;
	int		level			= 1;	
	int		prevExpTableIdx	= 0;
	int		CurExpTableIdx	= 0;
	float	ExpLossRate		= 1.0f;
	int		chaosClass		= 0;
	
	
	if ( pPC->level < 0 || pPC->level > MAX_NUMBER_OF_LEVEL)
		return 0;
	
	
	prevExpTableIdx = pPC->level -2;
	if ( prevExpTableIdx < 0 || prevExpTableIdx > MAX_NUMBER_OF_LEVEL)
	{
		prevTotalExp_OUT = 0;
		return 0;	
	}
	
	CurExpTableIdx  = pPC->level -1;
	if ( CurExpTableIdx< 0 || CurExpTableIdx>= MAX_NUMBER_OF_LEVEL)
	{
		prevTotalExp_OUT = 0;
		return 0;	
	}
	
	
	
	prevTotalExp = g_expTable[prevExpTableIdx].totalexp;

	
	

	
	NeedExpForNextLevel = g_expTable[CurExpTableIdx].exp;

	
	
	



	
	 


	 



	






	
	reduceExp		 = NeedExpForNextLevel / (pPC->level * 10);

	
	prevTotalExp_OUT = prevTotalExp;

	return reduceExp;
}









BOOL	GTH_NewDanBattlePc_DIE(playerCharacter_t* pAttackPC, playerCharacter_t* pDiePC)
{
	if ( FALSE == GTH_ProcessPlunder(pAttackPC, pDiePC) )
		return FALSE;	

	
	int		dropItemType		=	-1;
	int		dropItemPosition	=	0;	
	int		reduceExp			=	0;
	int		lLossNark			=   0;
	

	char	killerName[NAMESTRING+1];
	strcpy( killerName, "¹ÌÈ®ÀÎ" );		
	if( g_pc[pDiePC->killerIdx].active )
	{
		memcpy(killerName,g_pc[pDiePC->killerIdx].name,NAMESTRING);
		killerName[NAMESTRING]=NULL;				
	}

	GTH_SendPCEventMessage_Die( pDiePC, dropItemType, dropItemPosition );
	
	g_logSystem->Write( "User (%s) was Dead. decrease [%d] Experiance.", pDiePC->name, reduceExp );
	g_logSystem->WriteToLog( pDiePC, LOG_CLASS_PCDIE, "%d;%d;%d;;;%s;", reduceExp, pDiePC->killerType, pDiePC->curDamage, killerName );	

	return TRUE;

}







void	GTH_ProcessDanBattleForReJoin(playerCharacter_t* pPlayer)
{
	if ( pPlayer == NULL)
		return;
	
	NewDanBattleSystem::DanBattleInfo* pDanBattleInfo = NULL;
	pDanBattleInfo = g_NewDanBattleSystem.GetDanBattleInfo(pPlayer->guildIdx);
	
	if (pDanBattleInfo != NULL)
	{
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember*  pDanBattleMember = NULL;
		pDanBattleMember = g_NewDanBattleSystem.GetDanBattleMember(pDanBattleInfo, pPlayer->guildIdx, pPlayer->name);

		if ( pDanBattleMember == NULL)
			return;

		if ( pDanBattleInfo->nBattleMapIdx != pPlayer->worldIdx)
			return;

		if ( pDanBattleInfo->BattleStatus == 
			NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE)
		{
			
			
			
			
			pPlayer->DanBattleInfo.nDanBattleIdx = pDanBattleInfo->nDanBattleIdx;
			pPlayer->DanBattleInfo.BattleMode	 = pDanBattleInfo->BattleMode;
			pPlayer->DanBattleInfo.BattleStatus  = pDanBattleInfo->BattleStatus;
			pPlayer->DanBattleInfo.nBattleMapIdx= pDanBattleInfo->nBattleMapIdx;

			GTH_SendMessage_DanBattleInfo(pPlayer, pDanBattleInfo);
		}
	}
}






void	GTH_SendMessage_SuncMessageDanBattleInfo(NewDanBattleSystem::DanBattleInfo* pDanbattleInfo)
{
	if (NULL == pDanbattleInfo )
		return;	

	NewDanBattleSystem::DanBaseInfo* pRequestDanInfo  = NULL;
	NewDanBattleSystem::DanBaseInfo* pAcceptDanInfo  = NULL;
	pRequestDanInfo = &pDanbattleInfo->RequestDanInfo;	
	pAcceptDanInfo = &pDanbattleInfo->AcceptDanInfo;
	

	
	for (int nDanMemberIdx = 0; nDanMemberIdx < pRequestDanInfo->v_GuildMembers.size(); nDanMemberIdx++)
	{
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pDanBattleMember = NULL;
		pDanBattleMember = &pRequestDanInfo->v_GuildMembers[nDanMemberIdx];		
		playerCharacter_t *pPC = gcpTools->GetPlayerRecordPointer(pDanBattleMember->name);
		
		if ( pPC == NULL)
			continue;

		if ( pPC->worldIdx != pDanbattleInfo->nBattleMapIdx)
			continue;
		
		
		GTH_SendMessage_DanBattleInfo(pPC, pDanbattleInfo);
	}
	
	
	for (nDanMemberIdx = 0; nDanMemberIdx < pAcceptDanInfo->v_GuildMembers.size(); nDanMemberIdx++)
	{
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pDanBattleMember = NULL;
		pDanBattleMember = &pAcceptDanInfo->v_GuildMembers[nDanMemberIdx];		
		playerCharacter_t *pPC = gcpTools->GetPlayerRecordPointer(pDanBattleMember->name);
		if ( pPC == NULL)
			continue;

		if ( pPC->worldIdx != pDanbattleInfo->nBattleMapIdx)
			continue;
		
		GTH_SendMessage_DanBattleInfo(pPC, pDanbattleInfo);
	}
}


void ClearDanBattleInfo(playerCharacter_t* pPC)
{
	if ( pPC == NULL)
		return;

	memset(&pPC->DanBattleInfo, 0x00, sizeof(NewDanBattleSystem::DanBattleInfo));
	pPC->DanBattleInfo.nDanBattleIdx = -1;
	pPC->DanBattleInfo.nBattleMapIdx = -1;	

	GTH_UndoPcDanBattle(pPC);
	pPC->pvpMode = FALSE;

}

void GTH_ProcessDanbattlePowerEnd()
{
	
	g_NewDanBattleSystem.m_bActive = FALSE;

	for (int nBattleIdx =0; nBattleIdx < NewDanBattleSystem::MAX_DANBATTLE_COUNT; nBattleIdx++){
		
		NewDanBattleSystem::DanBattleInfo* pDanBattleInfo_StatusBattle = g_NewDanBattleSystem.GetDanBattleInfo(nBattleIdx, NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE);
		if ( NULL != pDanBattleInfo_StatusBattle )
		{				
			if ( pDanBattleInfo_StatusBattle->fRemainTime < NewDanBattleSystem::POWE_END_WAIT_SEC )
				continue;

			
			pDanBattleInfo_StatusBattle->fRemainTime = NewDanBattleSystem::POWE_END_WAIT_SEC + g_timer->GetAppMilliTime();
			GTH_SendMessage_SuncMessageDanBattleInfo(pDanBattleInfo_StatusBattle);
			GTH_SendMessage_DanBattleNoticeMsgToDanMember(pDanBattleInfo_StatusBattle, NewDanBattleSystem::noticetype::notice,
				gcpServerLpackCtrl->GetLpack(8));
			
			
		}

		
		NewDanBattleSystem::DanBattleInfo* pDanBattleInfo_StatusReady = g_NewDanBattleSystem.GetDanBattleInfo(nBattleIdx, NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_READY);
		if ( NULL != pDanBattleInfo_StatusReady)
		{
			g_NewDanBattleSystem.DecsionDanBattleBestMember(pDanBattleInfo_StatusReady);
			g_NewDanBattleSystem.DecsionDanBattleWinTO_TimeOut(pDanBattleInfo_StatusReady);
			GTH_SendMessage_DanBattleNoticeMsgToDanMember(pDanBattleInfo_StatusBattle, NewDanBattleSystem::noticetype::notice,
				gcpServerLpackCtrl->GetLpack(8));
				

			g_NewDanBattleSystem.SetDanBatttleStatus(pDanBattleInfo_StatusReady, NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_END);
			
		}		
	}
}

void GTH_SendMessage_DanBattleNoticeMsg(playerCharacter_t* pPlayer, NewDanBattleSystem::noticetype type, char* noticeMsg)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);	
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);
		MSG_WriteShort(&netMessage, GSC_DAN_BATTLE_NOTICE_MSG);
		MSG_WriteLong(&netMessage,	 type);
		MSG_WriteString(&netMessage, noticeMsg);
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void	GTH_SendMessage_DanBattleNoticeMsgToDanMember(NewDanBattleSystem::DanBattleInfo* pDanBattleInfo, NewDanBattleSystem::noticetype type, char* noticeMsg)
{	
	if ( pDanBattleInfo == NULL)
		return;	

	
	int MaxMemberNumber = pDanBattleInfo->RequestDanInfo.v_GuildMembers.size();
	for (int nMemberIdx = 0; nMemberIdx < MaxMemberNumber; nMemberIdx++)
	{
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pDanBattleMember = NULL;
		pDanBattleMember = &pDanBattleInfo->RequestDanInfo.v_GuildMembers[nMemberIdx];		
		playerCharacter_t* pPc = gcpTools->GetPlayerRecordPointer(pDanBattleMember->name);
		if ( NULL == pPc)
			continue;

		if ( pPc->worldIdx != pDanBattleInfo->nBattleMapIdx)
			continue;

		GTH_SendMessage_DanBattleNoticeMsg(pPc, type, noticeMsg);
	}


	
	MaxMemberNumber = pDanBattleInfo->AcceptDanInfo.v_GuildMembers.size();	
	for (nMemberIdx = 0; nMemberIdx < MaxMemberNumber; nMemberIdx++)
	{
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pDanBattleMember = NULL;
		pDanBattleMember = &pDanBattleInfo->AcceptDanInfo.v_GuildMembers[nMemberIdx];		
		playerCharacter_t* pPc = gcpTools->GetPlayerRecordPointer(pDanBattleMember->name);
		if ( NULL == pPc)
			continue;

		if ( pPc->worldIdx != pDanBattleInfo->nBattleMapIdx)
			continue;
		GTH_SendMessage_DanBattleNoticeMsg(pPc, type, noticeMsg);		
	}

}	
