



#include "..\global.h"

#include "CGonryunPracticeBattle.h"
#include "CGonryunPractice_ProcessCtrl.h"
#include "CGonryunPractice_WaiterCtrl.h"
#include "CGonryunPractice_Encoder.h"
#include "tagGonryunPracticePacket.h"
#include "..\Tools\CTools.h"
extern CTools* gcpTools;





CGonryunPractice_Encoder::CGonryunPractice_Encoder()
{
	m_bOK=FALSE;

	m_cpVirtualPracticeBattle	= NULL;
	m_cpVirtualProcessCtrl		= NULL;
	m_cpVirtualWaiterCtrl		= NULL;

	m_bOK=TRUE;

}

CGonryunPractice_Encoder::~CGonryunPractice_Encoder()
{

}

BOOL CGonryunPractice_Encoder::isOK(void)
{
	return m_bOK;
}


void CGonryunPractice_Encoder::Set(CGonryunPracticeBattle* pPracticeBattle)
{
	m_cpVirtualPracticeBattle	= pPracticeBattle;
	m_cpVirtualProcessCtrl		= pPracticeBattle->GetProcessCtrl();
	m_cpVirtualWaiterCtrl		= pPracticeBattle->GetWaiterCtrl();
}

void CGonryunPractice_Encoder::
SendMessage_BattingTrade_ItemBatting(
	playerCharacter_t* pPlayer,
	
	BOOL bMyItem,
	enum tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_ITEM::enumType Type,
	int pos,
	item_t *item,
	const int DeleteItemIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_ITEM);
		MSG_WriteByte(&netMessage, bMyItem);
		MSG_WriteByte(&netMessage, Type);
		MSG_WriteByte(&netMessage, pos);
		MSG_WriteItem(&netMessage, item);
		MSG_WriteShort(&netMessage, DeleteItemIdx);
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void CGonryunPractice_Encoder::SendWaitUserBattleEnd(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{	
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_END);
		NET_SendMessage ( &pPlayer->sock, &netMessage );
	}
	MSG_EndWriting(&netMessage);
}


void CGonryunPractice_Encoder::SendWaitUserBattleEnd(void)
{
	
	CGonryunPractice_WaiterCtrl::WAIT_ITOR	itor;
	struct tagGPWaitUser	WaitUser;

	for(itor = (m_cpVirtualWaiterCtrl->m_WaitUserList).begin();itor != (m_cpVirtualWaiterCtrl->m_WaitUserList).end(); itor++ ){
	
		WaitUser = *itor;		
	
		playerCharacter_t* pPlayer = m_cpVirtualWaiterCtrl->GetWaiter(WaitUser);
	
		if( NULL == pPlayer) continue;

		SendWaitUserBattleEnd(pPlayer);
	}	
}


void CGonryunPractice_Encoder::SendStatus(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{		
	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
	MSG_WriteByte(&netMessage, BATTLE_PRACTICE_STATE);
	MSG_WriteShort(&netMessage,m_cpVirtualProcessCtrl->GetStep());	
	NET_SendMessage ( &pPlayer->sock, &netMessage );
	}
	MSG_EndWriting(&netMessage);	
}


void CGonryunPractice_Encoder::SendEffect(playerCharacter_t* pPlayer,int type)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{		
	MSG_WriteByte(&netMessage, GSC_EXTEND);
	MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
	MSG_WriteByte(&netMessage, BATTLE_PRACTICE_EFFECT_MODE);
	MSG_WriteShort(&netMessage,type);	
	NET_SendMessage ( &pPlayer->sock, &netMessage );
	}
	MSG_EndWriting(&netMessage);		
}


void CGonryunPractice_Encoder::SendGonryunBattleInfo(playerCharacter_t* pToPlayer,const int time)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{		
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTEL_PRACTICE_INFO);
		MSG_WriteLong(&netMessage, time);	
   
		NET_SendMessage ( &(pToPlayer->sock), &netMessage );
	}
	MSG_EndWriting(&netMessage);
}




