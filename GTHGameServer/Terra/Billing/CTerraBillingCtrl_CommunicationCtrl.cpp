




#include "..\..\global.h"
#include "CTerraBillingCtrl_CommunicationCtrl.h"

#include <process.h>
#include <assert.h>


#include "CTerraBillingCtrl_CircularQueueCtrl.h"
#include "CTerraBillingCtrl_ReceiveCircualrQueueCtrl.h"

extern CTerraBillingCtrl_CircularQueueCtrl*			gcpTerraBillingCtrl_TransmiteCircularQueueCtrl;
extern CTerraBillingCtrl_ReceiveCircualrQueueCtrl*	gcpTerraBillingCtrl_ReceiveCircularQueueCtrl;

#include "CTerraBillingCtrl_Encoder.h"
extern CTerraBillingCtrl_Encoder* gcpTerraBillingCtrl_Encder;




CTerraBillingCtrl_CommunicationCtrl::CTerraBillingCtrl_CommunicationCtrl()
{
	m_bOK=FALSE;	

	m_sock =INVALID_SOCKET;
	m_ThreadVar.bLive=FALSE;
	m_ThreadVar.bReqExit=FALSE;
	m_Step = step_disconnect;



	{
		unsigned int threadID = 0 ;
		HANDLE       hThread = INVALID_HANDLE_VALUE;
		hThread = (HANDLE)_beginthreadex(0, 0, ThreadFunction, (void*)this, 0, &threadID) ;
		if(hThread == INVALID_HANDLE_VALUE) return;

		::CloseHandle(hThread);
		hThread = INVALID_HANDLE_VALUE;
	}
	

	m_bOK=TRUE;

}





CTerraBillingCtrl_CommunicationCtrl::~CTerraBillingCtrl_CommunicationCtrl()
{
	
	SocketDestory();	

	m_ThreadVar.bReqExit=TRUE;
	while(TRUE == m_ThreadVar.bLive){
		Sleep(100);
	}
	Sleep(10);
}






BOOL CTerraBillingCtrl_CommunicationCtrl::isOK(void) const
{
	return m_bOK; 
}





enum CTerraBillingCtrl_CommunicationCtrl::enumStep 
CTerraBillingCtrl_CommunicationCtrl::GetStep(void)
{
	return m_Step;
}
















inline void CTerraBillingCtrl_CommunicationCtrl::SocketDestory(void)
{
	if(INVALID_SOCKET != m_sock){
		closesocket(m_sock);
		m_sock=INVALID_SOCKET;
	}
}



inline BOOL CTerraBillingCtrl_CommunicationCtrl::SocketCreate(void)
{
	
	m_sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP) ;
	if ( INVALID_SOCKET == m_sock )
	{
		return FALSE;
	}

	return TRUE;
}






inline void CTerraBillingCtrl_CommunicationCtrl::Disconnect(void)
{
	
	assert(INVALID_SOCKET != m_sock);	

	SocketDestory() ;		
	gcpTerraBillingCtrl_ReceiveCircularQueueCtrl->Clear();
	gcpTerraBillingCtrl_TransmiteCircularQueueCtrl->Clear();
	
}








unsigned int __stdcall CTerraBillingCtrl_CommunicationCtrl::ThreadFunction(void* pV)
{	
	CTerraBillingCtrl_CommunicationCtrl* cpCommunicationCtrl = (CTerraBillingCtrl_CommunicationCtrl*)pV ;
	assert(NULL != cpCommunicationCtrl);	
	cpCommunicationCtrl->RealThreadFunction();
	return 1;
}










inline BOOL CTerraBillingCtrl_CommunicationCtrl::Connect(void)
{	
	
	

	SocketDestory();			
	gcpTerraBillingCtrl_ReceiveCircularQueueCtrl->Clear();
	gcpTerraBillingCtrl_TransmiteCircularQueueCtrl->Clear();
	
	if(FALSE == SocketCreate()) return FALSE;

	CInternetAddress servAddr(m_addr,m_port) ;	
	if(SOCKET_ERROR == connect(m_sock,(sockaddr*)&servAddr,sizeof(servAddr))) return FALSE;	

	return TRUE;
}




int CTerraBillingCtrl_CommunicationCtrl::RealThreadFunction(void)
{	
	struct tagAddBillPacket Packet;
	int ret ;
	

	m_ThreadVar.bLive=TRUE;
	while(FALSE == m_ThreadVar.bReqExit){

		switch(m_Step){
			
			case step_disconnect:
			default:
				Sleep(10);
				break;

			case step_connect:
				ret = recv( m_sock, (char*)&Packet, sizeof(struct tagAddBillPacket), 0 ) ;
				if((SOCKET_ERROR == ret) || (0 == ret)){			
					m_Step = step_disconnect;
					break;
				}
				gcpTerraBillingCtrl_ReceiveCircularQueueCtrl->Push((unsigned char*)&Packet, ret);				
				break;

			case step_connect_try:
				if(TRUE == Connect()) m_Step = step_connect;
				Sleep(10);
				break;			
		}
	}		

	m_ThreadVar.bLive=FALSE;
	return 0 ;
}




BOOL CTerraBillingCtrl_CommunicationCtrl::TryDisconnect(void)
{
	switch(m_Step){
		case step_connect:					
			SocketDestory();
			m_Step = step_disconnect_try;			
			return TRUE;
	}

	return FALSE;
}




BOOL CTerraBillingCtrl_CommunicationCtrl::
TryConnect(const unsigned long addr,const unsigned short port)
{
	switch(m_Step){
		case step_disconnect:
			m_addr = addr;
			m_port = port;
			m_Step = step_connect_try;
			return TRUE;
	}

	return FALSE;
}





BOOL CTerraBillingCtrl_CommunicationCtrl::
Send(const struct tagAddBillPacket& Packet)
{
	if(INVALID_SOCKET == m_sock) return FALSE;
	int ret = send(m_sock,(char*)&Packet, sizeof(struct tagAddBillPacket), 0 ) ;
	if(SOCKET_ERROR == ret) return FALSE;
	return TRUE;	
}
