#include "..\global.h"
#include "CGonryunPractice_ProcessCtrl.h"
#include "CGonryunPracticeBattle.h"
#include "CGonryunPractice_WaiterCtrl.h"
#include "CGonryunPractice_RewordCtrl.h"
#include "CGonryunPractice_Encoder.h"

#include "..\Tools\CTools.h"
#include "../Tools/CTempPlayerInventoryX.h"
extern CTools* gcpTools;
extern void GTH_DisconnectPlayer(playerCharacter_t* pPlayer,const BOOL bForceQuit);

#include "..\GolryunBattle\CGolryunBattle.h"
extern CGolryunBattle* gcpGolryunBattle;


void CGonryunPractice_ProcessCtrl::
GiveBattingWinner(playerCharacter_t* pwinor,playerCharacter_t* pdefeator)
{
	
	tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::errorCode Code = 
		CheckConfirmTradeBatting( pwinor,pdefeator);

	if(Code == tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::success){
		ConfirmTradeBattingWinerGive(pwinor,pdefeator);	
		m_cpVirtualEncoder->SendMessage_BattingTrade_ConfirmBatting( 
			pwinor,			
			tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::State_Win,
			Code);

		m_cpVirtualEncoder->SendMessage_BattingTrade_ConfirmBatting(pdefeator,
			tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::State_Defeate,
			Code);	

		InitTradeBatting( pwinor );
		InitTradeBatting( pdefeator);
		g_logSystem->Write("Success GiveBatting Name = %s", pwinor->name);

	} else{
		
		
		BattingRestore(pwinor);
		BattingRestore(pdefeator);
		m_cpVirtualEncoder->SendMessage_BattingTrade_ConfirmBatting( 
			pwinor,
			tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::State_Fail, Code);
		m_cpVirtualEncoder->SendMessage_BattingTrade_ConfirmBatting(
			pdefeator,
			tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::State_Fail, 
			Code);	
	
		g_logSystem->Write("NotGiveBatting Name = %s Code = %d", pwinor->name,Code);
	}

	GTH_PC_Calculate( pwinor, PCINFO_BASIC );
	GTH_PC_Calculate(pdefeator, PCINFO_BASIC );
}

CGonryunPractice_ProcessCtrl::CGonryunPractice_ProcessCtrl()
{
	m_bOK=FALSE;

	m_cpVirtualWaiterCtrl	= NULL;
	m_cpVirtualRewordCtrl	= NULL;
	m_cpVirtualEncoder		= NULL;
	
	m_Leader.pcIdx=-1;
	m_Openent.pcIdx=-1;
	m_Winner.pcIdx=-1;
	m_Loser.pcIdx=-1;
	
	m_BattleType = PCB_BATTLE_NONE;
	m_Step	= STEP_NONE;
	m_dwStatusStartTime	= 0;
	SetActive(FALSE);
	m_bIsBattleBatting = TRUE;
	m_bOK=TRUE;
}

BOOL CGonryunPractice_ProcessCtrl::isOK(void)
{
	return m_bOK;
}

void CGonryunPractice_ProcessCtrl::Init(void)
{
	m_Leader.pcIdx		= -1;
	m_Openent.pcIdx		= -1;
	m_Winner.pcIdx		= -1;
	m_Loser.pcIdx		= -1;
	m_BattleType = PCB_BATTLE_NONE;
	m_cpVirtualWaiterCtrl->DeleteAll();
	SetStep(STEP_NONE);
	m_dwStatusStartTime	= 0;
}

void CGonryunPractice_ProcessCtrl::Set(CGonryunPracticeBattle* pPracticeBattle)
{
	m_cpVirtualWaiterCtrl	  = pPracticeBattle->GetWaiterCtrl();
	m_cpVirtualRewordCtrl	  = pPracticeBattle->GetRewordCtrl();
	m_cpVirtualEncoder		  = pPracticeBattle->GetEncoder();
}

void CGonryunPractice_ProcessCtrl::
ConfirmTradeBattingWinerGive(playerCharacter_t* pwinor , playerCharacter_t* pdefeator)
{
	
	
    
	pwinor->curChargeSE+=pdefeator->GonryunBattlePractice.iBattingSE;
	
	if( pwinor->curChargeSE > MAX_OWN_NAK ) pwinor->curChargeSE = MAX_OWN_NAK;
	m_cpVirtualEncoder->SendMessageNake(pwinor);
	
	g_logSystem->Write(
		"ConfirmTradeBattingWinerGive WinorName = %s ObtainNarke = %d ",
		pwinor->name,
		pdefeator->GonryunBattlePractice.iBattingSE);
	
	pdefeator->curChargeSE-=pdefeator->GonryunBattlePractice.iBattingSE;
	if( pdefeator->curChargeSE < 0 ) pdefeator->curChargeSE = 0;
	m_cpVirtualEncoder->SendMessageNake(pdefeator);
	
	g_logSystem->Write(
		"ConfirmTradeBattingWinerGive LoserName = %s LoseNarke = %d ",
		pdefeator->name,
		pdefeator->GonryunBattlePractice.iBattingSE);

	ConfirmTradeBattingWinerGive_Item(pwinor,pdefeator);
}

void CGonryunPractice_ProcessCtrl::
ConfirmTradeBattingWinerGive_Item(playerCharacter_t* pwinor , playerCharacter_t* pdefeator)
{
	UndoBattingTradeBatting( pwinor);
	m_cpVirtualEncoder->SendMessage_BattingTrade_ExitBatting( pwinor);

	int playerItemArrayIdx = -1;
	item_t* item = NULL;
	for(int idx = 0; idx < playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE; idx++ ){
		if( pdefeator->GonryunBattlePractice.Inventory[idx] < 0 ) continue;
		
		playerItemArrayIdx = pdefeator->GonryunBattlePractice.Inventory[idx];		
		if( (0 > playerItemArrayIdx) || ( playerItemArrayIdx >= MAX_NUMBER_OF_OWNITEM) ) continue;
		
		item = &pdefeator->item[playerItemArrayIdx];
		if( (0 > item->itemTableIdx)  || ( MAX_NUMBER_OF_ITEM_TABLE <= item->itemTableIdx)  ) continue;
	
		if(TRUE == gcpTools->AddItemBatting(pwinor,*item)){
			g_logSystem->Write(
				"Success_AddBattingItem: UserName = %s ItemName = %s ItemNumber = %d", 
				pwinor->name,
				item->name,
				item->durability+1);

			GTH_DeleteMyItem( pdefeator, item );
			
			g_DBGameServer->RemoveItem(pdefeator, playerItemArrayIdx);
			
			
		} else{
			g_logSystem->Write(
				"Fail_AddBattingItem: UserName = %s ItemName = %s ItemNumber = %d", 
				pwinor->name,
				item->name,
				item->durability+1);
			
		}
	}
}

