               #include "../global.h"																	





extern bool IsGuildMasters( playerCharacter_t *pc );


BOOL InitDanBattleManager()
{
		if ( g_pDanBattleManager != NULL )
		return FALSE;

	g_pDanBattleManager = new CDanBattleManager;
	if ( g_pDanBattleManager == NULL )
		return FALSE;

	
	if ( !g_pDanBattleManager->InitManager() )
		return FALSE;

	g_pDanBattleManager->SetStatus( DBTL_STATUS_STARTUP );

	return TRUE;
}

BOOL InitDanBattleSystem()
{
	if ( g_pDanBattleSys != NULL )
		return FALSE;

	g_pDanBattleSys = new CDanBattleSys;
	if ( g_pDanBattleSys == NULL )
		return FALSE;

	

	return TRUE;
}

void GTH_ProcessDanBattleSys()
{
	if ( g_config.isManager )
	{
		g_pDanBattleManager->Update();
	}

	if ( g_config.isDanBattleMap )
	{
		g_pDanBattleSys->Update();
	}
}



BOOL CDanBattleManager::InitManager()
{
	int i;
	
	LPBATTLE_RECORD pBattle;
	for (i=0;i<SZ_BATTLE_CACHE;i++)
	{
		pBattle	= new BATTLE_RECORD;
		if ( pBattle == NULL )
			return FALSE;

		m_BattleCache.push_back( pBattle );
	}

	LPREQUEST_RECORD pRequest;
	for (i=0;i<SZ_BATTLE_REQUEST;i++)
	{
		pRequest = new REQUEST_RECORD;
		if ( pRequest ==  NULL )
			return FALSE;

		m_RequestCache.push_back( pRequest );
	}

	return TRUE;
}

LPVOID CDanBattleManager::Get(BATTLE_LIST* pList)
{
	if ( pList->empty() )
		return NULL;

	BATTLE_ITOR	itor	= pList->begin();
	LPVOID		pVoid	= (*itor);

	pList->pop_front();

	return pVoid;
}


void CDanBattleManager::Destroy()
{
	BATTLE_ITOR		iLoop;
	for( iLoop = m_BattleCache.begin(); iLoop != m_BattleCache.end(); iLoop++ )
	{
		delete (*iLoop);
		
	}

	for( iLoop = m_RequestCache.begin(); iLoop != m_RequestCache.end(); iLoop++ )
	{
		delete (*iLoop);
	
	}

	m_BattleCache.clear();
	m_RequestCache.clear();
}

void CDanBattleManager::Push(BATTLE_LIST* pList, LPVOID pVoid)
{
	pList->push_back( pVoid );
}

LPVOID CDanBattleManager::ViewFirst(BATTLE_LIST* pList)
{
	if ( pList->empty() )
		return NULL;

	BATTLE_ITOR	itor	= pList->begin();
	LPVOID		pVoid	= (*itor);

	return pVoid;
}

BOOL CDanBattleManager::First(BATTLE_LIST* pList,LPVOID* lppVoid)
{
	if ( pList->empty() )
		return FALSE;

	m_FindNextItor	= pList->begin();
	*lppVoid		= (*m_FindNextItor++);

	return TRUE;
}

BOOL CDanBattleManager::Next(BATTLE_LIST* pList,LPVOID* lppVoid)
{
	if ( pList->empty() )
		return FALSE;

	if ( m_FindNextItor == pList->end() )
		return FALSE;

	*lppVoid = ( *m_FindNextItor++ );

	return TRUE;
}


void CDanBattleManager::SetStatus( int bStatus )
{
	m_iBattleStatus		= bStatus;
	m_dwStatusStartTime	= g_globalTime;	



	switch (m_iBattleStatus)
	{
	case DBTL_STATUS_STARTUP	:	ShowLogInfo("BattleManager Status : DBTL_STATUS_STARTUP");	break;
	case DBTL_STATUS_READY		:	ShowLogInfo("BattleManager Status : DBTL_STATUS_READY");	break;
	case DBTL_STATUS_MOVE		:	ShowLogInfo("BattleManager Status : DBTL_STATUS_MOVE");		break;
	case DBTL_STATUS_START		:	ShowLogInfo("BattleManager Status : DBTL_STATUS_START");	break;
	case DBTL_STATUS_END		:	ShowLogInfo("BattleManager Status : DBTL_STATUS_END");		break;
	case DBTL_STATUS_CLEAR		:	ShowLogInfo("BattleManager Status : DBTL_STATUS_CLEAR");	break;
	}

	if (m_iBattleStatus==DBTL_STATUS_STARTUP)
		return;
	
	if ( g_config.isDanBattleMap )
	{
		g_pDanBattleSys->SetStatus(m_iBattleStatus);
	}
	else
	{
		i3socket_t* pDestSock=FindBattleMapServer();
		if (pDestSock==NULL)
			return;
		
		MSG_BeginWriting( &netMessage );
		MSG_Clear( &netMessage );		
		MSG_WriteByte( &netMessage, GSC_EXTEND );
		MSG_WriteByte( &netMessage, GMS_DAN_BATTLE );
		MSG_WriteByte( &netMessage, DANB_SETSTATUS );
		MSG_WriteByte( &netMessage, m_iBattleStatus );
		NET_SendMessage( pDestSock, &netMessage );
		MSG_EndWriting( &netMessage );	
	}
}

int	CDanBattleManager::GetStatus()
{
	return m_iBattleStatus;
}

BOOL CDanBattleManager::FindRequester(LPREQUEST_RECORD pRequest)
{
	BATTLE_ITOR			iLoop;
	LPREQUEST_RECORD	pReq;		
	










	for ( iLoop=m_RequestList.begin();iLoop!=m_RequestList.end();iLoop++)
	{
		pReq=static_cast<LPREQUEST_RECORD>(*iLoop);
		if (pRequest->DanIdx == pReq->DanIdx)
		{
			return TRUE;
		}
	}
		
	return FALSE;
}

LPVOID CDanBattleManager::GetRequestPointer(LPREQUEST_RECORD pRequest)
{
	BATTLE_ITOR			iLoop;
	LPREQUEST_RECORD	pReq;		
	

	
	for ( iLoop=m_RequestList.begin();iLoop!=m_RequestList.end();iLoop++)
	{
		pReq=static_cast<LPREQUEST_RECORD>(*iLoop);
		
		if ( pRequest != NULL)			
		{
			if ( pRequest->DanIdx == pReq->DanIdx )
				return pReq;			
		}
		else
			return pReq;
	}	
	return NULL;
}




BOOL CDanBattleManager::InsertRquester(LPREQUEST_RECORD pRequest)
{
	if ( FindRequester( pRequest ) )
		return FALSE;

	LPREQUEST_RECORD pCHRequest	= static_cast < LPREQUEST_RECORD > ( Get( &m_RequestCache ) );
	if ( pCHRequest == NULL )
		return FALSE;

	memcpy( pCHRequest, pRequest, sizeof( REQUEST_RECORD ) );

	Push( &m_RequestList, pCHRequest );

	return TRUE;
}

