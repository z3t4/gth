#include "..\global.h"
#include "CGonryunPracticeBattle.h"
#include "CGonryunPractice_ProcessCtrl.h"
#include "CGonryunPractice_RewordCtrl.h"
#include "CGonryunPractice_WaiterCtrl.h"
#include "CGonryunPractice_Decoder.h"
#include "CGonryunPractice_Encoder.h"
#include "..\Tools\CTools.h"
extern CTools* gcpTools;

extern void GTH_DisconnectPlayer(playerCharacter_t* pPlayer,const BOOL bForceQuit);

CGonryunPracticeBattle::CGonryunPracticeBattle()
{	
	m_bOK=FALSE;
	
	m_cpWaiterCtrl=NULL;
	m_cpRewordCtrl=NULL;	
	m_cpDecoder	 =NULL;	
	m_cpEncoder	 = NULL;	
	m_cpProcessCtrl	=NULL;
	if(FALSE == Init()) return;

	m_bOK=TRUE;
}

CGonryunPracticeBattle::~CGonryunPracticeBattle()
{
	if( NULL != m_cpProcessCtrl){    delete m_cpProcessCtrl;  m_cpProcessCtrl = NULL;}	

	if( NULL != m_cpWaiterCtrl){		delete m_cpWaiterCtrl;	m_cpWaiterCtrl = NULL;	}	

	if( NULL != m_cpRewordCtrl){		delete m_cpRewordCtrl; 	m_cpRewordCtrl = NULL;	}

	if( NULL != m_cpDecoder){		delete m_cpDecoder;		m_cpDecoder = NULL;	}

	if( NULL != m_cpEncoder){		delete m_cpEncoder;		m_cpEncoder = NULL;	}
}


BOOL CGonryunPracticeBattle::Init(void)
{
	m_cpWaiterCtrl	= new CGonryunPractice_WaiterCtrl();	
	if(NULL  == m_cpWaiterCtrl) return FALSE;
	if(FALSE == m_cpWaiterCtrl->isOK()){ delete m_cpWaiterCtrl; m_cpWaiterCtrl=NULL; return FALSE; }

	m_cpRewordCtrl	= new CGonryunPractice_RewordCtrl();
	if(NULL == m_cpRewordCtrl) return FALSE;
	if(FALSE == m_cpRewordCtrl->isOK()){ delete m_cpRewordCtrl; m_cpRewordCtrl=NULL; return FALSE; }
	
	m_cpDecoder		= new CGonryunPractice_Decoder();
	if(NULL == m_cpDecoder) return FALSE;
	if(FALSE == m_cpDecoder->isOK()){ delete m_cpDecoder; m_cpDecoder=NULL; return FALSE;}

	m_cpEncoder		= new CGonryunPractice_Encoder();
	if(NULL== m_cpEncoder) return FALSE;
	if(FALSE == m_cpEncoder->isOK()){ delete m_cpEncoder; m_cpEncoder=NULL; return FALSE; }

	m_cpProcessCtrl	= new CGonryunPractice_ProcessCtrl();
	if(NULL == m_cpProcessCtrl) return FALSE;
	if(FALSE == m_cpProcessCtrl->isOK()){ delete m_cpProcessCtrl; m_cpProcessCtrl=NULL; return FALSE; }
	
	m_cpWaiterCtrl->Set(this);
	m_cpRewordCtrl->Set(this);
	m_cpDecoder->Set(this);
	m_cpEncoder->Set(this);
	m_cpProcessCtrl->Set(this);

	return TRUE;
}

CGonryunPractice_RewordCtrl*  CGonryunPracticeBattle::GetRewordCtrl(void)
{
	return m_cpRewordCtrl;
}

CGonryunPractice_ProcessCtrl* CGonryunPracticeBattle::GetProcessCtrl(void)	
{
	return m_cpProcessCtrl;
}

CGonryunPractice_WaiterCtrl* CGonryunPracticeBattle::GetWaiterCtrl(void)
{
	return m_cpWaiterCtrl;
}

CGonryunPractice_Encoder* CGonryunPracticeBattle::GetEncoder(void)
{
	return m_cpEncoder;
}