void CGonryunPractice_ProcessCtrl::InitTradeBatting( playerCharacter_t* pPlayer )
{
	pPlayer->GonryunBattlePractice.iBattingSE = 0;		
	for(int idx = 0; idx < playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE; idx ++ ){
		pPlayer->GonryunBattlePractice.Inventory[idx] = -1;		
	}
	
}

int CGonryunPractice_ProcessCtrl::
UndoBattingTradeBatting(playerCharacter_t* pPlayer)
{
	item_t *item = NULL, *upItem = NULL;
	int playerItemArrayIdx=-1;
	for( int idx = 0; idx < playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE; idx ++ )
	{
		playerItemArrayIdx = pPlayer->GonryunBattlePractice.Inventory[idx];
		if( playerItemArrayIdx < 0 ) continue;
		if( playerItemArrayIdx >= MAX_NUMBER_OF_OWNITEM) continue;
		
		item = &pPlayer->item[playerItemArrayIdx];
		if( (0 > item->itemTableIdx) || (  item->itemTableIdx >= MAX_NUMBER_OF_ITEM_TABLE) ) continue;
	
		int stackItemIdx = GTH_StackMyItem( pPlayer, item );		
		if( stackItemIdx >= 0 ){
			upItem = &pPlayer->item[stackItemIdx];
			upItem->durability += item->durability + 1;			
			if(upItem->durability >= MAX_COUNT_STACK_ITEM ) upItem->durability = MAX_COUNT_STACK_ITEM - 1;

			pPlayer->GonryunBattlePractice.Inventory[idx]=-1;
			GTH_DeleteMyItem( pPlayer, item );

			g_DBGameServer->RemoveItem( pPlayer, playerItemArrayIdx );
			g_DBGameServer->SaveItem( pPlayer, upItem, stackItemIdx );
		
		} else {
			int inventoryPos = GTH_FindEmptyInventoryPos( pPlayer );
			if( inventoryPos >= 0 ) {
				pPlayer->inventory[inventoryPos] = playerItemArrayIdx;
				pPlayer->GonryunBattlePractice.Inventory[idx]=-1;
			}
		}
	}

	CheckItemInformation( pPlayer, LOG_CLASS_ITEM_MISSMATCH_TYPE_BATTINGTRADE );
	return 1;
}

void CGonryunPractice_ProcessCtrl::
GetWinnerPortalPos(playerCharacter_t* pPlayer)
{
	vec3_t pos;
	if( pPlayer->worldIdx == ARAYANWORLDMAP )
	{
		gonryunFieldPos_t taraynwinnerPos = g_logic.PCBGonryunCondition.m_tarayanwinnerOutPos;
		pos[0] = taraynwinnerPos.m_fbattlein_x;
		pos[1] = taraynwinnerPos.m_fbattlein_y;
		pos[2] = taraynwinnerPos.m_fbattlein_z;
		pPlayer->angles[1] = taraynwinnerPos.m_fbattlein_yaw;
		MakeTransPort( pPlayer, pos);
	
	} else if( pPlayer->worldIdx == SYENWORDMAP ){
		gonryunFieldPos_t tsyenwinnerPos = g_logic.PCBGonryunCondition.m_tsyenOutPos;
		pos[0] = tsyenwinnerPos.m_fbattlein_x;
		pos[1] = tsyenwinnerPos.m_fbattlein_y;
		pos[2] = tsyenwinnerPos.m_fbattlein_z;
		MakeTransPort( pPlayer, pos);
		g_logSystem->Write("GetWinnerPortalPos Spawn pPlayer  name=%s pos.x=%f,pos.y=%f,pos.z=%f",
		pPlayer->name,
		pPlayer->position[0],
		pPlayer->position[1],
		pPlayer->position[2]);
	}
}

void CGonryunPractice_ProcessCtrl::
IrregularQuitBattle(playerCharacter_t* pOutPlayer)
{	
	if(NULL == pOutPlayer){
		g_logSystem->Write("NULL == pOutPlayer void CGonryunPractice_ProcessCtrl::IrregularQuitBattle(playerCharacter_t* pOutPlayer)");
		return;
	}

	switch(m_Step){
	
	case STEP_NONE:
		break;

	case STEP_SUGGUEST:
		IrregularQuitBattle_Suggent(pOutPlayer);
		break;
	
	case STEP_BATTING:
		IrregularQuitBattle_Batting(pOutPlayer);
		break;

	case STEP_BATTLE_READY: 
	case STEP_FIGHTING:
		IrregularQuitBattle_Fighting(pOutPlayer);
		break;

	case STEP_BATTLE_END:
		IrregularQuitBattle_End(pOutPlayer);
		break;
	}
}

void CGonryunPractice_ProcessCtrl::
IrregularQuitBattle_Suggent(playerCharacter_t* pOutPlayer)
{
	switch(pOutPlayer->GonryunBattlePractice.MemberShip){		
		case playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE:			
		case playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT:
			return;

		case playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER:
			
			m_cpVirtualEncoder->SendWaitUserBattleEnd();
			m_cpVirtualEncoder->SendWaitUserBattleEnd(pOutPlayer);
			
			Init();	
			
			pOutPlayer->busyState = BUSY_STATE_NONE;
			pOutPlayer->GonryunBattlePractice.MemberShip = playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE;			
			
			pOutPlayer->GonryunBattlePractice.bisWaitrespond = FALSE;
			return;
	
		
		case playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_WAITER:
		
			m_cpVirtualEncoder->SendDeleteUser(pOutPlayer);
			m_cpVirtualWaiterCtrl->Delete(pOutPlayer);
			
			pOutPlayer->GonryunBattlePractice.MemberShip = playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE;		
			
			pOutPlayer->GonryunBattlePractice.bisWaitrespond = FALSE;
			pOutPlayer->busyState = BUSY_STATE_NONE;
		
			
			ShowLogInfo("DELETE USERNAME = %s Count = %d", pOutPlayer->name,m_cpVirtualWaiterCtrl->GetNum()+1);
			
			return;
		
		default :
			g_logSystem->Write("IrregularQuitBattle_Suggent: UserName = %s ", pOutPlayer->name);	
			return;
	}		

}