BOOL CDanBattleManager::DeleteRequester(LPREQUEST_RECORD pRequest)
{
	LPVOID		pDelete	= GetRequestPointer(pRequest);
	if ( pDelete == NULL)
		return FALSE;

	BATTLE_ITOR iLoop;
	LPVOID		pCheck;

	for (iLoop=m_RequestList.begin();iLoop!=m_RequestList.end();)
	{
		pCheck=(*iLoop);
		if (pDelete==pCheck)
		{
			m_RequestList.erase(iLoop++);
			memset(pDelete,0,sizeof(REQUEST_RECORD));
			Push(&m_RequestCache,pDelete);
			return TRUE;
		}
		else
			iLoop++;
	}
	return FALSE;
}


void CDanBattleManager::SendRequesterList(i3socket_t* sock,BYTE msgType,int serverID)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, msgType);
	MSG_WriteByte(&netMessage, DANB_REQUESTER_LIST);

	if ( msgType == GMS_DAN_BATTLE )				
	{
		
	}

	BYTE listSize = m_RequestList.size();
	MSG_WriteByte(&netMessage, listSize);

	BATTLE_ITOR			iLoop;
	LPREQUEST_RECORD	pReq;

	for ( iLoop=m_RequestList.begin();iLoop!=m_RequestList.end();iLoop++)
	{
		pReq=static_cast<LPREQUEST_RECORD>(*iLoop);
		MSG_WriteLong(&netMessage, pReq->DanIdx);
		MSG_WriteString(&netMessage, pReq->DanName);
		MSG_WriteString(&netMessage, pReq->MasterName);
	}

	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);	
}

void CDanBattleManager::BroadCast_RequesterList()
{
	LPREQUEST_RECORD	pReq;
	BATTLE_ITOR			iLoop;

	for ( iLoop=m_RequestList.begin();iLoop!=m_RequestList.end();iLoop++)
	{	

		
		
		pReq=static_cast<LPREQUEST_RECORD>(*iLoop);

			
		
		if( g_pc[pReq->ServerID].npcIdx == -1 || g_pc[pReq->ServerID].npcAi.aiType != 34 
			|| g_pc[pReq->ServerID].friendlyEntityType != ENTITY_NPC 
			|| g_pc[pReq->ServerID].busyState != BUSY_STATE_NPC )
			continue;

		if ( pReq->ServerNo == g_config.gameServerNo )
		{
			g_pDanBattleManager->SendRequesterList(&g_pc[pReq->ServerID].sock,GSC_DAN_BATTLE,0);	
		}
		else
		{
			g_pDanBattleManager->SendRequesterList(&g_memberServer[pReq->ServerID].sock,GMS_DAN_BATTLE,0);	
		}
	}
}

void CDanBattleManager::SendBattleDan(i3socket_t* sock,BYTE msgType,int serverID)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, msgType);
	MSG_WriteByte(&netMessage, DANB_BATTLE_DAN);

	if ( msgType == GMS_DAN_BATTLE )				
	{
		
	}

	MSG_WriteLong(&netMessage, m_BattleDan[0].DanIdx);
	MSG_WriteLong(&netMessage, m_BattleDan[1].DanIdx);
	MSG_WriteString(&netMessage, m_BattleDan[0].DanName);
	MSG_WriteString(&netMessage, m_BattleDan[1].DanName);

	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);	
}

void CDanBattleManager::BroadCast_BattleDan()
{
	LPREQUEST_RECORD	pReq;
	BATTLE_ITOR			iLoop;

	for ( iLoop=m_RequestList.begin();iLoop!=m_RequestList.end();iLoop++)
	{	

		
		
		pReq=static_cast<LPREQUEST_RECORD>(*iLoop);

		if ( pReq->ServerNo == g_config.gameServerNo )
		{
			g_pDanBattleManager->SendBattleDan(&g_pc[pReq->ServerID].sock,GSC_DAN_BATTLE,0);	
		}
		else
		{
			g_pDanBattleManager->SendBattleDan(&g_memberServer[pReq->ServerID].sock,GMS_DAN_BATTLE,0);	
		}
	}
}

BOOL CDanBattleManager::isSuggester(LPREQUEST_RECORD pSuggest)
{
	LPREQUEST_RECORD pRootRester = static_cast< LPREQUEST_RECORD > (ViewFirst(&m_RequestList));
	if ( pRootRester == NULL )
		return FALSE;

	if (pSuggest->DanIdx != pSuggest->DanIdx)
		return FALSE;

	

	return TRUE;
}

BOOL CDanBattleManager::SetSuggester(LPREQUEST_RECORD pSuggest)
{
	LPREQUEST_RECORD pSuggestData = static_cast<LPREQUEST_RECORD>(GetRequestPointer(pSuggest));
	if ( pSuggestData==NULL )
		return FALSE;

	m_pSuggester = pSuggestData;			

	return TRUE;
}

LPREQUEST_RECORD CDanBattleManager::GetSuggester()
{
	return m_pSuggester;
}

BOOL CDanBattleManager::MakeBattleDan()
{
	LPREQUEST_RECORD pReqA=static_cast<LPREQUEST_RECORD>(ViewFirst(&m_RequestList));
	if ( pReqA==NULL)
		return FALSE;

	if (m_pSuggester==NULL)
		return FALSE;

	BYTE bResult=MSG_ReadByte();		

	memcpy(&m_BattleDan[0],pReqA,sizeof(REQUEST_RECORD));			
	memcpy(&m_BattleDan[1],m_pSuggester,sizeof(REQUEST_RECORD));	


	
	g_pc[m_BattleDan[0].ServerID].curChargeSE -= g_logic.DanBattlePrice;
	g_pc[m_BattleDan[1].ServerID].curChargeSE -= g_logic.DanBattlePrice;

	
	SendGSM_Refresh_PersonData(&g_pc[m_BattleDan[0].ServerID], 0);
	SendGSM_Refresh_PersonData(&g_pc[m_BattleDan[1].ServerID], 0);
	
	

	

	SetStatus(DBTL_STATUS_MOVE);									

	
	BroadCast_BattleDan();

	

	if ( g_config.isDanBattleMap )
	{
		BATTLE_TEAM_INFO Team[2];
		
		Team[0].danIdx=m_BattleDan[0].DanIdx;
		memcpy(Team[0].DanName,m_BattleDan[0].DanName,GUILDNAMESTRING);
		Team[1].danIdx=m_BattleDan[1].DanIdx;
		memcpy(Team[1].DanName,m_BattleDan[1].DanName,GUILDNAMESTRING);
			
		g_pDanBattleSys->SetTeamInfo(0,&Team[0]);
		g_pDanBattleSys->SetTeamInfo(1,&Team[1]);
	}
		
	ClearRequestList();

	return TRUE;
}

void CDanBattleManager::ClearRequestList()
{
	m_pSuggester=NULL;								

	LPVOID	pVoid=Get(&m_RequestList);
	while (pVoid!=NULL)
	{
		memset(pVoid,0,sizeof(REQUEST_RECORD));
		Push(&m_RequestCache,pVoid);
		pVoid=Get(&m_RequestList);
	}
}