void CGonryunPractice_Encoder::SendGonryunBattleResult(playerCharacter_t* pPlayer,int result,char* pname)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{		
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTEL_PRACTICE_RESULT);
		MSG_WriteByte(&netMessage, result);
		MSG_WriteString(&netMessage,pname);
		NET_SendMessage ( &pPlayer->sock, &netMessage );
	}

	MSG_EndWriting( &netMessage );	
}


void CGonryunPractice_Encoder::SendMessage_NormalReject_toPlayer(playerCharacter_t* pLeaderPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{		
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_GENERATE_REJECT);
		NET_SendMessage ( &pLeaderPlayer->sock, &netMessage );
	}
	MSG_EndWriting(&netMessage);	
}


void CGonryunPractice_Encoder::SendMessageBattleReject(	playerCharacter_t* pLeaderPlayer )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{		
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTING_REJECT);
		NET_SendMessage ( &pLeaderPlayer->sock, &netMessage );
	}
	MSG_EndWriting(&netMessage);	
}


void CGonryunPractice_Encoder::
SendBattleMsg(
	playerCharacter_t* pPlayer, 
	const enum tagGonryunPracticePacket_BATTLE_PRACTICE_MSG::enumCode Code,
	const int Value1,
	const int Value2,
	const int Value3) const
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{		
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_MSG);
		MSG_WriteLong(&netMessage, Code);
		MSG_WriteLong(&netMessage, Value1);
		MSG_WriteLong(&netMessage, Value2);
		MSG_WriteLong(&netMessage, Value3);
		NET_SendMessage ( &pPlayer->sock , &netMessage );
	}
	MSG_EndWriting(&netMessage);	
}


void CGonryunPractice_Encoder::SendMessageNake(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{		
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTEL_PRACTICE_NAKE_RESULT);
		MSG_WriteLong(&netMessage, pPlayer->curChargeSE);
		NET_SendMessage ( &pPlayer->sock, &netMessage );
	}
	MSG_EndWriting(&netMessage);
}

void CGonryunPractice_Encoder::
SendReqeust_NomalBattle_toOpenent(
	const int   LeaderPCIdx,
	char* szpLeaderName,	
	playerCharacter_t* pOpenentPlayer)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{		
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_REQUEST_BATTELGENERATE);
			
		MSG_WriteLong(&netMessage,LeaderPCIdx);
		MSG_WriteString(&netMessage,szpLeaderName);	
		NET_SendMessage ( &pOpenentPlayer->sock, &netMessage );
	}
	MSG_EndWriting(&netMessage);	
}

void CGonryunPractice_Encoder::
SendWaitUserList(playerCharacter_t* pLeaderPlayer,playerCharacter_t* pToPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_GET_OTHERLISTINFO);
		MSG_WriteByte(&netMessage,m_cpVirtualWaiterCtrl->GetNum()+1);
		MSG_WriteByte(&netMessage,g_logic.PCBGonryunCondition.m_idifferLevel);
		MSG_WriteString(&netMessage,pLeaderPlayer->name);
		MSG_WriteShort(&netMessage, TRUE);	
		MSG_WriteByte(&netMessage, pLeaderPlayer->GonryunBattlePractice.Record.iwin);		
		MSG_WriteByte(&netMessage, pLeaderPlayer->GonryunBattlePractice.Record.idraw);		
		MSG_WriteByte(&netMessage, pLeaderPlayer->GonryunBattlePractice.Record.idefeat);
		MSG_WriteByte(&netMessage, pLeaderPlayer->level);
		
		CGonryunPractice_WaiterCtrl::WAIT_ITOR	itor;
		playerCharacter_t* pWaitPlayer = NULL;
		
		for( itor = (m_cpVirtualWaiterCtrl->m_WaitUserList).begin(); itor != (m_cpVirtualWaiterCtrl->m_WaitUserList).end(); itor++ ){
		
			pWaitPlayer = m_cpVirtualWaiterCtrl->GetWaiter(*itor);
			
			if(NULL == pWaitPlayer ) continue;

			MSG_WriteString(&netMessage,pWaitPlayer->name);	
			MSG_WriteShort(&netMessage, FALSE);	
			MSG_WriteByte(&netMessage, pWaitPlayer->GonryunBattlePractice.Record.iwin);		
			MSG_WriteByte(&netMessage, pWaitPlayer->GonryunBattlePractice.Record.idraw);		
			MSG_WriteByte(&netMessage, pWaitPlayer->GonryunBattlePractice.Record.idefeat);
			MSG_WriteByte(&netMessage, pWaitPlayer->level);
			
		}
		NET_SendMessage( &pToPlayer->sock, &netMessage);		
	}
	MSG_EndWriting(&netMessage);
}

