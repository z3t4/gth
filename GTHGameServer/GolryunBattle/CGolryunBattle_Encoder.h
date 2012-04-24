



#if !defined(AFX_CGOLRYUNBATTLE_ENCODER_H__2778E092_FB08_4928_ABC2_FE2CE3FA9F02__INCLUDED_)
#define AFX_CGOLRYUNBATTLE_ENCODER_H__2778E092_FB08_4928_ABC2_FE2CE3FA9F02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "CGolryunBattle_Data.h"
class CGolryunBattle_Process;

class CGolryunBattle_Encoder  
{
	private:
		BOOL m_bOK;
		CGolryunBattle_Data* m_cpData;
		CGolryunBattle_Process* m_cpProcess;
	public:
		BOOL isOK(void) const;
		void SetProcessClassPointer(CGolryunBattle_Process* cpProcess);

		void		SendTrasportGolryun(i3socket_t* sock);
		void		SendTrasportArayan(i3socket_t* sock);
		void        SendSuggestBattle(i3socket_t* sock, BYTE eType);
		void		SendOpenBattleTicket(i3socket_t* sock, BYTE eType);
		void		SendOpenShareWin(i3socket_t* sock);
		void		SendDisplayTournament(i3socket_t* sock);
		void		SendCheckPrize(i3socket_t* sock);

		void		SendObserveStart(playerCharacter_t* pToPlayer,const BOOL bSuccess);
		void		SendObserveEnd(playerCharacter_t* pToPlayer,const BOOL bSuccess);
		void		SendObserveMatchTime(playerCharacter_t* pToPlayer,const int time);
		
		void		SendErrorCode(
						i3socket_t* sock,
						const BYTE eCode,
						enum tagGolryunBattlePacket_GB_ERROR_CODE::enumCode eType) const;


		void		SendEffectCount(i3socket_t* sock, int angle);
		void		SendEffectResult(i3socket_t* sock, int index);
		void		SendStartMatch(i3socket_t* sock, const int time);		
		void		SendEndMatch(i3socket_t* sock,enum tagGolryunBattlePacket_GB_REQUEST_END_MATCH::enumCode code);
		void		SendCancelTournament(i3socket_t* sock);
		void		SendShareRate(playerCharacter_t* pPlayer);
		void		SendReNewShareRate(playerCharacter_t* pPlayer);
		void		SendJoinNumber(playerCharacter_t* pPlayer);
		void		SendNoticeBattle(i3socket_t* sock, char* notice);
		void		SendLocalUserNotice(i3socket_t* sock, int lpackIdx);
		void		SendAllUserData(i3socket_t* sock);
		void		SendEndMatchNotice(void);
		void		SendStartMatchNotice(playerCharacter_t* pPlayer, int redNum, int blueNum);
		void		Send_GB_BATTLE_RASA_Sync(playerCharacter_t* pPlayer);
		void		SendCheckJoinerNum(i3socket_t* sock, int num);
		void		SendStatusMatchNotice(i3socket_t* sock, int noticeIdx);
		void		SendBuyBattleTicket(playerCharacter_t* pPlayer);
		void		SendTwoGyunTotalNak(playerCharacter_t* pPlayer);
		void		SendPacketMatchTimePerSecond(playerCharacter_t* pPlayer, const int time);
		
		void		SendMessageToGM(const int LpackIdx);
		
		void		ManagerNoticeBattle(char* notice);
		void		ManagerLocalUserNotice(int lpackIdx);
		void		ManagerAllUserData();

	public:
		CGolryunBattle_Encoder(CGolryunBattle_Data* cpData);
		virtual ~CGolryunBattle_Encoder();

};

#endif 