BOOL CDanBattleManager::HaveBattleMap(int serviceCount,int* pMemberServer,int MapNo)
{
	int j;
	for (j=0;j<serviceCount;j++)
	{
		if (pMemberServer[j]==MapNo)
		{
			return TRUE;
		}
	}	
	return FALSE;
}

i3socket_t* CDanBattleManager::FindBattleMapServer()
{
	static int i;

	for (i=0;i<MAX_MEMBER_SERVER;i++)
	{
		if ( HaveBattleMap(g_memberServer[i].serviceWorldNumber,
								g_memberServer[i].serviceWorldList,DAN_BATTLEMAP_NO) )
		{
			return &g_memberServer[i].sock;
		}
	}
	return NULL;
}

void CDanBattleManager::Update()
{
	if ( m_iBattleStatus > DBTL_STATUS_MOVE )
		return;

	
	if ( m_iBattleStatus == DBTL_STATUS_READY )
	{
		UpdateRequester();	
		return;
	}

	DWORD remainTime=g_globalTime-m_dwStatusStartTime;

	if ( remainTime < g_logic.danBattleStatusTime[m_iBattleStatus] )
		return;

	switch ( m_iBattleStatus )
	{
	case DBTL_STATUS_STARTUP  :
		{
			SetStatus(DBTL_STATUS_READY);
		}
		break;
	case DBTL_STATUS_MOVE	:
		{
			SetStatus(DBTL_STATUS_START);
		}
		break;
	}
}


LPVOID		CDanBattleManager::GetBattleRequestDan()
{		
	return &m_BattleDan[0];
}
LPVOID		CDanBattleManager::GetBattleSuggestDan()
{
	return &m_BattleDan[1];
}





void CDanBattleManager::SendGSM_Refresh_PersonData(playerCharacter_t* pc, int type, int etc1, int etc2)
{
			
	
	
	
	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	
	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_DAN_BATTLE);
	MSG_WriteByte(&netMessage, DANB_BATTLE_REFRESH_PERSONDATA);	
	MSG_WriteLong(&netMessage, type);

	switch(type)
	{
		
	case 0:
		MSG_WriteLong(&netMessage, pc->curChargeSE);		
		break;		

		
		
	case 1:
		MSG_WriteLong(&netMessage, pc->rankPoint);
		MSG_WriteLong(&netMessage, g_guild[pc->guildIdx].famePoint);

		
	case 2:
		MSG_WriteLong(&netMessage, etc1);		
		break;		

		
	case 3:
		MSG_WriteLong(&netMessage, etc1);
		break;

	
	}	
	NET_SendUnreliableMessage(&pc->sock, &netMessage);
	
	MSG_EndWriting(&netMessage);

}





BOOL CDanBattleSys::InitDanBattleSys()
{
	return TRUE;
}

void CDanBattleSys::SendBattleReward(i3socket_t* sock,int type,int danFame,int userFame)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_DAN_BATTLE);
	MSG_WriteByte(&netMessage, DANB_BATTLE_REWARD);

	MSG_WriteLong(&netMessage, type);
	MSG_WriteLong(&netMessage, danFame);
	MSG_WriteLong(&netMessage, userFame);

	NET_SendMessage( sock, &netMessage);

	MSG_EndWriting(&netMessage);
}

void CDanBattleSys::CalcBattleResult()
{
	int winTeamNo	= GetWinTeam();
	int looseTeamNo;

	if (winTeamNo==2)
		return;

	if (winTeamNo==1)
		looseTeamNo=0;
	else
		looseTeamNo=1;

	g_guild[m_BattleTeamInfo[winTeamNo].danIdx].famePoint+=
												g_logic.danBattleReward[DANB_WIN_DAN];

	g_guild[m_BattleTeamInfo[looseTeamNo].danIdx].famePoint+=
												g_logic.danBattleReward[DANB_LOOSE_DAN];

	if (g_guild[m_BattleTeamInfo[looseTeamNo].danIdx].famePoint<0)						
		g_guild[m_BattleTeamInfo[looseTeamNo].danIdx].famePoint=0;

	


	TEAM_ITOR	iLoop;
	int			userIdx;

	for (iLoop=m_BattleTeam[winTeamNo].begin();iLoop!=m_BattleTeam[winTeamNo].end();iLoop++)
	{
		userIdx=(*iLoop);
		g_pc[userIdx].rankPoint+=g_logic.danBattleReward[DANB_WIN_USER];
		
		SendBattleReward(&g_pc[userIdx].sock,1,g_logic.danBattleReward[DANB_WIN_DAN],	
			g_logic.danBattleReward[DANB_WIN_USER]);


		g_pDanBattleManager->SendGSM_Refresh_PersonData(&g_pc[userIdx], 1);
		
		
			GTH_SendMessage_CompleteEffect_To_AllPC(&g_pc[userIdx],66);						
	}

	for (iLoop=m_BattleTeam[looseTeamNo].begin();iLoop!=m_BattleTeam[looseTeamNo].end();iLoop++)
	{
		userIdx=(*iLoop);
		g_pc[userIdx].rankPoint+=g_logic.danBattleReward[DANB_LOOSE_USER];
		
		SendBattleReward(&g_pc[userIdx].sock,0,g_logic.danBattleReward[DANB_LOOSE_DAN],	
			g_logic.danBattleReward[DANB_LOOSE_USER]);

		g_pDanBattleManager->SendGSM_Refresh_PersonData(&g_pc[userIdx], 1);

		
		
			GTH_SendMessage_CompleteEffect_To_AllPC(&g_pc[userIdx],67);		
	}
}



#include "../CTools.h"
extern CTools* gcpTools;
void CDanBattleSys::SetStatus(int iStatus)
{
	
	int oldStatus		= m_iBattleStatus;

	m_iBattleStatus		= iStatus;
	m_dwStatusStartTime	= g_globalTime;
	m_dwBroadCastTime	= g_globalTime;
	int tempcount =0;

	switch (m_iBattleStatus)
	{
	case DBTL_STATUS_STARTUP	:	ShowLogInfo("Battle Sys Status : DBTL_STATUS_STARTUP");	break;
	case DBTL_STATUS_READY		:	ShowLogInfo("Battle Sys Status : DBTL_STATUS_READY");	break;
	case DBTL_STATUS_MOVE		:	ShowLogInfo("Battle Sys Status : DBTL_STATUS_MOVE");	break;
	case DBTL_STATUS_START		:	
		{
			ShowLogInfo("Battle Sys Status : DBTL_STATUS_START");	
			SendBattleStart();

			m_BattleTeamInfo[0].startDanUserCount=m_BattleTeam[0].size();
			m_BattleTeamInfo[1].startDanUserCount=m_BattleTeam[1].size();
			
			
			

			
			
			
			
			AutoMoveDanMember();
			
			
			
			SendBattleInfo();
			
			break;
		}
	case DBTL_STATUS_END		:	
		{
			ShowLogInfo("Battle Sys Status : DBTL_STATUS_END");
						
			SendBattleResult();

			CalcBattleResult();

			ShowLogInfo("MessageCount = %d", tempcount);
			tempcount++;
			
			break;
		}
	case DBTL_STATUS_LOOSE_MOVE :
		{
			ShowLogInfo("Battle Sys Status : DBTL_STATUS_LOOSE_MOVE");
			int looseTeam=GetLooseTeam();
			if (looseTeam!=2)
			{
				ClearTeam(looseTeam);
			}
			else	
			{
				ClearTeam(0);
				ClearTeam(1);
			}
			break;
		}
	case DBTL_STATUS_CLEAR		:	
		{
			ShowLogInfo("Battle Sys Status : DBTL_STATUS_CLEAR");
			
			
			ClearTeam(0);
			ClearTeam(1);

			break;
		}
	
		case DBTL_STATUS_EVENT:	
		{
			ShowLogInfo("Battle Sys Status : DBTL_STATUS_EVENT");
			break;
		}	
	}

	
	int SendStatus =0;
	if ( oldStatus == DBTL_STATUS_EVENT)
		SendStatus = m_iBattleStatus;
	
	if ( m_iBattleStatus == DBTL_STATUS_EVENT)
		SendStatus = 5;
	
	if ( SendStatus ==0 )return;
	
	if ( g_config.isDanBattleMap == FALSE)
		return;
	
	for (int idx =0; idx < MAX_PCS; idx++){
		playerCharacter_t *pPC = gcpTools->GetPlayerRecordPointer(idx);
		if ( pPC == NULL) continue;			
		
		if ( pPC->worldIdx == DAN_BATTLEMAP_NO){
			g_pDanBattleManager->SendGSM_Refresh_PersonData(pPC, 3, SendStatus);
		}
	}

}