void CGonryunPractice_Encoder::
SendMyInfoToUser(
	 playerCharacter_t* pLeaderPlayer,
	 playerCharacter_t* pSuggestPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{

		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTEL_PRACTICE_MYINFO_TOUSER);
		MSG_WriteString(&netMessage,pSuggestPlayer->name);	
		MSG_WriteByte(&netMessage, pSuggestPlayer->GonryunBattlePractice.Record.iwin);		
		MSG_WriteByte(&netMessage, pSuggestPlayer->GonryunBattlePractice.Record.idraw);		
		MSG_WriteByte(&netMessage, pSuggestPlayer->GonryunBattlePractice.Record.idefeat);
		MSG_WriteByte(&netMessage, pSuggestPlayer->level);


		NET_SendMessage( &pLeaderPlayer->sock, &netMessage);		
		CGonryunPractice_WaiterCtrl::WAIT_ITOR	itor;
		playerCharacter_t* pWaitPlayer = NULL;		
		for( itor = (m_cpVirtualWaiterCtrl->m_WaitUserList).begin(); itor != (m_cpVirtualWaiterCtrl->m_WaitUserList).end(); itor++ ){
		
			pWaitPlayer = m_cpVirtualWaiterCtrl->GetWaiter(*itor);
		
			if(NULL == pWaitPlayer ) continue;			
		
			if(pSuggestPlayer == pWaitPlayer) continue;

			NET_SendMessage( &pWaitPlayer->sock, &netMessage);			
		}			
	}
	MSG_EndWriting(&netMessage);
}

void	CGonryunPractice_Encoder::SendDeleteUser(playerCharacter_t* pPlayer)
{
	playerCharacter_t* pLeaderPlayer = m_cpVirtualWaiterCtrl->GetWaiter(m_cpVirtualProcessCtrl->m_Leader);
	if( NULL == pLeaderPlayer )
	{
		
		SendWaitUserBattleEnd();
		m_cpVirtualProcessCtrl->Init();	
		return;
	}

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTEL_PRACTICE_DELETE_USER);
		MSG_WriteString(&netMessage,pPlayer->name);	
		NET_SendMessage( &pLeaderPlayer->sock, &netMessage);	
	
		CGonryunPractice_WaiterCtrl::WAIT_ITOR	itor;
		playerCharacter_t* pWaitPlayer = NULL;
		
		for( itor = (m_cpVirtualWaiterCtrl->m_WaitUserList).begin(); itor != (m_cpVirtualWaiterCtrl->m_WaitUserList).end(); itor++ ){
		
			pWaitPlayer = m_cpVirtualWaiterCtrl->GetWaiter(*itor);
		
			if(NULL == pWaitPlayer ) continue;
		
			if( 0 == strncmp(pWaitPlayer->name,pPlayer->name,NAMESTRING)) continue;
		
			NET_SendMessage( &pWaitPlayer->sock, &netMessage);	
		}
	}
	MSG_EndWriting(&netMessage);
}

void CGonryunPractice_Encoder::
SendMessage_BattingTrade_OkBatting( playerCharacter_t* pPlayer, BOOL type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_OK);
		MSG_WriteByte( &netMessage, type );
		NET_SendMessage( &pPlayer->sock, &netMessage );
	}
	MSG_EndWriting(&netMessage);
}