void CGonryunPractice_ProcessCtrl::
IrregularQuitBattle_Batting(playerCharacter_t* pOutPlayer)
{	
	switch(pOutPlayer->GonryunBattlePractice.MemberShip) {
		case playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE: 
			return;
		case playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER:
		case playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT:		
			break;
	}

	playerCharacter_t* pLeaderPlayer  = NULL;
	pLeaderPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Leader);
	
	if(NULL != pLeaderPlayer) {
	
		BattingRestore(pLeaderPlayer);
		InitNoticePC(pLeaderPlayer);
	}

	playerCharacter_t* pOpenentPlayer = NULL;	
	pOpenentPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Openent);
	
	if( NULL != pOpenentPlayer)	{
	
		BattingRestore(pOpenentPlayer);
		InitNoticePC(pOpenentPlayer);
	}

	Init();	
}


void CGonryunPractice_ProcessCtrl::
IrregularQuitBattle_Fighting(playerCharacter_t* pOutPlayer)
{
	
	if((pOutPlayer->GonryunBattlePractice.MemberShip == playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE)|| 
		(pOutPlayer->GonryunBattlePractice.MemberShip == playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_WAITER))
		return;
	
		CheckGameEnd(pOutPlayer,BATTLE_END_RESULT_BORN);
	
}

void CGonryunPractice_ProcessCtrl::
IrregularQuitBattle_End(playerCharacter_t* pOutPlayer)
{

	if(pOutPlayer->GonryunBattlePractice.MemberShip 
			== playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE) return;
	
	CheckAndMove();	
	Init();
}

void CGonryunPractice_ProcessCtrl::SetStep(const enum Step& step)
{
	m_Step	= step;
	m_dwStatusStartTime = g_globalTime ;

	switch ( m_Step ){
	case STEP_NONE:
		ShowLogInfo("PCB_STATUS : STEP_NONE,  COUNT = %d " , m_cpVirtualWaiterCtrl->GetNum());
		break;
	
	case STEP_SUGGUEST:
		ShowLogInfo("PCB_STATUS : STEP_SUGGUEST,  COUNT = %d " , m_cpVirtualWaiterCtrl->GetNum());
		break;
	
	case STEP_BATTING:
		ShowLogInfo("PCB_STATUS : STEP_BATTING,  COUNT = %d " , m_cpVirtualWaiterCtrl->GetNum());
		break;

	case STEP_BATTLE_READY:
		ShowLogInfo("PCB_STATUS : STEP_BATTLE_READY,  COUNT = %d " , m_cpVirtualWaiterCtrl->GetNum());
		break;

	case STEP_FIGHTING:		
		ShowLogInfo("PCB_STATUS : STEP_FIGHTING,  COUNT = %d " , m_cpVirtualWaiterCtrl->GetNum());
		break;

	case STEP_BATTLE_END :
		ShowLogInfo("PCB_STATUS : STEP_BATTLE_END,  COUNT = %d " , m_cpVirtualWaiterCtrl->GetNum());
		break;
	}
}

BOOL CGonryunPractice_ProcessCtrl::
SuggestPlayer(playerCharacter_t* pSuggestPlayer)
{
	switch(m_Step){
		case STEP_NONE:
		
			SetLeader( pSuggestPlayer );
			SetStep(STEP_SUGGUEST);
			
			ShowLogInfo( " USERCOUNT: %d " , m_cpVirtualWaiterCtrl->GetNum());
		
			m_cpVirtualEncoder->SendWaitUserList(pSuggestPlayer,pSuggestPlayer);
		
			ShowLogInfo( " USERCOUNT: %d " ,m_cpVirtualWaiterCtrl->GetNum()+1 );
		
			return TRUE;

		case  STEP_SUGGUEST:
			{
				playerCharacter_t* pLeaderPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Leader);
				
				if( NULL == pLeaderPlayer){
				
					Init();
					return FALSE;
				}
			
				if(pSuggestPlayer == pLeaderPlayer) return FALSE;


				struct tagGPWaitUser WaitUser;
				
				if(TRUE == m_cpVirtualWaiterCtrl->Find(WaitUser,pSuggestPlayer->name)){
					return FALSE;
				}				
				
				memset(&WaitUser,0x00,sizeof(struct tagGPWaitUser));
				WaitUser.pcIdx = pSuggestPlayer->idx;
				strncpy(WaitUser.Name,pSuggestPlayer->name,NAMESTRING);
				WaitUser.Name[NAMESTRING] = NULL;
				strncpy(WaitUser.ID,pSuggestPlayer->userID,IDSTRING);
				WaitUser.ID[IDSTRING] = NULL;				
				m_cpVirtualWaiterCtrl->Insert(WaitUser);

				pSuggestPlayer->GonryunBattlePractice.MemberShip =
					playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_WAITER;
				
				m_cpVirtualEncoder->SendWaitUserList(pLeaderPlayer,pSuggestPlayer);			
				m_cpVirtualEncoder->SendMyInfoToUser(pLeaderPlayer,pSuggestPlayer);

				int waitercount = m_cpVirtualWaiterCtrl->GetNum()+1;
				ShowLogInfo("USERCOUNT: %d",waitercount);
			}
			return TRUE;

		default:
			return FALSE;
	}
	return FALSE;
}

void CGonryunPractice_ProcessCtrl::Update_Sugguest(const int time)
{
	if ( time > g_logic.PCBGonryunCondition.PCBStatusTime[STEP_SUGGUEST] )	
	{
		
		playerCharacter_t* pLeader	= m_cpVirtualWaiterCtrl->GetWaiter(m_Leader);
		if(NULL != pLeader){			
			
			pLeader->GonryunBattlePractice.MemberShip = playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE;
			
			
			pLeader->GonryunBattlePractice.bisWaitrespond = FALSE;
			m_cpVirtualEncoder->SendWaitUserBattleEnd(pLeader);		
		}
		
		m_cpVirtualEncoder->SendWaitUserBattleEnd();
		
		Init();
		
		return;
	}	
}

