



#if !defined(AFX_CTERRABILLINGCTRL_CIRCULARQUEUECTRL_H__784A5F0E_B881_4660_BAA6_CF1333DD1626__INCLUDED_)
#define AFX_CTERRABILLINGCTRL_CIRCULARQUEUECTRL_H__784A5F0E_B881_4660_BAA6_CF1333DD1626__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 





































#include "tagAddBill.h"
class CTerraBillingCtrl_CircularQueueCtrl  
{
	protected:
		BOOL m_bOK;
		struct tagAddBillPacket* m_pBuffer;
		int                           m_Number;
		int							  m_ReadIdx;
		int							  m_WriteIdx;
		CRITICAL_SECTION			  m_critcalsection;		
	

	private:
		BOOL Init(const int Number);

	protected:
		inline BOOL isEmpty(void) const;
		inline BOOL isFull(void) const;


	public:
		BOOL isOK(void) const;		
		BOOL Push(const struct tagAddBillPacket* pData);
		BOOL Pop(struct tagAddBillPacket& Data);
		void Clear(void);



	
	public:
		CTerraBillingCtrl_CircularQueueCtrl(const int Number);
		virtual ~CTerraBillingCtrl_CircularQueueCtrl();

};

#endif 







