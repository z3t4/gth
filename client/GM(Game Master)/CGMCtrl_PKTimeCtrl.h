



#if !defined(AFX_CGMCTRL_PKTIMECTRL_H__66BE2053_FB35_47A2_8337_1754A3EF666D__INCLUDED_)
#define AFX_CGMCTRL_PKTIMECTRL_H__66BE2053_FB35_47A2_8337_1754A3EF666D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CGMCtrl_PKTimeCtrl  
{
	private:
		BOOL m_bOK;
		BOOL m_bPK;
	public:
		BOOL isOK(void) const;
		BOOL isPK(void) const;
		void SetPK(const BOOL bPK);
	
	public:
		void Decoder(void);
		inline void Decoder_sc_notify_change(void);
		inline void Decoder_sc_response_status(void);

	
	public:
		void TransmitePacket_cs_request_change(const BOOL bPK);
		void TransmitePacket_cs_request_status(void);

	
	public:
		CGMCtrl_PKTimeCtrl();
		virtual ~CGMCtrl_PKTimeCtrl();

};

#endif 
