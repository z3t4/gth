




#include "../../Global.h"
#include "CTerraBillingCtrl_Decoder.h"

#include "tagAddBill.h"

#include "CTerraBillingCtrl_Encoder.h"
extern CTerraBillingCtrl_Encoder* gcpTerraBillingCtrl_Encder;

#ifdef _MASTER_SERVER


#endif




CTerraBillingCtrl_Decoder::CTerraBillingCtrl_Decoder()
{
	m_bOK=FALSE;

	m_bOK=TRUE;

}





CTerraBillingCtrl_Decoder::~CTerraBillingCtrl_Decoder()
{

}





BOOL CTerraBillingCtrl_Decoder::isOK(void) const
{
	return m_bOK; 

}






#ifdef _GAME_SERVER
inline playerCharacter_t* CTerraBillingCtrl_Decoder::GetPlayerRecodPointer(const struct tagAddBillPacket& Packet) const
{
	char strIdx[tagAddBillPacket::maxbytes_session+1];
	char strPlayerID[IDSTRING+1];

	strncpy(strIdx,Packet.Session,tagAddBillPacket::maxbytes_session);
	strIdx[tagAddBillPacket::maxbytes_session]=NULL;

	strncpy(strPlayerID,Packet.User_ID,IDSTRING);
	strPlayerID[IDSTRING]=NULL;

	
	
	int Idx=atoi(strIdx);
	if(Idx < 0) return NULL;
	if(Idx > MAX_PCS) return NULL;

	playerCharacter_t* pPlayerRecord=&g_pc[Idx]; 	
	if(FALSE == pPlayerRecord->active) return NULL;
	if(FALSE == pPlayerRecord->ready) return NULL;

	if(0 != strcmp(strPlayerID,pPlayerRecord->userID)) return NULL;
	
	return pPlayerRecord;


}
#endif






#ifdef _GAME_SERVER
void CTerraBillingCtrl_Decoder::
Transmite_TerraGCPacket_Remain_Minute_NoticeMessage(
	playerCharacter_t* pToPlayerRecord,
	const int RemainMinute) const
{
	MSG_BeginWriting(&netMessage);
	
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_EXTEND_TERRA);
		MSG_WriteByte(&netMessage, tagTerraGCPacket::Remain_Minute_NoticeMessage);
		MSG_WriteLong(&netMessage, RemainMinute);		

		NET_SendMessage(&pToPlayerRecord->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}
#endif





#ifdef _GAME_SERVER
void CTerraBillingCtrl_Decoder::
Transmite_TerraGCPacket_NoticeMessage(
	playerCharacter_t* pToPlayerRecord,
	enum tagTerraGCPacket_NoticeMessage::enumLPACKType LPACKType,
	const int LPACKIdx) const
{
	MSG_BeginWriting(&netMessage);
	
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_EXTEND_TERRA);
		MSG_WriteByte(&netMessage, tagTerraGCPacket::NoticeMessage);

		MSG_WriteByte(&netMessage, LPACKType);
		MSG_WriteByte(&netMessage, LPACKIdx);

		NET_SendMessage(&pToPlayerRecord->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}
#endif





#ifdef _GAME_SERVER
void CTerraBillingCtrl_Decoder::
Transmite_TerraGCPacket_OpenMessageBox(
	playerCharacter_t* pToPlayerRecord,
	enum tagTerraGCPacket_OpenMessageBox::enumLPACKType LPACKType,
	const int LPACKIdx,
	enum tagTerraGCPacket_OpenMessageBox::enumAfter after) const
{
	MSG_BeginWriting(&netMessage);
	
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_EXTEND_TERRA);
		MSG_WriteByte(&netMessage, tagTerraGCPacket::Open_MessageBox);		

		MSG_WriteByte(&netMessage, LPACKType);
		MSG_WriteByte(&netMessage, LPACKIdx);
		MSG_WriteByte(&netMessage, after);

		NET_SendMessage(&pToPlayerRecord->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}