void CGonryunPractice_ProcessCtrl::Update_Batting(const int time)
{
	if ( time > g_logic.PCBGonryunCondition.PCBStatusTime[STEP_BATTING] )	
	{
		
		playerCharacter_t* pLeader	= m_cpVirtualWaiterCtrl->GetWaiter(m_Leader);
	
		if(NULL != pLeader){
		
			BattingRestore(pLeader);
			InitNoticePC(pLeader);
		}
			
		playerCharacter_t* pOpenent	= m_cpVirtualWaiterCtrl->GetWaiter(m_Openent);
	
		if(NULL != pOpenent){
		
			BattingRestore(pOpenent);
			InitNoticePC(pOpenent);
		}

		Init();			
	}
}

void CGonryunPractice_ProcessCtrl::Update_Battle_Ready(const int time)
{
	if ( time > g_logic.PCBGonryunCondition.PCBStatusTime[STEP_BATTLE_READY])	
	{

		SetStep(STEP_FIGHTING);
		
		
		DWORD remainTime = g_logic.PCBGonryunCondition.PCBStatusTime[STEP_FIGHTING];
		playerCharacter_t* pLeader = m_cpVirtualWaiterCtrl->GetWaiter(m_Leader);		
		
		if( NULL != pLeader ){
		
			m_cpVirtualEncoder->SendStatus(pLeader);
			m_cpVirtualEncoder->SendGonryunBattleInfo(pLeader,remainTime);
			pLeader->pvpMode = TRUE;
		
		}

		playerCharacter_t* pOpenent= m_cpVirtualWaiterCtrl->GetWaiter(m_Openent);	
		
		if(NULL != pOpenent){
		
			m_cpVirtualEncoder->SendStatus(pOpenent);
			m_cpVirtualEncoder->SendGonryunBattleInfo(pOpenent,remainTime);
			pOpenent->pvpMode = TRUE;

		}	
	}
}

void CGonryunPractice_ProcessCtrl::Update_Fighting(const int time)
{
	if ( time > g_logic.PCBGonryunCondition.PCBStatusTime[STEP_FIGHTING])	
	{
		
		CheckGameEnd(NULL, BATTLE_END_DRAW);
		
		return;
	}
}

void CGonryunPractice_ProcessCtrl::Update_Battle_End(const int time)
{
	if ( time > g_logic.PCBGonryunCondition.PCBStatusTime[STEP_BATTLE_END])	
	{	
		CheckAndMove();
		Init();

	}
}

void CGonryunPractice_ProcessCtrl::Update(void)
{
	if ( m_Step == STEP_NONE )									
		return;

	DWORD remainTime = g_globalTime - m_dwStatusStartTime;
	
	switch ( m_Step ){

	case STEP_NONE:
		break;
	
	case STEP_SUGGUEST:
		Update_Sugguest(remainTime);
		break;
	
	case STEP_BATTING:					
		Update_Batting(remainTime);		
		break;
	
	case STEP_BATTLE_READY:
		Update_Battle_Ready(remainTime);		
		break;
	
	case STEP_FIGHTING:
		Update_Fighting(remainTime);
		break;

	case STEP_BATTLE_END:
		Update_Battle_End(remainTime);		
		break;
	}
}

void CGonryunPractice_ProcessCtrl::MakeTransPort(playerCharacter_t* pPlayer,const vec3_t leaderposition)
{
	pPlayer->position[0] = leaderposition[0];
	pPlayer->position[1] = leaderposition[1];
	pPlayer->position[2] = leaderposition[2];
	
	pPlayer->zoneIdx = GTH_Zone_UpdateCurrentZone(
		ENTITY_PC, 
		pPlayer->idx, 
		pPlayer->worldIdx, 
		pPlayer->zoneIdx,
		pPlayer->position);
		
	GTH_SendPCEventMessage_Respawn( pPlayer );
	GTH_SendMessage_SyncItemObject( pPlayer );	

	g_logSystem->Write("GoryunBattlePractice Spawn pPlayer  name=%s pos.x=%f,pos.y=%f,pos.z=%f",
		pPlayer->name,
		pPlayer->position[0],
		pPlayer->position[1],
		pPlayer->position[2]);
}

void CGonryunPractice_ProcessCtrl::GeLoserPortalPos(playerCharacter_t* pLoserPlayer)
{
	vec3_t pos;
	if( pLoserPlayer->worldIdx == ARAYANWORLDMAP )
	{
		gonryunFieldPos_t tdefeaterPos = g_logic.PCBGonryunCondition.m_tarayandefeatorOutPos;
		pos[0] = tdefeaterPos.m_fbattlein_x;
		pos[1] = tdefeaterPos.m_fbattlein_y;
		pos[2] = tdefeaterPos.m_fbattlein_z;
		pLoserPlayer->angles[1] = tdefeaterPos.m_fbattlein_yaw;
		MakeTransPort( pLoserPlayer, pos);
		
	} else if( pLoserPlayer->worldIdx == SYENWORDMAP ){
		
		gonryunFieldPos_t tsyenwinnerPos = g_logic.PCBGonryunCondition.m_tsyenOutPos;
		pos[0] = tsyenwinnerPos.m_fbattlein_x;
		pos[1] = tsyenwinnerPos.m_fbattlein_y;
		pos[2] = tsyenwinnerPos.m_fbattlein_z;
		MakeTransPort( pLoserPlayer, pos);
		g_logSystem->Write("GeLoserPortalPos Spawn pPlayer  name=%s pos.x=%f,pos.y=%f,pos.z=%f",
		pLoserPlayer->name,
		pLoserPlayer->position[0],
		pLoserPlayer->position[1],
		pLoserPlayer->position[2]);
	}
}

