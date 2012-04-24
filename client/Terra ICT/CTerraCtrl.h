



#if !defined(AFX_CTERRACTRL_H__3F2768FC_2BE7_4B48_961C_984FFC90C028__INCLUDED_)
#define AFX_CTERRACTRL_H__3F2768FC_2BE7_4B48_961C_984FFC90C028__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 



#include "..\global_def.h"
class CTerraCtrl  
{
	private:
		BOOL m_bOK;

	protected:
		struct tagIdentify{
			public:
				enum{
					MaxBytes_ID =20,
					MaxBytes_Password=20,
				};

				struct tagTerra{
					public:
						enum{
							maxbytes_tid=200,
						};
					public:
						BOOL bActive;
						char id[maxbytes_tid+1];
				};
			
			public:				
				char			id[MaxBytes_ID+1];
				char			password[MaxBytes_Password+1];
				struct tagTerra Terra;
		};

		struct tagIdentify m_Identify;	
	

	public:
		BOOL isOK(void) const;
		void CommandlineParam(const char* pCommandLine);
		void GTH_SendMessage_RequestLogin( void );
		void GTH_ProcessMessage_ReplyLogin_SetID(const char* pID);
		BOOL isTerraIDUse(void) const;

	public:
		void GTH_ProcessMessage_ReplyLogin(void);
		void Decoder(void);
		inline void Decoder_Open_MessageBox(void);
		inline void Decoder_NoticeMessage(void);
		inline void Decoder_Remain_Minute_NoticeMessage(void);

	


	
	public:
		CTerraCtrl();
		virtual ~CTerraCtrl();

};

#endif 
