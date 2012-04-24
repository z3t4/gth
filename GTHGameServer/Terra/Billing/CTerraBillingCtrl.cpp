



#include "..\..\global.h"
#include "CTerraBillingCtrl.h"
#include <windowsx.h>
#include <assert.h>


#include "tagTerraPacket.h"


#include "CTerraBillingCtrl_CircularQueueCtrl.h"
CTerraBillingCtrl_CircularQueueCtrl*		gcpTerraBillingCtrl_TransmiteCircularQueueCtrl=NULL;

#include "CTerraBillingCtrl_ReceiveCircualrQueueCtrl.h"
#include "CTerraBillingCtrl_CommunicationCtrl.h"


CTerraBillingCtrl_ReceiveCircualrQueueCtrl* gcpTerraBillingCtrl_ReceiveCircularQueueCtrl=NULL;
CTerraBillingCtrl_CommunicationCtrl*        gcpTerraBillingCtrl_CommunicationCtrl=NULL;


#include "CTerraBillingCtrl_Encoder.h"
CTerraBillingCtrl_Encoder* gcpTerraBillingCtrl_Encder=NULL;

#include "CTerraBillingCtrl_Decoder.h"
CTerraBillingCtrl_Decoder* gcpTerraBillingCtrl_Decoder=NULL;

#include "tagTerraBilling.h"
struct tagTerraBilling gTerraBilling;

#include "CTerraBillingCtrl_LogCtrl.h"
CTerraBillingCtrl_LogCtrl* gcpTerraCtrl_LogCtrl=NULL;

extern unsigned int g_netTime;


#ifdef _MASTER_SERVER
#include "..\\CTerraCtrl.h"
extern CTerraCtrl* gcpTerraCtrl;

#include "..\Terra Billing\tagTerraPacket.h"
#endif





CTerraBillingCtrl::CTerraBillingCtrl()
{
	m_bOK=FALSE;

	char buffer[_MAX_PATH];
	wsprintf(buffer,"struct tagAddBillPacket size=%d",sizeof(struct tagAddBillPacket));
	assert(196 == sizeof(struct tagAddBillPacket));

	m_Step = step_none;
	if(FALSE == Init()) return;


	gcpTerraCtrl_LogCtrl->Write("----------------------------------------------------------");
	gcpTerraCtrl_LogCtrl->Write("start terra billing ctrl");
	
	gcpTerraCtrl_LogCtrl->Write("game No             = %d",gTerraBilling.Game_No);
	gcpTerraCtrl_LogCtrl->Write("billing server ip   = %s",gTerraBilling.IP);
	gcpTerraCtrl_LogCtrl->Write("billing server port = %d",gTerraBilling.Port);
	gcpTerraCtrl_LogCtrl->Write("game server GUID    = %s",gTerraBilling.serverGUIDstring);
	gcpTerraCtrl_LogCtrl->Write("game No             = %d",gTerraBilling.Game_No);
	gcpTerraCtrl_LogCtrl->Write("log output          = %s",CTerraBillingCtrl_LogCtrl::GetYESNOstring(gTerraBilling.Log.bOutput));	
	gcpTerraCtrl_LogCtrl->Write("log update cycle    = %d mseccond",gTerraBilling.Log.UpdateCycle);
	gcpTerraCtrl_LogCtrl->Write("----------------------------------------------------------");




	m_bOK=TRUE;

}