void CGonryunPractice_ProcessCtrl::CheckAndMove(void)															
{
	playerCharacter_t* pWinnerPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Winner);
	playerCharacter_t* pLoserPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Loser);

	playerCharacter_t* pLeaderPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Leader);
	playerCharacter_t* pOpenentPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Openent);
	SetStep(STEP_NONE);
	if( ( NULL == pWinnerPlayer ) && ( NULL == pLoserPlayer ) ){
		
		if( NULL != pLeaderPlayer ){
		
			GetWinnerPortalPos(pLeaderPlayer);	
			
			InitNoticePC(pLeaderPlayer);
			m_cpVirtualEncoder->SendStatus(pLeaderPlayer);
			
			m_cpVirtualEncoder->SendBattleMsg(
				pLeaderPlayer,
				tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::gonryun_Lest_UseCount,
				pLeaderPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount);
			
		}
		
		if( NULL != pOpenentPlayer ){
		
			GetWinnerPortalPos(pOpenentPlayer);	
				
			InitNoticePC(pOpenentPlayer);
			m_cpVirtualEncoder->SendStatus(pOpenentPlayer);
			
			m_cpVirtualEncoder->SendBattleMsg(
				pOpenentPlayer,
				tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::gonryun_Lest_UseCount,
				pOpenentPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount);
			
		}
		return;
	}

	
	if(NULL != pWinnerPlayer){	
		
		GetWinnerPortalPos(pWinnerPlayer);	
			InitNoticePC(pWinnerPlayer);
			m_cpVirtualEncoder->SendStatus(pWinnerPlayer);
		
			m_cpVirtualEncoder->SendBattleMsg(
				pWinnerPlayer,
				tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::gonryun_Lest_UseCount,
				pWinnerPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount);
			
		
	}

	if(NULL != pLoserPlayer){
		
		GeLoserPortalPos(pLoserPlayer);
			pLoserPlayer->alive = TRUE;
			pLoserPlayer->event = GTH_EV_CHAR_IDLE;
			InitNoticePC(pLoserPlayer);
			m_cpVirtualEncoder->SendStatus(pLoserPlayer);

			m_cpVirtualEncoder->SendBattleMsg(
				pLoserPlayer,
				tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::gonryun_Lest_UseCount,
				pLoserPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount);
	}	
}
					
void CGonryunPractice_ProcessCtrl::CheckOverFlowUseCount(playerCharacter_t* pUserPlayer)
{
	int* pPlayer = &(pUserPlayer->PremiumInfo.GonyounPracticeBattle.iDecreseCount);

	(*pPlayer)--;
	
	if(*pPlayer < MAX_OVERFLOW)
		*pPlayer = 0;
}


void CGonryunPractice_ProcessCtrl::
CheckGameEnd(playerCharacter_t* pOutPlayer,const enum gameEndType& endType)
{
	playerCharacter_t* pLeaderPlayer  = NULL;
	playerCharacter_t* pOpenentPlayer = NULL;

	
	if( BATTLE_END_RESULT_BORN == endType){
		
		switch(pOutPlayer->GonryunBattlePractice.MemberShip) {
		
		case playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER:
			{
				m_Leader.pcIdx = pOutPlayer->idx;
				strncpy(m_Leader.ID,pOutPlayer->userID,IDSTRING);
				strncpy(m_Leader.Name,pOutPlayer->name,NAMESTRING);
				
				
				pOpenentPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Openent);
				if( NULL != pOpenentPlayer) {
				
					m_Winner = m_Openent;
					m_Loser	 = m_Leader;
			
				} else{

					m_Winner.pcIdx = -1;
					m_Loser = m_Leader;
				}
			}
			break;
		
		case playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT:
			{
				m_Openent.pcIdx = pOutPlayer->idx;
				strncpy(m_Openent.ID,pOutPlayer->userID,IDSTRING);
				strncpy(m_Openent.Name,pOutPlayer->name,NAMESTRING);
				

				
				pLeaderPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Leader);
				if( NULL != pLeaderPlayer) {
				
					m_Winner = m_Leader;
					m_Loser	 = m_Openent;
			
				} else{

					m_Winner.pcIdx = -1;
					m_Loser = m_Openent;
				}
			}
			
			break;
		default:
			g_logSystem->Write("BadEndGame PlayerName = %s",pOutPlayer->name);
			GTH_DisconnectPlayer(pOutPlayer,FALSE);
			return;
		}

	} else{
		
		pLeaderPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Leader);
		pOpenentPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Openent);
	
		m_Winner.pcIdx = -1;
		m_Loser.pcIdx  = -1;
	}

	playerCharacter_t* pWinnerPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Winner);
	playerCharacter_t* pLoserPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_Loser);

	SetStep(STEP_BATTLE_END);
	if( (NULL == pWinnerPlayer) && (NULL == pLoserPlayer) ){
		
		
		if( NULL != pLeaderPlayer ){		
			pLeaderPlayer->pvpMode = FALSE;
			pLeaderPlayer->GonryunBattlePractice.Record.idraw++;
			GTH_SendMessage_CompleteEffect_To_AllPC( pLeaderPlayer, 66 );	
			if( NULL != pOpenentPlayer ){
				m_cpVirtualEncoder->SendGonryunBattleResult(pLeaderPlayer,1,pOpenentPlayer->name);			
			}
		
			m_cpVirtualEncoder->SendEffect(pLeaderPlayer, PCB_EFF_WIN );
			m_cpVirtualEncoder->SendStatus(pLeaderPlayer);
			g_DBGameServer->SaveGonryunBattle(pLeaderPlayer);
			if(PCB_BATTLE_BATTING == m_BattleType) BattingRestore(pLeaderPlayer);
			
			g_logSystem->Write("DrawEndGame: Name = %s Win = %d  Defeat = %d Draw = %d",
			pLeaderPlayer->name,
			pLeaderPlayer->GonryunBattlePractice.Record.iwin,
			pLeaderPlayer->GonryunBattlePractice.Record.idefeat,
			pLeaderPlayer->GonryunBattlePractice.Record.idraw);	
		}

		if( NULL != pOpenentPlayer ){	
		
			pOpenentPlayer->pvpMode = FALSE;
			pOpenentPlayer->GonryunBattlePractice.Record.idraw++;
			GTH_SendMessage_CompleteEffect_To_AllPC( pOpenentPlayer, 66 );
			
			if( NULL != pLeaderPlayer ){
				m_cpVirtualEncoder->SendGonryunBattleResult(pOpenentPlayer,1,pLeaderPlayer->name);			
			}	
		
			m_cpVirtualEncoder->SendEffect(pOpenentPlayer, PCB_EFF_WIN);
			m_cpVirtualEncoder->SendStatus(pOpenentPlayer);
			g_DBGameServer->SaveGonryunBattle(pOpenentPlayer);
		
			if(PCB_BATTLE_BATTING == m_BattleType) BattingRestore(pOpenentPlayer);
		
			g_logSystem->Write("DrawEndGame: Name = %s Win = %d  Defeat = %d Draw = %d",
			pOpenentPlayer->name,
			pOpenentPlayer->GonryunBattlePractice.Record.iwin,
			pOpenentPlayer->GonryunBattlePractice.Record.idefeat,
			pOpenentPlayer->GonryunBattlePractice.Record.idraw);	
		}
	
		return;
	}	
	
	if(NULL != pWinnerPlayer){	
	
		pWinnerPlayer->pvpMode = FALSE;
		GTH_SendMessage_CompleteEffect_To_AllPC( pWinnerPlayer, 66 );	
		m_cpVirtualEncoder->SendEffect( pWinnerPlayer, PCB_EFF_WIN );
		m_cpVirtualEncoder->SendStatus(pWinnerPlayer);
		m_cpVirtualRewordCtrl->rewordField( pWinnerPlayer );
		pWinnerPlayer->GonryunBattlePractice.Record.iwin++;
		g_DBGameServer->SaveGonryunBattle( pWinnerPlayer );
	
		if(NULL != pLoserPlayer){
			if(PCB_BATTLE_BATTING == m_BattleType) GiveBattingWinner(pWinnerPlayer, pLoserPlayer);
			m_cpVirtualEncoder->SendGonryunBattleResult( pWinnerPlayer,2,pLoserPlayer->name);
		}

		g_logSystem->Write("EndGame: pWinnerPlayer_Name = %s Win = %d  Defeat = %d Draw = %d",
			pWinnerPlayer->name,
			pWinnerPlayer->GonryunBattlePractice.Record.iwin,
			pWinnerPlayer->GonryunBattlePractice.Record.idefeat,
			pWinnerPlayer->GonryunBattlePractice.Record.idraw);	
	}

	if(NULL != pLoserPlayer){
	
		pLoserPlayer->pvpMode = FALSE;
		GTH_SendMessage_CompleteEffect_To_AllPC( pLoserPlayer, 67 );
		m_cpVirtualEncoder->SendEffect( pLoserPlayer, PCB_EFF_DEFEAT );
		m_cpVirtualEncoder->SendStatus(pLoserPlayer);
		pLoserPlayer->GonryunBattlePractice.Record.idefeat++;
		g_DBGameServer->SaveGonryunBattle( pLoserPlayer );
		
		if(NULL != pWinnerPlayer)
			m_cpVirtualEncoder->SendGonryunBattleResult( pLoserPlayer,0,pWinnerPlayer->name );
		
		m_cpVirtualEncoder->SendPCEventMessage_DieGonryun( pLoserPlayer);	
		
		g_logSystem->Write("EndGame: pLoserPlayer_Name = %s Win = %d  Defeat = %d Draw = %d",
			pLoserPlayer->name,
			pLoserPlayer->GonryunBattlePractice.Record.iwin,
			pLoserPlayer->GonryunBattlePractice.Record.idefeat,
			pLoserPlayer->GonryunBattlePractice.Record.idraw);		
	}

	
}







