#ifndef _CGONRYUNPRACTICEBATTLE_H_
#define _CGONRYUNPRACTICEBATTLE_H_

class	CGonryunPractice_ProcessCtrl;
class	CGonryunPractice_WaiterCtrl;
class	CGonryunPractice_RewordCtrl;
class	CGonryunPractice_Decoder;
class   CGonryunPractice_Encoder;

class CGonryunPracticeBattle{

private:
	BOOL m_bOK;
	CGonryunPractice_ProcessCtrl*	m_cpProcessCtrl;
	CGonryunPractice_WaiterCtrl*	m_cpWaiterCtrl;
	CGonryunPractice_RewordCtrl*	m_cpRewordCtrl;
	CGonryunPractice_Decoder*		m_cpDecoder;
	CGonryunPractice_Encoder*		m_cpEncoder;
private:
	BOOL Init(void);
	
public:
	int NPCClick(playerCharacter_t* pPlayer);

	CGonryunPractice_ProcessCtrl* GetProcessCtrl(void);
	CGonryunPractice_WaiterCtrl*  GetWaiterCtrl(void);
	CGonryunPractice_Encoder*     GetEncoder(void);
	CGonryunPractice_RewordCtrl*  GetRewordCtrl(void);
	CGonryunPractice_Decoder*	  GetDecoder(void);
	
public:		
	CGonryunPracticeBattle();
	virtual ~CGonryunPracticeBattle();
	
};

#endif