CTerraBillingCtrl::~CTerraBillingCtrl()
{

	gcpTerraCtrl_LogCtrl->Write("end terra billing ctrl");



	if(NULL != gcpTerraBillingCtrl_Encder){
		delete gcpTerraBillingCtrl_Encder;
		gcpTerraBillingCtrl_Encder=NULL; 
	}

	if(NULL != gcpTerraBillingCtrl_Decoder){
		delete gcpTerraBillingCtrl_Decoder;
		gcpTerraBillingCtrl_Decoder=NULL; 
	}	


	if(NULL != gcpTerraBillingCtrl_CommunicationCtrl){
		delete gcpTerraBillingCtrl_CommunicationCtrl;
		gcpTerraBillingCtrl_CommunicationCtrl=NULL; 
	}



	if(NULL != gcpTerraBillingCtrl_TransmiteCircularQueueCtrl){
		delete gcpTerraBillingCtrl_TransmiteCircularQueueCtrl;
		gcpTerraBillingCtrl_TransmiteCircularQueueCtrl=NULL;
	}

	if(NULL != gcpTerraBillingCtrl_ReceiveCircularQueueCtrl){
		delete gcpTerraBillingCtrl_ReceiveCircularQueueCtrl; 
		gcpTerraBillingCtrl_ReceiveCircularQueueCtrl=NULL;
	}

	if(NULL != gcpTerraCtrl_LogCtrl){
		delete gcpTerraCtrl_LogCtrl;
		gcpTerraCtrl_LogCtrl=NULL;
	}

}





BOOL CTerraBillingCtrl::isOK(void) const
{
	return m_bOK;
}





BOOL CTerraBillingCtrl::LoadServerConfigFile(void)
{	
	
	gTerraBilling.Log.bOutput=FALSE;
	gTerraBilling.Log.UpdateCycle = 1000;

	strncpy(gTerraBilling.serverGUIDstring,g_config.serviceIP,tagAddBillPacket::maxbytes_serverGUIDstring);


	int size;
	char *token;
	char* pFileBuffer=NULL;

	
	{
		FILE* fp = NULL;
		fp = fopen( "server.cfg" , "rb" );
		if(NULL == fp) return FALSE;

		fseek(fp,0,SEEK_END);
		size = ftell(fp);
		pFileBuffer=(char*)GlobalAllocPtr(GMEM_FIXED,sizeof(char) * size);
		if(NULL == pFileBuffer){
			if(NULL != fp){ fclose(fp); fp=NULL; }
			return FALSE;
		}
		fseek(fp,0,SEEK_SET);
		fread(pFileBuffer,size,1,fp);
		if(NULL != fp){ fclose(fp); fp=NULL; }
	}

	curpos = pFileBuffer;
	endpos = curpos + size;	
	

	while((token = NextToken()) != NULL) {	

		

		if(0 == stricmp(token,"TERRA_BILLING_SERVER_IP")){
			strncpy(gTerraBilling.IP,NextArg(),IPSTRING);
			gTerraBilling.IP[IPSTRING]=NULL;
			continue;
		}

		if(0 == stricmp(token,"TERRA_BILLING_SERVER_PORT")){
			gTerraBilling.Port=atoi(NextArg());			
			continue;
		}

		if(0 == stricmp(token,"TERRA_BILLING_GTH_GAME_NO")){
			gTerraBilling.Game_No=atoi(NextArg());
			continue;
		}


		if(0 == stricmp(token,"TERRA_BILLING_LOG_OUTPUT")){
			char* pTemp = NextArg();
			if((NULL != strstr("yes",pTemp)) || (NULL != strstr("YES",pTemp))) gTerraBilling.Log.bOutput=TRUE;
			else gTerraBilling.Log.bOutput=FALSE;			
			continue;
		}


		if(0 == stricmp(token,"TERRA_BILLING_LOG_UPDATE_MILISECOND")){
			gTerraBilling.Log.UpdateCycle = atoi(NextArg());
			continue;
		}
	}

	if(NULL != pFileBuffer){ GlobalFreePtr(pFileBuffer); pFileBuffer=NULL; }
	return TRUE;
}