void CGonryunPractice_ProcessCtrl::BattingRestore( playerCharacter_t* pPlayer )
{
	UndoBattingTradeBatting( pPlayer);
	m_cpVirtualEncoder->SendMessage_BattingTrade_ExitBatting( pPlayer);
	InitTradeBatting( pPlayer );
}

void CGonryunPractice_ProcessCtrl::MakeTransPosition( playerCharacter_t* pLeaderPlayer,playerCharacter_t* pOpenentPlayer )
{
	vec3_t leaderposition;
	vec3_t openentposition;

	if( pLeaderPlayer->worldIdx == ARAYANWORLDMAP ){
	
		gonryunFieldPos_t LeaderPos = g_logic.PCBGonryunCondition.m_tarayanReaderPos;
		gonryunFieldPos_t OpenentPos = g_logic.PCBGonryunCondition.m_tarayanOpenentPos;
		
		leaderposition[0]=LeaderPos.m_fbattlein_x;
		leaderposition[1]=LeaderPos.m_fbattlein_y;
		leaderposition[2]=LeaderPos.m_fbattlein_z;
		pLeaderPlayer->angles[1]=LeaderPos.m_fbattlein_yaw;
		
		openentposition[0]=OpenentPos.m_fbattlein_x;
		openentposition[1]=OpenentPos.m_fbattlein_y;
		openentposition[2]=OpenentPos.m_fbattlein_z;	
		pOpenentPlayer->angles[1]=OpenentPos.m_fbattlein_yaw;

	} else if( pLeaderPlayer->worldIdx == SYENWORDMAP ){
		
		gonryunFieldPos_t LeadersyenPos = g_logic.PCBGonryunCondition.m_tasyenReaderPos;
		gonryunFieldPos_t OpenentsyenPos = g_logic.PCBGonryunCondition.m_tasyenOpenentPos;

		leaderposition[0]=LeadersyenPos.m_fbattlein_x;
		leaderposition[1]=LeadersyenPos.m_fbattlein_y;
		leaderposition[2]=LeadersyenPos.m_fbattlein_z;
		pLeaderPlayer->angles[1]=LeadersyenPos.m_fbattlein_yaw;
		
		openentposition[0]=OpenentsyenPos.m_fbattlein_x;
		openentposition[1]=OpenentsyenPos.m_fbattlein_y;
		openentposition[2]=OpenentsyenPos.m_fbattlein_z;	
		pOpenentPlayer->angles[1]=OpenentsyenPos.m_fbattlein_yaw;
	}
	
	MakeTransPort( pLeaderPlayer,leaderposition );
	MakeTransPort( pOpenentPlayer,openentposition );
}

void CGonryunPractice_ProcessCtrl::SetActive(const int& active)
{
	m_bIsactive = active;
}