int CDanBattleSys::GetStatus()
{
	return m_iBattleStatus;
}

void CDanBattleSys::SendBattleResult()
{
	TEAM_ITOR	iLoop;
	int			userIdx;
	BYTE		WinTeam;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );		
	MSG_WriteByte( &netMessage, GSC_EXTEND );
	MSG_WriteByte( &netMessage, GSC_DAN_BATTLE );
	MSG_WriteByte( &netMessage, DANB_BATTLE_RESULT );

	WinTeam=GetWinTeam();

	MSG_WriteByte( &netMessage,	WinTeam );

	for (iLoop=m_BattleTeam[0].begin();iLoop!=m_BattleTeam[0].end();iLoop++)
	{
		userIdx=(*iLoop);
		
		NET_SendMessage( &g_pc[userIdx].sock, &netMessage );
	}
		
	for (iLoop=m_BattleTeam[1].begin();iLoop!=m_BattleTeam[1].end();iLoop++)
	{
		userIdx=(*iLoop);
		
		NET_SendMessage( &g_pc[userIdx].sock, &netMessage );
	}
		
	MSG_EndWriting( &netMessage );		
}

void CDanBattleSys::SendBattleStatus()
{
	if ( g_config.isManager )
	{
		g_pDanBattleManager->SetStatus(DBTL_STATUS_STARTUP);	
	}
	else
	{
		MSG_BeginWriting( &netMessage );
		MSG_Clear( &netMessage );		
		MSG_WriteByte( &netMessage, GSC_EXTEND );
		MSG_WriteByte( &netMessage, GMS_DAN_BATTLE );
		MSG_WriteByte( &netMessage, DANB_SETSTATUS );
		MSG_WriteByte( &netMessage, m_iBattleStatus );
		NET_SendMessage( &localSocket, &netMessage );
		MSG_EndWriting( &netMessage );		
	}
}

void CDanBattleSys::BroadCast_BattleInfo()			
{													
	if ( m_iBattleStatus != DBTL_STATUS_START )
		return;

	DWORD remainTime=g_globalTime-m_dwBroadCastTime;
	if ( remainTime >=DAN_BATTLE_BROADCAST_TIME)
	{
		
		SendBattleInfo();
		m_dwBroadCastTime = g_globalTime;
	}
}



int	CDanBattleSys::GetWinTeam()
{

	int LiveCount[2];

	LiveCount[0]=CountLiveUser(0);
	LiveCount[1]=CountLiveUser(1);

	if (LiveCount[0]>LiveCount[1])
		return 0;

	if (LiveCount[1]>LiveCount[0])
		return 1;

	if (m_BattleTeamInfo[0].BattlePoint>m_BattleTeamInfo[1].BattlePoint)
		return 0;
	if (m_BattleTeamInfo[1].BattlePoint>m_BattleTeamInfo[0].BattlePoint)
		return 1;
	if (m_BattleTeamInfo[1].BattlePoint==m_BattleTeamInfo[0].BattlePoint)
		return 2;

	return 2;
}

int	CDanBattleSys::GetLooseTeam()
{
	
	if (m_BattleTeamInfo[0].BattlePoint>m_BattleTeamInfo[1].BattlePoint)
		return 1;
	if (m_BattleTeamInfo[1].BattlePoint>m_BattleTeamInfo[0].BattlePoint)
		return 0;
	if (m_BattleTeamInfo[1].BattlePoint==m_BattleTeamInfo[0].BattlePoint)
		return 2;
	return 2;
}


void CDanBattleSys::UpdateStatus()					
{													
	switch ( m_iBattleStatus )
	{
	case DBTL_STATUS_START  :
		{
			SetStatus(DBTL_STATUS_END);
		}
		break;
	case DBTL_STATUS_END	:
		{
			SetStatus(DBTL_STATUS_LOOSE_MOVE);
		}
		break;
	case DBTL_STATUS_LOOSE_MOVE :
		{
			SetStatus(DBTL_STATUS_CLEAR);
		}
		break;
	case DBTL_STATUS_CLEAR	:
		{
			SetStatus(DBTL_STATUS_STARTUP);
			SendBattleStatus();
			
		}
		break;
	}

	
	
}


void CDanBattleSys::Update()
{

	
	if ( m_iBattleStatus == DBTL_STATUS_MOVE )
	{
		DWORD remainTime = g_globalTime - m_dwBroadCastTime;
		if ( remainTime > DAN_BATTLE_BROADCAST_TIME )		
		{
			
			SendRemainTime();
			m_dwBroadCastTime = g_globalTime;
		}		
	}
	
	

	if (m_iBattleStatus < DBTL_STATUS_START )		
		return;

	DWORD remainTime=g_globalTime-m_dwStatusStartTime;

	if ( remainTime < g_logic.danBattleStatusTime[m_iBattleStatus] )	
	{
		BroadCast_BattleInfo();						
		return;
	}

	UpdateStatus();									
}

void CDanBattleSys::InsertTeam(int teamNo,int ServerID)
{
	
	m_BattleTeam[teamNo].push_back(ServerID);

	
	DWORD	RemainTime = 
		g_logic.danBattleStatusTime[m_iBattleStatus]-(g_globalTime-m_dwStatusStartTime);

	
	g_pDanBattleManager->SendGSM_Refresh_PersonData(&g_pc[ServerID], 2, RemainTime);
	
	
	g_pDanBattleManager->SendGSM_Refresh_PersonData(&g_pc[ServerID], 3, m_iBattleStatus);	
	
	

}

void CDanBattleSys::DeleteTeam(int teamNo,int ServerID)
{
	TEAM_ITOR	iLoop;
	int			listID;
	
	for (iLoop=m_BattleTeam[teamNo].begin();iLoop!=m_BattleTeam[teamNo].end();)
	{
		listID	= (*iLoop);
		if ( ServerID == listID )
		{
			m_BattleTeam[teamNo].erase(iLoop++);
		}
		else
			iLoop++;
	}
}