BOOL CTerraBillingCtrl::Init(void)
{
	if(FALSE == LoadServerConfigFile()) return FALSE;	

	gcpTerraCtrl_LogCtrl= new CTerraBillingCtrl_LogCtrl();
	if(NULL == gcpTerraCtrl_LogCtrl) return FALSE;
	if(FALSE == gcpTerraCtrl_LogCtrl->isOK()){ delete gcpTerraCtrl_LogCtrl; gcpTerraCtrl_LogCtrl=NULL; return FALSE; }

	gcpTerraBillingCtrl_TransmiteCircularQueueCtrl=new CTerraBillingCtrl_CircularQueueCtrl(MaxNum_TrnamisteCircularQueueEle);
	if(NULL == gcpTerraBillingCtrl_TransmiteCircularQueueCtrl) return FALSE;
	if(FALSE == gcpTerraBillingCtrl_TransmiteCircularQueueCtrl->isOK()){
		delete gcpTerraBillingCtrl_TransmiteCircularQueueCtrl;
		gcpTerraBillingCtrl_TransmiteCircularQueueCtrl=NULL;
		return FALSE;
	}

	gcpTerraBillingCtrl_ReceiveCircularQueueCtrl = new CTerraBillingCtrl_ReceiveCircualrQueueCtrl(MaxNum_ReceiveCircularQueueEle);
	if(NULL == gcpTerraBillingCtrl_ReceiveCircularQueueCtrl) return FALSE;
	if(FALSE == gcpTerraBillingCtrl_ReceiveCircularQueueCtrl->isOK()){
		delete gcpTerraBillingCtrl_ReceiveCircularQueueCtrl; 
		gcpTerraBillingCtrl_ReceiveCircularQueueCtrl=NULL;
		return FALSE;
	}

	gcpTerraBillingCtrl_CommunicationCtrl= new CTerraBillingCtrl_CommunicationCtrl();
	if(NULL == gcpTerraBillingCtrl_CommunicationCtrl) return FALSE;
	if(FALSE == gcpTerraBillingCtrl_CommunicationCtrl->isOK()){
		delete gcpTerraBillingCtrl_CommunicationCtrl;
		gcpTerraBillingCtrl_CommunicationCtrl=NULL;
		return FALSE;
	}

	gcpTerraBillingCtrl_Encder = new CTerraBillingCtrl_Encoder();
	if(NULL == gcpTerraBillingCtrl_Encder) return FALSE;
	if(FALSE == gcpTerraBillingCtrl_Encder->isOK()){
		delete gcpTerraBillingCtrl_Encder;
		gcpTerraBillingCtrl_Encder=NULL;
		return FALSE;
	}

	gcpTerraBillingCtrl_Decoder= new CTerraBillingCtrl_Decoder();
	if(NULL == gcpTerraBillingCtrl_Decoder) return FALSE;
	if(FALSE == gcpTerraBillingCtrl_Decoder->isOK()){
		delete gcpTerraBillingCtrl_Decoder;
		gcpTerraBillingCtrl_Decoder=NULL; 
		return FALSE;
	}

	return TRUE;
}







BOOL CTerraBillingCtrl::ServiceStart(void)
{

	
	
	m_Step = step_connect_try;
	return TRUE;

	
}




inline void CTerraBillingCtrl::TransmiteAllPacketInTransmiteCircularQueue(void) const
{
	struct tagAddBillPacket Packet;
	while(TRUE == gcpTerraBillingCtrl_TransmiteCircularQueueCtrl->Pop(Packet)){	
		gcpTerraBillingCtrl_CommunicationCtrl->Send(Packet);
	}
}




void CTerraBillingCtrl::ServiceEnd(void)
{
	
	
	gcpTerraBillingCtrl_Encder->Transmite_Server_Reset();
	TransmiteAllPacketInTransmiteCircularQueue();
	m_Step = step_disconnect_try;



	
	
}














inline void CTerraBillingCtrl::Process_step_service(void)
{
	switch(gcpTerraBillingCtrl_CommunicationCtrl->GetStep()){
		case CTerraBillingCtrl_CommunicationCtrl::enumStep::step_disconnect:
			m_Step = step_connect_try;
			return;
	}

	struct tagAddBillPacket Packet;
	while(TRUE == gcpTerraBillingCtrl_ReceiveCircularQueueCtrl->Pop(Packet)){
		gcpTerraBillingCtrl_Decoder->Decoder(Packet);		
	}	
	TransmiteAllPacketInTransmiteCircularQueue();
}




