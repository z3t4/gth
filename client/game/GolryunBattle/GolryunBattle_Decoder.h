#ifndef _GOLRYUNBATTLE_DECODER_H_																			
#define _GOLRYUNBATTLE_DECODER_H_

#include "../game/GolryunBattle/GolryunBattle.h"
#include "../game/GolryunBattle/GolryunBattle_Encoder.h"

class CGolryunBattle_Decoder
{
private:
	
public :
	CGolryunBattle_Decoder();
	virtual ~CGolryunBattle_Decoder();


public :
	void	RecvGolryunBattle();
	void    RecvTransGolryun();
	void    RecvTransArayan();
	void    RecvSuggestBattle();
	void	RecvActiveBattle();
	void	RecvOpenBattleTicket();
	void	RecvShareRate();
	void	RecvReNewBattleTicket(void);
	void	RecvOpenShareWin();
	void	RecvDisplayTournament();
	void	RecvCheckPrize();
	void	RecvEffectCount();
	void	RecvEffectResult();
	void	RecvStartMatch();
	void	RecvEndMatch();
	void	RecvCancelTournament();
	void	RecvErrorCode();
	void	RecvJoinNumber();
	void	RecvLocalUserNotice();
	void    RecvAllUserData();
	void	RecvStartMatchNotice(void);
	void	RecvEndMatchNotice(void);
	void	RecvSyncRASAMatch(void);
	void	RecvCheckJoinerNum(void);
	void	RecvStatusMatchNotice(void);
	void	RecvNoticeBattle(void);
	void	RecvBuyBattleTicket(void);
	void	RecvTwoGyunTotalNak(void);
	void	RecvObserveStart(void);
	void	RecvObserveEnd(void);
	void	RecvObserveMatchTime(void);
	void	RecvMatchProgressTime(void);
	void	RecvMessageToGM(void);
	
};

extern CGolryunBattle_Decoder g_GolryunBattle_Decoder;

#endif