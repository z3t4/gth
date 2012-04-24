


#include "..\global.h"
#include "CGonryunPractice_Decoder.h"
#include "CGonryunPracticeBattle.h"
#include "CGonryunPractice_ProcessCtrl.h"
#include "CGonryunPractice_WaiterCtrl.h"
#include "CGonryunPractice_Encoder.h"
#include "..\Tools\CTools.h"
extern CTools* gcpTools;






extern void GTH_DisconnectPlayer(playerCharacter_t* pPlayer,const BOOL bForceQuit);

void CGonryunPractice_Decoder::recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM(playerCharacter_t* pPlayer)
{
	if((TRUE == IsBadWritePtr(pPlayer,sizeof(playerCharacter_t))) || 
       (TRUE == IsBadReadPtr(pPlayer,sizeof(playerCharacter_t)))){

		return;
	}

	
	if((pPlayer->GonryunBattlePractice.MemberShip != playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER) &&
		(pPlayer->GonryunBattlePractice.MemberShip != playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT)){
		
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}
	
	int TargetIdx	= MSG_ReadShort();	
	
	playerCharacter_t* pTargetPlayer= gcpTools->GetPlayerRecordPointer(TargetIdx);	
	if(NULL == pTargetPlayer){		
		GTH_DisconnectPlayer( pPlayer,FALSE );		
		return;
	}

	if(FALSE == pTargetPlayer->GonryunBattlePractice.bBattingAdmitSignal){
		pPlayer->GonryunBattlePractice.bBattingAdmitSignal = TRUE;	
		m_cpVirtualEncoder->SendMessage_BattingTrade_ConfirmBatting(
			pTargetPlayer,
			tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::State_Admit,
			tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::success );		

		return;
	}	


	
  	playerCharacter_t* pLeaderPlayer   = m_cpVirtualWaiterCtrl->GetWaiter(m_cpVirtualProcessCtrl->m_Leader);
	playerCharacter_t* pOpponentPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_cpVirtualProcessCtrl->m_Openent);	
	if( ( NULL == pLeaderPlayer ) && ( NULL == pOpponentPlayer ) ){
		
		m_cpVirtualProcessCtrl->Init();
		return;
	}

	if( NULL == pLeaderPlayer ){
		
		m_cpVirtualProcessCtrl->IrregularQuitBattle(pOpponentPlayer);
		return;
	}

	if(NULL == pOpponentPlayer ){
	
		m_cpVirtualProcessCtrl->IrregularQuitBattle(pLeaderPlayer);
		return;
	}

	{

		enum tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::errorCode Leader_code = 
			m_cpVirtualProcessCtrl->CheckConfirmTradeBatting(pLeaderPlayer,pOpponentPlayer);

		


		if(tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::success != Leader_code){
			m_cpVirtualEncoder->SendMessage_BattingTrade_ConfirmBatting(
				pLeaderPlayer,
				tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::State_Check,
				Leader_code);	


			pLeaderPlayer->GonryunBattlePractice.bBattingOkSignal = FALSE;
			pLeaderPlayer->GonryunBattlePractice.bBattingAdmitSignal = FALSE;
			
			pOpponentPlayer->GonryunBattlePractice.bBattingOkSignal = FALSE;
			pOpponentPlayer->GonryunBattlePractice.bBattingAdmitSignal = FALSE;

			m_cpVirtualEncoder->SendMessage_BattingTrade_CancelBatting( pLeaderPlayer, TRUE );
			m_cpVirtualEncoder->SendMessage_BattingTrade_CancelBatting( pOpponentPlayer,FALSE );
			return;

	
		}

		enum tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::errorCode Opponet_code = 
			m_cpVirtualProcessCtrl->CheckConfirmTradeBatting(pOpponentPlayer,pLeaderPlayer);

		if(tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::success != Opponet_code){
			m_cpVirtualEncoder->SendMessage_BattingTrade_ConfirmBatting(
				pOpponentPlayer,
				tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::State_Check,
				Opponet_code);	

			pOpponentPlayer->GonryunBattlePractice.bBattingOkSignal = FALSE;
			pOpponentPlayer->GonryunBattlePractice.bBattingAdmitSignal = FALSE;
			
			pLeaderPlayer->GonryunBattlePractice.bBattingOkSignal = FALSE;
			pLeaderPlayer->GonryunBattlePractice.bBattingAdmitSignal = FALSE;

			m_cpVirtualEncoder->SendMessage_BattingTrade_CancelBatting( pOpponentPlayer, TRUE );
			m_cpVirtualEncoder->SendMessage_BattingTrade_CancelBatting( pLeaderPlayer,FALSE );
			
			
			return;
		}
	}

	pPlayer->GonryunBattlePractice.bBattingAdmitSignal = TRUE;		

	m_cpVirtualProcessCtrl->SetBattleType(CGonryunPractice_ProcessCtrl::PCB_BATTLE_BATTING);
	m_cpVirtualProcessCtrl->SetStep(CGonryunPractice_ProcessCtrl::STEP_BATTLE_READY);
	m_cpVirtualProcessCtrl->MakeTransPosition(pLeaderPlayer,pOpponentPlayer);
	
	g_logSystem->Write("TransPort_In_Batting: LeaderName = %s Openent = %s WordIdx = %d",
		pLeaderPlayer->name,
		pOpponentPlayer->name,
		pOpponentPlayer->worldIdx);


	
	pLeaderPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount -= 1;
	if(0 > pLeaderPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount) 
		pLeaderPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount = 0;
	
	pOpponentPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount -= 1;
	if(0 > pOpponentPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount) 
		pOpponentPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount = 0;




	int royalty =  g_logic.PCBGonryunCondition.m_iroyalty;
	pLeaderPlayer->curChargeSE = pLeaderPlayer->curChargeSE - royalty;
	if( pLeaderPlayer->curChargeSE < 0 ) pLeaderPlayer->curChargeSE = 0;
	m_cpVirtualEncoder->SendMessageNake(pLeaderPlayer);
	m_cpVirtualEncoder->SendStatus(pLeaderPlayer);
	m_cpVirtualEncoder->SendEffect(pLeaderPlayer,CGonryunPractice_ProcessCtrl::PCB_EFF_READY);
	m_cpVirtualEncoder->SendMessage_BattingTrade_ConfirmBatting( 
		pLeaderPlayer,
		tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::State_None,
		tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::success );


	pOpponentPlayer->curChargeSE=	pOpponentPlayer->curChargeSE-royalty;
	if( pOpponentPlayer->curChargeSE < 0 ) pOpponentPlayer->curChargeSE=0;
	m_cpVirtualEncoder->SendMessageNake(pOpponentPlayer);
	m_cpVirtualEncoder->SendStatus(pOpponentPlayer);
	m_cpVirtualEncoder->SendEffect(pOpponentPlayer,CGonryunPractice_ProcessCtrl::PCB_EFF_READY);
	m_cpVirtualEncoder->SendMessage_BattingTrade_ConfirmBatting( 
		pOpponentPlayer,
		tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::State_None,
		tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::success );


	
	
	int itemIdx = -1;
	item_t* item = NULL;
	
	{
		g_logSystem->Write(
			"Success_recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM: Leader_Name = %s Narke = %d Win = %d Lose = %d Draw = %d", 
			pLeaderPlayer->name,
			pLeaderPlayer->GonryunBattlePractice.iBattingSE,
			pLeaderPlayer->GonryunBattlePractice.Record.iwin,
			pLeaderPlayer->GonryunBattlePractice.Record.idefeat,
			pLeaderPlayer->GonryunBattlePractice.Record.idraw);
		
	
		for(int idx = 0; idx < playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE; idx++ ){
			if( pLeaderPlayer->GonryunBattlePractice.Inventory[idx] < 0 ) continue;
			
			itemIdx = pLeaderPlayer->GonryunBattlePractice.Inventory[idx];
			
			if( (0 > itemIdx) || ( itemIdx >= MAX_NUMBER_OF_OWNITEM) ) continue;
			
			item = &pLeaderPlayer->item[itemIdx];
			if( (0 > item->itemTableIdx)  || ( MAX_NUMBER_OF_ITEM_TABLE <= item->itemTableIdx)  ) continue;
			
			g_logSystem->Write(
				"ConfirmItem: LeaderName = %s ItemName = %s ItemNumber = %d",
				pLeaderPlayer->name,
				item->name,
				item->durability+1);
			
		}
		
	}

	
	{
		g_logSystem->Write(
			"Success_recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM: Openent_Name = %s Narke = %d Win = %d Lose =%d Draw =%d",
			pOpponentPlayer->name,
			pOpponentPlayer->GonryunBattlePractice.iBattingSE,
			pOpponentPlayer->GonryunBattlePractice.Record.iwin,
			pOpponentPlayer->GonryunBattlePractice.Record.idefeat,
			pOpponentPlayer->GonryunBattlePractice.Record.idraw );
		
		for(int idx = 0; idx < playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE; idx++ ){
		
			if( pOpponentPlayer->GonryunBattlePractice.Inventory[idx] < 0 ) continue;
			
			itemIdx = pOpponentPlayer->GonryunBattlePractice.Inventory[idx];
			if( (0 > itemIdx) || ( itemIdx >= MAX_NUMBER_OF_OWNITEM) ) continue;
			
			item = &pOpponentPlayer->item[itemIdx];
			if( (0 > item->itemTableIdx)  || ( MAX_NUMBER_OF_ITEM_TABLE <= item->itemTableIdx)  ) continue;
			
			g_logSystem->Write(
				"ConfirmItem: OpenentName = %s ItemName = %s ItemNumber = %d", 
				pOpponentPlayer->name,
				item->name,
				item->durability+1);
			
		}
	}

}

