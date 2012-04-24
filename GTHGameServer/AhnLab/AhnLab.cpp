




#include "../Global.h"
#include "AhnLab.h"
#include "AntiCpSvrFunc.h"
#include "..\Tools\CTools.h"
extern CTools* gcpTools;


extern void TransmitePacket_sc_disconnect(playerCharacter_t* pPlayer, int MessageIdx =0);
extern void GTH_DisconnectPlayer(playerCharacter_t* pPlayer,const BOOL bForceQuit);










CAhnLab::CAhnLab(const char* filename)
{	
	m_bOK			= FALSE;
	m_pBasePointer	= NULL;
	m_dmMemSize		= 0;
	m_NextSendTime	= 0;
	m_CheckPcIdx	= 0;
	
	if ( NULL == filename)
		return;	

	DWORD dwRet = _AntiCpSvr_Initialize( filename, &m_pBasePointer, &m_dmMemSize);
	if( dwRet != ERROR_SUCCESS )
	{
		TRACE("_AntiCp_Initialize failed. %x", dwRet);
		return;
	}
	m_bOK = TRUE;
}

CAhnLab::~CAhnLab()
{
	_AntiCpSvr_Finalize(m_pBasePointer);
}

BOOL CAhnLab::IsOK()
{
	return m_bOK;
}






void CAhnLab::UpdateAhnLab()
{		
	if ( FALSE == IsOK()) 	return;
	if ( NULL == m_pBasePointer) return;
	if ( g_globalTime < m_NextSendTime )	
		return;

	int nSendCount = 0;
	for (int nIdx = 0; nIdx < MAX_PCS ; nIdx++, m_CheckPcIdx++)	
	{
		playerCharacter_t* pPlayer = NULL;
		if ( m_CheckPcIdx >= MAX_PCS) m_CheckPcIdx = 0;			
		pPlayer =gcpTools->GetPlayerRecordPointer(m_CheckPcIdx);
		if(NULL == pPlayer) continue;
		if(FALSE == gcpTools->isReady(pPlayer)) continue;		

		
		if ( pPlayer->m_AhnLabMsg.MessageSend == TRUE)
			continue;		


		playerCharacter_t::sUserHackAckMsg* Msg = NULL;	
		Msg = &(pPlayer->m_AhnLabMsg);
		if ( NULL == Msg ) return;
		
		memset(Msg->byReqInfo, 0, ANTICPSVRREQLEN);
		memset(Msg->byReqMsg,  0, ANTICPSVRREQLEN);
		memset(Msg->byAckMsg,  0, ANTICPCNTACKLEN);	
		
		DWORD dwRet = _AntiCpSvr_MakeReqMsg(m_pBasePointer, m_dmMemSize, 
			Msg->byReqMsg, Msg->byReqInfo);
		
		if ( dwRet != ERROR_SUCCESS )
			continue;	
		
		GTH_SendMessageAhnLabMessage(pPlayer);
		pPlayer->m_AhnLabMsg.MessageSend = TRUE;
		pPlayer->m_AhnLabMsg.SendTime = g_globalTime;		
		pPlayer->m_AhnLabMsg.SendCount += 1;

		nSendCount++;
		
		if ( nSendCount > PROCESSNUMperFRAME)
			break;
	}	
	
	m_NextSendTime = g_globalTime + SEND_DELAY;	

}






void CAhnLab::GTH_SendMessageAhnLabMessage(playerCharacter_t *pPlayer)
{
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,  EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, AHNLAB);

		for (int nMsgIdx = 0; nMsgIdx < ANTICPSVRREQLEN; nMsgIdx++)
			MSG_WriteByte(&netMessage, pPlayer->m_AhnLabMsg.byReqMsg[nMsgIdx]);

		NET_SendUnreliableMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void CAhnLab::GTH_ProcessMessageAhnLabMessage(playerCharacter_t *pPlayer)
{
	if ( NULL == pPlayer)
		return;	





	
	for (int nMsgIdx = 0; nMsgIdx < ANTICPCNTACKLEN; nMsgIdx++)
		pPlayer->m_AhnLabMsg.byAckMsg[nMsgIdx] = MSG_ReadByte();

	if ( pPlayer->m_AhnLabMsg.MessageSend == FALSE )
		return;

	DWORD dwRet	= _AntiCpSvr_AnalyzeAckMsg(pPlayer->m_AhnLabMsg.byAckMsg, 
		m_pBasePointer, pPlayer->m_AhnLabMsg.byReqInfo);
	
	switch( dwRet )
	{
		
	case ERROR_ANTICPSVR_ANALACKMSG_HSHIELD_ERROR:
		{
			TransmitePacket_sc_disconnect(pPlayer, ERROR_ANTICPSVR_ANALACKMSG_HSHIELD_ERROR);
			GTH_DisconnectPlayer(pPlayer,FALSE);
			g_logSystem->Write("User [%s] was HackShield Error(1). Closed.", pPlayer->userID);
		}break;

		
	case ERROR_ANTICPSVR_ANALACKMSG_PACKET_ERROR:
		{
			TransmitePacket_sc_disconnect(pPlayer, ERROR_ANTICPSVR_ANALACKMSG_PACKET_ERROR);
			GTH_DisconnectPlayer(pPlayer,FALSE);		
			g_logSystem->Write("User [%s] was HackShield Error(2). Closed.", pPlayer->userID);
		}break;

		
	case ERROR_ANTICPSVR_ANALACKMSG_FILE_ERROR:
		{
			TransmitePacket_sc_disconnect(pPlayer, ERROR_ANTICPSVR_ANALACKMSG_FILE_ERROR);
			GTH_DisconnectPlayer(pPlayer,FALSE);		
			g_logSystem->Write("User [%s] was HackShield Error(3). Closed.", pPlayer->userID);
		}break;
	}	
		
	pPlayer->m_AhnLabMsg.MessageSend = FALSE;
	pPlayer->m_AhnLabMsg.SendCount = 0;
}







void	CAhnLab::GTH_CheckReciveTIme(playerCharacter_t *pPlayer)
{
	if ( pPlayer->m_AhnLabMsg.MessageSend == FALSE)
		return;

	if ( (g_globalTime - pPlayer->m_AhnLabMsg.SendTime ) > DisConnectTIme )
	{
		if ( pPlayer->m_AhnLabMsg.SendCount < AhnLabMessageSendCount )		
		{			
			GTH_SendMessageAhnLabMessage(pPlayer);
			pPlayer->m_AhnLabMsg.MessageSend = TRUE;
			pPlayer->m_AhnLabMsg.SendTime = g_globalTime;
			pPlayer->m_AhnLabMsg.SendCount += 1;
			return;

		}
		TransmitePacket_sc_disconnect(pPlayer, ERROR_ANTICPSVR_ANALACKMSG_PACKET_ERROR);
		GTH_DisconnectPlayer(pPlayer,FALSE);		
		g_logSystem->Write("User [%s] was HackShield Error(2). Closed.", pPlayer->userID);
	}	
}