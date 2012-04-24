
#include "..\global.h"
#include "CGonryunPractice_WaiterCtrl.h"
#include "CGonryunPracticeBattle.h"
#include "CGonryunPractice_ProcessCtrl.h"
#include "..\Tools\CTools.h"
extern CTools* gcpTools;



CGonryunPractice_WaiterCtrl::CGonryunPractice_WaiterCtrl()
{
	m_bOK=FALSE;	
	
	m_cpVirtualProcessCtrl = NULL;	

	m_bOK=TRUE;
}


CGonryunPractice_WaiterCtrl::~CGonryunPractice_WaiterCtrl()
{

}

BOOL CGonryunPractice_WaiterCtrl::isOK(void)
{
	return m_bOK;
}

void CGonryunPractice_WaiterCtrl::DeleteAll(void)
{	
	WAIT_ITOR itor;
	struct tagGPWaitUser	tempWaiter;
	playerCharacter_t* pPlayer = NULL;

	for (itor=m_WaitUserList.begin(); itor != m_WaitUserList.end(); itor++){

		tempWaiter	= *itor;
		pPlayer = GetWaiter(tempWaiter);
	
		if( NULL == pPlayer ) 	continue;
	
		pPlayer->GonryunBattlePractice.bisWaitrespond = FALSE;
		pPlayer->GonryunBattlePractice.MemberShip = playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE;
		pPlayer->busyState = BUSY_STATE_NONE;
	}

	m_WaitUserList.clear();
}


void CGonryunPractice_WaiterCtrl::Delete(playerCharacter_t* pPlayer)
{
	WAIT_ITOR itor;
	struct tagGPWaitUser	tempWaiter;
	playerCharacter_t* pComparePlayer = NULL;
	for (itor=m_WaitUserList.begin();itor != m_WaitUserList.end(); ){
	
		tempWaiter	= *itor;
		pComparePlayer = GetWaiter(tempWaiter);
	
		if( (NULL == pComparePlayer) || (pPlayer != pComparePlayer)){
		
			itor++;
			continue;
		}
	
		m_WaitUserList.erase(itor++);
		pPlayer->GonryunBattlePractice.bisWaitrespond = FALSE;
		pPlayer->busyState = BUSY_STATE_NONE;
		break;
	}
}

void CGonryunPractice_WaiterCtrl::Insert(const struct tagGPWaitUser& WaitUser)
{	
	m_WaitUserList.push_back(WaitUser);
}

void CGonryunPractice_WaiterCtrl::Set( CGonryunPracticeBattle* pPracticeBattle)
{

	m_cpVirtualProcessCtrl	  = pPracticeBattle->GetProcessCtrl();
}

playerCharacter_t* CGonryunPractice_WaiterCtrl::GetWaiter(const struct tagGPWaitUser& Waiter)const
{
	playerCharacter_t* pPlayer= gcpTools->GetPlayerRecordPointer(Waiter.pcIdx);
	if(NULL == pPlayer) return NULL;
	if(NULL != strncmp(Waiter.Name,pPlayer->name,NAMESTRING)) return NULL;
	if(NULL != strncmp(Waiter.ID,pPlayer->userID,IDSTRING)) return NULL;

	return pPlayer;
}

int CGonryunPractice_WaiterCtrl::GetNum(void)
{
	return m_WaitUserList.size();
}

BOOL CGonryunPractice_WaiterCtrl::Find(struct tagGPWaitUser& Waiter,const char* szpFindPlayerName)
{
	WAIT_ITOR	itor;
	struct tagGPWaitUser tempWaiter;
	
	for (itor=m_WaitUserList.begin(); itor != m_WaitUserList.end(); itor++){
		
		tempWaiter	= (*itor);		
	
		if(0 == strncmp(szpFindPlayerName,tempWaiter.Name,NAMESTRING)){
			Waiter=tempWaiter;
			return TRUE;
		}
	}

	return FALSE;
}








