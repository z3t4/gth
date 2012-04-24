



#if !defined(AFX_CGMCTRL_H__0A3C6621_C407_4361_81A0_8BBB2184D655__INCLUDED_)
#define AFX_CGMCTRL_H__0A3C6621_C407_4361_81A0_8BBB2184D655__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "tagGMPacket.h"
#include "CGMCtrl_PKTimeCtrl.h"
#include "CGMCtrl_ChatBlockCtrl.h"
#include "CGMCtrl_ServerStateDataCtrl.h"


class CGMCtrl  
{
	private:
		BOOL m_bOK;
	public:
		CGMCtrl_PKTimeCtrl* m_cpPKTimeCtrl;
		CGMCtrl_ChatBlockCtrl* m_cpChatBlockCtrl;
		CGMCtrl_ServerStateDataCtrl*	m_cpServerStateDataCtrl;
	private:
		BOOL Init(void);

	public:
		BOOL isOK(void) const;
		void Decoder(playerCharacter_t* pPlayer);	
		void ProcessPerSecond(playerCharacter_t* pPlayer);
		CGMCtrl_ServerStateDataCtrl* GetServerStateDataCtrlPointer(void) const;
		BOOL isPK(void) const;
		BOOL isNProtectAUTH(void) const;

		

	
	private:
		void Decoder_NProtect(playerCharacter_t* pPlayer);
		inline void Decoder_NProtect_cs_request_AUTH(playerCharacter_t* pPlayer);
		inline void Decoder_NProtect_cs_request_AUTH_dll_change(playerCharacter_t* pPlayer);
		inline void Decoder_NProtect_cs_request_AUTHstate(playerCharacter_t* in_pPlayer);
		inline void Decoder_NProtect_msgs_notify_change(void);
		inline void Decoder_NProtect_gsms_request_change(void);


		inline void TransmitePacket_NProtect_msgs_notify_change(const BOOL in_bEnable) const;
		inline void TransmitePacket_NProtect_gsms_request_change(const BOOL in_bEnable) const;
		void TransmitePacket_NProtect_sc_response_AUTHstate(playerCharacter_t* in_pPlayer) const;
		void TransmitePacket_NProtect_sc_response_AUTH(playerCharacter_t* pPlayer) const;
		void TransmitePacket_NProtect_sc_response_AUTH_dll_change(playerCharacter_t* in_pPlayer,const BOOL in_bSuccess,const char* in_strDLLname);
	


	
	public:
		CGMCtrl();
		virtual ~CGMCtrl();

};

#endif 