#endif






#ifdef _GAME_SERVER
inline void CTerraBillingCtrl_Decoder::
Decoder_game_start(const struct tagAddBillPacket& Packet) const
{

	playerCharacter_t* pPlayerRecord=GetPlayerRecodPointer(Packet);
	if(NULL == pPlayerRecord){
		gcpTerraBillingCtrl_Encder->Transmite_Game_End(Packet);
		return;
	}


	switch(ntohl(Packet.Packet_Result)){
		case tagAddBillPacket_Game_Start::enumResult::success:
			pPlayerRecord->AddBill.step = playerCharacter_t::tagAddBill::step_billing;
			break;

		case tagAddBillPacket_Game_Start::enumResult::already_connected:
			Transmite_TerraGCPacket_OpenMessageBox(
				pPlayerRecord, 
				tagTerraGCPacket_OpenMessageBox::enumLPACKType::Billing,
				1,
				tagTerraGCPacket_OpenMessageBox::enumAfter::after_client_exit);
			GTH_DisconnectGameServer(pPlayerRecord->idx);
			break;

		case tagAddBillPacket_Game_Start::enumResult::illegal_user:
		case tagAddBillPacket_Game_Start::enumResult::shortage_point:
		case tagAddBillPacket_Game_Start::enumResult::not_exist_authorization:
			Transmite_TerraGCPacket_OpenMessageBox(
				pPlayerRecord, 
				tagTerraGCPacket_OpenMessageBox::enumLPACKType::Billing,
				2, 
				tagTerraGCPacket_OpenMessageBox::enumAfter::after_client_exit);
			GTH_DisconnectGameServer(pPlayerRecord->idx);
			break;


		case tagAddBillPacket_Game_Start::enumResult::access_deny_db:
		case tagAddBillPacket_Game_Start::enumResult::network_error:
		case tagAddBillPacket_Game_Start::enumResult::not_exist_session:
		case tagAddBillPacket_Game_Start::enumResult::unknown_packet:
		case tagAddBillPacket_Game_Start::enumResult::error_com:
		case tagAddBillPacket_Game_Start::enumResult::error_timeout_response_billserver:
		case tagAddBillPacket_Game_Start::enumResult::connect_fail_billingDB:
		case tagAddBillPacket_Game_Start::enumResult::unknown_game_no:
		case tagAddBillPacket_Game_Start::enumResult::error_billing_account_service_module:
		case tagAddBillPacket_Game_Start::enumResult::not_exist_authorization_this_game:
		case tagAddBillPacket_Game_Start::enumResult::error_busy:
		case tagAddBillPacket_Game_Start::enumResult::unknown:
		case tagAddBillPacket_Game_Start::enumResult::com_error_when_ip_check:
		case tagAddBillPacket_Game_Start::enumResult::not_enougf_service_provider:
		case tagAddBillPacket_Game_Start::enumResult::com_error_when_authorization_key_check:
		case tagAddBillPacket_Game_Start::enumResult::not_found_authorization_key:
		default:
			Transmite_TerraGCPacket_OpenMessageBox(
				pPlayerRecord, 
				tagTerraGCPacket_OpenMessageBox::enumLPACKType::Billing,
				3,
				tagTerraGCPacket_OpenMessageBox::enumAfter::after_client_exit);
			GTH_DisconnectGameServer(pPlayerRecord->idx);
			break;
	}

}
#endif






#ifdef _GAME_SERVER
inline void CTerraBillingCtrl_Decoder::
Decoder_user_sync(const struct tagAddBillPacket& Packet) const
{
	playerCharacter_t* pPlayerRecord=GetPlayerRecodPointer(Packet);
	if(NULL == pPlayerRecord){
		gcpTerraBillingCtrl_Encder->Transmite_User_Sync(Packet);
		return;
	}

	gcpTerraBillingCtrl_Encder->Transmite_User_Sync(pPlayerRecord);
}
#endif