void CGonryunPractice_Decoder::
recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_OK(playerCharacter_t* pPlayer)
{
	if((TRUE == IsBadWritePtr(pPlayer,sizeof(playerCharacter_t))) || 
	   (TRUE == IsBadReadPtr(pPlayer,sizeof(playerCharacter_t)))) {
		
		return;
	}
	
	int TargetIdx = MSG_ReadShort();
	playerCharacter_t* pTargetPlayer= gcpTools->GetPlayerRecordPointer(TargetIdx);
	
	if(NULL == pTargetPlayer) {
	
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;	
	}

	pPlayer->GonryunBattlePractice.bBattingOkSignal = TRUE;
	m_cpVirtualEncoder->SendMessage_BattingTrade_OkBatting( pPlayer, TRUE );
	m_cpVirtualEncoder->SendMessage_BattingTrade_OkBatting( pTargetPlayer, FALSE );
	
}


void CGonryunPractice_Decoder::
recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_REQUEST(playerCharacter_t* pLeaderPlayer)
{
	
	if((TRUE == IsBadWritePtr(pLeaderPlayer,sizeof(playerCharacter_t))) || 
	   (TRUE == IsBadReadPtr(pLeaderPlayer,sizeof(playerCharacter_t)))) {
		
		return;
	}
	

	if(FALSE == m_cpVirtualProcessCtrl->isBattingPlayActive()){
	
		m_cpVirtualEncoder->SendBattleMsg(pLeaderPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::busy_GonryunBattlePractice);
		return;
	}

	if( playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER != pLeaderPlayer->GonryunBattlePractice.MemberShip){
	
		GTH_DisconnectPlayer(pLeaderPlayer,FALSE);
		return;
	}

	if(pLeaderPlayer->GonryunBattlePractice.MemberShip != playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER){
		
		m_cpVirtualEncoder->SendBattleMsg(pLeaderPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::busy_suggest);
		return;
	}

	char szWaitName[NAMESTRING+1]="";
	strncpy( szWaitName , MSG_ReadString() , NAMESTRING );
    szWaitName[NAMESTRING] = NULL;	

	
	playerCharacter_t* pWaiterPlayer = gcpTools->GetPlayerRecordPointer(szWaitName);
	
	if(NULL == pWaiterPlayer){
		
		m_cpVirtualEncoder->SendBattleMsg(pLeaderPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::unkonwn);		
		return;
	}

	if( playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_WAITER != pWaiterPlayer->GonryunBattlePractice.MemberShip ){
	
		m_cpVirtualEncoder->SendBattleMsg(pLeaderPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::unkonwn);		
		return;
	}

	int differlevel = abs( pLeaderPlayer->level - pWaiterPlayer->level );
	
	if(differlevel > g_logic.PCBGonryunCondition.m_idifferLevel) {
	
		m_cpVirtualEncoder->SendBattleMsg(pLeaderPlayer,
			tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::differLevel,
			g_logic.PCBGonryunCondition.m_idifferLevel);
		return;
	}

	
	pLeaderPlayer->GonryunBattlePractice.bisWaitrespond = TRUE;
	m_cpVirtualEncoder->SendMessage_BattingTrade_RequestBatting( pWaiterPlayer, pLeaderPlayer, 0 );
	m_cpVirtualEncoder->SendMessage_BattingTrade_RequestBatting( pLeaderPlayer,pWaiterPlayer, 1 );

	g_logSystem->Write(
		"Success_recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_REQUEST: LeaderName = %s WaiterName = %s",
		pLeaderPlayer->name,
		pWaiterPlayer->name);

}