const int&  CGonryunPractice_ProcessCtrl::GetActive(void)const		
{
	return m_bIsactive;
}

const CGonryunPractice_ProcessCtrl::Step& CGonryunPractice_ProcessCtrl::GetStep(void)const
{
	return m_Step;
}


void CGonryunPractice_ProcessCtrl::SetBattleType(const battleMode& type)
{
	m_BattleType = type;
}

const CGonryunPractice_ProcessCtrl::battleMode& CGonryunPractice_ProcessCtrl::GetBattleType()const
{
	return m_BattleType;
}

void  CGonryunPractice_ProcessCtrl::SetLeader( playerCharacter_t* pLeaderPlayer)
{
	m_Leader.pcIdx = pLeaderPlayer->idx;
	strncpy(m_Leader.ID,pLeaderPlayer->userID,IDSTRING);
	m_Leader.ID[IDSTRING] = NULL;
	
	strncpy(m_Leader.Name,pLeaderPlayer->name,NAMESTRING);
	m_Leader.Name[NAMESTRING] = NULL;
	pLeaderPlayer->GonryunBattlePractice.MemberShip =
		playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER;
}

void  CGonryunPractice_ProcessCtrl::SetOpenent( playerCharacter_t* pOpenetPlayer)
{
	m_Openent.pcIdx = pOpenetPlayer->idx;
	strncpy(m_Openent.ID,pOpenetPlayer->userID,IDSTRING);
	m_Openent.ID[IDSTRING] = NULL;
	
	strncpy(m_Openent.Name,pOpenetPlayer->name,NAMESTRING);
	m_Openent.Name[NAMESTRING] = NULL;
	pOpenetPlayer->GonryunBattlePractice.MemberShip = playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT;
}

void CGonryunPractice_ProcessCtrl::SetBattingBattle(const BOOL bvalue)
{
	m_bIsBattleBatting=bvalue;
}

BOOL CGonryunPractice_ProcessCtrl::isBattingPlayActive(void) const
{
	return m_bIsBattleBatting;
}

BOOL CGonryunPractice_ProcessCtrl::CheckItemInformation( playerCharacter_t* pPlayer, const int checkType )
{

	
	int ownItemTableIndex = -1;
	int ownItemNumber = -1;
	
	for(int inventoryIdx = 0; inventoryIdx < MAX_INVENTORY_SIZE; inventoryIdx++){
		
		ownItemTableIndex	= pPlayer->inventory[inventoryIdx];
		 
		if( 0 > ownItemTableIndex) continue;
		 
		if( 0 <= pPlayer->item[ownItemTableIndex].itemTableIdx ){
		
			ownItemNumber++;
		 
		 } else{
			 
			 
			 g_logSystem->Write( 
				 "InventoryItem Data Error!! checkType = %d UserName = %s InventoryIdx = %d ItemIdx = %d",
				 pPlayer->name,
				 inventoryIdx ,
				 ownItemTableIndex );
		 }
	}

	for(int equipmentIdx = 0; equipmentIdx < MAX_EQUIPMENT; equipmentIdx++ ){
		
		ownItemTableIndex	= pPlayer->equipment[equipmentIdx];
		
		if( 0 > ownItemTableIndex) continue;
		
		if( 0 <= pPlayer->item[ownItemTableIndex].itemTableIdx ){
			ownItemNumber++;
		
		} else{
			
			
			g_logSystem->Write( 
				"equipment Data Error!! checkType = %d UserName = %s InventoryIdx = %d ItemIdx = %d",
				pPlayer->name,
				equipmentIdx ,
				ownItemTableIndex );
		}
	}

	for(int depotIdx = 0; depotIdx < MAX_DEPOT_SIZE; depotIdx++ ){
		
		ownItemTableIndex	= pPlayer->depot[depotIdx];
		
		if( 0 > ownItemTableIndex) continue;
		
		if( 0 <= pPlayer->item[ownItemTableIndex].itemTableIdx ){
			ownItemNumber++;
		
		} else{
			
			
			g_logSystem->Write( 
				"depot Data Error!! checkType = %d UserName = %s InventoryIdx = %d ItemIdx = %d",
				pPlayer->name,
				depotIdx ,
				ownItemTableIndex );
		}
	}


	for( int precocityIdx = 0; precocityIdx < MAX_PRECOCITY_SIZE; precocityIdx ++ ){
	
		ownItemTableIndex	= pPlayer->precocityInventory[precocityIdx];
		
		if( 0 > ownItemTableIndex) continue;
		
		if( 0 <= pPlayer->item[ownItemTableIndex].itemTableIdx ){
			ownItemNumber++;
		
		} else{
			
			
			g_logSystem->Write( 
				"precocityInventory Data Error!! checkType = %d UserName = %s InventoryIdx = %d ItemIdx = %d",
				pPlayer->name,
				precocityIdx ,
				ownItemTableIndex );
		}
	}
		
	for(int GBPinventory = 0; GBPinventory < playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE; GBPinventory++)
	{
		ownItemTableIndex	= pPlayer->GonryunBattlePractice.Inventory[GBPinventory];
		
		if( 0 > ownItemTableIndex) continue;
		
		if( 0 <= pPlayer->item[ownItemTableIndex].itemTableIdx ){
			ownItemNumber++;
		
		} else{
			
			
			g_logSystem->Write( 
				"GBPinventory Data Error!! checkType = %d UserName = %s InventoryIdx = %d ItemIdx = %d",
				pPlayer->name,
				GBPinventory ,
				ownItemTableIndex );
		}
	}

	if( 0 <= pPlayer->mouseInventory){
		
		if( 0 <= pPlayer->item[pPlayer->mouseInventory].itemTableIdx ){
			ownItemNumber++;
		
		} else{
			
			
			g_logSystem->Write( 
				"mouseInventory Data Error!! checkType = %d UserName = %s ItemIdx = %d",
				pPlayer->name,
				pPlayer->mouseInventory );
		}
	}
	
	return (ownItemNumber == pPlayer->itemNumber) ? TRUE: FALSE;
}

