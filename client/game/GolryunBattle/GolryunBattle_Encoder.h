#ifndef _GOLRYUNBATTLE_ENCODER_H_																			
#define _GOLRYUNBATTLE_ENCODER_H_


class CGolryunBattle_Encoder
{

public :



private :
	
	
public :
	CGolryunBattle_Encoder();
	virtual ~CGolryunBattle_Encoder();

public :
	void	SendRequestStartBattle(int iManorIdx);
	void	SendRequestStartJoin(char* message);
	void	SendRequestCloseJoin();
	void	SendRequestStartBetting();

	void    SendRequestReadyTournament(int iRound);
	void	SendRequestStartCount();
	void	SendRequestEndMatch();
	void	SendRequestWaitTournament();
	void	SendRequestCloseBattle();
	void    SendRequestStartPrize();
	void	SendRequestCancelTournament();
	
	void    SendAcceptTransGolryun();
	void    SendAcceptToBattle();
	void    SendBuyBattleTicket(int itemTableIdx, int number);
	void    SendReNewBattleTicket();
	void	SendReNewDisplayTournament();

	void	SendCheckStartMatch();
	void	SendCheckJoinerNum();
	void	SendTransWaitingRoom();
	void	SendTransWatchingRoom();
	void	SendTransBattleStadium();
	void	SendNoticeBattle(char* notice);
	void	SendWalkOverMatch(int joinNum);
	void	SendTwoGyunTotalNak(void);
	void	SendObserveEnd(void);
	void	SendOpenBattleTicket(void);
	void	SendOpenShareWin(void);
	void	SendDisplayTournament(void);
	void    SendRequestPauseMatch(void);
	void    SendRequestReStartMatch(void);
	void	SendInsertJoinList(char* name);
	void    SendAssignFighterNumber(char* name, int number);
	void    SendRequestStartReCallFighter(char* message);
	void	SendForceEject(char* name);
	void	SendCheckStatusPvPMode(void);
	
};


extern CGolryunBattle_Encoder g_GolryunBattle_Encoder;

#endif