int	CDanBattleSys::FindMyTeam(const int danIdx) const
{
	if (m_BattleTeamInfo[0].danIdx == danIdx)
	{
		return 0;
	}
	

	if (m_BattleTeamInfo[1].danIdx == danIdx)
	{
		return 1;
	}

	return -1;







}

void CDanBattleSys::InsertTeam(playerCharacter_t* pUser)
{
	
	int teamIdx=FindMyTeam(pUser->guildIdx);

	if (teamIdx==0 || teamIdx==1)
		InsertTeam(teamIdx,pUser->idx);
}

void CDanBattleSys::DeleteTeam(playerCharacter_t* pUser)
{
	
	g_logSystem->WriteRLog("Delete Team....");
	int teamIdx=FindMyTeam(pUser->guildIdx);
	g_logSystem->WriteRLog("Delete Team Index :%d");

	if (teamIdx==0 || teamIdx==1)
	{
		DeleteTeam(teamIdx,pUser->idx);
		g_logSystem->WriteRLog("Team Size : %d",m_BattleTeam[teamIdx]);
		int LiveCount=CountLiveUser(teamIdx);
		g_logSystem->WriteRLog("Live Count : %d",LiveCount);
		if (LiveCount==0)
		{
			SetStatus(DBTL_STATUS_END);
		}
	}
}


void CDanBattleSys::ClearTeam(int teamNo)
{
	TEAM_ITOR	iLoop;
	int			svrIdx;

	
	


	for (iLoop=m_BattleTeam[teamNo].begin();iLoop!=m_BattleTeam[teamNo].end();iLoop++)
	{
		svrIdx=(*iLoop);
		GTH_PC_Respawn( &g_pc[svrIdx], g_pc[svrIdx].curRA, g_pc[svrIdx].curSA, 0, 0 );		

		
	}
	m_BattleTeam[teamNo].clear();
}

void CDanBattleSys::SendBattleMassage(i3socket_t* sock,BYTE msgType,BYTE mCode,BYTE mType)
{

}

void CDanBattleSys::SendBattleMassage(int teamNo,BYTE msgType,BYTE mCode,BYTE mType)
{
}

void CDanBattleSys::SendBattleMassage(BYTE msgType,BYTE mCode,BYTE mType)
{
}

void CDanBattleSys::SetTeamInfo(int teamNo,LPBATTLE_TEAM_INFO pInfo)
{
	m_BattleTeamInfo[teamNo].danIdx				= pInfo->danIdx;
	m_BattleTeamInfo[teamNo].BattlePoint		= 0;
	m_BattleTeamInfo[teamNo].startDanUserCount	= 0;
	memcpy(m_BattleTeamInfo[teamNo].DanName,pInfo->DanName,GUILDNAMESTRING);
}

void CDanBattleSys::IncTeamPoint(int teamNo, int incValue)
{
	m_BattleTeamInfo[teamNo].BattlePoint += incValue;
}

void CDanBattleSys::DecTeamPoint(int teamNo, int decValue)
{
	m_BattleTeamInfo[teamNo].BattlePoint -= decValue;
}

void CDanBattleSys::SendBattleStart()
{
	TEAM_ITOR	iLoop;
	int			userIdx;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );		
	MSG_WriteByte( &netMessage, GSC_EXTEND );
	MSG_WriteByte( &netMessage, GSC_DAN_BATTLE );
	MSG_WriteByte( &netMessage, DANB_BATTLE_START );
	for (iLoop=m_BattleTeam[0].begin();iLoop!=m_BattleTeam[0].end();iLoop++)
	{
		userIdx=(*iLoop);

		NET_SendMessage( &g_pc[userIdx].sock, &netMessage );
		ShowLogInfo("SendName = %s",g_pc[userIdx].name);
	}

	for (iLoop=m_BattleTeam[1].begin();iLoop!=m_BattleTeam[1].end();iLoop++)
	{
		userIdx=(*iLoop);

		NET_SendMessage( &g_pc[userIdx].sock, &netMessage );
		ShowLogInfo("SendName = %s",g_pc[userIdx].name);
	}

	MSG_EndWriting( &netMessage );		
}


void CDanBattleSys::SendBattleInfo()
{
	TEAM_ITOR	iLoop;
	int			userIdx;
	int			totalUser;
	int			aliveUser;

	DWORD remainTime=g_logic.danBattleStatusTime[m_iBattleStatus]-(g_globalTime-m_dwStatusStartTime);

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );		
	MSG_WriteByte( &netMessage, GSC_EXTEND );
	MSG_WriteByte( &netMessage, GSC_DAN_BATTLE );
	MSG_WriteByte( &netMessage, DANB_BATTLE_INFO );
	MSG_WriteLong( &netMessage, m_BattleTeamInfo[0].BattlePoint );
	MSG_WriteLong( &netMessage, m_BattleTeamInfo[1].BattlePoint );
	MSG_WriteLong( &netMessage, remainTime );
	totalUser=m_BattleTeam[0].size();
	aliveUser=CountLiveUser(0);
	MSG_WriteLong( &netMessage, totalUser );
	MSG_WriteLong( &netMessage, aliveUser );
	totalUser=m_BattleTeam[1].size();
	aliveUser=CountLiveUser(1);
	MSG_WriteLong( &netMessage, totalUser );
	MSG_WriteLong( &netMessage, aliveUser );
	MSG_WriteLong( &netMessage, m_BattleTeamInfo[0].startDanUserCount );
	MSG_WriteLong( &netMessage, m_BattleTeamInfo[1].startDanUserCount );

	
	
	MSG_WriteString( &netMessage, m_BattleTeamInfo[0].DanName);
	MSG_WriteString( &netMessage, m_BattleTeamInfo[1].DanName);
	
	

	




	for (iLoop=m_BattleTeam[0].begin();iLoop!=m_BattleTeam[0].end();iLoop++)
	{
		userIdx=(*iLoop);

		NET_SendMessage( &g_pc[userIdx].sock, &netMessage );
	}

	for (iLoop=m_BattleTeam[1].begin();iLoop!=m_BattleTeam[1].end();iLoop++)
	{
		userIdx=(*iLoop);

		NET_SendMessage( &g_pc[userIdx].sock, &netMessage );
	}

	MSG_EndWriting( &netMessage );		

}

void CDanBattleSys::CalcPoint(playerCharacter_t* pDead, playerCharacter_t* pKiller)
{
	if ( GetStatus() != DBTL_STATUS_START )
		return;

	int TeamNo=FindMyTeam(pKiller->guildIdx);
	if (TeamNo==0 || TeamNo==1)	
	{
		IncTeamPoint(TeamNo,DAN_KILL_POINT);
		
		SendBattleInfo();
		

		TeamNo=FindMyTeam(pDead->guildIdx);
		if (TeamNo==0 || TeamNo==1)
		{
			int LiveCount=CountLiveUser(TeamNo);
			if (LiveCount==0)
			{
				SetStatus(DBTL_STATUS_END);
			}
		}

	}
}

