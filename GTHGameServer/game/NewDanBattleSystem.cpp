




#include "../Global.h"
#include "../Tools/CTools.h"
extern CTools* gcpTools;







NewDanBattleSystem::NewDanBattleSystem()
{	
	

	
	m_bActive			 = FALSE;


	m_nMinPcCount		 = 0;	
	m_nBattleTimeForMin1 = 0;	
	m_nBattleTimeForMin2 = 0;	
	m_fItemDropRate		 = 1.0f;

	for (int nDanBattleIdx =0; nDanBattleIdx < MAX_DANBATTLE_COUNT; nDanBattleIdx ++)
	{
		memset(&m_DanBattleInfo[nDanBattleIdx], 0x00, sizeof(DanBattleInfo));
		m_DanBattleInfo[nDanBattleIdx].nDanBattleIdx = nDanBattleIdx;		

		
		m_DanBattleInfo[nDanBattleIdx].AcceptDanInfo.nDanIdx =-1;
		m_DanBattleInfo[nDanBattleIdx].RequestDanInfo.nDanIdx =-1;
	}
	

	

}

NewDanBattleSystem::~NewDanBattleSystem()
{

}







void NewDanBattleSystem::Update()
{
	for (int nBattleIdx = 0; nBattleIdx < MAX_DANBATTLE_COUNT; nBattleIdx ++ )
	{		
		switch ( m_DanBattleInfo[nBattleIdx].BattleStatus )
		{

		case DanBattleInfo::enumBattleStatus::BATTLESTATUS_NONE:
		case DanBattleInfo::enumBattleStatus::BATTLESTATUS_REGIST:
			break;

		case DanBattleInfo::enumBattleStatus::BATTLESTATUS_READY:
			BattleReady(&m_DanBattleInfo[nBattleIdx]);
			break;

		case DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE:
			Battle(&m_DanBattleInfo[nBattleIdx]);
			break;

		case DanBattleInfo::enumBattleStatus::BATTLESTATUS_END:
			BattleEnd(&m_DanBattleInfo[nBattleIdx]);
			break;
		}		
	}
}







void NewDanBattleSystem::BattleReady(DanBattleInfo* pDanBattleInfo)
{	
	if ( pDanBattleInfo == NULL)
		return;	
	
	if ( pDanBattleInfo->fRemainTime <= g_timer->GetAppMilliTime()) {
		
		DanBaseInfo *pRequestDanInfo = &pDanBattleInfo->RequestDanInfo;
		DanBaseInfo *pAcceptDanInfo = &pDanBattleInfo->AcceptDanInfo;

		
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
		
		
		pDanBattleInfo->fRemainTime = pDanBattleInfo->fBattleSec + g_timer->GetAppMilliTime();
		
		pDanBattleInfo->fMinBattleProcessTime = g_timer->GetAppMilliTime() + MIN_BATTLE_PROCESS_SEC;

















		GTH_ProcessMessage_NewDanBattleStart( 
			pRequestDanInfo->nDanIdx,			
			pAcceptDanInfo->nDanIdx,
			pDanBattleInfo->nBattleMapIdx,			
			pDanBattleInfo->fBattleSec);

		GTH_ProcessMessage_NewDanBattleStart(
			pAcceptDanInfo->nDanIdx,
			pRequestDanInfo->nDanIdx,			
			pDanBattleInfo->nBattleMapIdx,			
			pDanBattleInfo->fBattleSec );			
		

		
		g_NewDanBattleSystem.SetDanBatttleStatus(pDanBattleInfo, 
			NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE);

		
		

		
	}
}