CGonryunPractice_Decoder* CGonryunPracticeBattle::GetDecoder(void)
{
	return m_cpDecoder;
}


int CGonryunPracticeBattle::NPCClick(playerCharacter_t* pPlayer)
{	
	if(FALSE == m_cpProcessCtrl->GetActive()){
		m_cpEncoder->SendBattleMsg(pPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::gonryun_NotUse);		
		return 0;
	}

	switch(m_cpProcessCtrl->GetStep()){
	
	case CGonryunPractice_ProcessCtrl::STEP_NONE: 
			break;

		case CGonryunPractice_ProcessCtrl::STEP_SUGGUEST:
			{
				int waitercount = m_cpWaiterCtrl->GetNum()+1;
				if( waitercount >= CGonryunPractice_ProcessCtrl::MAX_WAIT_USER ){
					m_cpEncoder->SendBattleMsg(
						pPlayer,
						tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::overflow_user_number);
					return 0;
				}				
			}
			break;

		default:
			m_cpEncoder->SendBattleMsg(pPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::gonryun_NotUse);
			return 0;
	}	
	
	
	switch( pPlayer->worldIdx ){
	
	case CGonryunPractice_ProcessCtrl::ARAYANWORLDMAP:
		
		if((pPlayer->level < g_logic.PCBGonryunCondition.m_iminiLevel) ||			
			   (pPlayer->level > g_logic.PCBGonryunCondition.m_imaxLevel)){

				m_cpEncoder->SendBattleMsg(pPlayer,
					tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::missmatch_user_level,
						g_logic.PCBGonryunCondition.m_iminiLevel,
						g_logic.PCBGonryunCondition.m_imaxLevel);
		
				return 0;
			}			
			break;

	case CGonryunPractice_ProcessCtrl::SYENWORDMAP:
		
		if( (pPlayer->level < g_logic.PCBGonryunCondition.m_isyenminilevel ) ||
			(pPlayer->level > g_logic.PCBGonryunCondition.m_isyenmaxlevel)){	

			m_cpEncoder->SendBattleMsg(pPlayer,
				tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::missmatch_user_level,
				g_logic.PCBGonryunCondition.m_isyenminilevel,
				g_logic.PCBGonryunCondition.m_isyenmaxlevel);	
			return 0;			
		}
	
		break;

	default:
		GTH_DisconnectPlayer(pPlayer,FALSE);		
		return 0;
	}



	
    if( pPlayer->curChargeSE < g_logic.PCBGonryunCondition.m_iroyalty ) {
	
		m_cpEncoder->SendBattleMsg(
			pPlayer,
			tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::not_enoughf_nak,				
			g_logic.PCBGonryunCondition.m_iroyalty);
	
		return 0;
    }


	time_t		lCurrentTime;
	struct tm	CurrentDate_tm, SaveDate_tm;
	
	time(&lCurrentTime);
	CurrentDate_tm	= *localtime(&lCurrentTime);
	SaveDate_tm		= *localtime(&pPlayer->PremiumInfo.GonyounPracticeBattle.lDateV2);


	if( CurrentDate_tm.tm_year != SaveDate_tm.tm_year ||
	    CurrentDate_tm.tm_mon  != SaveDate_tm.tm_mon  ||
	    CurrentDate_tm.tm_mday != SaveDate_tm.tm_mday) {

		pPlayer->PremiumInfo.GonyounPracticeBattle.lDateV2 = lCurrentTime;
		pPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount =
			pPlayer->PremiumInfo.GonyounPracticeBattle.iMaxCount;
	}
	

	if( 0 >= pPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount ){

		m_cpEncoder->SendBattleMsg(
			pPlayer, 
			tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::overflow_oneday_use_count,
			pPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount);
		return 0;
	}

	if(FALSE == m_cpProcessCtrl->SuggestPlayer(pPlayer)){
	
		m_cpEncoder->SendBattleMsg(
			pPlayer,
			tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::unkonwn);
		return 0;			
	}
	
	
	m_cpEncoder->SendMessage_Open(pPlayer);
	return 1;			
}