void CTerraBillingCtrl::Process(void)
{
	
	gcpTerraCtrl_LogCtrl->Update();

	switch(m_Step){
		case step_disconnect_try:
			switch(gcpTerraBillingCtrl_CommunicationCtrl->GetStep()){
				case CTerraBillingCtrl_CommunicationCtrl::enumStep::step_connect:
					gcpTerraBillingCtrl_CommunicationCtrl->TryDisconnect();
					break;
			}
			break;

		case step_connect_try:
			switch(gcpTerraBillingCtrl_CommunicationCtrl->GetStep()){
				case CTerraBillingCtrl_CommunicationCtrl::enumStep::step_disconnect:
					gcpTerraBillingCtrl_CommunicationCtrl->TryConnect(inet_addr(gTerraBilling.IP),gTerraBilling.Port);
					break;

				case CTerraBillingCtrl_CommunicationCtrl::enumStep::step_connect:
					m_Step = step_transmite_packet_conn_and_reset;
					break;
			}
			break;

		case step_transmite_packet_conn_and_reset:
			gcpTerraBillingCtrl_Encder->Transmite_Server_Conn();
			gcpTerraBillingCtrl_Encder->Transmite_Server_Reset();
			m_Step = step_service;
			break;

		case step_service:
			Process_step_service();
			break;
	}	
}




void CTerraBillingCtrl::OpenNotePadMessage(const char* pMessage)
{		
	FILE* fp=fopen("report.txt","wt");
	if(NULL != fp){
		fprintf(fp,"이화일은 디버그 모드에서만 출력됩니다.\n");
		fprintf(fp,"%s",pMessage);
		fclose(fp); 
		fp=NULL;
	}
	WinExec("notepad.exe report.txt",SW_SHOW);
}






#ifdef _GAME_SERVER
void CTerraBillingCtrl::
Player_GameStart(playerCharacter_t* pPlayerRecord)
{
	
	if(pPlayerRecord->AddBill.step != playerCharacter_t::tagAddBill::step_none) return;

	pPlayerRecord->AddBill.step = playerCharacter_t::tagAddBill::step_wait_game_start_packet;
	pPlayerRecord->AddBill.time_trGameStartPacket = g_netTime;
	gcpTerraBillingCtrl_Encder->Transmite_Game_Start(pPlayerRecord);

	gcpTerraCtrl_LogCtrl->Write("game start name=%-20s id=%-20s",pPlayerRecord->name,pPlayerRecord->userID);
	

}
#endif





#ifdef _GAME_SERVER
void CTerraBillingCtrl::Player_GameEnd(playerCharacter_t* pPlayerRecord)
{
	
	if(pPlayerRecord->AddBill.step != playerCharacter_t::tagAddBill::step_billing) return;

	pPlayerRecord->AddBill.step = playerCharacter_t::tagAddBill::step_end;
	gcpTerraBillingCtrl_Encder->Transmite_Game_End(pPlayerRecord);
	gcpTerraCtrl_LogCtrl->Write("game end   name=%-20s id=%-20s",pPlayerRecord->name,pPlayerRecord->userID);
}
#endif




#ifdef _GAME_SERVER
BOOL CTerraBillingCtrl::CheckPlayer(playerCharacter_t* pPlayerRecord)
{
	if(pPlayerRecord->AddBill.step != playerCharacter_t::tagAddBill::step_wait_game_start_packet) return FALSE;

	if(g_netTime - pPlayerRecord->AddBill.time_trGameStartPacket >= 5 * 1000){

		gcpTerraBillingCtrl_Decoder->Transmite_TerraGCPacket_OpenMessageBox(
				pPlayerRecord, 
				tagTerraGCPacket_OpenMessageBox::enumLPACKType::Billing,
				3,
				tagTerraGCPacket_OpenMessageBox::enumAfter::after_client_exit);
		GTH_DisconnectGameServer(pPlayerRecord->idx);
		return TRUE;
	}

	return FALSE;
}
#endif











