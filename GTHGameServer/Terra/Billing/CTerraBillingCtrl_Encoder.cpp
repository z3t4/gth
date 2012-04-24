



#include "../../Global.h"
#include "CTerraBillingCtrl_Encoder.h"


#include "CTerraBillingCtrl_CircularQueueCtrl.h"
extern CTerraBillingCtrl_CircularQueueCtrl*		gcpTerraBillingCtrl_TransmiteCircularQueueCtrl;


#include "tagTerraBilling.h"
extern struct tagTerraBilling gTerraBilling;




CTerraBillingCtrl_Encoder::CTerraBillingCtrl_Encoder()
{
	m_bOK=FALSE;

	m_bOK=TRUE;

}





CTerraBillingCtrl_Encoder::~CTerraBillingCtrl_Encoder()
{

}





BOOL CTerraBillingCtrl_Encoder::isOK(void) const
{
	return m_bOK; 
}





inline void CTerraBillingCtrl_Encoder::
SetSessionStr(struct tagAddBillPacket& Packet,const playerCharacter_t* pPlayer) const
{
	char szPlayerIdx[20+1]="";
	char szPlayerID[IDSTRING+1]="";

	_snprintf(szPlayerIdx,20,"%d",pPlayer->idx);
	szPlayerIdx[20]=NULL;

	_snprintf(szPlayerID,IDSTRING,"%s",pPlayer->userID);
	szPlayerID[IDSTRING]=NULL;

	SetSessionStr(Packet,szPlayerIdx,szPlayerID);	
}




inline void CTerraBillingCtrl_Encoder::
SetSessionStr(struct tagAddBillPacket& Packet,const char* pszPlayerIdx,const char* pszPlayerID) const
{
	char temp[_MAX_PATH];
	wsprintf(temp,"%s-%s-01234567890123456789012345678901",pszPlayerIdx,pszPlayerID);	
	strncpy(Packet.Session,temp,tagAddBillPacket::maxbytes_session);	
}


















void CTerraBillingCtrl_Encoder::Transmite_Server_Reset(void)
{
	struct tagAddBillPacket Packet;

	memset(&Packet,0x00,sizeof(struct tagAddBillPacket));

	Packet.Packet_Type = htonl(tagAddBillPacket::packet_type_server_reset);	
	memcpy(Packet.Game_Server,gTerraBilling.serverGUIDstring,tagAddBillPacket::maxbytes_serverGUIDstring);	
	Packet.Game_No = htonl(gTerraBilling.Game_No);

	gcpTerraBillingCtrl_TransmiteCircularQueueCtrl->Push(&Packet);
}















void CTerraBillingCtrl_Encoder::Transmite_Server_Conn(void)
{
	struct tagAddBillPacket Packet;

	memset(&Packet,0x00,sizeof(struct tagAddBillPacket));

	Packet.Packet_Type = htonl(tagAddBillPacket::packet_type_server_conn);
	
	memcpy(Packet.Game_Server,gTerraBilling.serverGUIDstring,tagAddBillPacket::maxbytes_serverGUIDstring);
	
	Packet.Game_No = htonl(gTerraBilling.Game_No);

	gcpTerraBillingCtrl_TransmiteCircularQueueCtrl->Push(&Packet);
}













#ifdef _MASTER_SERVER	
void CTerraBillingCtrl_Encoder::
Transmite_Billing_Authorization(const i3client_t* pPlayer)
{
	struct tagAddBillPacket Packet;

	memset(&Packet,0x00,sizeof(struct tagAddBillPacket));
	Packet.Packet_Type = htonl(tagAddBillPacket::packet_type_billing_authorization);

	SetSessionStr(Packet,pPlayer);	


	wsprintf(Packet.User_CC,"TICT");
	strncpy(Packet.User_ID,pPlayer->id,tagAddBillPacket::maxbytes_user_id);
	wsprintf(Packet.User_IP,"%d:%d:%d:%d",
		pPlayer->sock.addr.sin_addr.S_un.S_un_b.s_b1,
		pPlayer->sock.addr.sin_addr.S_un.S_un_b.s_b2,
		pPlayer->sock.addr.sin_addr.S_un.S_un_b.s_b3,
		pPlayer->sock.addr.sin_addr.S_un.S_un_b.s_b4);

	Packet.Game_No = htonl(gTerraBilling.Game_No);


	gcpTerraBillingCtrl_TransmiteCircularQueueCtrl->Push(&Packet);
}
#endif
















#ifdef _GAME_SERVER
void CTerraBillingCtrl_Encoder::
Transmite_Game_Start(const playerCharacter_t* pPlayerRecord) const
{
	static struct tagAddBillPacket Packet;

	memset(&Packet,0x00,sizeof(struct tagAddBillPacket));
	Packet.Packet_Type = htonl(tagAddBillPacket::packet_type_game_start);
	memcpy(Packet.Game_Server,gTerraBilling.serverGUIDstring,tagAddBillPacket::maxbytes_serverGUIDstring);

	SetSessionStr(Packet,pPlayerRecord);
	

	wsprintf(Packet.User_CC,"TICT");

	strncpy(Packet.User_ID,pPlayerRecord->userID,tagAddBillPacket::maxbytes_user_id);

	wsprintf(Packet.User_IP,"%d.%d.%d.%d",
		pPlayerRecord->sock.addr.sin_addr.S_un.S_un_b.s_b1,
		pPlayerRecord->sock.addr.sin_addr.S_un.S_un_b.s_b2,
		pPlayerRecord->sock.addr.sin_addr.S_un.S_un_b.s_b3,
		pPlayerRecord->sock.addr.sin_addr.S_un.S_un_b.s_b4);


	Packet.Game_No = htonl(gTerraBilling.Game_No);

	gcpTerraBillingCtrl_TransmiteCircularQueueCtrl->Push(&Packet);
}
#endif