void CGonryunPractice_Decoder::
recvRespond_NormalResult_fromOpenent(playerCharacter_t* pPlayer)
{
	if((TRUE == IsBadWritePtr(pPlayer,sizeof(playerCharacter_t))) || 
	   (TRUE == IsBadReadPtr(pPlayer,sizeof(playerCharacter_t)))) {
		
		return;
	}
	
	
	if(pPlayer->GonryunBattlePractice.MemberShip == playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER) {
		
		char szWaiterName[NAMESTRING+1]="";
		strncpy(szWaiterName,MSG_ReadString(),NAMESTRING); szWaiterName[NAMESTRING-1] = NULL;
		playerCharacter_t* pWaiter = gcpTools->GetPlayerRecordPointer(szWaiterName);
		
		if( NULL == pWaiter){
				m_cpVirtualEncoder->SendBattleMsg(pPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::unkonwn);		
							
		} 
		
		else if(pWaiter->GonryunBattlePractice.MemberShip != playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_WAITER){
				GTH_DisconnectPlayer(pPlayer,FALSE);
		}
		
		else{
			m_cpVirtualEncoder->SendMessage_NormalReject_toPlayer( pWaiter );
		}
		

		pPlayer->GonryunBattlePractice.bisWaitrespond = FALSE;
		return;
	}

	if(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_WAITER != pPlayer->GonryunBattlePractice.MemberShip ) {
		
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}

	int LeaderPCIdx = MSG_ReadLong();
	char szLeaderName[NAMESTRING+1]="";
	strncpy(szLeaderName,MSG_ReadString(),NAMESTRING); szLeaderName[NAMESTRING-1] = NULL;

	playerCharacter_t* pLeaderPlayer = gcpTools->GetPlayerRecordPointer(LeaderPCIdx);
	
	if(NULL == pLeaderPlayer){
		m_cpVirtualEncoder->SendBattleMsg(pPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::unkonwn);		
		return;
	}

	if((playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER != 
		pLeaderPlayer->GonryunBattlePractice.MemberShip ) ||
	   (pLeaderPlayer->idx != m_cpVirtualProcessCtrl->m_Leader.pcIdx) ||
	   (0 != strcmp(szLeaderName,pLeaderPlayer->name)) ||
	   (0 != strcmp(szLeaderName,m_cpVirtualProcessCtrl->m_Leader.Name)) ||
	
		(pLeaderPlayer->GonryunBattlePractice.bisWaitrespond != TRUE)){
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}	
	
	BOOL bAccept = (BOOL)MSG_ReadByte();

	if(FALSE == bAccept ){
	
		
		m_cpVirtualEncoder->SendMessage_NormalReject_toPlayer( pLeaderPlayer );
	
		pLeaderPlayer->GonryunBattlePractice.bisWaitrespond = FALSE;
		return;
	}
	
	playerCharacter_t* pOpenentPlayer=pPlayer;
	int royalty = g_logic.PCBGonryunCondition.m_iroyalty;


	
	m_cpVirtualEncoder->SendWaitUserBattleEnd();
	m_cpVirtualEncoder->SendWaitUserBattleEnd(pLeaderPlayer);
	m_cpVirtualWaiterCtrl->DeleteAll();	
	m_cpVirtualProcessCtrl->SetOpenent(pOpenentPlayer);	

	pOpenentPlayer->busyState = BUSY_STATE_BATTLEPRACTICE;
	pOpenentPlayer->curChargeSE = pOpenentPlayer->curChargeSE-royalty;

	
	if(pOpenentPlayer->curChargeSE < 0) pOpenentPlayer->curChargeSE=0;

	m_cpVirtualEncoder->SendMessageNake( pOpenentPlayer );
	pLeaderPlayer->busyState = BUSY_STATE_BATTLEPRACTICE;		
	pLeaderPlayer->curChargeSE  = pLeaderPlayer->curChargeSE-royalty;

	
	if(pLeaderPlayer->curChargeSE < 0) pLeaderPlayer->curChargeSE =0;

	m_cpVirtualEncoder->SendMessageNake(pLeaderPlayer);	
	
	m_cpVirtualProcessCtrl->SetBattleType(CGonryunPractice_ProcessCtrl::PCB_BATTLE_NORMAL);
	m_cpVirtualProcessCtrl->SetStep(CGonryunPractice_ProcessCtrl::Step::STEP_BATTLE_READY);
	
	
	m_cpVirtualProcessCtrl->MakeTransPosition(pLeaderPlayer,pOpenentPlayer);

	
	pLeaderPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount -= 1;
	if(0 > pLeaderPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount) 
		pLeaderPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount = 0;
	
	pOpenentPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount -= 1;
	if(0 > pOpenentPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount) 
		pOpenentPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount = 0;

	g_logSystem->Write("TransPort_In_Generater: LeaderName = %s Openent = %s WordIdx = %d",
		pLeaderPlayer->name,
		pOpenentPlayer->name,
		pOpenentPlayer->worldIdx);
	
	m_cpVirtualEncoder->SendStatus(pLeaderPlayer);
	m_cpVirtualEncoder->SendEffect(pLeaderPlayer,CGonryunPractice_ProcessCtrl::PCB_EFF_READY);
	
	m_cpVirtualEncoder->SendStatus(pOpenentPlayer);
	m_cpVirtualEncoder->SendEffect(pOpenentPlayer,CGonryunPractice_ProcessCtrl::PCB_EFF_READY);
	
	ShowLogInfo("BattlePractic_WaiterCount: %d",m_cpVirtualWaiterCtrl->GetNum());

	g_logSystem->Write( 
		"recvRespond_NormalResult_fromOpenent!!  Leader_Name = %s Openent_Name = %s ",
		pLeaderPlayer->name,
		pOpenentPlayer->name);
	
	return;
}