void CGonryunPractice_Encoder::
SendMessage_BattingTrade_RequestBatting( playerCharacter_t* pPlayer, playerCharacter_t* ptarget, int type  )
{
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_REQUEST);
		MSG_WriteShort(&netMessage, ptarget->idx);
		MSG_WriteByte(&netMessage, type);
		MSG_WriteString(&netMessage, ptarget->name );
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}
 
void CGonryunPractice_Encoder::
SendMessage_BattingTrade_AdmitBatting( playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_RESULT);
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void CGonryunPractice_Encoder::
SendMessage_BattingTrade_RejectBatting( playerCharacter_t* player, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_REJECT);
		MSG_WriteByte(&netMessage, type);
		NET_SendMessage(&player->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void CGonryunPractice_Encoder::
SendMessage_BattingTrade_ExitBatting( playerCharacter_t* player)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_EXIT);
		NET_SendMessage(&player->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void CGonryunPractice_Encoder::
SendMessage_BattingTrade_CancelBatting( playerCharacter_t* player, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_CANCEL);
		MSG_WriteByte(&netMessage, type);
		NET_SendMessage(&player->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void CGonryunPractice_Encoder::
SendMessage_BattingTrade_ConfirmBatting( 
	playerCharacter_t* player, 
	const enum tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::enumState& state,	
	const enum tagGonryunPracticePacket_BATTLE_PRACTICE_BATTINGTRADE_CONFIRM::errorCode& Code)
{

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_CONFIRM);
		MSG_WriteByte(&netMessage, state);
		MSG_WriteByte(&netMessage, Code);
		NET_SendMessage(&player->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void CGonryunPractice_Encoder::
SendMessage_BattingTrade_SEBatting( playerCharacter_t* player, int error, int type, int se )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_SE);
		MSG_WriteByte(&netMessage, type);
		MSG_WriteLong(&netMessage, se);
		MSG_WriteByte(&netMessage, error);
		
		NET_SendMessage(&player->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}









CGonryunPracticeBattle* CGonryunPractice_Encoder::GetPracticeBattlePointer()const
{
	return m_cpVirtualPracticeBattle;
}

CGonryunPractice_ProcessCtrl* CGonryunPractice_Encoder::GetProcessCtrl()const
{
	return m_cpVirtualProcessCtrl;
}



void CGonryunPractice_Encoder::SendMessage_Open(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_ANSWER_GONRYUNBATTLEPRACTICE);
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_OPEN);
		MSG_WriteLong(&netMessage, g_logic.PCBGonryunCondition.m_iroyalty);
		MSG_WriteLong(&netMessage, g_logic.PCBGonryunCondition.m_ibattingRoyalty);
		NET_SendMessage(&(pPlayer->sock), &netMessage);
	}
	MSG_EndWriting(&netMessage);

}



int CGonryunPractice_Encoder::SendPCEventMessage_DieGonryun( playerCharacter_t* pPlayer )
{
	
	playerCharacter_t* pTarketPlayer = NULL;
	
	GTH_GetPCListInValidRange(pPlayer->worldIdx, pPlayer->position);
	if (g_zonePCListNumber < 1) return false;

	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EVENT_DIE);
		MSG_WriteShort(&netMessage, pPlayer->idx);
		MSG_WriteByte(&netMessage, pPlayer->entityType);
		
		MSG_WriteShort(&netMessage, pPlayer->curDamage);
		MSG_WriteByte(&netMessage, pPlayer->lastAttackState);
		
		MSG_WriteShort(&netMessage, pPlayer->killerIdx);
		MSG_WriteByte(&netMessage, pPlayer->killerType);
		MSG_Write64Int(&netMessage, pPlayer->exp );
		MSG_WriteLong(&netMessage, pPlayer->curChargeSE);
		
		MSG_WriteShort(&netMessage, -1);
		MSG_WriteShort(&netMessage, 0);
		
		
		for (int i=0; i < g_zonePCListNumber; i++)
		{
			pTarketPlayer = gcpTools->GetPlayerRecordPointer(g_zonePCList[i].idx);
			if ( NULL == pTarketPlayer ) continue;
			
			
			
			
			NET_SendUnreliableMessage(&pTarketPlayer->sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
	return 1;
}

