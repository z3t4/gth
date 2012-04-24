



#if !defined(AFX_CGUILDCTRL_H__D01F90E3_A39A_4210_B27E_89A6BC7ACB44__INCLUDED_)
#define AFX_CGUILDCTRL_H__D01F90E3_A39A_4210_B27E_89A6BC7ACB44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#pragma warning( disable : 4786 )
#include <MAP>

class CGuildCtrl  
{
	public:

		enum enumClientGsvr{
			fromClient_toGsvr_Summary_Req=0,
			fromGsvr_toClient_Summary_Res,  
			fromClient_toGsvr_MarkChange_Req,
			fromGsvr_toClient_MarkCheck_Req,

			fromClient_toGsvr_MarkImage_Req,
			fromGsvr_toClient_MarkImage_Res,

			fromGsvr_toClient_DisOrganize_Notify,
			fromGsvr_toClient_MarkChange_Res,
		};

		struct tagPacket_fromGsvr_toClient_MarkChange_Res{
			enum enumCode{
				success=0,
				error_delay_hour,
				error_unknown,
			};
		};
			

		

	private:
		BOOL m_bOK;


		struct tagSummaryRecord{
			public:
				struct tagRequest{
					BOOL bDo;
					int  Time;
				};

				struct tagSync{					
					public:
						BOOL bComplete;					
						struct tagRequest Request;
				};

				struct tagMark{
					int ChangeCnt;
					BOOL bActive;
					struct tagRequest Request;
				};

			public:
				struct tagSync Sync;
				char Name[GUILDNAMESTRING+1];
				struct tagMark Mark;
		};

		std::map<int,struct tagSummaryRecord> m_SummaryRecordMap;
		
	protected:		
		inline void TransmitePackete_Summary_Req(const int guildIdx) const;
		inline BOOL Init(void);

	public:
		BOOL isOK(void) const;
		BOOL GetSummaryRecord(const int Idx,struct tagSummaryRecord& GuildSummaryRecord);

		
		
		BOOL Prepare(void);
		void Reset(void);
		

	
	public:				
		void TransmitePacket_MarkChange_Req(const int GuildIdx,const unsigned char* pMark) const;		
		void TransmitePacket_MarkImage_Req(const int GuildIdx) const;

	
	public:
		void Decoder(void);
		inline void Decoder_Summary_Res(void);		
		inline void Decoder_MarkCheck_Req(void);		
		inline void Decoder_MarkImage_Res(void);
		inline void Decoder_DisOrganize_Notify(void);
		inline void Decoder_MarkChange_Res(void);

	public:
		CGuildCtrl();
		virtual ~CGuildCtrl();

};

#endif 