#ifdef _GAME_SERVER
inline void CTerraBillingCtrl_Decoder::
Decoder_user_alert(const struct tagAddBillPacket& Packet) const
{
	static char message[_MAX_PATH];
	playerCharacter_t* pPlayerRecord=GetPlayerRecodPointer(Packet);
	if(NULL == pPlayerRecord) return;


	switch(ntohl(Packet.Packet_Result)){
		case tagAddBillPacket_User_Alert::enumResult::remain_time:
			Transmite_TerraGCPacket_Remain_Minute_NoticeMessage(
				pPlayerRecord,
				ntohl(Packet.Bill_Remain));			
			return;

		case tagAddBillPacket_User_Alert::enumResult::now_log_off:
			Transmite_TerraGCPacket_OpenMessageBox(
				pPlayerRecord, 
				tagTerraGCPacket_OpenMessageBox::enumLPACKType::Billing,
				4,
				tagTerraGCPacket_OpenMessageBox::enumAfter::after_client_exit);
			GTH_DisconnectGameServer(pPlayerRecord->idx);
			return;


		case tagAddBillPacket_User_Alert::enumResult::refund:
			Transmite_TerraGCPacket_OpenMessageBox(
				pPlayerRecord, 
				tagTerraGCPacket_OpenMessageBox::enumLPACKType::Billing,
				5,
				tagTerraGCPacket_OpenMessageBox::enumAfter::after_client_exit);
			GTH_DisconnectGameServer(pPlayerRecord->idx);
			return;

		case tagAddBillPacket_User_Alert::enumResult::double_login:
		   Transmite_TerraGCPacket_OpenMessageBox(
				pPlayerRecord, 
				tagTerraGCPacket_OpenMessageBox::enumLPACKType::Billing,
				1,
				tagTerraGCPacket_OpenMessageBox::enumAfter::after_client_exit);
			GTH_DisconnectGameServer(pPlayerRecord->idx);
			return;

		case tagAddBillPacket_User_Alert::enumResult::force_exit:
			Transmite_TerraGCPacket_OpenMessageBox(
				pPlayerRecord, 
				tagTerraGCPacket_OpenMessageBox::enumLPACKType::Billing,
				6,
				tagTerraGCPacket_OpenMessageBox::enumAfter::after_client_exit);
			GTH_DisconnectGameServer(pPlayerRecord->idx);
			return;


		case tagAddBillPacket_User_Alert::enumResult::start_reserved_bill:
			Transmite_TerraGCPacket_NoticeMessage(
				pPlayerRecord,
				tagTerraGCPacket_NoticeMessage::enumLPACKType::Billing,
				7);
			return;		
	}
	
	
}
#endif




inline void CTerraBillingCtrl_Decoder::
Decoder_server_alive(const struct tagAddBillPacket& ) const
{
	
	
	
	
	
	
	
	
	

	
	
	

}




void CTerraBillingCtrl_Decoder::Decoder(const struct tagAddBillPacket& Packet)
{
	switch(ntohl(Packet.Packet_Type)){
		case tagAddBillPacket::packet_type_server_alive:
			Decoder_server_alive(Packet);
			break;


		#ifdef _GAME_SERVER
		case tagAddBillPacket::packet_type_user_alert:
			Decoder_user_alert(Packet);
			break;
		
		case tagAddBillPacket::packet_type_user_sync:
			Decoder_user_sync(Packet);
			break;
			
		case tagAddBillPacket::packet_type_game_start:
			Decoder_game_start(Packet);
			break;
		#endif


		#ifdef _MASTER_SERVER
		case tagAddBillPacket::packet_type_billing_authorization:
			Decoder_billing_authorization(Packet);
			break;			
		#endif
	}
}





