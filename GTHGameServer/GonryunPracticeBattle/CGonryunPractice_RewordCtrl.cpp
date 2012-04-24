 
#include "..\global.h"
#include "CGonryunPractice_RewordCtrl.h"
#include "CGonryunPractice_WaiterCtrl.h"
#include "CGonryunPractice_ProcessCtrl.h"
#include "CGonryunPracticeBattle.h"
#include "CGonryunPractice_Ability.h"
#include "CGonryunPractice_RewordItem.h"
#include "CGonryunPractice_Encoder.h"
#include "..\Tools\CTools.h"



extern void GTH_PC_ExpUp( int idx, __int64 &exp, __int64 &genExp, int &lootNSE, int &level, int &genLevel );
extern void GTH_DisconnectPlayer(playerCharacter_t* pPlayer,const BOOL bForceQuit);
extern CTools* gcpTools;


CGonryunPractice_RewordCtrl::CGonryunPractice_RewordCtrl()
{
	m_bOK=FALSE;
	m_cpVirtualProcessCtrl = NULL;
	m_cpVirtualEncoder	   = NULL;
	m_bOK=TRUE;


}

CGonryunPractice_RewordCtrl::~CGonryunPractice_RewordCtrl()
{
	Destroy();
}



BOOL CGonryunPractice_RewordCtrl::isOK(void)
{
	return m_bOK;

}
void CGonryunPractice_RewordCtrl::Set(CGonryunPracticeBattle* pPracticeBattle)
{
	m_cpVirtualProcessCtrl		= pPracticeBattle->GetProcessCtrl();
	m_cpVirtualEncoder			= pPracticeBattle->GetEncoder();
}

void CGonryunPractice_RewordCtrl::Destroy(void)
{
	for_each(m_rewordItemList.begin(),m_rewordItemList.end(),TDelete<CGonryunPractice_RewordItem>());
	m_rewordItemList.clear();
	for_each(m_rewordList.begin(),m_rewordList.end(),TDelete<CGonryunPractice_Ability>());
	m_rewordList.clear();
}

CGonryunPractice_Ability* CGonryunPractice_RewordCtrl::GetReword(const int Idx)			
{
	REWORD_ITOR iLoop;
	CGonryunPractice_Ability* pReword	= NULL;
	
	for( iLoop = m_rewordList.begin(); iLoop!=m_rewordList.end(); iLoop++ ) {
	
		pReword=(CGonryunPractice_Ability*)(*iLoop);
		
		if (pReword->GetID() == Idx){
			
			return pReword;
		}
	}

	return NULL;
}

void CGonryunPractice_RewordCtrl::rewordField(playerCharacter_t* puser)
{
	CGonryunPractice_Ability* pReword= NULL;
	
	switch(puser->worldIdx) {
	
	case  CGonryunPractice_ProcessCtrl::ARAYANWORLDMAP:
		pReword = GetReword(1);
		break;
	
	case CGonryunPractice_ProcessCtrl::SYENWORDMAP:
		pReword = GetReword(2);
		break;

	default:
		GTH_DisconnectPlayer(puser,FALSE);
		return;
	}

	if ( NULL == pReword ) {
	
		ShowLogInfo("Reward Fail______________");
		return;
	}

	
	float rewordExperiencePay=0.0f;
	float rewordItemRatePay=0.0f;
	
	if( puser->PremiumInfo.iMemberShipType == playerCharacter_t::tagPremiumInfo::Pay ){
		
		rewordExperiencePay = pReword->GetExperience()*g_logic.PCBGonryunCondition.fExpRate;
		rewordItemRatePay = pReword->GetItemRate()*g_logic.PCBGonryunCondition.fItemRate;

	} else {
		
		rewordExperiencePay = pReword->GetExperience();
		rewordItemRatePay = pReword->GetExpRate();
	}
	
	float value = rewordExperiencePay/10000.0f;
	
	int GiveExp = g_expTable[puser->level-1].exp*value;
	
	int GiveGenExp = g_expTable[puser->genLevel-1].genExp*(value/2.0f);

	ShowLogInfo("Reqward Exp Value : %d, GenExp : %d, value : %d",GiveExp,GiveGenExp,int(value*100));
	
	float frandValue = GTH_Random();
	float ExpGiveRate = pReword->GetExpRate()/100.0f;

	ShowLogInfo(
		"Exp Reword Rand Value : %d, GiveRate : %d,Freemeum : %d ",
		int(frandValue*100),
		int(ExpGiveRate*100),
		puser->PremiumInfo.iMemberShipType);
	
	if ( frandValue <= ExpGiveRate) {
		GetLootByBattleReword( puser,GiveExp, GiveGenExp );
	}
	
	frandValue = GTH_Random();

	float ItemGiveRate = rewordItemRatePay/100.0f;

	ShowLogInfo("Item Reword Rand Value : %d, GiveRate : %d",int(frandValue*100),int(ItemGiveRate*100));
	
	if ( frandValue <= ItemGiveRate) {
		addItemBattle(puser);
	}

}

