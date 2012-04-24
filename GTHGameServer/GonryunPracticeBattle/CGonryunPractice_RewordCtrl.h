
#ifndef _CGONRYUNPRACTICE_REWORDCTRL_H_
#define _CGONRYUNPRACTICE_REWORDCTRL_H_


#include <List>
using namespace std;
class CGonryunPractice_Ability;
class CGonryunPractice_RewordItem;
class CGonryunPractice_ProcessCtrl;
class CGonryunPractice_WaiterCtrl;
class CGonryunPracticeBattle;
class CGonryunPractice_ProcessCtrl;


class CGonryunPractice_RewordCtrl					
{
public:
	enum enumLevelValue{
		LEVEL_FIFTEEN = 15,
		LEVEL_THIRTY = 30,
	};
	
private:

	typedef std::list<CGonryunPractice_RewordItem*>	REWORDITEMLIST;
	typedef REWORDITEMLIST::iterator		REWORDITEM_ITOR;
	REWORDITEMLIST							m_rewordItemList;

	typedef std::list<CGonryunPractice_Ability*>	REWORDLIST;
	typedef REWORDLIST::iterator		REWORD_ITOR;
	REWORDLIST								m_rewordList;  

private:
	
	BOOL m_bOK;
	

	CGonryunPractice_ProcessCtrl*			m_cpVirtualProcessCtrl;
	CGonryunPractice_Encoder*				m_cpVirtualEncoder;
public:

	CGonryunPractice_RewordCtrl();	
	virtual~CGonryunPractice_RewordCtrl();

public:
	BOOL									isOK(void);
	void									Set(CGonryunPracticeBattle* pPracticeBattle);
	CGonryunPractice_Ability*				GetReword(const int Idx);
	void									rewordField(playerCharacter_t *puser);		
	REWORDITEMLIST&							GetRewordItemList();
	REWORDLIST&								GetRewordList();
	CGonryunPractice_ProcessCtrl*			GetProcessCtrl();

private:

	void			 GetLootByBattleReword(playerCharacter_t* pPlayer, __int64 exp,__int64 genExp);
	void			 addItemBattleField(void *pPc, int *pParam );	
	BOOL			 addItemBattle(playerCharacter_t *puser);	
	void			 Destroy(void);
	void			 ExpUp_Player(playerCharacter_t* pPlayer,int& exp, int& genExp,int& level,int& genLevel );

};

#endif