



#if !defined(AFX_CTERRABILLINGCTRL_RECEIVECIRCUALRQUEUECTRL_H__CAC70141_0F1A_40C4_97EA_44AE33CE941B__INCLUDED_)
#define AFX_CTERRABILLINGCTRL_RECEIVECIRCUALRQUEUECTRL_H__CAC70141_0F1A_40C4_97EA_44AE33CE941B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "tagAddBill.h"
#include "CTerraBillingCtrl_CircularQueueCtrl.h"

class CTerraBillingCtrl_ReceiveCircualrQueueCtrl : public CTerraBillingCtrl_CircularQueueCtrl
{
	private:
		BOOL m_bOK;

		enum{
			MaxBytes_LineBuffer= sizeof(struct tagAddBillPacket) * 2,
		};
		unsigned char m_LineBuffer[MaxBytes_LineBuffer];
		int           m_LineBufferStoreBytes;


	public:
		BOOL isOK(void) const;
		BOOL Push(const unsigned char* pNewData,const int NewDataBytes);

	
	public:
		CTerraBillingCtrl_ReceiveCircualrQueueCtrl(const int Number);
		virtual ~CTerraBillingCtrl_ReceiveCircualrQueueCtrl();

};

#endif 
