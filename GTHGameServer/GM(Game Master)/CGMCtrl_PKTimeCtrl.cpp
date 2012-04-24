



#include "..\global.h"
#include "CGMCtrl_PKTimeCtrl.h"

#include "..\CTools.h"
extern CTools* gcpTools;

#include <assert.h>





CGMCtrl_PKTimeCtrl::CGMCtrl_PKTimeCtrl(CGMCtrl_ServerStateDataCtrl*	cpServerStateDataCtrl)
{
	m_bOK=FALSE;
	assert(NULL != cpServerStateDataCtrl);
	m_cpVirtualServerStateDataCtrl = cpServerStateDataCtrl;
	
	m_bOK=TRUE;
}





CGMCtrl_PKTimeCtrl::~CGMCtrl_PKTimeCtrl()
{

}





BOOL CGMCtrl_PKTimeCtrl::isOK(void) const
{
	return m_bOK;
}







inline void CGMCtrl_PKTimeCtrl::TransmitePacket_msgs_notify_change(void)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_PKtime);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagPKtime::msgs_notify_change);
		MSG_WriteLong(&netMessage,m_cpVirtualServerStateDataCtrl->isPK());

		for(int i=1; i < MAX_MEMBER_SERVER; i++){ 
			if ( !g_memberServer[i].active )	continue;
			NET_SendUnreliableMessage(&g_memberServer[i].sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);

}




inline void CGMCtrl_PKTimeCtrl::TransmitePacket_gsms_request_change(const BOOL bPK)
{

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_PKtime);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagPKtime::gsms_request_change);
		MSG_WriteLong(&netMessage,bPK);
		NET_SendUnreliableMessage(&localSocket, &netMessage);		
	}
	MSG_EndWriting(&netMessage);
}







inline void CGMCtrl_PKTimeCtrl::TransmitePacket_sc_notify_change(void)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_PKtime);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagPKtime::sc_notify_change);
		MSG_WriteLong(&netMessage,m_cpVirtualServerStateDataCtrl->isPK());

		playerCharacter_t* pTempPlayer=NULL;
		for(int pcIdx = 0; pcIdx < MAX_PCS; pcIdx++ ){
			pTempPlayer = gcpTools->GetPlayerRecordPointer(pcIdx);
			if(NULL == pTempPlayer) continue;			

			NET_SendMessage(&pTempPlayer->sock, &netMessage);		
		}	
	}

	MSG_EndWriting(&netMessage);
}




inline void CGMCtrl_PKTimeCtrl::Decoder_gsms_request_change(void)
{
	
	if(TRUE == MSG_ReadLong()) m_cpVirtualServerStateDataCtrl->SetPK(TRUE);
	else m_cpVirtualServerStateDataCtrl->SetPK(FALSE);
	m_cpVirtualServerStateDataCtrl->SaveODBC();	

	TransmitePacket_sc_notify_change();
	TransmitePacket_msgs_notify_change();
}








inline void CGMCtrl_PKTimeCtrl::Decoder_cs_request_change(playerCharacter_t* pPlayer)
{
	BOOL bPK=FALSE;	
	if(TRUE == MSG_ReadLong()) bPK=TRUE;

	if(g_config.isManager){
		m_cpVirtualServerStateDataCtrl->SetPK(bPK);
		m_cpVirtualServerStateDataCtrl->SaveODBC();			
		
		TransmitePacket_sc_notify_change();
		TransmitePacket_msgs_notify_change();
	}else{		
		TransmitePacket_gsms_request_change(bPK);
	}
}




inline void CGMCtrl_PKTimeCtrl::Decoder_msgs_notify_change(void)
{
	BOOL bPK=FALSE;
	if(TRUE == MSG_ReadLong()) bPK=TRUE;

	m_cpVirtualServerStateDataCtrl->SetPK(bPK);
	TransmitePacket_sc_notify_change();
}




inline void CGMCtrl_PKTimeCtrl::TransmitePacket_sc_response_status(playerCharacter_t* pPlayer)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );

	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);		
		MSG_WriteShort(&netMessage, GMsystem);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::GMsystem_PKtime);
		MSG_WriteShort(&netMessage, tagExtendSecondPacket_GMsystem::tagPKtime::sc_response_status);
		MSG_WriteLong(&netMessage, m_cpVirtualServerStateDataCtrl->isPK());
		NET_SendMessage(&pPlayer->sock, &netMessage);		
	}

	MSG_EndWriting(&netMessage);
}




inline void CGMCtrl_PKTimeCtrl::Decoder_cs_request_status(playerCharacter_t* pPlayer)
{
	if(NULL == pPlayer) return;

	TransmitePacket_sc_response_status(pPlayer);
}




void CGMCtrl_PKTimeCtrl::Decoder(playerCharacter_t* pPlayer)
{
	switch(MSG_ReadShort()){
		case tagExtendSecondPacket_GMsystem::tagPKtime::cs_request_change:
			Decoder_cs_request_change(pPlayer);
			break;
		case tagExtendSecondPacket_GMsystem::tagPKtime::gsms_request_change:
			Decoder_gsms_request_change();
			break;
		case tagExtendSecondPacket_GMsystem::tagPKtime::msgs_notify_change:
			Decoder_msgs_notify_change();
			break;

		case tagExtendSecondPacket_GMsystem::tagPKtime::cs_request_status:
			Decoder_cs_request_status(pPlayer);
			break;
	}

}
