#ifdef _GAME_SERVER
void CTerraBillingCtrl_Encoder::
Transmite_Game_End(const char* pszPlayerIdx,const char* pPlayerID,const char* pIPaddress,const char* pUser_Status) const
{
	static struct tagAddBillPacket Packet;

	memset(&Packet,0x00,sizeof(struct tagAddBillPacket));
	Packet.Packet_Type = htonl(tagAddBillPacket::packet_type_game_end);

	
	SetSessionStr(Packet,pszPlayerIdx,pPlayerID);	
	wsprintf(Packet.User_CC,"TICT");

	strncpy(Packet.User_ID,pPlayerID,tagAddBillPacket::maxbytes_user_id);
	strncpy(Packet.User_IP,pIPaddress,tagAddBillPacket::maxbytes_user_ip);
	strncpy(Packet.User_Status,pUser_Status,tagAddBillPacket::maxbytes_user_status);

	Packet.Game_No = htonl(gTerraBilling.Game_No);


	gcpTerraBillingCtrl_TransmiteCircularQueueCtrl->Push(&Packet);

}
#endif

#ifdef _GAME_SERVER
void CTerraBillingCtrl_Encoder::
Transmite_Game_End(const struct tagAddBillPacket& Packet) const
{
	Transmite_Game_End(Packet.Session,Packet.User_ID,Packet.User_IP,Packet.User_Status);
}
#endif

#ifdef _GAME_SERVER
void CTerraBillingCtrl_Encoder::
Transmite_Game_End(const playerCharacter_t* pPlayerRecord) const
{

	static char strPlayerIdx[_MAX_PATH];
	wsprintf(strPlayerIdx,"%d",pPlayerRecord->idx);

	static char strIPaddress[tagAddBillPacket::maxbytes_user_ip];
	wsprintf(strIPaddress,"%d.%d.%d.%d",
		pPlayerRecord->sock.addr.sin_addr.S_un.S_un_b.s_b1,
		pPlayerRecord->sock.addr.sin_addr.S_un.S_un_b.s_b2,
		pPlayerRecord->sock.addr.sin_addr.S_un.S_un_b.s_b3,
		pPlayerRecord->sock.addr.sin_addr.S_un.S_un_b.s_b4);



	Transmite_Game_End(
		strPlayerIdx,
		pPlayerRecord->userID,		
		strIPaddress,
		pPlayerRecord->AddBill.User_Status);

}
#endif


















#ifdef _GAME_SERVER
void CTerraBillingCtrl_Encoder::
Transmite_User_Sync(
	BOOL bExist,
	const char* pszPlayerIdx,
	const char* pPlayerID,
	const char* pIPaddress,
	const char* pUser_Status) const
{
	static struct tagAddBillPacket Packet;

	memset(&Packet,0x00,sizeof(struct tagAddBillPacket));
	Packet.Packet_Type = htonl(tagAddBillPacket::packet_type_user_sync);

	if(TRUE == bExist) Packet.Packet_Result = htonl(1);
	else Packet.Packet_Result = htonl(0);


	SetSessionStr(Packet,pszPlayerIdx,pPlayerID);
	

	strncpy(Packet.User_ID,pPlayerID,tagAddBillPacket::maxbytes_user_id);
	strncpy(Packet.User_IP,pIPaddress,tagAddBillPacket::maxbytes_user_ip);
	strncpy(Packet.User_Status,pUser_Status,tagAddBillPacket::maxbytes_user_status);

	gcpTerraBillingCtrl_TransmiteCircularQueueCtrl->Push(&Packet);
}
#endif

#ifdef _GAME_SERVER
void CTerraBillingCtrl_Encoder::Transmite_User_Sync(const struct tagAddBillPacket& Packet) const
{
	Transmite_User_Sync(
		FALSE,
		Packet.Session,
		Packet.User_ID,
		Packet.User_IP,
		Packet.User_Status);
}
#endif

#ifdef _GAME_SERVER
void CTerraBillingCtrl_Encoder::Transmite_User_Sync(const playerCharacter_t* pPlayerRecord) const
{
	static char strPlayerIdx[_MAX_PATH];
	wsprintf(strPlayerIdx,"%d",pPlayerRecord->idx);

	static char strIPaddress[tagAddBillPacket::maxbytes_user_ip];
	wsprintf(strIPaddress,"%d.%d.%d.%d",
		pPlayerRecord->sock.addr.sin_addr.S_un.S_un_b.s_b1,
		pPlayerRecord->sock.addr.sin_addr.S_un.S_un_b.s_b2,
		pPlayerRecord->sock.addr.sin_addr.S_un.S_un_b.s_b3,
		pPlayerRecord->sock.addr.sin_addr.S_un.S_un_b.s_b4);

	Transmite_User_Sync(
		TRUE,
		strPlayerIdx,
		pPlayerRecord->userID,
		strIPaddress,
		pPlayerRecord->AddBill.User_Status);
}
#endif