void CGonryunPractice_Decoder::recvReques_NormalBattle_FromLeader(playerCharacter_t* pLeaderPlayer)
{
	if((TRUE == IsBadWritePtr(pLeaderPlayer,sizeof(playerCharacter_t))) || 
	   (TRUE == IsBadReadPtr(pLeaderPlayer,sizeof(playerCharacter_t)))) {
	
		return;
	}

	
	if( playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER != pLeaderPlayer->GonryunBattlePractice.MemberShip) {
	
		GTH_DisconnectPlayer(pLeaderPlayer,FALSE);
		return;
	}


	
	if(FALSE != pLeaderPlayer->GonryunBattlePractice.bisWaitrespond ){
		m_cpVirtualEncoder->SendBattleMsg(pLeaderPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::busy_suggest);			
		return;
	}	

	struct tagGPWaitUser TempWaiter;
	strncpy( TempWaiter.Name , MSG_ReadString() , NAMESTRING );
	TempWaiter.Name[NAMESTRING-1] = NULL;
	
	
	if(FALSE == m_cpVirtualWaiterCtrl->Find(TempWaiter,TempWaiter.Name)){
		m_cpVirtualEncoder->SendBattleMsg(pLeaderPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::unkonwn);		
		return;
	}	
    
	playerCharacter_t* pWaiterPlayer = m_cpVirtualWaiterCtrl->GetWaiter(TempWaiter);
	if(NULL == pWaiterPlayer ){
		m_cpVirtualEncoder->SendBattleMsg(pLeaderPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::unkonwn);
		return;
	}
	
	
	int differlevel = abs(pLeaderPlayer->level - pWaiterPlayer->level);

	if(differlevel > g_logic.PCBGonryunCondition.m_idifferLevel) {

		m_cpVirtualEncoder->SendBattleMsg(
			pLeaderPlayer,
			tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::differLevel,			
			g_logic.PCBGonryunCondition.m_idifferLevel);		
		
		return;
	}	
	
	
	pLeaderPlayer->GonryunBattlePractice.bisWaitrespond = TRUE;
	m_cpVirtualEncoder->SendReqeust_NomalBattle_toOpenent(
		pLeaderPlayer->idx,
		pLeaderPlayer->name,
		pWaiterPlayer);
	
	g_logSystem->Write( 
		"recvReques_NormalBattle_FromLeader!!  Leader_Name = %s Openent_Name = %s ",
		pLeaderPlayer->name,
		pWaiterPlayer->name);
	
}






void CGonryunPractice_Decoder::
recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_ADMIT(playerCharacter_t* pWaiterPlayer)
{	
	if((TRUE == IsBadWritePtr(pWaiterPlayer,sizeof(playerCharacter_t))) || 
	   (TRUE == IsBadReadPtr(pWaiterPlayer,sizeof(playerCharacter_t)))) {
	
		return;
	}

	playerCharacter_t* pLeaderPlayer=NULL;
	int LeaderIdx=-1;

	if(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_WAITER != pWaiterPlayer->GonryunBattlePractice.MemberShip){
		GTH_DisconnectPlayer(pWaiterPlayer,FALSE);
		return;
	}


	LeaderIdx = MSG_ReadShort();
	pLeaderPlayer= gcpTools->GetPlayerRecordPointer(LeaderIdx);

	if( NULL == pLeaderPlayer ){
		m_cpVirtualEncoder->SendBattleMsg(pWaiterPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::unkonwn);
		return;
	}
	
	if( playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER != pLeaderPlayer->GonryunBattlePractice.MemberShip ){
		GTH_DisconnectPlayer(pWaiterPlayer,FALSE);
		return;
	}	

	
	if(TRUE != pLeaderPlayer->GonryunBattlePractice.bisWaitrespond){
	
	m_cpVirtualEncoder->SendBattleMsg(pWaiterPlayer,tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::unkonwn);
		return;
	}

	
	
  	m_cpVirtualEncoder->SendWaitUserBattleEnd();
	m_cpVirtualEncoder->SendWaitUserBattleEnd(pLeaderPlayer);
	m_cpVirtualWaiterCtrl->DeleteAll();

	playerCharacter_t* pOpenentPlayer = pWaiterPlayer;
	m_cpVirtualProcessCtrl->SetOpenent( pOpenentPlayer );
	m_cpVirtualProcessCtrl->SetStep(CGonryunPractice_ProcessCtrl::Step::STEP_BATTING);
	
	pLeaderPlayer->GonryunBattlePractice.iBattingSE = 0;	
	pLeaderPlayer->GonryunBattlePractice.bBattingAdmitSignal = FALSE;
	pLeaderPlayer->GonryunBattlePractice.bBattingOkSignal = FALSE;
	m_cpVirtualProcessCtrl->InitTradeBatting(pLeaderPlayer );
	m_cpVirtualEncoder->SendMessage_BattingTrade_AdmitBatting( pLeaderPlayer);
	pLeaderPlayer->busyState = BUSY_STATE_BATTLEPRACTICE;

	pOpenentPlayer->GonryunBattlePractice.iBattingSE = 0;
	pOpenentPlayer->GonryunBattlePractice.bBattingAdmitSignal = FALSE;
	pOpenentPlayer->GonryunBattlePractice.bBattingOkSignal = FALSE;
	m_cpVirtualProcessCtrl->InitTradeBatting( pOpenentPlayer );
	m_cpVirtualEncoder->SendMessage_BattingTrade_AdmitBatting( pOpenentPlayer);
	pOpenentPlayer->busyState = BUSY_STATE_BATTLEPRACTICE;
	g_logSystem->Write( 
		"recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_ADMIT!!  Leader_Name = %s Openent_Name = %s ",
		pLeaderPlayer->name,
		pOpenentPlayer->name);
}