void NewDanBattleSystem::Battle(DanBattleInfo* pDanBattleInfo)
{
	
	if ( pDanBattleInfo->fNextSyncdTime <= g_timer->GetAppMilliTime() )
	{				
		GTH_SendMessage_SuncMessageDanBattleInfo(pDanBattleInfo);
		pDanBattleInfo->fNextSyncdTime = g_timer->GetAppMilliTime() + SYNC_TIME;
	}	

	DanBaseInfo *pRequestDanInfo = &pDanBattleInfo->RequestDanInfo;
	DanBaseInfo *pAcceptDanInfo = &pDanBattleInfo->AcceptDanInfo;
	
	
	if ( TRUE == DecsionDanBattleWinTO_LiveMemberNumber(pDanBattleInfo) )
	{	
		if (pDanBattleInfo->AcceptDanInfo.wincode != tagGCPacket_NewDanBattle::enumwincode::draw)
	{
		pDanBattleInfo->endType = DanBattleInfo::enumEndType::REMAIN_MEMBER_COUNT;		
		DecsionDanBattleBestMember(pDanBattleInfo);
		}
		else
		{
			pDanBattleInfo->endType = DanBattleInfo::enumEndType::SYSTEM;
		}







		
		GTH_ProcessMessage_NewDanBattleEnd(pRequestDanInfo->nDanIdx,pAcceptDanInfo->nDanIdx, pDanBattleInfo->nBattleMapIdx, 1);

		GTH_ProcessMessage_NewDanBattleEnd(pAcceptDanInfo->nDanIdx,pRequestDanInfo->nDanIdx,pDanBattleInfo->nBattleMapIdx,	1);


		g_NewDanBattleSystem.SetDanBatttleStatus(pDanBattleInfo, 
			NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_END);


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

			

		return;
	}
	
	
	


	
	if (pDanBattleInfo->fRemainTime <= g_timer->GetAppMilliTime()) {
	
		DecsionDanBattleWinTO_TimeOut(pDanBattleInfo);
		DecsionDanBattleBestMember(pDanBattleInfo);
		

		pDanBattleInfo->endType = DanBattleInfo::enumEndType::TIME_OUT;		






		


		GTH_ProcessMessage_NewDanBattleEnd(pRequestDanInfo->nDanIdx,pAcceptDanInfo->nDanIdx, pDanBattleInfo->nBattleMapIdx,1);


		GTH_ProcessMessage_NewDanBattleEnd(pAcceptDanInfo->nDanIdx,pRequestDanInfo->nDanIdx,pDanBattleInfo->nBattleMapIdx,	1);

		g_NewDanBattleSystem.SetDanBatttleStatus(pDanBattleInfo, 
			NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_END);

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


}







void NewDanBattleSystem::BattleEnd(DanBattleInfo* pDanBattleInfo)
{

	
	DanBaseInfo *pRequestDanInfo = &pDanBattleInfo->RequestDanInfo;
	DanBaseInfo *pAcceptDanInfo = &pDanBattleInfo->AcceptDanInfo;
	
	int RequestWinCount  = 0;
	int RequestLoseCount = 0;

	int AcceeptWinCount  = 0;
	int AcceeptLoseCount = 0;
	if ( pRequestDanInfo->nDanIdx >= 0 || pRequestDanInfo->nDanIdx < MAX_GUILDS)
	{	
		RequestWinCount	 = g_guild[pRequestDanInfo->nDanIdx].wincount;
		RequestLoseCount = g_guild[pRequestDanInfo->nDanIdx].Losecount;
	}

	if ( pAcceptDanInfo->nDanIdx >= 0 || pAcceptDanInfo->nDanIdx < MAX_GUILDS)
	{	
		AcceeptWinCount	 = g_guild[pAcceptDanInfo->nDanIdx].wincount;
		AcceeptLoseCount = g_guild[pAcceptDanInfo->nDanIdx].Losecount;
	}


	
	for (int nDanMemberIdx = 0; nDanMemberIdx < pRequestDanInfo->v_GuildMembers.size(); nDanMemberIdx++)
	{
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pDanBattleMember = NULL;
		pDanBattleMember = &pRequestDanInfo->v_GuildMembers[nDanMemberIdx];
		playerCharacter_t *pPC = gcpTools->GetPlayerRecordPointer(pDanBattleMember->name);

		if ( pPC == NULL )
			continue;
		
		
		if ( pPC->worldIdx != pDanBattleInfo->nBattleMapIdx)
			continue;
		
	
		GTH_SendMessage_DanBattleResult( pPC , pRequestDanInfo->wincode, pAcceptDanInfo->szGuildName, pDanBattleInfo->szBestID, RequestWinCount, RequestLoseCount, pDanBattleMember->killPoint, pDanBattleInfo->endType);
		if ( pRequestDanInfo->wincode != tagGCPacket_NewDanBattle::enumwincode::loss)
			GTH_SendMessage_CompleteEffect_To_AllPC(pPC,156);
			
		

		ClearDanBattleInfo(pPC);
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

	
		GTH_SendMessage_DanBattleResult( pPC, pAcceptDanInfo->wincode, pRequestDanInfo->szGuildName, pDanBattleInfo->szBestID, AcceeptWinCount, AcceeptLoseCount, pDanBattleMember->killPoint, pDanBattleInfo->endType);
		if ( pAcceptDanInfo->wincode != tagGCPacket_NewDanBattle::enumwincode::loss)
			GTH_SendMessage_CompleteEffect_To_AllPC(pPC,156);



			

		ClearDanBattleInfo(pPC);
	}

	
	g_DBGameServer->Delete_Danbattle(pDanBattleInfo->RequestDanInfo.nDanIdx,
		pDanBattleInfo->AcceptDanInfo.nDanIdx);

	
	InitDanBatlleInfo(pDanBattleInfo);
	
}






NewDanBattleSystem::DanBattleInfo* NewDanBattleSystem::GetEmptyDanBattleSlot(int& nIdx_OUT)
{
	for (int nSlotIdx =0; nSlotIdx < MAX_DANBATTLE_COUNT; nSlotIdx++)
	{
		if ( m_DanBattleInfo[nSlotIdx].BattleStatus == DanBattleInfo::enumBattleStatus::BATTLESTATUS_NONE)
		{			
			nIdx_OUT = nSlotIdx;
			return &m_DanBattleInfo[nSlotIdx];
		}
	}

	return NULL;
}







NewDanBattleSystem::DanBattleInfo*	 
NewDanBattleSystem::InsertDanBattle(int RequestDanIdx, 
									int RequestPcIdx, 
									std::vector<DanBaseInfo::sDanBattleMember>& vRequestGuildMember,
									int AcceptDanIdx, 
									int AcceptPcIdx, 
									std::vector<DanBaseInfo::sDanBattleMember>& vAcceptGuildMember,
									enum NewDanBattleSystem::DanBattleInfo::enumBattleMode battlemode, 
									int WorldIdx, 
									int BattleTIme ,
									__int64 RequestPcExp,
									__int64 AcceptPcExp,
									BOOL IsAllowPrimiumItemPlunder)

{
	NewDanBattleSystem::DanBattleInfo* pBattleInfo = NULL;
	int DanBattleIdx = -1;
	pBattleInfo = GetEmptyDanBattleSlot(DanBattleIdx);
	if ( pBattleInfo == NULL){
		return NULL;
	}

	if ( RequestDanIdx < 0 || RequestDanIdx >= MAX_GUILDS)
		return NULL;

	if ( AcceptDanIdx < 0 || AcceptDanIdx >= MAX_GUILDS)
		return NULL;

	InitDanBatlleInfo(pBattleInfo);	
	pBattleInfo->BattleMode		= battlemode;

	
	
	pBattleInfo->BattleStatus	= DanBattleInfo::enumBattleStatus::BATTLESTATUS_REGIST;	
	pBattleInfo->nBattleMapIdx	= WorldIdx;	
	pBattleInfo->nDanBattleIdx	= DanBattleIdx;

	pBattleInfo->fBattleSec		= BattleTIme;
	
	
	pBattleInfo->RequestDanInfo.nDanIdx			= RequestDanIdx;
	pBattleInfo->RequestDanInfo.nKillPoint		= 0;	
	pBattleInfo->RequestDanInfo.nRemainPoint	= vRequestGuildMember.size();;
	pBattleInfo->RequestDanInfo.nRequest_AcceptPcIdx = RequestPcIdx;

	_snprintf(pBattleInfo->RequestDanInfo.szGuildName, GUILDNAMESTRING, 
	g_guild[RequestDanIdx].name);

	pBattleInfo->RequestDanInfo.szGuildName[GUILDNAMESTRING] = NULL;
	

	for (int nMemberIdx =0; nMemberIdx < vRequestGuildMember.size(); nMemberIdx++)
		pBattleInfo->RequestDanInfo.v_GuildMembers.push_back(vRequestGuildMember[nMemberIdx]);
	

	
	pBattleInfo->AcceptDanInfo.nDanIdx			= AcceptDanIdx;
	pBattleInfo->AcceptDanInfo.nKillPoint		= 0;	
	pBattleInfo->AcceptDanInfo.nRemainPoint		= vAcceptGuildMember.size();
	pBattleInfo->AcceptDanInfo.nRequest_AcceptPcIdx = AcceptPcIdx;
	

	for (nMemberIdx =0; nMemberIdx < vAcceptGuildMember.size(); nMemberIdx++)
		pBattleInfo->AcceptDanInfo.v_GuildMembers.push_back(vAcceptGuildMember[nMemberIdx]);


	_snprintf(pBattleInfo->AcceptDanInfo.szGuildName, GUILDNAMESTRING, 
		g_guild[AcceptDanIdx].name);

	pBattleInfo->AcceptDanInfo.szGuildName[GUILDNAMESTRING] = NULL;

	pBattleInfo->fWaitTimeforReady = g_NewDanBattleSystem.m_WaitTimeforReady;
	pBattleInfo->fWaitTimeforEnd = g_NewDanBattleSystem.m_WaitTimeforEnd;
	
	pBattleInfo->AcceptDanInfo.nRequest_AcceptPcEXP  = AcceptPcExp;
	pBattleInfo->RequestDanInfo.nRequest_AcceptPcEXP = RequestPcExp;
	
	
	pBattleInfo->bPlunderPremiumItem = IsAllowPrimiumItemPlunder;
	
	return pBattleInfo;	
}







NewDanBattleSystem::DanBattleInfo* 	NewDanBattleSystem::GetDanBattleInfo(const int nGuildIdx)
{
	if ( nGuildIdx < 0 || nGuildIdx >= MAX_GUILDS)
		return NULL;

	for (int nDanBattleIdx = 0; nDanBattleIdx < MAX_DANBATTLE_COUNT; nDanBattleIdx++)
	{
		DanBattleInfo *pDanBattleInfo = NULL;
		pDanBattleInfo = &m_DanBattleInfo[nDanBattleIdx];
		
		
		if (pDanBattleInfo->RequestDanInfo.nDanIdx == nGuildIdx)
			return pDanBattleInfo;

		if (pDanBattleInfo->AcceptDanInfo.nDanIdx == nGuildIdx)
			return pDanBattleInfo;
	}	
	return NULL;
}







NewDanBattleSystem::DanBattleInfo* 	NewDanBattleSystem::GetDanBattleInfo(const int nBattleIdx, const int nGuildIdx)
{
	if ( nBattleIdx < 0 || nBattleIdx >= MAX_GUILDS)
		return NULL;
	



	if ( (m_DanBattleInfo[nBattleIdx].RequestDanInfo.nDanIdx != nGuildIdx) &&
		(m_DanBattleInfo[nBattleIdx].AcceptDanInfo.nDanIdx != nGuildIdx))
		return NULL;

	return &m_DanBattleInfo[nBattleIdx];
	
}



NewDanBattleSystem::DanBattleInfo* 	NewDanBattleSystem::GetDanBattleInfo(const int nBattleIdx, DanBattleInfo::enumBattleStatus status)
{
	if ( nBattleIdx < 0 || nBattleIdx >= MAX_GUILDS)
		return NULL;

	if ( m_DanBattleInfo[nBattleIdx].BattleStatus != status )
		return NULL;	

	return &m_DanBattleInfo[nBattleIdx];
	
}

NewDanBattleSystem::DanBattleInfo*	 NewDanBattleSystem::GetDanBattleInfo(const char* szGuildName)
{
	for (int nDanBattleIdx = 0; nDanBattleIdx < MAX_DANBATTLE_COUNT; nDanBattleIdx++)
	{
		DanBattleInfo *pDanBattleInfo = NULL;
		pDanBattleInfo = &m_DanBattleInfo[nDanBattleIdx];		
		
		if ( stricmp (pDanBattleInfo->RequestDanInfo.szGuildName, szGuildName) == 0)
			return pDanBattleInfo;

		if ( stricmp (pDanBattleInfo->AcceptDanInfo.szGuildName, szGuildName) == 0)		
			return pDanBattleInfo;
	}	
	return NULL;
}






NewDanBattleSystem::DanBaseInfo*	NewDanBattleSystem::GetEnemyDanBaseInfo(const int nBattleIdx, const int nGuildIdx)
{
	if ( nBattleIdx < 0 || nBattleIdx >= MAX_GUILDS)
		return NULL;	

	if ( (m_DanBattleInfo[nBattleIdx].RequestDanInfo.nDanIdx != nGuildIdx) )
	{
		if ( m_DanBattleInfo[nBattleIdx].RequestDanInfo.nDanIdx >0 &&
			m_DanBattleInfo[nBattleIdx].RequestDanInfo.nDanIdx < MAX_GUILDS)
		return &m_DanBattleInfo[nBattleIdx].RequestDanInfo;
	}
	else
	{
		if ( m_DanBattleInfo[nBattleIdx].AcceptDanInfo.nDanIdx >0 &&
			m_DanBattleInfo[nBattleIdx].AcceptDanInfo.nDanIdx < MAX_GUILDS)
		return &m_DanBattleInfo[nBattleIdx].AcceptDanInfo;
	}

	return NULL;

}






void NewDanBattleSystem::InitDanBatlleInfo(DanBattleInfo* pDanBattleInfo)
{
	if ( pDanBattleInfo == NULL)
		return;

	memset(pDanBattleInfo, 0x00, sizeof(DanBattleInfo));
	pDanBattleInfo->BattleMode	 = DanBattleInfo::enumBattleMode::BATTLEMODE_FRIENDSHIP;
	pDanBattleInfo->BattleStatus = DanBattleInfo::enumBattleStatus::BATTLESTATUS_NONE;

	pDanBattleInfo->RequestDanInfo.nDanIdx = -1;
	pDanBattleInfo->RequestDanInfo.nRequest_AcceptPcIdx = -1;		
	pDanBattleInfo->RequestDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::draw;

	pDanBattleInfo->AcceptDanInfo.nDanIdx = -1;
	pDanBattleInfo->AcceptDanInfo.nRequest_AcceptPcIdx = -1;	
	pDanBattleInfo->AcceptDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::draw;
	pDanBattleInfo->nBattleMapIdx = -1;
	

	pDanBattleInfo->fItemDropRate = 1.0f;
	pDanBattleInfo->fBattleSec = g_NewDanBattleSystem.m_nBattleTimeForMin1;	

	pDanBattleInfo->nDanBattleIdx = -1;
	pDanBattleInfo->endType = DanBattleInfo::enumEndType::REMAIN_MEMBER_COUNT;
	
	
	
}





void NewDanBattleSystem::SetDanBatttleStatus(DanBattleInfo* pDanBattleInfo, 
											 DanBattleInfo::enumBattleStatus status)
{
	if ( pDanBattleInfo == NULL)
		return ;

	pDanBattleInfo->BattleStatus = status;	


	switch( status )
	{	
	case DanBattleInfo::enumBattleStatus::BATTLESTATUS_READY:
		{
			
			g_logSystem->WriteToNewDanBattleLog("%s, %d, %s, %s, %d, %d, %d",
				"LOG_CLASS_NEWDANBATTLE_START",
				pDanBattleInfo->nDanBattleIdx,
				pDanBattleInfo->RequestDanInfo.szGuildName,
				pDanBattleInfo->AcceptDanInfo.szGuildName,
				pDanBattleInfo->BattleMode,				
				pDanBattleInfo->fBattleSec,
				pDanBattleInfo->nBattleMapIdx);
		}
		break;
	case DanBattleInfo::enumBattleStatus::BATTLESTATUS_END:
		{
			
				g_logSystem->WriteToNewDanBattleLog("%s, %d, %s, %s, %d, %d, %d",
				"LOG_CLASS_NEWDANBATTLE_END",
				pDanBattleInfo->nDanBattleIdx,
				pDanBattleInfo->RequestDanInfo.szGuildName,
				pDanBattleInfo->AcceptDanInfo.szGuildName,
				pDanBattleInfo->BattleMode,
				pDanBattleInfo->endType,
				pDanBattleInfo->nBattleMapIdx);
		}
		break;
	}
	

}








BOOL NewDanBattleSystem::IsDanBattleEnemy(int myPcIdx,int EnemyIdx)
{
	playerCharacter_t* pMyPc = NULL;
	pMyPc = gcpTools->GetPlayerRecordPointer(myPcIdx);
	if ( pMyPc == NULL)
		return FALSE;

	playerCharacter_t* pEnemyPc = NULL;
	pEnemyPc = gcpTools->GetPlayerRecordPointer(EnemyIdx);
	if ( pEnemyPc == NULL)
		return FALSE;


	DanBattleInfo* pDanBattleInfo  = NULL;
	pDanBattleInfo = GetDanBattleInfo(pMyPc->DanBattleInfo.nDanBattleIdx, pMyPc->guildIdx);

	if ( pDanBattleInfo == NULL)
		return FALSE;

	
	if ( pDanBattleInfo->BattleStatus != DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE)
		return FALSE;

	if ( pMyPc->DanBattleInfo.nDanBattleIdx != pEnemyPc->DanBattleInfo.nDanBattleIdx)
		return FALSE;
	

	
	if ( pMyPc->guildIdx == pEnemyPc->guildIdx)
		return FALSE;

	



















	return TRUE;
	
}






NewDanBattleSystem::DanBaseInfo* NewDanBattleSystem::GetDanBaseInfo(DanBattleInfo* pDanBattleInfo, const int nGuildIdx)
{
	if ( pDanBattleInfo == NULL)
		return NULL;

	if ( pDanBattleInfo->RequestDanInfo.nDanIdx == nGuildIdx)
		return &(pDanBattleInfo->RequestDanInfo);	

	if ( pDanBattleInfo->AcceptDanInfo.nDanIdx == nGuildIdx)
		return &(pDanBattleInfo->AcceptDanInfo);

	return NULL;
}







BOOL NewDanBattleSystem::InsertDanBattleMemeber(int DanBattleIdx, int GuildIdx, int Pcidx)
{
	playerCharacter_t* pMyPc = NULL;
	pMyPc = gcpTools->GetPlayerRecordPointer(Pcidx);
	if ( pMyPc == NULL)
		return FALSE;

	DanBattleInfo* pDanBattleInfo  = NULL;
	pDanBattleInfo = GetDanBattleInfo(DanBattleIdx, pMyPc->guildIdx);

	if ( pDanBattleInfo == NULL)
		return FALSE;

	DanBaseInfo* pMyDanBaseInfo = NULL;
	pMyDanBaseInfo = GetDanBaseInfo(pDanBattleInfo, GuildIdx);

	if ( pMyDanBaseInfo == NULL)
		return FALSE;

	DanBaseInfo::sDanBattleMember danbattlemember;
	memset(&danbattlemember, 0x00, sizeof(DanBaseInfo::sDanBattleMember));
	danbattlemember.PcIdx = Pcidx;

	memcpy(danbattlemember.name, pMyPc->name, NAMESTRING);
	danbattlemember.name[NAMESTRING] = NULL;
	danbattlemember.nAccumulationBattleKillPoint = pMyPc->nAccumulationBattleKillPoint;


	pMyDanBaseInfo->v_GuildMembers.push_back(danbattlemember);

	return TRUE;

}







BOOL NewDanBattleSystem::IsInsertdDanBattle(int GuildIdx)
{
	for (int nBattleIdx = 0; nBattleIdx < MAX_DANBATTLE_COUNT; nBattleIdx ++ )
	{		
		DanBaseInfo *pDanBaseInfo = NULL;

		if ( m_DanBattleInfo[nBattleIdx].BattleStatus ==
			DanBattleInfo::enumBattleStatus::BATTLESTATUS_NONE )
			continue;

		pDanBaseInfo = GetDanBaseInfo(&m_DanBattleInfo[nBattleIdx], GuildIdx);		
		if ( pDanBaseInfo != NULL)
			return TRUE;
	}
	return FALSE;

}






NewDanBattleSystem::DanBaseInfo::sDanBattleMember* 
NewDanBattleSystem::GetDanBattleMember(DanBattleInfo* pDanBattleInfo, int GuildIdx, char* pcname)
{
	if ( pDanBattleInfo == NULL)
		return NULL;

	if ( pcname == NULL)
		return NULL;
	
	NewDanBattleSystem::DanBaseInfo* pMyDanBaseInfo	= NULL;	
	pMyDanBaseInfo = g_NewDanBattleSystem.GetDanBaseInfo(pDanBattleInfo, GuildIdx);

	if ( pMyDanBaseInfo == NULL)
		return NULL;

	for (int nMemberIdx = 0; nMemberIdx < pMyDanBaseInfo->v_GuildMembers.size(); nMemberIdx++)
	{
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pMember = NULL;
		pMember = &pMyDanBaseInfo->v_GuildMembers[nMemberIdx];
		if ( stricmp (pMember->name, pcname) == 0)
			return pMember;	
	}

	return NULL;
}






BOOL NewDanBattleSystem::DecsionDanBattleWinTO_LiveMemberNumber(DanBattleInfo* pDanBattleInfo)
{
	if ( pDanBattleInfo == NULL)
		return FALSE;

	

	int RequestDanLivePcNumber = 0;
	int AcceptDanLivePcNumber  = 0;

	int RequestDanIdx = pDanBattleInfo->RequestDanInfo.nDanIdx;
	int AcceptDanIdx  = pDanBattleInfo->AcceptDanInfo.nDanIdx;
	
	int MaxMemberNumber = pDanBattleInfo->RequestDanInfo.v_GuildMembers.size();
	int NowMemberNumber = 0;


	
	pDanBattleInfo->RequestDanInfo.nRemainPoint = 0;
	for (int nMemberIdx = 0; nMemberIdx < MaxMemberNumber; nMemberIdx++)
	{
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pDanBattleMember = NULL;
		pDanBattleMember = &pDanBattleInfo->RequestDanInfo.v_GuildMembers[nMemberIdx];		
		playerCharacter_t* pPc = gcpTools->GetPlayerRecordPointer(pDanBattleMember->name);
		if ( NULL == pPc)
			continue;

		if ( pPc->worldIdx != pDanBattleInfo->nBattleMapIdx)
			continue;

		pDanBattleInfo->RequestDanInfo.nRemainPoint++;
	}


	
	MaxMemberNumber = pDanBattleInfo->AcceptDanInfo.v_GuildMembers.size();
	pDanBattleInfo->AcceptDanInfo.nRemainPoint = 0;
	for (nMemberIdx = 0; nMemberIdx < MaxMemberNumber; nMemberIdx++)
	{
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pDanBattleMember = NULL;
		pDanBattleMember = &pDanBattleInfo->AcceptDanInfo.v_GuildMembers[nMemberIdx];		
		playerCharacter_t* pPc = gcpTools->GetPlayerRecordPointer(pDanBattleMember->name);
		if ( NULL == pPc)
			continue;

		if ( pPc->worldIdx != pDanBattleInfo->nBattleMapIdx)
			continue;

		pDanBattleInfo->AcceptDanInfo.nRemainPoint++;
	}
	
#ifndef _DEV_TEST	
	RequestDanLivePcNumber = pDanBattleInfo->RequestDanInfo.nRemainPoint;
	AcceptDanLivePcNumber  = pDanBattleInfo->AcceptDanInfo.nRemainPoint;

	
	
	if ( RequestDanLivePcNumber <= 0 && AcceptDanLivePcNumber <=0 )
	{
		pDanBattleInfo->AcceptDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::draw;
		pDanBattleInfo->RequestDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::draw;
		return TRUE;
	}

	
	if ( pDanBattleInfo->fMinBattleProcessTime > g_timer->GetAppMilliTime() )	
	{
		
		
		if ( (RequestDanLivePcNumber < MIN_GUILD_MEMBER_NUMBER_FOR_DAN_BATTLE) && 
			(AcceptDanLivePcNumber < MIN_GUILD_MEMBER_NUMBER_FOR_DAN_BATTLE) )
		{	
			pDanBattleInfo->AcceptDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::draw;
			pDanBattleInfo->RequestDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::draw;
			return TRUE;	
		}
		return FALSE;	
	}	


	
	if ( RequestDanLivePcNumber < MIN_GUILD_MEMBER_NUMBER_FOR_DAN_BATTLE)
	{		
		pDanBattleInfo->AcceptDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::win;


		if ( AcceptDanIdx >=0 && AcceptDanIdx < MAX_GUILDS){		
			g_DBGameServer->Save_danbattle_guild_AddWinpoint(AcceptDanIdx);
			g_guild[AcceptDanIdx].wincount +=1;
		}
		
		
		
		pDanBattleInfo->RequestDanInfo.wincode= tagGCPacket_NewDanBattle::enumwincode::loss;
		if ( RequestDanIdx >=0 && RequestDanIdx < MAX_GUILDS){		
			g_DBGameServer->Save_danbattle_guild_AddLosepoint(RequestDanIdx);
			g_guild[RequestDanIdx].Losecount +=1;
		}				
		
		
		return TRUE;
	}
	
	
	if (AcceptDanLivePcNumber < MIN_GUILD_MEMBER_NUMBER_FOR_DAN_BATTLE ){
		pDanBattleInfo->RequestDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::win;
		
		if ( RequestDanIdx >=0 && RequestDanIdx < MAX_GUILDS){		
			g_DBGameServer->Save_danbattle_guild_AddWinpoint(pDanBattleInfo->RequestDanInfo.nDanIdx);
			g_guild[RequestDanIdx].wincount +=1;
		}
		
		pDanBattleInfo->AcceptDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::loss;
		if ( AcceptDanIdx >=0 && AcceptDanIdx< MAX_GUILDS){		
			g_DBGameServer->Save_danbattle_guild_AddLosepoint(AcceptDanIdx);
			g_guild[AcceptDanIdx ].Losecount +=1;
		}
		return TRUE;
	}
	
#endif	


	return FALSE;

}







BOOL NewDanBattleSystem::DecsionDanBattleWinTO_TimeOut(DanBattleInfo* pDanBattleInfo)
{
	if ( pDanBattleInfo == NULL)
		return FALSE;

	int RequestDanIdx = pDanBattleInfo->RequestDanInfo.nDanIdx;
	int AcceptDanIdx = pDanBattleInfo->AcceptDanInfo.nDanIdx;
	
	if ( pDanBattleInfo->AcceptDanInfo.nKillPoint > pDanBattleInfo->RequestDanInfo.nKillPoint)
	{
		pDanBattleInfo->AcceptDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::win;
		if ( AcceptDanIdx >=0 && AcceptDanIdx < MAX_GUILDS){		
			g_DBGameServer->Save_danbattle_guild_AddWinpoint(AcceptDanIdx);
			g_guild[AcceptDanIdx].wincount +=1;
		}


		pDanBattleInfo->RequestDanInfo.wincode= tagGCPacket_NewDanBattle::enumwincode::loss;
		if ( RequestDanIdx >=0 && RequestDanIdx < MAX_GUILDS){
			g_DBGameServer->Save_danbattle_guild_AddLosepoint(RequestDanIdx);
			g_guild[RequestDanIdx].Losecount +=1;
		}

		return TRUE;
	}
	

	
	if ( pDanBattleInfo->AcceptDanInfo.nKillPoint < pDanBattleInfo->RequestDanInfo.nKillPoint)
	{
		pDanBattleInfo->RequestDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::win;
		if ( RequestDanIdx >=0 && RequestDanIdx < MAX_GUILDS){
			g_DBGameServer->Save_danbattle_guild_AddWinpoint(RequestDanIdx);
			g_guild[RequestDanIdx].wincount +=1;
		}

		pDanBattleInfo->AcceptDanInfo.wincode= tagGCPacket_NewDanBattle::enumwincode::loss;
		if ( AcceptDanIdx >=0 && AcceptDanIdx < MAX_GUILDS){		
			g_DBGameServer->Save_danbattle_guild_AddLosepoint(AcceptDanIdx);
			g_guild[AcceptDanIdx].Losecount +=1;
		}
		return TRUE;
	}

	
	if ( pDanBattleInfo->AcceptDanInfo.nKillPoint ==  pDanBattleInfo->RequestDanInfo.nKillPoint)
	{
		if ( pDanBattleInfo->AcceptDanInfo.nRequest_AcceptPcEXP > pDanBattleInfo->RequestDanInfo.nRequest_AcceptPcEXP )
		{
			pDanBattleInfo->AcceptDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::win;
			if ( AcceptDanIdx >=0 && AcceptDanIdx < MAX_GUILDS){		
				g_DBGameServer->Save_danbattle_guild_AddWinpoint(AcceptDanIdx);
				g_guild[AcceptDanIdx].wincount +=1;
			}
			
			
			pDanBattleInfo->RequestDanInfo.wincode= tagGCPacket_NewDanBattle::enumwincode::loss;
			if ( RequestDanIdx >=0 && RequestDanIdx < MAX_GUILDS){
				g_DBGameServer->Save_danbattle_guild_AddLosepoint(RequestDanIdx);
				g_guild[RequestDanIdx].Losecount +=1;
			}
		}
		else
		{
			pDanBattleInfo->RequestDanInfo.wincode = tagGCPacket_NewDanBattle::enumwincode::win;
			if ( RequestDanIdx >=0 && RequestDanIdx < MAX_GUILDS){
				g_DBGameServer->Save_danbattle_guild_AddWinpoint(RequestDanIdx);
				g_guild[RequestDanIdx].wincount +=1;
			}
			
			pDanBattleInfo->AcceptDanInfo.wincode= tagGCPacket_NewDanBattle::enumwincode::loss;
			if ( AcceptDanIdx >=0 && AcceptDanIdx < MAX_GUILDS){		
				g_DBGameServer->Save_danbattle_guild_AddLosepoint(AcceptDanIdx);
				g_guild[AcceptDanIdx].Losecount +=1;
			}
			return TRUE;
		}

	}
	return TRUE;

}

void NewDanBattleSystem::DecsionDanBattleBestMember(DanBattleInfo* pDanBattleInfo)
{
	if ( pDanBattleInfo == NULL)
		return;	
	
	
	NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pBestBattleMember = NULL;

	for (int nMemberIdx =0; nMemberIdx < pDanBattleInfo->RequestDanInfo.v_GuildMembers.size() ; nMemberIdx++)
	{
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pBattleMember = 
		pBattleMember =  &pDanBattleInfo->RequestDanInfo.v_GuildMembers[nMemberIdx];

		if ( pBestBattleMember == NULL){
			pBestBattleMember = pBattleMember;
			continue;
		}

		
		if ( pBestBattleMember->killPoint < pBattleMember->killPoint)		{
			pBestBattleMember = pBattleMember;
			continue;
		}
		
		else if (pBestBattleMember->killPoint == pBattleMember->killPoint)
		{			







			
			if (pBestBattleMember->exp > pBattleMember->exp)
					pBestBattleMember = pBattleMember;
		}
	}

	for ( nMemberIdx =0; nMemberIdx < pDanBattleInfo->AcceptDanInfo.v_GuildMembers.size() ; nMemberIdx++)
	{
		NewDanBattleSystem::DanBaseInfo::sDanBattleMember* pBattleMember = 
			pBattleMember =  &pDanBattleInfo->AcceptDanInfo.v_GuildMembers[nMemberIdx];
		
		if ( pBestBattleMember == NULL){
			pBestBattleMember = pBattleMember;
			continue;
		}
		
		
		if ( pBestBattleMember->killPoint < pBattleMember->killPoint)		{
			pBestBattleMember = pBattleMember;
			continue;
		}
		
		else if (pBestBattleMember->killPoint == pBattleMember->killPoint)
		{			







			
			if (pBestBattleMember->exp > pBattleMember->exp)
					pBestBattleMember = pBattleMember;
		}	
	}
	

	strncpy(pDanBattleInfo->szBestID, pBestBattleMember->name, NAMESTRING);
	pDanBattleInfo->szBestID[NAMESTRING-1] = NULL;
}

#include "..\GolryunBattle\CServerLpackCtrl.h"
extern CServerLpackCtrl* gcpServerLpackCtrl;
void NewDanBattleSystem::RemainTimeBroadCastMsg(DanBattleInfo* pDanBattleInfo)
{
	int currenttime = pDanBattleInfo->fRemainTime;
	if ( pDanBattleInfo->fRemainTime == 60000)
	{
		char Msg[256];		
		_snprintf(Msg, 256, gcpServerLpackCtrl->GetLpack(11));
		Msg[255] = NULL;
		GTH_SendMessage_DanBattleNoticeMsgToDanMember(pDanBattleInfo, 
			NewDanBattleSystem::noticetype::killpoint_notice, Msg);	
		return;
	}

	
	if ( currenttime % 3600000 == 0)
	{		
		char Msg[256];		
		_snprintf(Msg, 256, gcpServerLpackCtrl->GetLpack(9),  currenttime);
		Msg[255] = NULL;
		GTH_SendMessage_DanBattleNoticeMsgToDanMember(pDanBattleInfo, 
			NewDanBattleSystem::noticetype::killpoint_notice, Msg);	
	}

	
	
	if ( currenttime % 600000 == 0)
	{		
		char Msg[256];		
		_snprintf(Msg, 256, gcpServerLpackCtrl->GetLpack(9),  currenttime);
		Msg[255] = NULL;
		GTH_SendMessage_DanBattleNoticeMsgToDanMember(pDanBattleInfo, 
			NewDanBattleSystem::noticetype::killpoint_notice, Msg);	
	}

	
	

}