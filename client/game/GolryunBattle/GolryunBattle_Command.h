#ifndef _GOLRYUNBATTLE_COMMAND_H_																			
#define _GOLRYUNBATTLE_COMMAND_H_

#include "../game/GolryunBattle/GolryunBattle.h"
#include "../game/GolryunBattle/GolryunBattle_Encoder.h"

class CGolryunBattle_Command
{
private:
	
public :
	CGolryunBattle_Command();
	virtual ~CGolryunBattle_Command();


public :
	void ProcessCommandToGM(char message[], char command[], chatMessage param[]);
	void CommandToGMBattleNotice(char message[], char command[], chatMessage param[]);
	void CommandToGMBattleStart(char message[], char command[], chatMessage param[]);
	void CommandToGMJoinStart(char message[], char command[], chatMessage param[]);
	void CommandToGMRecallStart(char message[], char command[], chatMessage param[]);
	void CommandToGMJoinClose(char message[], char command[], chatMessage param[]);
	void CommandToGMBettingStart(char message[], char command[], chatMessage param[]);
	void CommandToGMReadyTournament(char message[], char command[], chatMessage param[]);
	void CommandToGMStartTournament(char message[], char command[], chatMessage param[]);
	void CommandToGMPauseTournament(char message[], char command[], chatMessage param[]);
	void CommandToGMReStartTournament(char message[], char command[], chatMessage param[]);
	void CommandToGMCloseBattle(char message[], char command[], chatMessage param[]);
	void CommandToGMTransportGolryun(char message[], char command[], chatMessage param[]);
	void CommandToGMTransportWaitingRoom(char message[], char command[], chatMessage param[]);
	void CommandToGMTransportBattleField(char message[], char command[], chatMessage param[]);
	void CommandToGMWalkOverMatch(char message[], char command[], chatMessage param[]);
	void CommandToGMCheckJoinNumber(char message[], char command[], chatMessage param[]);
	void CommandToGMStartDrawPrize(char message[], char command[], chatMessage param[]);
	void CommandToGMCancelTournament(char message[], char command[], chatMessage param[]);
	void CommandToGMTaltalNakBetting(char message[], char command[], chatMessage param[]);
	void CommandToGMForceJoinList(char message[], char command[], chatMessage param[]);
	void CommandToGMAssignFighterNumber(char message[], char command[], chatMessage param[]);
	void CommandToGMForceEject(char message[], char command[], chatMessage param[]);
	void CommandToGMCheckStatusPvPMode(char message[], char command[], chatMessage param[]);
	
};

extern CGolryunBattle_Command g_GolryunBattle_Command;

#endif