CGonryunPractice_Decoder::CGonryunPractice_Decoder()
{
	m_bOK=FALSE;

	m_cpVirtualPracticeBattle = NULL;
	m_cpVirtualEncoder	= NULL;	
	m_cpVirtualProcessCtrl = NULL;
	m_cpVirtualWaiterCtrl =NULL;	

	m_bOK=TRUE;

}

CGonryunPractice_Decoder::~CGonryunPractice_Decoder()
{

}

void CGonryunPractice_Decoder::recvPacket(void)
{
	byte command = MSG_ReadByte();
	switch ( command ){

	
	case BATTLE_PRACTICE_REQUEST_BATTELGENERATE :
		recvReques_NormalBattle_FromLeader(g_curPC);
		break;
	
	case BATTLE_PRACTICE_GENERATE_RESULT :
		recvRespond_NormalResult_fromOpenent(g_curPC);
		break;
	case BATTLE_PRACTICE_BATTINGTRADE_RESULT :
		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_ADMIT(g_curPC);
		break;
	case BATTEL_PRACTICE_GET_CANCEL:
		recvRespond_CancelBattle_fromOpenent(g_curPC);
		break;
	case BATTLE_PRACTICE_BATTINGTRADE_REQUEST:	
		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_REQUEST(g_curPC);		
		break;
	case BATTLE_PRACTICE_BATTINGTRADE_REJECT:
		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_REJECT(g_curPC);		
		break;
	case BATTLE_PRACTICE_BATTINGTRADE_EXIT :
		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_EXIT(g_curPC);
		break;
	case BATTLE_PRACTICE_BATTINGTRADE_OK:			
		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_OK(g_curPC);
		break;
	case BATTLE_PRACTICE_BATTINGTRADE_CANCEL:
		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_CANCEL(g_curPC);
		break;
	case BATTLE_PRACTICE_BATTINGTRADE_CONFIRM:			
		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM(g_curPC);
		break;
	case BATTLE_PRACTICE_BATTINGTRADE_ITEM:		
		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_ITEM(g_curPC);
		break;
	case BATTLE_PRACTICE_BATTINGTRADE_SE:		
		recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_SE(g_curPC);
		break;
	case BATTLE_PRACTICE_BAN:
		recPaketProcces_BATTLE_PRACTICE_BAN();
		break;
	case BATTLE_PRACTICE_START:
		recPaketProcces_BATTLE_PRACTICE_START(g_curPC);
		break;
	case BATTLE_PRACTICE_POSITION:
		recPatketProcces_BATTLE_PRACTICE_POSITION(g_curPC);
		break;
	case BATTLE_PRACTICE_BAN_BATTING:
		recPatketProcces_BanBattingBattle();
		break;
	case BATTLE_PRACTICE_CANCEL_BATTING:
		recPatketProcces_CancelBattingBattle();
		break;
	case BATTLE_PRACTICE_FAILADDITEM:
		recPatketProcces_BATTLE_PRACTICE_FAILADDITEM(g_curPC);
		break;
	}
}

void CGonryunPractice_Decoder::recPatketProcces_BATTLE_PRACTICE_FAILADDITEM(playerCharacter_t* pPlayer)
{
	GTH_DisconnectPlayer(pPlayer,FALSE);	
}


BOOL CGonryunPractice_Decoder::isOK(void)
{
	return m_bOK;
}

void CGonryunPractice_Decoder::Set(CGonryunPracticeBattle* pPracticeBattle)
{
	m_cpVirtualPracticeBattle	= pPracticeBattle;
	m_cpVirtualProcessCtrl		= pPracticeBattle->GetProcessCtrl();
	m_cpVirtualWaiterCtrl		= pPracticeBattle->GetWaiterCtrl();
	m_cpVirtualEncoder			= pPracticeBattle->GetEncoder();
}

