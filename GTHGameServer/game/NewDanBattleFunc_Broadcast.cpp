

#include "../Global.h"







void GTH_ProcessMessage_BroadCastDanBatttStatus()
{
	NewDanBattleSystem::DanBattleInfo::enumBattleStatus status =
	(NewDanBattleSystem::DanBattleInfo::enumBattleStatus)MSG_ReadLong();

	int GuildIdx		= MSG_ReadLong();
	int EnemyGuildIdx	= MSG_ReadLong();
	int nWorldIdx		= MSG_ReadLong();
	int Value			= MSG_ReadLong();
	int Value2			= MSG_ReadLong();

	GTH_SendMessage_BroadCastDanBattleStatus(status, GuildIdx, EnemyGuildIdx, nWorldIdx, Value, Value2);
}







void	GTH_SendMessage_BroadCastDanBattleStatus(NewDanBattleSystem::DanBattleInfo::enumBattleStatus status,
										int nGuildIdx,
										int nEnemyGuildIDx,
										int nWorldIdx,
										int value,
										int value2)
{


	if ( g_config.isManager == FALSE)
	{
		GTH_SendMessage_DanBattleStatusToManager(status, nGuildIdx, nEnemyGuildIDx, nWorldIdx, value, value2);	
	}
	else{
		
		
		switch(status){
		case NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_READY:
			GTH_ProcessMessage_NewDanBattleReady(nGuildIdx, nEnemyGuildIDx, nWorldIdx, value, value2);
			GTH_SendMessage_DanBattleStatusToMemberServer_Ready(nGuildIdx, nEnemyGuildIDx, nWorldIdx, value, value2);
			break;
			

		case NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE:
			GTH_ProcessMessage_NewDanBattleStart(nGuildIdx, nEnemyGuildIDx, nWorldIdx, value);			
			GTH_SendMessage_DanBattleStatusToMemberServer_Start(nGuildIdx, nEnemyGuildIDx, nWorldIdx, value);
			break;

		case NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_END:
			GTH_ProcessMessage_NewDanBattleEnd(nGuildIdx, nEnemyGuildIDx, nWorldIdx, value);			
			GTH_SendMessage_DanBattleStatusToMemberServer_End(nGuildIdx, nEnemyGuildIDx, nWorldIdx, value);	
			break;

		default:
			return;
		}	
	}
}






void	GTH_SendMessage_DanBattleStatusToManager(NewDanBattleSystem::DanBattleInfo::enumBattleStatus status,
										int nGuildIdx, 	
										int nEnemyGuildIdx,
										int nWorldIdx, 
										int value,
										int value2)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);
		MSG_WriteShort(&netMessage, SS_DAN_BATTLE_BROADCAST);
		MSG_WriteLong(&netMessage, status);
		MSG_WriteLong(&netMessage, nGuildIdx);
		MSG_WriteLong(&netMessage, nEnemyGuildIdx);		
		MSG_WriteLong(&netMessage, nWorldIdx);
		MSG_WriteLong(&netMessage, value);		
		MSG_WriteLong(&netMessage, value2);	
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	
}









void GTH_SendMessage_DanBattleStatusToMemberServer_Ready( int nGuildIdx, int nEnemyGuildIdx, int nWorldIdx, int value, int value2)
{
			
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);
		MSG_WriteShort(&netMessage, SS_DAN_BATTLE_READY);		
		MSG_WriteLong(&netMessage,	nGuildIdx);
		MSG_WriteLong(&netMessage,	nEnemyGuildIdx);		
		MSG_WriteByte(&netMessage,	nWorldIdx);
		MSG_WriteLong(&netMessage,	value);
		MSG_WriteByte(&netMessage,	value2);
		
		
		for (int nServerIdx = 1; nServerIdx < MAX_MEMBER_SERVER; nServerIdx++) 
		{
			if ( !g_memberServer[nServerIdx].active )	continue;
			NET_SendUnreliableMessage(&g_memberServer[nServerIdx].sock, &netMessage);
		}		
	}
	MSG_EndWriting(&netMessage);

}






void GTH_SendMessage_DanBattleStatusToMemberServer_Start( int nGuildIdx, int nEnemyGuildIdx, int nWorldIdx, int value)
{			
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);
		MSG_WriteShort(&netMessage, SS_DAN_BATTLE_START);		
		MSG_WriteLong(&netMessage,  nGuildIdx);
		MSG_WriteLong(&netMessage,  nEnemyGuildIdx);		
		MSG_WriteByte(&netMessage,  nWorldIdx);
		MSG_WriteLong(&netMessage,  value);		
		
		
		for (int nServerIdx = 1; nServerIdx < MAX_MEMBER_SERVER; nServerIdx++) 
		{
			if ( !g_memberServer[nServerIdx].active )	continue;
			NET_SendUnreliableMessage(&g_memberServer[nServerIdx].sock, &netMessage);
		}		
	}
	MSG_EndWriting(&netMessage);

}







void GTH_SendMessage_DanBattleStatusToMemberServer_End( int nGuildIdx, int nEnemyGuildIdx, int nWorldIdx, int value)
{
			
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, NEW_DANBATTLE);
		MSG_WriteShort(&netMessage, SS_DAN_BATTLE_END);
		MSG_WriteLong(&netMessage, nGuildIdx);
		MSG_WriteLong(&netMessage, nEnemyGuildIdx);
		MSG_WriteByte(&netMessage, nWorldIdx);
		MSG_WriteByte(&netMessage, value);
		
		
		for (int nServerIdx = 1; nServerIdx < MAX_MEMBER_SERVER; nServerIdx++) 
		{
			if ( !g_memberServer[nServerIdx].active )	continue;
			NET_SendUnreliableMessage(&g_memberServer[nServerIdx].sock, &netMessage);
		}		
	}
	MSG_EndWriting(&netMessage);

}