#ifdef _MASTER_SERVER
inline i3client_t* CTerraBillingCtrl_Decoder::GetPlayerRecodPointer(const struct tagAddBillPacket& Packet) const
{
	char strIdx[tagAddBillPacket::maxbytes_session+1];
	char strPlayerID[IDSTRING+1];


	strncpy(strIdx,Packet.Session,tagAddBillPacket::maxbytes_session);
	strIdx[tagAddBillPacket::maxbytes_session]=NULL;
	if(NULL != strchr(strIdx,'-')) *strchr(strIdx,'-')=NULL;
	int Idx=atoi(strIdx);
	if(Idx < 0) return NULL;
	if(Idx > MAX_CLIENTS) return NULL;

	strncpy(strPlayerID,Packet.User_ID,IDSTRING);
	strPlayerID[IDSTRING]=NULL;
	

	i3client_t* pPlayerRecord=&g_clients[Idx]; 	
	if(!pPlayerRecord->active) return NULL;
	if(0 != strcmp(strPlayerID,pPlayerRecord->id)) return NULL;
	
	return pPlayerRecord;
};
#endif





#ifdef _MASTER_SERVER
inline void CTerraBillingCtrl_Decoder::Decoder_billing_authorization(const struct tagAddBillPacket& Packet) const
{


	i3client_t* pPlayerRecord = GetPlayerRecodPointer(Packet);
	if(NULL == pPlayerRecord){
		
		return;
	}

	int result=ntohl(Packet.Packet_Result);

	switch(result){
		case tagAddBillPacket::enumResult_billing_authorization::network_error:
			GTH_ReplyLoginV2(pPlayerRecord,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::billing_network_error);
			return;

		case tagAddBillPacket::enumResult_billing_authorization::success:

			
			GTH_ReplyLoginV2(pPlayerRecord,TRUE,tagMCPaket_MSC_REPLYLOGIN::enumError::none);
			g_DBAccountServer->InsertLogin(pPlayerRecord);			
			return;

		case tagAddBillPacket::enumResult_billing_authorization::already_connected:
			
			GTH_ReplyLoginV2(pPlayerRecord,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::double_login);
			return;


		case tagAddBillPacket::enumResult_billing_authorization::illegal_user:
		case tagAddBillPacket::enumResult_billing_authorization::shortage_point:
		case tagAddBillPacket::enumResult_billing_authorization::not_exist_authorization:
			
			GTH_ReplyLoginV2(pPlayerRecord,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::billing_point_not_enough);
			return;
		
		case tagAddBillPacket::enumResult_billing_authorization::access_deny_db:
		
		




		case tagAddBillPacket::enumResult_billing_authorization::not_exist_session:
		case tagAddBillPacket::enumResult_billing_authorization::unknown_packet:
		case tagAddBillPacket::enumResult_billing_authorization::error_com:
		case tagAddBillPacket::enumResult_billing_authorization::error_timeout_response_billserver:
		case tagAddBillPacket::enumResult_billing_authorization::connect_fail_billingDB:
		case tagAddBillPacket::enumResult_billing_authorization::unknown_game_no:
		case tagAddBillPacket::enumResult_billing_authorization::error_billing_account_service_module:
		case tagAddBillPacket::enumResult_billing_authorization::not_exist_authorization_this_game:
		case tagAddBillPacket::enumResult_billing_authorization::error_busy:
		case tagAddBillPacket::enumResult_billing_authorization::unknown:
		case tagAddBillPacket::enumResult_billing_authorization::com_error_when_ip_check:
		case tagAddBillPacket::enumResult_billing_authorization::not_enougf_service_provider:
		case tagAddBillPacket::enumResult_billing_authorization::com_error_when_authorization_key_check:
		case tagAddBillPacket::enumResult_billing_authorization::not_found_authorization_key:
			break;
	}	

	
	GTH_ReplyLoginV2(pPlayerRecord,FALSE,tagMCPaket_MSC_REPLYLOGIN::enumError::unknown);
	return;

}
#endif


