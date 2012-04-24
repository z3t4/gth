




#include "..\..\global.h"
#include "CTerraBillingCtrl_CircularQueueCtrl.h"
#include <assert.h>
#include <windowsx.h>




CTerraBillingCtrl_CircularQueueCtrl::
CTerraBillingCtrl_CircularQueueCtrl(const int Number)
{
	assert(Number > 0);

	m_bOK=FALSE;
	m_pBuffer=NULL;
	m_Number=0;
	m_ReadIdx = 0;
	m_WriteIdx = 0;
	InitializeCriticalSection(&m_critcalsection);
	if(FALSE == Init(Number)) return;	
	

	m_bOK=TRUE;

}








CTerraBillingCtrl_CircularQueueCtrl::~CTerraBillingCtrl_CircularQueueCtrl()
{
	DeleteCriticalSection(&m_critcalsection);
	if(NULL != m_pBuffer){ GlobalFreePtr(m_pBuffer); m_pBuffer=NULL; }

}





BOOL CTerraBillingCtrl_CircularQueueCtrl::isOK(void) const
{
	return m_bOK; 
}




BOOL CTerraBillingCtrl_CircularQueueCtrl::
Init(const int Number)
{
	m_Number = Number;
	m_pBuffer=(struct tagAddBillPacket*)GlobalAllocPtr(GMEM_FIXED,sizeof(struct tagAddBillPacket) * m_Number);
	if(NULL == m_pBuffer) return FALSE;

	memset(m_pBuffer,0x00,sizeof(struct tagAddBillPacket) * m_Number);

	return TRUE;
}










inline BOOL CTerraBillingCtrl_CircularQueueCtrl::
isEmpty(void) const
{
	if(m_ReadIdx == m_WriteIdx) return TRUE;
	return FALSE;
}









inline BOOL CTerraBillingCtrl_CircularQueueCtrl::
isFull(void) const
{
	int tempWriteIdx = m_WriteIdx;
	tempWriteIdx+=1;
	if(tempWriteIdx >= m_Number) tempWriteIdx=0;
	if(tempWriteIdx == m_ReadIdx) return TRUE;
	return FALSE;
}







BOOL CTerraBillingCtrl_CircularQueueCtrl::
Push(const struct tagAddBillPacket* pData)
{
	EnterCriticalSection(&m_critcalsection);

	if(TRUE == isFull()){
		LeaveCriticalSection(&m_critcalsection);
		return FALSE;
	}

	m_pBuffer[m_WriteIdx] = *pData;
	m_WriteIdx+=1;
	if(m_WriteIdx >= m_Number) m_WriteIdx=0;

	LeaveCriticalSection(&m_critcalsection);
	return TRUE;
}









BOOL CTerraBillingCtrl_CircularQueueCtrl::
Pop(struct tagAddBillPacket& Data)
{
	EnterCriticalSection(&m_critcalsection);
	if(TRUE == isEmpty()){
		LeaveCriticalSection(&m_critcalsection);
		return FALSE;
	}

	Data = m_pBuffer[m_ReadIdx];
	m_ReadIdx+=1;
	if(m_ReadIdx >= m_Number) m_ReadIdx=0;

	LeaveCriticalSection(&m_critcalsection);
	return TRUE;
}




void CTerraBillingCtrl_CircularQueueCtrl::Clear(void)
{
	m_ReadIdx=m_WriteIdx=0;
}
