int CDanBattleSys::CountLiveUser(int teamNo)
{
	TEAM_ITOR	iLoop;
	int			userIdx;
	int			liveCount=0;

	for (iLoop=m_BattleTeam[teamNo].begin();iLoop!=m_BattleTeam[teamNo].end();iLoop++)
	{
		userIdx=(*iLoop);
		if ( g_pc[userIdx].alive && g_pc[userIdx].worldIdx == DAN_BATTLEMAP_NO)
		{
			liveCount++;
		}
	}
	return liveCount;
}

void CDanBattleSys::MakeDanBattlePortal(int type)
{

	LPREQUEST_RECORD	requester = static_cast<LPREQUEST_RECORD>(g_pDanBattleManager->GetBattleRequestDan());
	LPREQUEST_RECORD	suggester = static_cast<LPREQUEST_RECORD>(g_pDanBattleManager->GetBattleSuggestDan());

	
	if ( requester->ServerID < 0 || suggester->ServerID < 0 )
	{
		
		return;
	}
	

	
	if ( type == DBTL_STATUS_READY)
	{
		GTH_MakePortal_fordan(LOOTTYPE_DANBATTLE_TEAMS, g_curPC->entityType, 
			g_logic.danBattleStatusTime[DBTL_STATUS_MOVE] /2 , 
			g_logic.danbattlePortal[DANBATTLE_START_PORTAL].StartworldIdx,
			g_logic.danbattlePortal[DANBATTLE_START_PORTAL].StartPos, 
			g_logic.danbattlePortal[DANBATTLE_START_PORTAL].TargetWorldIdx, 
			g_logic.danbattlePortal[DANBATTLE_START_PORTAL].TargetPos,
			g_logic.danbattlePortal[DANBATTLE_START_PORTAL].TargetWorldIdx2, 
			g_logic.danbattlePortal[DANBATTLE_START_PORTAL].TargetPos2
			);
		
		ShowLogInfo(" RequestUserName = %s", g_pc[requester->ServerID].name);
		ShowLogInfo(" SussgstUserName = %s", g_pc[suggester->ServerID].name);
		
		GTH_DeleteNoticePC(&g_pc[requester->ServerID]);	
		GTH_DeleteNoticePC(&g_pc[suggester->ServerID]);
		
		ShowLogInfo(" RequestUserState = %d", g_pc[requester->ServerID].busyState);
		ShowLogInfo(" SussgstUserState = %d", g_pc[suggester->ServerID].busyState);
		
		
	}
	else if ( type == DBTL_STATUS_START)
	{
		
		
		
		GTH_MakePortal_fordan(ENTITY_NPC, NULL, 
			g_logic.danBattleStatusTime[DBTL_STATUS_MOVE] / 2,		
			g_logic.danbattlePortal[DANBATTLE_ATEAM_PORTAL].StartworldIdx, 
			g_logic.danbattlePortal[DANBATTLE_ATEAM_PORTAL].StartPos, 
			g_logic.danbattlePortal[DANBATTLE_ATEAM_PORTAL].TargetWorldIdx,
			g_logic.danbattlePortal[DANBATTLE_ATEAM_PORTAL].TargetPos,
			NULL,NULL);
		
		
		
		GTH_MakePortal_fordan(ENTITY_NPC, NULL, 
			g_logic.danBattleStatusTime[DBTL_STATUS_MOVE] / 2,
			g_logic.danbattlePortal[DANBATTLE_BTEAM_PORTAL].StartworldIdx, 
			g_logic.danbattlePortal[DANBATTLE_BTEAM_PORTAL].StartPos, 
			g_logic.danbattlePortal[DANBATTLE_BTEAM_PORTAL].TargetWorldIdx,
			g_logic.danbattlePortal[DANBATTLE_BTEAM_PORTAL].TargetPos,
			NULL,	NULL);
	}
	
}


void CDanBattleFunc::RecvCC_DanBattleMessage()							
{
	BYTE bCommand = MSG_ReadByte();

	switch ( bCommand )
	{
	case DANB_SETSTATUS			: RecvCC_SetStatus();			break;
	case DANB_REQUEST_BATTLE	: RecvCC_ReqeustBattle();		break;
	case DANB_SUGGEST_BATTLE	: RecvCC_SuggestBattle();		break;
	case DANB_SUGGEST_RESULT	: RecvCC_SuggestResult();		break;
	case DANB_BATTLE_READY		: RecvCC_BattleReady();			break;	
	case DANB_BATTLE_START		: RecvCC_Battle_Start();		break;  
	case DANB_REQUEST_CANCEL	: RecvCC_CancelRequest();		break;
	case DANB_INIT_CHAO			: RecvCC_InitChao();			break;	
	case DANB_BATTLE_TEST_MODE  : RecvCC_SetDanTestMode();		break;	
	}

}



void CDanBattleFunc::RecvGSM_DanBattleMessage()							
{
	BYTE bCommand = MSG_ReadByte();

	switch ( bCommand )
	{
	case DANB_SETSTATUS			: RecvGSM_SetStatus();			break;
	case DANB_REQUEST_BATTLE	: RecvGSM_RequestBattle();		break;
	}
}

void CDanBattleFunc::SendErrorCode(i3socket_t* sock,BYTE msgType,BYTE eCode,BYTE eType)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, msgType);
	MSG_WriteByte(&netMessage, DANB_ERROR);
	MSG_WriteByte(&netMessage, eCode);
	MSG_WriteByte(&netMessage, eType);
		
	switch ( msgType )
	{
	case GMS_DAN_BATTLE : NET_SendUnreliableMessage(sock, &netMessage);	break;
	case GSC_DAN_BATTLE : NET_SendMessage( sock, &netMessage); break;
	}

	MSG_EndWriting(&netMessage);
}

void CDanBattleFunc::SendGSM_RequestBattle()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
		
	MSG_WriteByte(&netMessage, CC_EXTEND);
	MSG_WriteByte(&netMessage, GSM_DAN_BATTLE);
	MSG_WriteByte(&netMessage, DANB_REQUEST_BATTLE);
		
	MSG_WriteString(&netMessage, g_curPC->name);
	MSG_WriteString(&netMessage, g_curPC->guildName);
		
	NET_SendUnreliableMessage(&localSocket, &netMessage);

	MSG_EndWriting(&netMessage);	
}

int	CDanBattleFunc::CountDanUser(int danIdx)
{
	int danCount=0;
	for (int i=0;i<MAX_GUILD_MATES;i++)
	{
		if (g_guild[danIdx].mateName[i][0]!=NULL)
		{
			danCount++;
		}
	}
	return danCount;
}