BOOL CGonryunPractice_RewordCtrl::addItemBattle(playerCharacter_t* puser)
{
	int rewardIdx=0;		
	
	switch(puser->worldIdx) {
	
	case  CGonryunPractice_ProcessCtrl::ARAYANWORLDMAP:
		rewardIdx = 1;		
		break;
	
	case CGonryunPractice_ProcessCtrl::SYENWORDMAP:
		rewardIdx = 2;		
		break;
	
	default:
		GTH_DisconnectPlayer(puser,FALSE);
		return FALSE;
	}

	float frandValue			= GTH_Random()*100.0f;

	CGonryunPractice_RewordItem* pitem	= NULL;
	float sumValue				= 0.0f;
	int addItemData[4];
	REWORDITEM_ITOR iLoop ;
	
	for( iLoop = m_rewordItemList.begin(); iLoop!=m_rewordItemList.end(); iLoop++ )
	{
		pitem=(CGonryunPractice_RewordItem*)(*iLoop);

		if ( pitem->GetItemIndex() != rewardIdx )
			continue;
		
		if ( sumValue <= frandValue && (pitem->GetItemRate()+sumValue) >= frandValue ){
		
			itemTable_t* pItemTable = NULL;
			pItemTable = gcpTools->GetItemTablePointer( pitem->GetItemNumber() );
			if(NULL == pItemTable){		
				
				return FALSE;
			}
			
			if( puser->curItemWeight + pItemTable->weight > puser->calItemWeight ){ 
			
				m_cpVirtualEncoder->SendBattleMsg(
					puser,
					tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::Weight_Over);
				return FALSE;
			}

				
			for(int InventoryIdx = 0; InventoryIdx < MAX_INVENTORY_SIZE; InventoryIdx++){			
				
				if( 0 > puser->item[InventoryIdx].itemTableIdx) break;
			}
			
			if( MAX_INVENTORY_SIZE <= InventoryIdx ) {
				
				m_cpVirtualEncoder->SendBattleMsg(
					puser,
					tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::NotEnough_InventorySpace);
				return FALSE;
			}
						
			if( pItemTable->stackFlag ){
				
				for(InventoryIdx=0; InventoryIdx < MAX_INVENTORY_SIZE; InventoryIdx++){			
					
					if(puser->item[InventoryIdx].itemTableIdx != pItemTable->idx) continue;
					int numberIdx = puser->item[InventoryIdx].durability + pitem->GetItemCount();
					if(numberIdx >= MAX_COUNT_STACK_ITEM) {
					
						m_cpVirtualEncoder->SendBattleMsg(
							puser,
							tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::NotEnough_InventorySpace);
						return FALSE;
					}
					
					break;	
				}
			}
						
			
			addItemData[0]=pitem->GetItemNumber();
			addItemData[1]=-1;
			addItemData[2]=pitem->GetItemCount();
			addItemData[3] =1;	
		
			ShowLogInfo("Reward Item ,No :%d, Count : %d",pitem->GetItemNumber(),pitem->GetItemCount());
			addItemBattleField(puser, addItemData);
			
			break;
		}

		sumValue=sumValue+pitem->GetItemRate();
	}

	return TRUE;
}



