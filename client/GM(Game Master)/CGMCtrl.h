



#if !defined(AFX_CGMCTRL_H__09C4E5E3_376E_4EDA_B133_1F185A1A259B__INCLUDED_)
#define AFX_CGMCTRL_H__09C4E5E3_376E_4EDA_B133_1F185A1A259B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "CGMCtrl_PKTimeCtrl.h"
#include "CGMCtrl_ChatBlockCtrl.h"

class CGMCtrl  
{
	private:
		BOOL m_bOK;
	public:
		CGMCtrl_PKTimeCtrl* m_cpPKTimeCtrl;
		CGMCtrl_ChatBlockCtrl* m_cpChatBlockCtrl;

	private:
		
		BOOL Init(void);

	public:
		BOOL isOK(void) const;
		BOOL RunGMChatCommand( char message[], char command[], chatMessage param[] );

		void Decoder(void);

	
	private:
		void		Decoder_NProtect(void);
		inline void Decoder_NProtect_sc_response_AUTH(void);
		inline void Decoder_NProtect_sc_response_AUTH_dll_change(void);
		inline void Decoder_NProtect_sc_response_AUTHstate(void);

		void TransmitePacket_cs_request_CSAUTH(const BOOL bEnable);
		void TransmitePacket_cs_request_AUTH_dll_change(const char* in_strDLLFileName);
		void TransmitePacket_cs_request_AUTHstate(void);
		

	
	public:
		CGMCtrl();
		virtual ~CGMCtrl();

};

#endif 
