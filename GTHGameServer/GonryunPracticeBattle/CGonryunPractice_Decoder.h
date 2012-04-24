#ifndef _CGONRYUNPRACTICE_DECODER_H_
#define _CGONRYUNPRACTICE_DECODER_H_


class CGonryunPracticeBattle;
class CGonryunPractice_Encoder;
class CGonryunPractice_Decoder;
class CGonryunPractice_ProcessCtrl;
class CGonryunPractice_WaiterCtrl;
  
class CGonryunPractice_Decoder{
private:
	BOOL m_bOK;



public:
	CGonryunPractice_Decoder();
	virtual ~CGonryunPractice_Decoder();

public:	
	BOOL		isOK(void);			
	void		recvPacket(void);									
	void		Set(CGonryunPracticeBattle* pPracticeBattle);

private:

	void		recvRespond_NormalResult_fromOpenent(playerCharacter_t* pOpenentPlayer);
	void		recvReques_NormalBattle_FromLeader(playerCharacter_t* pLeaderPlayer);						
	void		recvRespond_CancelBattle_fromOpenent(playerCharacter_t* pOpenentPlayer);
	void		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_REQUEST(playerCharacter_t* pPlayer);
	
	void		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_ADMIT(playerCharacter_t* pPlayer);			
	void		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_EXIT(playerCharacter_t* pPlayer);				
	void		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_ITEM(playerCharacter_t* pPlayer);				
	void		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_OK(playerCharacter_t* pPlayer);				
	void		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_CANCEL(playerCharacter_t* pPlayer);			
	void		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM(playerCharacter_t* pPlayer);			
	void		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_SE(playerCharacter_t* pPlayer);				
	void		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_REJECT(playerCharacter_t* pPlayer);			
   	void		recPaketProcces_BATTEL_PRACTICE_GET_CANCEL(void);				
	void		recPaketProcces_BATTLE_PRACTICE_REQUEST_BATTELGENERATE(void);   
	void		recPaketProcces_BATTLE_PRACTICE_GENERATE_RESULT(void);			
	void		recPaketProcces_BATTLE_PRACTICE_BAN(void);						
	void		recPaketProcces_BATTLE_PRACTICE_START(playerCharacter_t* pPlayer);					
	void		recPatketProcces_BATTLE_PRACTICE_POSITION(playerCharacter_t* pPlayer);
	void		recPatketProcces_BATTLE_PRACTICE_FAILADDITEM(playerCharacter_t* pPlayer);
	void		recPatketProcces_BanBattingBattle(void);
	void		recPatketProcces_CancelBattingBattle(void);
private:
	CGonryunPracticeBattle*			m_cpVirtualPracticeBattle;
	CGonryunPractice_Encoder*		m_cpVirtualEncoder;	
	CGonryunPractice_ProcessCtrl*	m_cpVirtualProcessCtrl;
	CGonryunPractice_WaiterCtrl*	m_cpVirtualWaiterCtrl;

};


#endif 
