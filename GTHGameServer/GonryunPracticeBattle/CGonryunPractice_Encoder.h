#ifndef _CGONRYUNPRACTICE_ENCODER_H_
#define _CGONRYUNPRACTICE_ENCODER_H_



class CGonryunPracticeBattle;
class CGonryunPractice_ProcessCtrl;
class CGonryunPractice_WaiterCtrl;

#include "tagGonryunPracticePacket.h"

class CGonryunPractice_Encoder  
{
private:
	BOOL m_bOK;
	
public:
	
	CGonryunPractice_Encoder();
	virtual ~CGonryunPractice_Encoder();

public:
	BOOL					isOK(void);
	void					Set(CGonryunPracticeBattle* pPracticeBattle);
	void					SendEffect(playerCharacter_t* pPlayer,int type);						
	void					SendStatus(playerCharacter_t* pPlayer);	
	void					SendGonryunBattleInfo(playerCharacter_t* pPlayer,const int time); 
	void					SendGonryunBattleResult(playerCharacter_t* pPlayer,int result,char* pname); 
	void					SendMessageBattleReject(playerCharacter_t* pLeaderPlayer);										
	void					SendReqeust_NomalBattle_toOpenent(
								const int   LeaderPCIdx,
								char* szpLeaderName,	
								playerCharacter_t* pOpenentPlayer);

	void					SendMessage_NormalReject_toPlayer(playerCharacter_t* pLeaderPlayer);
	void					SendBattleMsg(
								playerCharacter_t* pPlayer, 
								const enum tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::enumCode Code,
								const int Value1=0,
								const int Value2=0,
								const int Value3=0) const;
	void					SendMessageNake(playerCharacter_t* pPlayer);					 
	void					SendWaitUserList(playerCharacter_t* pLeaderPlayer,playerCharacter_t* pToPlayer);
	void					SendMyInfoToUser( playerCharacter_t* pLeaderPlayer,playerCharacter_t* pSuggestPlayer);
	void					SendDeleteUser(playerCharacter_t* pPlayer);
	void					SendWaitUserBattleEnd(playerCharacter_t* pPlayer);
	void					SendWaitUserBattleEnd(void);
	
	void					SendMessage_BattingTrade_ItemBatting(
								playerCharacter_t* pPlayer,
								BOOL bMyItem,
								enum tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_ITEM::enumType Type,
								int pos,
								item_t *item,
								const int DeleteItemIdx = 0);
	
	void					SendMessage_BattingTrade_AdmitBatting( playerCharacter_t* pc);
	void					SendMessage_BattingTrade_RequestBatting( playerCharacter_t* pPlayer,playerCharacter_t* ptarget, int type  );
	void					SendMessage_BattingTrade_RejectBatting( playerCharacter_t* pc, int type );
	void					SendMessage_BattingTrade_ExitBatting( playerCharacter_t* pc );
	void					SendMessage_BattingTrade_OkBatting( playerCharacter_t* pc,BOOL type );
	void					SendMessage_BattingTrade_CancelBatting( playerCharacter_t* pc, int type );
	
	void SendMessage_BattingTrade_ConfirmBatting( 
			playerCharacter_t* player, 
			const enum tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::enumState& state,	
			const enum tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::errorCode& Code);

	void					SendMessage_BattingTrade_SEBatting( playerCharacter_t *pc, int error, int type, int se );
	
	
	void					SendMessage_Open(playerCharacter_t* pPlayer);
	int						SendPCEventMessage_DieGonryun( playerCharacter_t* pPlayer );
private:

	CGonryunPracticeBattle*			GetPracticeBattlePointer()const;
	CGonryunPractice_ProcessCtrl*	GetProcessCtrl()const;


private:

	CGonryunPracticeBattle*			m_cpVirtualPracticeBattle;
	CGonryunPractice_ProcessCtrl*	m_cpVirtualProcessCtrl;
	CGonryunPractice_WaiterCtrl*    m_cpVirtualWaiterCtrl;

};

#endif 