void CGonryunPractice_RewordCtrl::GetLootByBattleReword(playerCharacter_t* pPlayer, __int64 exp,__int64 genExp)
{
	int lootNSE=0;
	int upFlag = FALSE;
	int level = 0, genLevel = 0;
	int skillIdx[50], skillNumber = 0;
	
	
	


	exp = (__int64)( exp * g_logic.expRate );
	genExp = (__int64)( genExp * g_logic.expRate );


	
	if( exp < 0 )			exp = 0;
	
	if( exp == 0 ) 
		return;
	

	GTH_PC_ExpUp( pPlayer->idx, exp, genExp, lootNSE, level, genLevel );

	if( level > pPlayer->level )
	{
		pPlayer->bonusStatusPoint += ( level - pPlayer->level );
		GTH_PC_LevelUp(pPlayer, level );
		upFlag = true;
	}
	if( genLevel > pPlayer->genLevel )
	{

		GTH_PC_GenLevelUp( pPlayer, genLevel );
		upFlag = true;
	}
	
	if( upFlag )
	{
		
		GTH_PC_Calculate( pPlayer, PCINFO_BASIC );
		GTH_SendPCMessage_LevelUp( 
			pPlayer, pPlayer->exp,pPlayer->genExp, pPlayer->curChargeSE, pPlayer->genCapability[ pPlayer->selectedGen], skillIdx, skillNumber, 1 );
	}
	else
	{
		GTH_SendMessage_LootValue( 
			pPlayer, pPlayer->exp, pPlayer->genExp, pPlayer->curChargeSE, pPlayer->genCapability[ pPlayer->selectedGen], 4 );
	}

}


void CGonryunPractice_RewordCtrl::ExpUp_Player(playerCharacter_t* pPlayer,	int& exp, int& genExp, int& level, int& genLevel)
{
	if( (pPlayer->level < LEVEL_THIRTY) && (exp > g_expTable[LEVEL_THIRTY-1].exp)){
		
		exp = g_expTable[LEVEL_THIRTY-1].exp;
		
	} else if((pPlayer->level >= LEVEL_THIRTY) && (exp > g_expTable[pPlayer->level-1].exp) ){
	
		exp = g_expTable[pPlayer->level-1].exp;
	}
	

	if( exp > 0 ) {
		
		pPlayer->exp  = pPlayer->exp + exp;
		level = GTH_GetNextLevel( pPlayer->exp  );
	}

	if((pPlayer->genLevel < LEVEL_FIFTEEN) && (genExp > g_expTable[LEVEL_FIFTEEN-1].genExp)){
	
		genExp = g_expTable[LEVEL_FIFTEEN-1].genExp;
	
	} else if((pPlayer->genLevel >= LEVEL_FIFTEEN) && (genExp > g_expTable[pPlayer->genLevel-1].genExp)){
		genExp = g_expTable[pPlayer->genLevel-1].genExp;
	}

	if( genExp > 0 ) {
		
		pPlayer->genExp  = pPlayer->genExp + genExp;
		genLevel = GTH_GetNextGenLevel( pPlayer->genExp  );
	}
}

void CGonryunPractice_RewordCtrl::addItemBattleField( void* pPc, int* pParam )
{
	AI_NPC_AddItem(pPc,pParam);
}


CGonryunPractice_ProcessCtrl* CGonryunPractice_RewordCtrl::GetProcessCtrl()
{
	return m_cpVirtualProcessCtrl;	
}

CGonryunPractice_RewordCtrl::REWORDITEMLIST&	 CGonryunPractice_RewordCtrl::GetRewordItemList()
{
	return m_rewordItemList;
}

CGonryunPractice_RewordCtrl::REWORDLIST&	CGonryunPractice_RewordCtrl::GetRewordList()
{
	return m_rewordList;
}