int CDanBattleFunc::RecvCC_ReqeustBattle(playerCharacter_t* pc)
{	
	
	playerCharacter_t *pPc = NULL;
	if ( pc != NULL)
		 pPc = pc;
	else
	{
		if ( g_curPC != NULL)
			pPc = g_curPC;
	}
	if ( pPc == NULL )
		return 0;	


	
	if(NULL == g_pDanBattleManager) return 1;
	if( !g_config.isManager ) return 1;
	


	
	if ( !IsGuildMasters( pPc ) )			
	{
		
		GTH_SendMessage_ChattingMsg(pPc, 1, 9, 59 );
		return -1;							
	}
	
	if ( g_pDanBattleManager->m_bDanTestMode == FALSE )
	{
		if ( CountDanUser(pPc->guildIdx)<9 )	
			return -2;
	}

	if ( !g_config.isManager )				
	{
		SendGSM_RequestBattle();
		return 1;
	}

	if ( g_pDanBattleManager->GetStatus() != DBTL_STATUS_READY )	
	{										
		SendErrorCode(&pPc->sock,GSC_DAN_BATTLE,1,0);
		return -3;
	}

	REQUEST_RECORD	reqRecord;

	reqRecord.No		= 0;
	reqRecord.ServerNo	= g_config.gameServerNo;
	reqRecord.ServerID	= pPc->idx;
	reqRecord.DanIdx	= pPc->guildIdx;
	memcpy(reqRecord.DanName,pPc->guildName,GUILDNAMESTRING);
	memcpy(reqRecord.MasterName,pPc->name,NAMESTRING);

	if ( !g_pDanBattleManager->InsertRquester(&reqRecord) )
	{
		SendErrorCode(&pPc->sock,GSC_DAN_BATTLE,1,0);		
	}

	g_pDanBattleManager->BroadCast_RequesterList();
	
	return 1;
}


#include "../Tools/CTools.h"
extern CTools* gcpTools;





void CDanBattleManager::UpdateRequester()
{
	BATTLE_ITOR			iLoop;
	LPREQUEST_RECORD	pReq;		
	
	for ( iLoop=m_RequestList.begin();iLoop!=m_RequestList.end();iLoop++)
	{
		LPREQUEST_RECORD pReq=static_cast<LPREQUEST_RECORD>(*iLoop);
		
		if ( pReq->ServerID <0 || pReq->ServerID>= MAX_PCS)
		{
			DeleteRequester(pReq);
			g_pDanBattleManager->BroadCast_RequesterList();
			return;
		}

		playerCharacter_t* pPC = gcpTools->GetPlayerRecordPointer(pReq->ServerID);
		if ( pPC == NULL){			
			DeleteRequester(pReq);		
			g_pDanBattleManager->BroadCast_RequesterList();
			return;
		}

		if ( pPC->active == FALSE)
		{
			DeleteRequester(pReq);
			g_pDanBattleManager->BroadCast_RequesterList();
			return;
		}
	}		
}


void CDanBattleFunc::RecvGSM_RequestBattle()
{
	if ( g_pDanBattleManager->GetStatus() != DBTL_STATUS_READY )
	{										
		entityPCList_t reqPl;
		char userName[NAMESTRING];

		sstrncpy( userName, MSG_ReadString(), NAMESTRING - 1 );

		if( g_entityListMng.FindPCList( userName, &reqPl ) == -1 )
			return;

		SendErrorCode(&g_memberServer[reqPl.serverIdx].sock,GMS_DAN_BATTLE,1,0);
		return;
	}
}

void CDanBattleFunc::RecvCC_SetStatus()
{
	if ( !g_config.isManager )				
	{
		SendGSM_SetStatus();
		return;
	}

	BYTE	bStatus = MSG_ReadByte();

	g_pDanBattleManager->SetStatus( bStatus );
}



void CDanBattleFunc::SendGSM_SetStatus()
{
	BYTE	bStatus	= MSG_ReadByte();

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
		
	MSG_WriteByte(&netMessage, CC_EXTEND);
	MSG_WriteByte(&netMessage, GSM_DAN_BATTLE);
	MSG_WriteByte(&netMessage, DANB_SETSTATUS);
	MSG_WriteByte(&netMessage, bStatus);

	NET_SendUnreliableMessage(&localSocket, &netMessage);

	MSG_EndWriting(&netMessage);	
}

void CDanBattleFunc::RecvGSM_SetStatus()
{
	BYTE	bStatus = MSG_ReadByte();
	g_pDanBattleManager->SetStatus( bStatus );
}

void CDanBattleFunc::RecvCC_SuggestBattle()
{
	if ( !IsGuildMasters( g_curPC ) )		
		return;


	

	REQUEST_RECORD	reqRecord;
	REQUEST_RECORD	sugRecord;

	reqRecord.No		= 0;
	reqRecord.ServerNo	= g_config.gameServerNo;
	reqRecord.ServerID	= g_curPC->idx;
	reqRecord.DanIdx	= g_curPC->guildIdx;
	memcpy(reqRecord.DanName,g_curPC->guildName,GUILDNAMESTRING);
	memcpy(reqRecord.MasterName,g_curPC->name,NAMESTRING);
		

	if ( !g_pDanBattleManager->isSuggester(&reqRecord))			
	{
		SendErrorCode(&g_curPC->sock,GSC_DAN_BATTLE,1,0);		
		return;
	}

	
	
	if ( g_curPC->curChargeSE < g_logic.DanBattlePrice)
	{
		
		SendGSM_SuggestResult(2);
		return;
	}

	sugRecord.DanIdx=MSG_ReadLong();

	sstrncpy( sugRecord.DanName, MSG_ReadString(), GUILDNAMESTRING - 1 );	

	if ( !g_pDanBattleManager->SetSuggester(&sugRecord))
	{
		SendErrorCode(&g_curPC->sock,GSC_DAN_BATTLE,1,0);				
		return;
	}	

	LPREQUEST_RECORD pSuggest=g_pDanBattleManager->GetSuggester();

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
		
	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_DAN_BATTLE);
	MSG_WriteByte(&netMessage, DANB_SUGGEST_BATTLE);

	
	LPREQUEST_RECORD pSugg=static_cast<LPREQUEST_RECORD>(g_pDanBattleManager->GetRequestPointer(&sugRecord));


	if ( pSuggest->ServerID >= MAX_PCS || pSuggest->ServerID <0)
		return;

	NET_SendUnreliableMessage(&g_pc[pSuggest->ServerID].sock, &netMessage);

	MSG_EndWriting(&netMessage);

}

void CDanBattleFunc::RecvCC_SuggestResult()
{
	if ( !IsGuildMasters( g_curPC ) )		
		return;

	
	LPREQUEST_RECORD pSuggest=g_pDanBattleManager->GetSuggester();
	LPREQUEST_RECORD pRequest=static_cast<LPREQUEST_RECORD>(g_pDanBattleManager->GetRequestPointer(NULL));

	if ( pSuggest == NULL )
		return ;

	if ( pSuggest->ServerID >= MAX_PCS || pSuggest->ServerID < 0)
		return;

	
	
	if ( g_pc[pSuggest->ServerID].curChargeSE < g_logic.DanBattlePrice)
	{
		GTH_SendMessage_ChattingMsg(&g_pc[pSuggest->ServerID], 0, 0, 36 );
		SendGSM_SuggestResult(0);
		return;
	}

	
	BYTE bCommand = MSG_ReadByte();	
	SendGSM_SuggestResult(bCommand);
	if ( bCommand <= 0)
		return;




	

	if (pSuggest==NULL)
	{
		SendErrorCode(&g_curPC->sock,GSC_DAN_BATTLE,1,0);		
		return;
	}

	if (strcmp(pSuggest->DanName,g_curPC->guildName))
	{
		SendErrorCode(&g_curPC->sock,GSC_DAN_BATTLE,1,0);		
		return;
	}
	if (strcmp(pSuggest->MasterName,g_curPC->name))
	{
		SendErrorCode(&g_curPC->sock,GSC_DAN_BATTLE,1,0);		
		return;
	}
	
	
	
	g_pDanBattleManager->MakeBattleDan();

	
	
}