void CGonryunPractice_Decoder::
recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_ITEM(playerCharacter_t* pPlayer)
{
	if((TRUE == IsBadWritePtr(pPlayer,sizeof(playerCharacter_t))) ||
	   (TRUE == IsBadReadPtr(pPlayer,sizeof(playerCharacter_t)))){
		return;
	}
	
	item_t* pMouseItem = NULL;
	item_t* pPlayerArrayItem = NULL;
	int	pos = -1;	
	int playerArrayItemIdx_forAddItem = -1;
	int GBPInventoryIdx = 0;
	BOOL bisExchange = TRUE;
	playerCharacter_t* pTargetPlayer = NULL;	

	int	TargetIdx = MSG_ReadShort();
	pTargetPlayer = gcpTools->GetPlayerRecordPointer(TargetIdx);

	if(NULL == pTargetPlayer) goto unknown_error_player_disconnect;
	
	pos = MSG_ReadByte();
	
	if( ( 0 > pos ) || ( playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE <= pos ) ) goto unknown_error_player_disconnect;
	
	switch(pPlayer->GonryunBattlePractice.MemberShip){
	
	case playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER:
			if(pTargetPlayer->GonryunBattlePractice.MemberShip != 
			  playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT) 
				goto unknown_error_player_disconnect;
			break;
	
	case playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT:
			if(pTargetPlayer->GonryunBattlePractice.MemberShip != 
			  playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER) 
				goto unknown_error_player_disconnect;
			break;
	
	default:
			goto unknown_error_player_disconnect;
	}

	
	if( ( pPlayer->mouseInventory >= 0 ) && 
		( pPlayer->GonryunBattlePractice.Inventory[pos] >= 0 ) ) {
		
		pMouseItem = gcpTools->GetMyItem(pPlayer,pPlayer->mouseInventory);
		if(NULL == pMouseItem) goto unknown_error_player_disconnect;
		
		if( TRUE == gcpTools->IsStackItem(pMouseItem) ) {
			for( GBPInventoryIdx = 0; GBPInventoryIdx < playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE; GBPInventoryIdx++){
				
				if( 0 > pPlayer->GonryunBattlePractice.Inventory[GBPInventoryIdx] )continue;
				
				pPlayerArrayItem = gcpTools->GetMyItem(pPlayer,pPlayer->GonryunBattlePractice.Inventory[GBPInventoryIdx]);
				
				if( NULL == pPlayerArrayItem ) goto unknown_error_player_disconnect;
				
				if( pPlayerArrayItem->itemTableIdx != pMouseItem->itemTableIdx ) continue;
				bisExchange = FALSE;
				break;
			
			}	
		}
	
		if( TRUE == bisExchange ){
			
			int tempItemArrayIdx = pPlayer->mouseInventory;
			pPlayer->mouseInventory = pPlayer->GonryunBattlePractice.Inventory[pos];
			pPlayer->GonryunBattlePractice.Inventory[pos] = tempItemArrayIdx;		
			
			pPlayerArrayItem = gcpTools->GetMyItem(pPlayer,tempItemArrayIdx);
			if(NULL == pPlayerArrayItem) goto unknown_error_player_disconnect;
			
			m_cpVirtualEncoder->SendMessage_BattingTrade_ItemBatting(
				pTargetPlayer,
				FALSE,
				tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_ITEM::TYPE_EXCHANGE,
				pos,
				pPlayerArrayItem);
			
			m_cpVirtualEncoder->SendMessage_BattingTrade_ItemBatting(
				pPlayer,
				TRUE,
				tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_ITEM::TYPE_EXCHANGE,
				pos,
				pPlayerArrayItem);
			return;
		}
	
	}

	
	if( pPlayer->mouseInventory >= 0 ) {
		
		pMouseItem = gcpTools->GetMyItem(pPlayer,pPlayer->mouseInventory);
		if(NULL == pMouseItem) goto unknown_error_player_disconnect;
		
		if( TRUE == gcpTools->IsStackItem(pMouseItem) ) {
			for( GBPInventoryIdx = 0; GBPInventoryIdx < playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE; GBPInventoryIdx++){
				
				if( 0 > pPlayer->GonryunBattlePractice.Inventory[GBPInventoryIdx] )continue;
				
				pPlayerArrayItem = gcpTools->GetMyItem(pPlayer,pPlayer->GonryunBattlePractice.Inventory[GBPInventoryIdx]);
				if( NULL == pPlayerArrayItem ) goto unknown_error_player_disconnect;
				if( pPlayerArrayItem->itemTableIdx != pMouseItem->itemTableIdx ) continue;
				
				pPlayerArrayItem->durability += pMouseItem->durability+1;
				if(pPlayerArrayItem->durability >= MAX_COUNT_STACK_ITEM ) pPlayerArrayItem->durability = MAX_COUNT_STACK_ITEM - 1;
				g_DBGameServer->SaveItem( pPlayer, pPlayerArrayItem,pPlayer->GonryunBattlePractice.Inventory[GBPInventoryIdx]);

				GTH_DeleteMyItem( pPlayer, pMouseItem );
				g_DBGameServer->RemoveItem( pPlayer, pPlayer->mouseInventory );
			
				m_cpVirtualEncoder->SendMessage_BattingTrade_ItemBatting(
					pTargetPlayer,
					FALSE,
					tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_ITEM::TYPE_ADD,
					GBPInventoryIdx,
					pPlayerArrayItem);
				
				m_cpVirtualEncoder->SendMessage_BattingTrade_ItemBatting(
					pPlayer,
					TRUE,
					tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_ITEM::TYPE_STACK_ADD,
					GBPInventoryIdx,
					pPlayerArrayItem,
					pPlayer->mouseInventory);
				
				pPlayer->mouseInventory=-1;	
			    
				return;
			}	
		}

		for( GBPInventoryIdx = 0; GBPInventoryIdx < playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE; GBPInventoryIdx++){
			
			if( 0 <= pPlayer->GonryunBattlePractice.Inventory[GBPInventoryIdx] )continue;
			
			pPlayer->GonryunBattlePractice.Inventory[GBPInventoryIdx]  = pPlayer->mouseInventory;
			pPlayer->mouseInventory=-1;
		
			pPlayerArrayItem = gcpTools->GetMyItem(pPlayer,pPlayer->GonryunBattlePractice.Inventory[GBPInventoryIdx]);
			if( NULL == pPlayerArrayItem ) goto unknown_error_player_disconnect;
			
			m_cpVirtualEncoder->SendMessage_BattingTrade_ItemBatting(
				pTargetPlayer,
				FALSE,
				tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_ITEM::TYPE_ADD,
				GBPInventoryIdx,
				pPlayerArrayItem);
			
			m_cpVirtualEncoder->SendMessage_BattingTrade_ItemBatting(
				pPlayer,
				TRUE,
				tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_ITEM::TYPE_ADD,
				GBPInventoryIdx,
				pPlayerArrayItem);
			return;
		}
		goto unknown_error_player_disconnect;
	}


	
	if( pPlayer->mouseInventory < 0 && 
		pPlayer->GonryunBattlePractice.Inventory[pos] >= 0 ) {
		
		pPlayerArrayItem = gcpTools->GetMyItem(pPlayer,pPlayer->GonryunBattlePractice.Inventory[pos]);
		if(NULL == pPlayerArrayItem) goto unknown_error_player_disconnect;
		
		
		pPlayer->mouseInventory = pPlayer->GonryunBattlePractice.Inventory[pos];
		pPlayer->GonryunBattlePractice.Inventory[pos]=-1;
		m_cpVirtualEncoder->SendMessage_BattingTrade_ItemBatting(
			pTargetPlayer,
			FALSE,
			tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_ITEM::TYPE_DELETE,
			pos,
			pPlayerArrayItem);

		m_cpVirtualEncoder->SendMessage_BattingTrade_ItemBatting(pPlayer,
			TRUE,
			tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_ITEM::TYPE_DELETE,
			pos,
			pPlayerArrayItem);
		
		return;
	}

unknown_error_player_disconnect:
	GTH_DisconnectPlayer(pPlayer,FALSE);
	return;
}

