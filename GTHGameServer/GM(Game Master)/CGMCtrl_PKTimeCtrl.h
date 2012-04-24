



#if !defined(AFX_CGMCTRL_PKTIMECTRL_H__4F25DB05_3D18_4CF9_BB10_1EDF9F7EE9AA__INCLUDED_)
#define AFX_CGMCTRL_PKTIMECTRL_H__4F25DB05_3D18_4CF9_BB10_1EDF9F7EE9AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "CGMCtrl_ServerStateDataCtrl.h"

class CGMCtrl_PKTimeCtrl  
{
	public:
		BOOL m_bOK;
		CGMCtrl_ServerStateDataCtrl*	m_cpVirtualServerStateDataCtrl;
		
	public:
		BOOL isOK(void) const;		
		
		


	
	public:
		void Decoder(playerCharacter_t* pPlayer);
		inline void Decoder_cs_request_change(playerCharacter_t* pPlayer);
		inline void Decoder_gsms_request_change(void);
		inline void Decoder_msgs_notify_change(void);
		inline void Decoder_cs_request_status(playerCharacter_t* pPlayer);


	
	public:
		inline void TransmitePacket_sc_notify_change(void);
		inline void TransmitePacket_sc_response_status(playerCharacter_t* pPlayer);
		inline void TransmitePacket_gsms_request_change(const BOOL bPK);
		inline void TransmitePacket_msgs_notify_change(void);

	
	public:
		CGMCtrl_PKTimeCtrl(CGMCtrl_ServerStateDataCtrl*	cpServerStateDataCtrl);
		virtual ~CGMCtrl_PKTimeCtrl();

};

#endif 
