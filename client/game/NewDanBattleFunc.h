



#if !defined(AFX_NEWDANBATTLEFUNC_H__7D6A2834_7709_41C2_87CB_E0EB507EF460__INCLUDED_)
#define AFX_NEWDANBATTLEFUNC_H__7D6A2834_7709_41C2_87CB_E0EB507EF460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "../game/tagNewDanPacket.h"

enum enumNewDanBattlePacket
{
	
	CC_ACTIVE_SYSTEM		= 0X0000,		

	GSC_ACTIVE_SYSTEM		= 0X0001,		
	
	GSC_DAN_BATTLE_INFO		= 0X0002,		
											

	CC_REQUEST_DANBATTLE	= 0X0003,		
	GSC_REQUEST_DANBATTLE	= 0X0004,		

	GSC_SUGGEST_DANBATTLE	= 0X0005,		
	CC_SUGGEST_DANBATTLE	= 0X0006,		
	
	GSC_ACCEPT_DANBATTLE	= 0X0007,		
	
	GSC_DAN_BATTLE_RESULT	= 0X0008,		

	CC_DAN_BATTLE_WIN_OPEN	= 0X0009,		
	GSC_DAN_BATTLE_WIN_OPEN	= 0X000a,		
	
	GSC_DAN_BATTLE_READY	= 0X000b,		
	GSC_DAN_BATTLE_START	= 0X000c,		
	GSC_DAN_BATTLE_END		= 0X000d,		

	GSC_DAN_BATTLE_PLUNDER	= 0X000f,		
	SS_DAN_BATTLE_BROADCAST = 0X0010,

	SS_DAN_BATTLE_READY		= 0X0011,
	SS_DAN_BATTLE_START		= 0X0012,
	SS_DAN_BATTLE_END		= 0X0013,
	SS_DAN_BATTLE_RESULT	= 0X0014,

	CS_DAN_BATTLE_CHANGE_STATUS	= 0X0015,		
	GSC_DAN_BATTLE_POWER_END	= 0X0016,
	GSC_DAN_BATTLE_NOTICE_MSG	= 0X0017,
};


void	GTH_DecodeDanBattleSystem();
void	GTH_ProcessMessage_DanBattleActiveSystem();
void	GTH_ProcessMessage_DanBattleActiveSystem();
void	GTH_ProcessMessage_RequestDanBattle();
void	GTH_ProcessMessage_AcceptDanBattle();
void	GTH_ProcessMessage_SuggestDanBattle();
void	GTH_ProcessMessage_NewDanBattleWinOpen();
void	GTH_ProcessMessage_NewDanBattleReady();
void	GTH_ProcessMessage_NewDanBattleStart();
void	GTH_ProcessMessage_NewDanBattleEnd();
void	GTH_ProcessMessage_NewDanBattlePlunder();
void	GTH_ProcessMessage_SuggestDanBattle();
void	GTH_ProcessMessage_DanBattleResult();
void	GTH_ProcessMessage_DanBattleInfo();
void	GTH_ProcessMessage_DanBattleNoticeMsg();




void	GTH_SendMessage_DanBattleInfo();
void	GTH_SendMessage_DanBattleResult();
void	GTH_SendMessage_RequestDanBattle();
void	GTH_SendMessage_Request_NewDanBattleWinOpen();
void	GTH_SendMessage_ReplySuggest(tagGCPacket_NewDanBattle::enumCode code);

void	GTH_SendMessage_ChangeStatus(NewDanBattleSystem::DanBattleInfo::enumBattleStatus status, char* guildname);




#endif 