void CDanBattleFunc::RecvCC_ChangeBattleMap()
{
	if (g_pDanBattleManager->GetStatus()!=DBTL_STATUS_MOVE)
	{
		SendErrorCode(&g_curPC->sock,GSC_DAN_BATTLE,1,0);		
		return;
	}

	

	
}


void CDanBattleFunc::SendGSM_SuggestResult(BYTE isallow)
{
	
	LPREQUEST_RECORD pSuggest=g_pDanBattleManager->GetSuggester();		
	LPREQUEST_RECORD pRequest=static_cast<LPREQUEST_RECORD>(g_pDanBattleManager->GetRequestPointer(NULL));

	if ( pRequest->ServerID >= MAX_PCS || pRequest->ServerID < 0)
		return;

	if (pSuggest==NULL)
	{
		SendErrorCode(&g_pc[pRequest->ServerID].sock,GSC_DAN_BATTLE,1,0);		
		return;
	}	
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
		
	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_DAN_BATTLE);
	MSG_WriteByte(&netMessage, DANB_SUGGEST_RESULT);
	MSG_WriteByte(&netMessage, isallow);

	NET_SendUnreliableMessage(&g_pc[pRequest->ServerID].sock, &netMessage);

	MSG_EndWriting(&netMessage);

}


void CDanBattleFunc::RecvCC_Battle_Start()
{

}

void CDanBattleFunc::RecvCC_CancelRequest()
{
	if ( !IsGuildMasters( g_curPC ) )		
		return;

	REQUEST_RECORD	canRecord;

	canRecord.No		= 0;
	canRecord.ServerNo	= g_config.gameServerNo;
	canRecord.ServerID	= g_curPC->idx;
	canRecord.DanIdx	= g_curPC->guildIdx;
	memcpy(canRecord.DanName,g_curPC->guildName,GUILDNAMESTRING);
	memcpy(canRecord.MasterName,g_curPC->name,NAMESTRING);
		

	if ( !g_pDanBattleManager->DeleteRequester(&canRecord))			
	{
		SendErrorCode(&g_curPC->sock,GSC_DAN_BATTLE,1,0);
		return;
	}	

	
	g_pDanBattleManager->BroadCast_RequesterList();
}


void CDanBattleFunc::SendGSM_SuggestBattle()
{

}

void CDanBattleFunc::RecvCC_InitChao()
{
	g_curPC->chaosPoint=0;
}


void CDanBattleFunc::RecvCC_SetDanTestMode()
{	
	g_pDanBattleManager->m_bDanTestMode=MSG_ReadByte();
}



void CDanBattleFunc::RecvCC_BattleReady()
{
	LPREQUEST_RECORD	requester = static_cast<LPREQUEST_RECORD>(g_pDanBattleManager->GetBattleRequestDan());
	LPREQUEST_RECORD	suggester = static_cast<LPREQUEST_RECORD>(g_pDanBattleManager->GetBattleSuggestDan());

	
	
	if ( requester->ServerID < 0 || suggester->ServerID < 0 )
	{
		
		return;
	}
	

	SendGSM_BattleReady(&g_pc[requester->ServerID].sock);
	SendGSM_BattleReady(&g_pc[suggester->ServerID].sock);
	
	g_pDanBattleSys->MakeDanBattlePortal(DBTL_STATUS_READY);
	
}

void CDanBattleFunc::SendGSM_BattleReady(i3socket_t* sock)
{
	
	int		Status			= g_pDanBattleSys->GetStatus();
	DWORD	StatusStartTime = g_pDanBattleSys->GetStatusStartTime();
	
	
	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
		
	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_DAN_BATTLE);
	MSG_WriteByte(&netMessage, DANB_BATTLE_READY);		

	
	

	NET_SendUnreliableMessage(sock, &netMessage);

	MSG_EndWriting(&netMessage);
	
	
}




void CDanBattleSys::AutoMoveDanMember()
{
	int				userIdx;
	int				zoneIdx;
	typedef std::list<int>		TEAM_LIST;
	typedef TEAM_LIST::iterator	TEAM_ITOR;

	TEAM_ITOR	iLoop;

	for (int TeamCount = 0; TeamCount < 2; TeamCount ++)
	{	
		for (iLoop=m_BattleTeam[TeamCount].begin();iLoop!=m_BattleTeam[TeamCount].end();iLoop++)
		{
			userIdx=(*iLoop);			
			
			vec_copy( g_logic.danbattlePortal[DANBATTLE_ATEAM_PORTAL +TeamCount].TargetPos, g_pc[userIdx].position );
			zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, g_pc[userIdx].idx, g_pc[userIdx].worldIdx, g_pc[userIdx].zoneIdx, g_pc[userIdx].position);
			g_pc[userIdx].zoneIdx = zoneIdx;
			
			GTH_SendPCEventMessage_Respawn( &g_pc[userIdx] );
			GTH_SendMessage_SyncItemObject( &g_pc[userIdx] );				
			
			
		}
	}
}







void CDanBattleSys::TakeConsideration(playerCharacter_t *pc, int considerTableIdx)
{
	if ( pc == NULL)
		return;	

	int addItemData[6];	
	memset(addItemData, 0, 6);

	addItemData[0] = g_ConsiderationTable[considerTableIdx].Idx;	
	addItemData[1] = -1;											
	addItemData[2] = g_ConsiderationTable[considerTableIdx].Quantity;	
	addItemData[3] = 1;												

	AI_NPC_AddItem( pc, addItemData);
	GTH_PC_GetLoot( pc->idx, g_ConsiderationTable[considerTableIdx].Exp, 
	g_ConsiderationTable[considerTableIdx].GenExp, 
	0, 0, 0 );

	
	
}

DWORD CDanBattleSys::GetStatusStartTime()
{
	return m_dwStatusStartTime;
}

void CDanBattleSys::SendRemainTime()
{
	TEAM_ITOR	iLoop;
	int			userIdx;
	DWORD		RemainTime = 
		g_logic.danBattleStatusTime[m_iBattleStatus]-(g_globalTime-m_dwStatusStartTime);


	for (iLoop=m_BattleTeam[0].begin();iLoop!=m_BattleTeam[0].end();iLoop++)
	{
		userIdx=(*iLoop);				
		g_pDanBattleManager->SendGSM_Refresh_PersonData(&g_pc[userIdx], 2, RemainTime);		
	}

	for (iLoop=m_BattleTeam[1].begin();iLoop!=m_BattleTeam[1].end();iLoop++)
	{
		userIdx=(*iLoop);		
		g_pDanBattleManager->SendGSM_Refresh_PersonData(&g_pc[userIdx], 2, RemainTime);	
	}
}
