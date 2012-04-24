



#if !defined(AFX_CGMCTRL_CHATBLOCKCTRL_H__8D691368_D718_4293_8540_EC9CA2D61721__INCLUDED_)
#define AFX_CGMCTRL_CHATBLOCKCTRL_H__8D691368_D718_4293_8540_EC9CA2D61721__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 




class CGMCtrl_ChatBlockCtrl  
{
	private:
		BOOL m_bOK;

	private:
		char* GetHourMinSecondString(const int Time) const;

	public:
		BOOL isOK(void) const;

	public:
		void Decoder_ChatBan(void);
		void Decoder_ChatBan_sc_response_chat_block(void);
		void Decoder_ChatBan_sc_chat_block(void);
		void Decoder_ChatBan_sc_chat_allow(void);
		void Decoder_ChatBan_sc_response_chat_allow(void);
		void Decoder_ChatBan_sc_chat_response_remain_block_time(void) const;


	public:
		void TransmitePacket_GMsystem_ChatBan_Block(const char* szpApplyPlayerName,const int ApplyTime);
		void TransmitePacket_GMsystem_ChatBan_Allow(const char* szpApplyPlayerName);
		void TransmitePacket_GMsystem_ChatBan_Req_ReaminTime(void) const;

	
	public:
		CGMCtrl_ChatBlockCtrl();
		virtual ~CGMCtrl_ChatBlockCtrl();

};

#endif 
