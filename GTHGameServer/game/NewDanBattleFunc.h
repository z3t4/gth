




#if !defined(AFX_NEWDANBATTLEFUNC_H__7D6A2834_7709_41C2_87CB_E0EB507EF460__INCLUDED_)
#define AFX_NEWDANBATTLEFUNC_H__7D6A2834_7709_41C2_87CB_E0EB507EF460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 



enum enumNewDanBattlePacket
{
	
	CC_ACTIVE_SYSTEM			= 0X0000,		
	GSC_ACTIVE_SYSTEM			= 0X0001,		
	
	GSC_DAN_BATTLE_INFO			= 0X0002,		

	CC_REQUEST_DANBATTLE		= 0X0003,		
	GSC_REQUEST_DANBATTLE		= 0X0004,				

	GSC_SUGGEST_DANBATTLE		= 0X0005,		
	CC_SUGGEST_DANBATTLE		= 0X0006,		
	
	GSC_ACCEPT_DANBATTLE		= 0X0007,	
	
	GSC_DAN_BATTLE_RESULT		= 0X0008,		

	CC_DAN_BATTLE_WIN_OPEN		= 0X0009,		
	GSC_DAN_BATTLE_WIN_OPEN		= 0X000a,		
	

	GSC_DAN_BATTLE_READY		= 0X000b,		
	GSC_DAN_BATTLE_START		= 0X000c,		
	GSC_DAN_BATTLE_END			= 0X000d,		
	
	GSC_DAN_BATTLE_PLUNDER		= 0X000F,		
	SS_DAN_BATTLE_BROADCAST		= 0X0010,

	SS_DAN_BATTLE_READY			= 0X0011,		
	SS_DAN_BATTLE_START			= 0X0012,		
	SS_DAN_BATTLE_END			= 0X0013,		
	SS_DAN_BATTLE_RESULT		= 0X0014,		
	CS_DAN_BATTLE_CHANGE_STATUS = 0X0015,

	GSC_DAN_BATTLE_POWER_END	= 0X0016,		
	GSC_DAN_BATTLE_NOTICE_MSG	= 0X0017,		
	
};


void	GTH_DecodeDanBattleSystem();




void	GTH_ProcessMessage_DanBattleActiveSystem();
void	GTH_ProcessMessage_DanBattleChangeStatus();






void	GTH_ProcessMessage_NewDanBattleWinOpen();

void	GTH_SendMessage_NewDanBattleWinOpenReply(playerCharacter_t* pPlayer, BOOL Reply);	

void	GTH_ProcessMessage_RequestDanBattle();

void	GTH_SendMessage_RequestDanBattleReply(playerCharacter_t* pPlayer, BOOL Reply, int ErrorIdx);

void	GTH_SendMessage_SuggestDanBattle(playerCharacter_t* pPlayer, NewDanBattleSystem::DanBattleInfo* pInfo);

void	GTH_ProcessMessage_SuggestDanBattleReply();

void	GTH_ProcessInsertDanBattle(NewDanBattleSystem::DanBattleInfo*, tagGCPacket_NewDanBattle::enumDanBattleInsertcode code);

void	GTH_SendMessage_AcceptDanBattle(playerCharacter_t* pPlayer, NewDanBattleSystem::DanBattleInfo* pDanBattleInfo);









void	GTH_SendMessage_SuncMessageDanBattleInfo(NewDanBattleSystem::DanBattleInfo* pDanbattleInfo);



void	GTH_ProcessDanBattleForReJoin(playerCharacter_t* pPlayer);



void	GTH_SendMessage_DanBattleInfo(playerCharacter_t* pPlayer, NewDanBattleSystem::DanBattleInfo* pDanBattleInfo);


void	GTH_ProcessMessage_NewDanBattleReady(int nGuildIdx, int enemyguildidx, int nWorldIdx, int RemainTime, int mode);
void	GTH_SendMessage_NewDanBattleReady(playerCharacter_t* pPlayer, 
										  int EneumGuildIdx, 
										  char* szEnemyGuildName,
										  int nWorldIdx, 
										  int RemainTime,
										  NewDanBattleSystem::DanBattleInfo::enumBattleMode mode);


void	GTH_ProcessMessage_NewDanBattleStart(int nGuildIdx, int enemyguildidx, int nWorldIdx, int RemainTime);
void	GTH_SendMessage_NewDanBattleStart(playerCharacter_t* pPlayer, int EneumGuildIdx, int nWorldIdx, int RemainTime);


void	GTH_ProcessMessage_NewDanBattlePlunder();
void	GTH_SendMessage_NewDanBattlePlunder(playerCharacter_t* pPlayer, tagGCPacket_NewDanBattle::PlunderPcCode, tagGCPacket_NewDanBattle::PlunderItemCode, int DeleteInvenPos, item_t* pItem = NULL );


void	GTH_ProcessMessage_NewDanBattleEnd(int nGuildIdx, int enemyguildidx, int nWorldIdx, int RemainTime);	
void	GTH_SendMessage_NewDanBattleEnd(playerCharacter_t* pPlayer, int EneumGuildIdx, int nWorldIdx, int RemainTime);



void	GTH_SendMessage_DanBattleResult(playerCharacter_t* pPlayer, tagGCPacket_NewDanBattle::enumwincode code, char* enemyGuildName, char* bestid, int wincount, int losecount, int mypoint, NewDanBattleSystem::DanBattleInfo::enumEndType type);
void	GTH_SendMessage_DanBattleNoticeMsg(playerCharacter_t* pPlayer, NewDanBattleSystem::noticetype type, char* noticeMsg);
void	GTH_SendMessage_DanBattleNoticeMsgToDanMember(NewDanBattleSystem::DanBattleInfo* pDanbattleInfo, NewDanBattleSystem::noticetype type, char* noticeMsg);








int		GTH_Get_OnlineGuildMemeberCount(int WorldIdx, 
										int GuildIdx, 
										vector<NewDanBattleSystem::DanBaseInfo::sDanBattleMember> *pv_LiveGuiledmembers = NULL);


void	GTH_UndoPcDanBattle(playerCharacter_t* pPc);

void	GTH_CalcPointNewDanBattle(playerCharacter_t *pPC);	


BOOL	GTH_ProcessPlunder(playerCharacter_t* pAttackPC, playerCharacter_t* pDiePC);

BOOL	GTH_ProcessPlunderExp(playerCharacter_t* pAttackPC, playerCharacter_t* pDiePC);


BOOL	GTH_ProcessPlunderItem(playerCharacter_t* pAttackPC, playerCharacter_t* pDiePC, BOOL bPrimiumItem = FALSE);
BOOL	ISAllowPlunderItem(int nItemIdx, BOOL bPrimiumItem);


int		GTH_GetReduceEXP(playerCharacter_t*, float percent, int& prevTotalExp_OUT);
BOOL	GTH_NewDanBattlePc_DIE(playerCharacter_t* pAttackPC, playerCharacter_t* pDiePC);


void	ClearDanBattleInfo(playerCharacter_t* pPC);
void	GTH_ProcessDanbattlePowerEnd();


#endif 
