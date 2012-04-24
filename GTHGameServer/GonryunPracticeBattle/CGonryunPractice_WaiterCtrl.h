#ifndef _CGONRYUNPRACTICE_WAITERCTRL_H_
#define _CGONRYUNPRACTICE_WAITERCTRL_H_

#include <List>
using namespace std;

class CGonryunPracticeBattle;
class CGonryunPractice_ProcessCtrl;
#include "tagGonryunPractice.h"

class CGonryunPractice_WaiterCtrl {
	
public:	
	enum _define{
		WAIT_MAX_USER_COUNT	= 20,
	};	
	
	typedef std::list<struct tagGPWaitUser>   WAIT_USER;
	typedef WAIT_USER::iterator		        WAIT_ITOR;
	WAIT_USER								m_WaitUserList;

private:
	 
	CGonryunPractice_ProcessCtrl*			m_cpVirtualProcessCtrl;
	BOOL m_bOK;

public:	
	CGonryunPractice_WaiterCtrl();
	virtual ~CGonryunPractice_WaiterCtrl();

public:
	BOOL isOK(void);
	void		Set( CGonryunPracticeBattle* pPracticeBattle);
	void		Insert(const struct tagGPWaitUser& WaitUser);
	void		Delete(playerCharacter_t* pPlayer);
	void		DeleteAll(void);
	BOOL		Find(struct tagGPWaitUser& Waiter,const char* szpFindPlayerName);
	int         GetNum(void);
	playerCharacter_t* GetWaiter(const struct tagGPWaitUser& Waiter) const;

	
};



#endif