void CGonryunPractice_Decoder::recvRespond_CancelBattle_fromOpenent(playerCharacter_t* pPlayer)
{
	if(TRUE == IsBadWritePtr(pPlayer,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pPlayer,sizeof(playerCharacter_t))) {
	
		return;
	}
	
	if((playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE 
		== pPlayer->GonryunBattlePractice.MemberShip))  {
	
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}

	m_cpVirtualProcessCtrl->IrregularQuitBattle(pPlayer);
}

void CGonryunPractice_Decoder::recPatketProcces_BATTLE_PRACTICE_POSITION(playerCharacter_t* pPlayer)
{
	if(TRUE == IsBadWritePtr(pPlayer,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pPlayer,sizeof(playerCharacter_t))){
	
		return;
	}

	vec3_t position;
	position[0]=MSG_ReadFloat();
	position[1]=MSG_ReadFloat();
	position[2]=MSG_ReadFloat();
	m_cpVirtualProcessCtrl->MakeTransPort(pPlayer,position);
}

void CGonryunPractice_Decoder::recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_EXIT(playerCharacter_t* pPlayer)
{
	if(TRUE == IsBadWritePtr(pPlayer,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pPlayer,sizeof(playerCharacter_t))) {
	
		return;
	}
		
	if((pPlayer->GonryunBattlePractice.MemberShip != playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER) &&
		(pPlayer->GonryunBattlePractice.MemberShip != playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT)){

		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}

	int TargetIdx = MSG_ReadShort();
	playerCharacter_t* pTargetPlayer = gcpTools->GetPlayerRecordPointer( TargetIdx );
	if( NULL == pTargetPlayer ) {
	
		m_cpVirtualEncoder->SendBattleMsg(
			pPlayer,
			tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::unkonwn);		
		return;
	}

	if((pTargetPlayer->GonryunBattlePractice.MemberShip != playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER) &&
		(pTargetPlayer->GonryunBattlePractice.MemberShip != playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT)){
		
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}

	m_cpVirtualProcessCtrl->IrregularQuitBattle(pPlayer);
}

void CGonryunPractice_Decoder::
recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_REJECT(playerCharacter_t* pPlayer)
{
	if(TRUE == IsBadWritePtr(pPlayer,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pPlayer,sizeof(playerCharacter_t))) {
	
		return;
	}
	
	if((playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER != pPlayer->GonryunBattlePractice.MemberShip ) &&
		(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_WAITER != pPlayer->GonryunBattlePractice.MemberShip )){
	
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}

	int TargetIdx = MSG_ReadShort();
	playerCharacter_t* pTargetPlayer = gcpTools->GetPlayerRecordPointer(TargetIdx);

	if( NULL == pTargetPlayer ) {
		
		m_cpVirtualEncoder->SendBattleMsg(
			pPlayer,
			tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::unkonwn);
		return;
	}
	

	if((playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER != pTargetPlayer->GonryunBattlePractice.MemberShip ) &&
		(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_WAITER != pTargetPlayer->GonryunBattlePractice.MemberShip )) {
		
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}

	
	
	
	if( playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER
		== pPlayer->GonryunBattlePractice.MemberShip ){
	
		
		pPlayer->GonryunBattlePractice.bisWaitrespond = FALSE;
		m_cpVirtualEncoder->SendMessage_BattingTrade_RejectBatting( pPlayer, TRUE );
		m_cpVirtualEncoder->SendMessage_BattingTrade_RejectBatting( pTargetPlayer, FALSE );
	}
	
	else if( playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER
		== pTargetPlayer->GonryunBattlePractice.MemberShip ) {
		
		
		pTargetPlayer->GonryunBattlePractice.bisWaitrespond = FALSE;
		m_cpVirtualEncoder->SendMessage_BattingTrade_RejectBatting( pPlayer, FALSE );
		m_cpVirtualEncoder->SendMessage_BattingTrade_RejectBatting( pTargetPlayer, TRUE );
	}
	
	g_logSystem->Write(
		"Success_recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_REQUEST: RejectName = %s beRejectName = %s", 
		pPlayer->name,
		pTargetPlayer->name);
}