void CGonryunPractice_ProcessCtrl::InitNoticePC( playerCharacter_t* pPlayer )
{
	pPlayer->busyState = BUSY_STATE_NONE;
	pPlayer->GonryunBattlePractice.MemberShip = playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE;
	pPlayer->GonryunBattlePractice.bisWaitrespond = FALSE;
}

void CGonryunPractice_ProcessCtrl::CheckPlayerPos_and_Transport(playerCharacter_t* pPlayer)
{
	
	if((playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_LEADER == pPlayer->GonryunBattlePractice.MemberShip) ||
		(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_OPENENT == pPlayer->GonryunBattlePractice.MemberShip))
		return;
	
	vec3_t position;
	if( pPlayer->worldIdx == ARAYANWORLDMAP ) {
		
		if ( GTH_GetDistance2D(g_logic.PCBGonryunCondition.origin_ArayanOutPos,pPlayer->position) <= g_logic.PCBGonryunCondition.pcpBattleIn ){
			
			if( FALSE == pPlayer->pvpMode){
				position[0] = g_logic.PCBGonryunCondition.m_tarayanwinnerOutPos.m_fbattlein_x;
				position[1] = g_logic.PCBGonryunCondition.m_tarayanwinnerOutPos.m_fbattlein_y;
				position[2] = g_logic.PCBGonryunCondition.m_tarayanwinnerOutPos.m_fbattlein_z;
				
				MakeTransPort(pPlayer,position);
				
				g_logSystem->Write("CheckPlayerPos_and_Transport Spawn pPlayer  name=%s STATE = %d WorldIdx = %d pos.x=%f,pos.y=%f,pos.z=%f",
					pPlayer->name,
					pPlayer->GonryunBattlePractice.PC_State,
					pPlayer->worldIdx,
					pPlayer->position[0],
					pPlayer->position[1],
					pPlayer->position[2]);
				
				
			} else{
				GTH_DisconnectPlayer(pPlayer,FALSE);
			}
			
			
		}
	}
	
	else if( pPlayer->worldIdx== SYENWORDMAP ) {
		
		if ( GTH_GetDistance2D(g_logic.PCBGonryunCondition.origin_syenOutPos,pPlayer->position) <= g_logic.PCBGonryunCondition.pcpBattleIn ){
			if(FALSE == pPlayer->pvpMode){
				position[0] = g_logic.PCBGonryunCondition.m_tsyenOutPos.m_fbattlein_x;
				position[1] = g_logic.PCBGonryunCondition.m_tsyenOutPos.m_fbattlein_y;
				position[2] = g_logic.PCBGonryunCondition.m_tsyenOutPos.m_fbattlein_z;
				
				MakeTransPort(pPlayer,position);
				
				g_logSystem->Write("CheckPlayerPos_and_Transport Spawn pPlayer  name=%s STATE = %d WorldIdx = %d pos.x=%f,pos.y=%f,pos.z=%f",
					pPlayer->name,
					pPlayer->GonryunBattlePractice.PC_State,
					pPlayer->worldIdx,
					pPlayer->position[0],
					pPlayer->position[1],
					pPlayer->position[2]);
			} else{
				GTH_DisconnectPlayer(pPlayer,FALSE);
			}
			
		}
	}
}


const tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::errorCode CGonryunPractice_ProcessCtrl::
CheckConfirmTradeBatting(playerCharacter_t* pToPlayer,playerCharacter_t* pFromPlayer)
{
	class CTempPlayerInventoryX TempInventory(pToPlayer);
	int playerItemArrayIdx = -1;
	const item_t* pItem = NULL;	


	for(int GBPInventoryIdx = 0; GBPInventoryIdx < playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE; GBPInventoryIdx++){
		
		playerItemArrayIdx = pFromPlayer->GonryunBattlePractice.Inventory[GBPInventoryIdx]; 
		if( (0 > playerItemArrayIdx) || ( playerItemArrayIdx >= MAX_NUMBER_OF_OWNITEM) ) continue;
		
		pItem = &pFromPlayer->item[playerItemArrayIdx];
		if( NULL == pItem) continue;
		if( (0 > pItem->itemTableIdx)  || ( pItem->itemTableIdx  >= MAX_NUMBER_OF_ITEM_TABLE)  ) continue;
		
		switch(TempInventory.Insert(pItem)) {
		case CTempPlayerInventoryX::ErrorCode_Success:
			break;
			
		case CTempPlayerInventoryX::ErrorCode_Over_Weight:
			return tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::ERROR_Weight_Over;
					
		case CTempPlayerInventoryX::ErrorCode_stacknum_overflow:
			return tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::ErrorCode_stacknum_overflow;
					
		case CTempPlayerInventoryX::ErrorCode_NotEnough_Inventory:
			return tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::ERROR_NotEnough_InventorySpace;
					
		case CTempPlayerInventoryX::ErrorCode_unknown:
			return tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::ErrorCode_unknown;
			
		}
	}		



	for(GBPInventoryIdx = 0; GBPInventoryIdx < playerCharacter_t::tagGonryunBattlePractice::MAX_GBP_INVENTORY_SIZE; GBPInventoryIdx++){
		
		playerItemArrayIdx = pToPlayer->GonryunBattlePractice.Inventory[GBPInventoryIdx]; 
		if( (0 > playerItemArrayIdx) || ( playerItemArrayIdx >= MAX_NUMBER_OF_OWNITEM) ) continue;
		
		pItem = &pToPlayer->item[playerItemArrayIdx];
		if( NULL == pItem) continue;
		if( (0 > pItem->itemTableIdx)  || ( pItem->itemTableIdx  >= MAX_NUMBER_OF_ITEM_TABLE)  ) continue;
		
		switch(TempInventory.Insert(pItem)) {
		case CTempPlayerInventoryX::ErrorCode_Success:
			break;
			
		case CTempPlayerInventoryX::ErrorCode_Over_Weight:
			return tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::ERROR_Weight_Over;
					
		case CTempPlayerInventoryX::ErrorCode_stacknum_overflow:
			return tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::ErrorCode_stacknum_overflow;
					
		case CTempPlayerInventoryX::ErrorCode_NotEnough_Inventory:
			return tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::ERROR_NotEnough_InventorySpace;
					
		case CTempPlayerInventoryX::ErrorCode_unknown:
			return tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::ErrorCode_unknown;
			
		}
	}		
	
	return tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::success;
}

