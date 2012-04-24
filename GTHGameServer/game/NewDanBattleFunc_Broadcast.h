
#ifndef _DAN_FATTLE_BROADCAST_H
#define _DAN_FATTLE_BROADCAST_H












void	GTH_ProcessMessage_BroadCastDanBatttStatus();


void	GTH_SendMessage_BroadCastDanBattleStatus(NewDanBattleSystem::DanBattleInfo::enumBattleStatus status,
int nGuildIdx,	int nEmenyGuild, int nWorldIdx,int value, int value2 = 1);


void	GTH_SendMessage_DanBattleStatusToManager(NewDanBattleSystem::DanBattleInfo::enumBattleStatus status,
int nGuildIdx, 	int nEmenyGuild, int WorldIdx, int value, int value2);


void	GTH_SendMessage_DanBattleStatusToMemberServer_Ready(
int nGuildIdx, 	int nEmenyGuild, int WorldIdx, int value, int value2);


void	GTH_SendMessage_DanBattleStatusToMemberServer_Start(
int nGuildIdx, 	int nEmenyGuild, int WorldIdx, int value);


void	GTH_SendMessage_DanBattleStatusToMemberServer_End(
int nGuildIdx, 	int nEmenyGuild, int WorldIdx, int value);

#endif