void CGonryunPractice_Decoder::
recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_SE(playerCharacter_t* pPlayer)
{
	if(TRUE == IsBadWritePtr(pPlayer,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pPlayer,sizeof(playerCharacter_t))) {
	
		return;
	}

	if( (playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER != pPlayer->GonryunBattlePractice.MemberShip) &&
	(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT != pPlayer->GonryunBattlePractice.MemberShip) ) {
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}

	int TargetIdx = MSG_ReadShort();
	playerCharacter_t* pTargetPlayer = gcpTools->GetPlayerRecordPointer(TargetIdx);	
	if(NULL == pTargetPlayer){	
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}

	if( (playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER != pTargetPlayer->GonryunBattlePractice.MemberShip) &&
	(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT != pTargetPlayer->GonryunBattlePractice.MemberShip) ) {
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}

	int narke = MSG_ReadLong();	

	if( narke > (pPlayer->curChargeSE - g_logic.PCBGonryunCondition.m_iroyalty)){
		m_cpVirtualEncoder->SendMessage_BattingTrade_SEBatting( 
			pPlayer,
			tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::ERROR_NotEnough_Nake,
			TRUE,
			narke );		
		return;
	}

	
	if( g_logic.PCBGonryunCondition.m_ibattingRoyalty < narke){
	
		m_cpVirtualEncoder->SendMessage_BattingTrade_SEBatting( 
			pPlayer,
			tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::ERROR_Over_BattingNake,
			TRUE,
			narke );
		
		return;
	}

	pPlayer->GonryunBattlePractice.iBattingSE = narke;
	m_cpVirtualEncoder->SendMessage_BattingTrade_SEBatting( 
		pPlayer,
		tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::success,
		TRUE,
		narke );
	
	m_cpVirtualEncoder->SendMessage_BattingTrade_SEBatting( 
		pTargetPlayer, 
		tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::success,
		FALSE,
		narke );
}

void CGonryunPractice_Decoder::recPaketProcces_BATTLE_PRACTICE_BAN()
{
	m_cpVirtualProcessCtrl->SetActive(FALSE);
}

void CGonryunPractice_Decoder::recPaketProcces_BATTLE_PRACTICE_START(playerCharacter_t* pPlayer)			
{
   if(TRUE == IsBadWritePtr(pPlayer,sizeof(playerCharacter_t)) 
		|| TRUE == IsBadReadPtr(pPlayer,sizeof(playerCharacter_t))) {
		
	   return;
	}

	int minLevel=MSG_ReadLong();
	
	if( ( 0 >= minLevel ) || ( 99 < minLevel ) ){
	
		GTH_DisconnectPlayer( pPlayer,FALSE );		
		return;
	}
	
	int maxLevel=MSG_ReadLong();
	
	if( ( 0 >= maxLevel ) || ( 99 < maxLevel ) ) {
	
		GTH_DisconnectPlayer( pPlayer,FALSE );		
		return;
	}
	
	if( minLevel >= maxLevel ) return;
	
	m_cpVirtualProcessCtrl->SetActive(TRUE);
	
	switch (pPlayer->worldIdx)
	{
	
	case CGonryunPractice_ProcessCtrl::ARAYANWORLDMAP:					
		{
			g_logic.PCBGonryunCondition.m_iminiLevel	= minLevel;
			g_logic.PCBGonryunCondition.m_imaxLevel		= maxLevel;
		}
		break;

	case CGonryunPractice_ProcessCtrl::SYENWORDMAP:					
		{
			g_logic.PCBGonryunCondition.m_isyenminilevel = minLevel;
			g_logic.PCBGonryunCondition.m_isyenmaxlevel	 = maxLevel;
		}
		break;
	}
}





void CGonryunPractice_Decoder::
recPaketProcces_BATTLE_PRACTICE_BATTINGTRADE_CANCEL(playerCharacter_t* pPlayer)
{
	if((TRUE == IsBadWritePtr(pPlayer,sizeof(playerCharacter_t))) ||
	   (TRUE == IsBadReadPtr(pPlayer,sizeof(playerCharacter_t)))) {
		
		return;
	}

	if( (playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER != pPlayer->GonryunBattlePractice.MemberShip) &&
	(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT != pPlayer->GonryunBattlePractice.MemberShip) ) {
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}

	int Tagetidx = MSG_ReadShort();
	playerCharacter_t* pTargetPlayer= gcpTools->GetPlayerRecordPointer(Tagetidx);

	if(NULL == pTargetPlayer){
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}
	
	if( (playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER != pTargetPlayer->GonryunBattlePractice.MemberShip) &&
	(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT != pTargetPlayer->GonryunBattlePractice.MemberShip) ) {
		GTH_DisconnectPlayer(pPlayer,FALSE);
		return;
	}

	pPlayer->GonryunBattlePractice.bBattingOkSignal = FALSE;
	pPlayer->GonryunBattlePractice.bBattingAdmitSignal = FALSE;
	
	pTargetPlayer->GonryunBattlePractice.bBattingOkSignal = FALSE;
	pTargetPlayer->GonryunBattlePractice.bBattingAdmitSignal = FALSE;

	m_cpVirtualEncoder->SendMessage_BattingTrade_CancelBatting( pPlayer, TRUE );
	m_cpVirtualEncoder->SendMessage_BattingTrade_CancelBatting( pTargetPlayer,FALSE );
}


void CGonryunPractice_Decoder::
recPatketProcces_BanBattingBattle(void)
{
	m_cpVirtualProcessCtrl->SetBattingBattle(FALSE);
}

void CGonryunPractice_Decoder::
recPatketProcces_CancelBattingBattle(void)
{
	m_cpVirtualProcessCtrl->SetBattingBattle(TRUE);
}

