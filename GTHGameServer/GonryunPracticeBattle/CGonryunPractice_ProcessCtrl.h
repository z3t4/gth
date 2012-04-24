#ifndef _CGONRYUNPRACTICE_PROCESSCTRL_H_
#define _CGONRYUNPRACTICE_PROCESSCTRL_H_



class CGonryunPracticeBattle;
class CGonryunPractice_WaiterCtrl;
class CGonryunPractice_RewordCtrl;
class CGonryunPractice_Encoder;

#include "tagGonryunPractice.h"
#include "tagGonryunPracticePacket.h"
class CGonryunPractice_ProcessCtrl{

public:
	enum _define{
		MAX_WAIT_USER	= 20,
			ARAYANWORLDMAP	= 0,
			SYENWORDMAP		= 9,
			MAX_OVERFLOW = 0,
			
	};
	
	enum efffecmode{
		PCB_EFF_NONE =0,
			PCB_EFF_READY,
			PCB_EFF_WIN,
			PCB_EFF_DRAW,
			PCB_EFF_DEFEAT,
	};
	
	enum Step{
		STEP_NONE=0,								
			STEP_SUGGUEST,							
			STEP_BATTING,							
			STEP_BATTLE_READY,						
			STEP_BATTLE_COUNTDOWN,					
			STEP_FIGHTING,							
			STEP_BATTLE_END,						
			STEP_MAX,					
	};
	
	enum battleMode{
		PCB_BATTLE_NONE			= 0,
			PCB_BATTLE_NORMAL		= 1,			
			PCB_BATTLE_BATTING		= 2,			
	};

	enum gameEndType{
		BATTLE_END_DRAW			= 0,				
		BATTLE_END_RESULT_BORN	= 1,			    
	};
	
	enum confirmState{
		Confirm_None			= 0,
		Confirm_Admit			= 1,
		Confirm_Win				= 2,
		Confirm_Defeate			= 3,
		Confirm_Fail			= 4,
	};

public:
	CGonryunPractice_ProcessCtrl();
	virtual ~CGonryunPractice_ProcessCtrl(){}
	
public:
	BOOL						isOK(void);
	void						Init(void);
	void						Set(CGonryunPracticeBattle* pPracticeBattle);
	BOOL						SuggestPlayer(playerCharacter_t* pSuggestPlayer);
	void						SetActive(const int& active);
	const int&					GetActive(void)const;	
	void						SetStep(const Step& step);
	const enum Step&			GetStep(void)const;
	void						MakeTransPosition(playerCharacter_t* pLeaderPlayer,playerCharacter_t* pOpenentPlayer);
	void						MakeTransPort(playerCharacter_t* pc,const vec3_t leaderposition);								
	void						SetBattleType(const battleMode& type);
	const						battleMode& GetBattleType()const;
	void						SetLeader( playerCharacter_t* pLeaderPlayer);
	void						SetOpenent(playerCharacter_t* Waiter);
	
	
	void						IrregularQuitBattle(playerCharacter_t* pOutPlayer);
	void						IrregularQuitBattle_Suggent(playerCharacter_t* pOutPlayer);
	void						IrregularQuitBattle_Batting(playerCharacter_t* pOutPlayer);
	void						IrregularQuitBattle_Fighting(playerCharacter_t* pOutPlayer);
	void						IrregularQuitBattle_End(playerCharacter_t* pOutPlayer);

	void						Update(void);										
	void						CheckGameEnd(playerCharacter_t* pOutPlayer,const enum gameEndType& endType);	
	void						InitTradeBatting( playerCharacter_t* pc ); 
	int							UndoBattingTradeBatting(playerCharacter_t* pPlayer);	
	void						BattingRestore(playerCharacter_t* pOutPlayer);
	void						SetBattingBattle(const BOOL bvalue);
	BOOL						isBattingPlayActive(void) const;
	void						InitNoticePC( playerCharacter_t* pPlayer );
	void						CheckPlayerPos_and_Transport(playerCharacter_t* pPlayer);

	const tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::errorCode
								CheckConfirmTradeBatting( 
								playerCharacter_t *pPlayer, 
								playerCharacter_t* pOpenent);	


private:
	void						CheckNormalGameEnd(playerCharacter_t* pWinner,playerCharacter_t* pDefeator);							 
	void						CheckBattingGameEnd(playerCharacter_t* pWinner,playerCharacter_t* pDefeator);	
	void						CheckOverFlowUseCount(playerCharacter_t* pPlayer);
	void						CheckAndMove(void);	
	int							DeleteBattingItem(playerCharacter_t* pdefeator);
	


	void						GetWinnerPortalPos(playerCharacter_t* pPlayer);
	void						GeLoserPortalPos(playerCharacter_t* pLoserPlayer);
	void						GiveBattingWinner(playerCharacter_t *pwinor,playerCharacter_t *pdefeator);		

	void					    ConfirmTradeBattingWinerGive(
									playerCharacter_t *pwinor,
									playerCharacter_t *pdefeator); 

	inline void					ConfirmTradeBattingWinerGive_Item(
									playerCharacter_t* pwinor, 
									playerCharacter_t* pdefeator);




	
	


	void						Update_Sugguest(const int time);
	void						Update_Batting(const int Time);
	void						Update_Battle_Ready(const int Time);
	void						Update_Fighting(const int Time);
	void						Update_Battle_End(const int Time);
	playerCharacter_t*			GetPlayerPointer(const struct tagGPWaitUser& Fighter) const;
	BOOL						CheckItemInformation( playerCharacter_t* pPlayer,const int checkType = 0 );

private:
	enum Step							m_Step;
	DWORD								m_dwStatusStartTime;
	BOOL								m_bIsactive;
	enum battleMode					    m_BattleType;	
	BOOL								m_bOK;
	BOOL								m_bIsBattleBatting;

private:
	CGonryunPractice_WaiterCtrl*		m_cpVirtualWaiterCtrl;
	CGonryunPractice_RewordCtrl*		m_cpVirtualRewordCtrl;
	CGonryunPractice_Encoder*			m_cpVirtualEncoder;

public:
	struct tagGPWaitUser				m_Leader;
	struct tagGPWaitUser				m_Openent;
	struct tagGPWaitUser				m_Winner;
	struct tagGPWaitUser				m_Loser;
};

#endif