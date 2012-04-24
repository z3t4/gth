




#include "../../global.h"
#include "CTerraBillingCtrl_ReceiveCircualrQueueCtrl.h"




CTerraBillingCtrl_ReceiveCircualrQueueCtrl::
CTerraBillingCtrl_ReceiveCircualrQueueCtrl(const int Number):CTerraBillingCtrl_CircularQueueCtrl(Number)
{

	m_bOK=FALSE;

	if(FALSE == CTerraBillingCtrl_CircularQueueCtrl::isOK()) return;	
	m_LineBufferStoreBytes=0;

	m_bOK=TRUE;
}





CTerraBillingCtrl_ReceiveCircualrQueueCtrl::~CTerraBillingCtrl_ReceiveCircualrQueueCtrl()
{

}





BOOL CTerraBillingCtrl_ReceiveCircualrQueueCtrl::isOK(void) const
{
	return m_bOK;
}








BOOL CTerraBillingCtrl_ReceiveCircualrQueueCtrl::
Push(const unsigned char* pNewData,const int NewDataBytes)
{
	EnterCriticalSection(&m_critcalsection);


	int RemainBytesBeforeCompletePacket=0;
	int RemainBytesNewData             = NewDataBytes;
	unsigned char* pTempInsetStNewData =(unsigned char*)pNewData;

	while(TRUE){

		RemainBytesBeforeCompletePacket = sizeof(struct tagAddBillPacket) - m_LineBufferStoreBytes;
		if(RemainBytesBeforeCompletePacket > RemainBytesNewData){

			if(0 == m_LineBufferStoreBytes) memcpy(&m_LineBuffer[0],pTempInsetStNewData,RemainBytesNewData);
			else memcpy(&m_LineBuffer[m_LineBufferStoreBytes-1],pTempInsetStNewData,RemainBytesNewData);
			m_LineBufferStoreBytes += RemainBytesNewData;
			break;
		}


		if(0 == m_LineBufferStoreBytes) memcpy(&m_LineBuffer[0],pTempInsetStNewData,RemainBytesBeforeCompletePacket);
		else memcpy(&m_LineBuffer[m_LineBufferStoreBytes-1],pTempInsetStNewData,RemainBytesBeforeCompletePacket);

		if(FALSE == CTerraBillingCtrl_CircularQueueCtrl::Push((struct tagAddBillPacket*)m_LineBuffer)){
			LeaveCriticalSection(&m_critcalsection);
			return FALSE;
		}

		pTempInsetStNewData += RemainBytesBeforeCompletePacket;
		RemainBytesNewData  -= RemainBytesBeforeCompletePacket;

		if(RemainBytesNewData <= 0) break;
	}



	LeaveCriticalSection(&m_critcalsection);	
	return TRUE;
}