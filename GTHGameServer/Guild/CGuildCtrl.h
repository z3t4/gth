



#if !defined(AFX_CGUILDCTRL_H__5DBD128C_7D39_4CF2_8E24_F92494C75936__INCLUDED_)
#define AFX_CGUILDCTRL_H__5DBD128C_7D39_4CF2_8E24_F92494C75936__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CGuildCtrl  
{
	public:
		
		enum enumGsvrMsvr{
			fromGsvr_toMsvr_Summary_Req=0,
			fromMsvr_toGsvr_Summary_Res,  

			fromMsvr_toGsvr_Organize_Notify,
			fromMsvr_toGsvr_DisOrganize_Notify,

			fromGsvr_toMsvr_MarkChange_Req,
			fromMsvr_toGsvr_MarkChange_Notify,

		};
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
				Success=0,
				error_delay_hour,
				error_unknown,
			};


		};

		


	private:
		struct tagGuildSummarySyncRecord{
			public:
				struct tagMark{
					unsigned char Image[GUILDMARKSIZE];
					long		  ChangeDate;
				};

				struct tagSync{
					public:
						struct tagRequest{
							BOOL bDo;
							int  Time;
						};
					public:					
						BOOL bComplete;
						struct tagRequest Request;
				};

			public:
				struct tagSync Sync;
				BOOL   bActive;
				char   Name[GUILDNAMESTRING+1];
				struct tagMark Mark;				
		};
		BOOL m_bOK;
		struct tagGuildSummarySyncRecord m_SummarySyncRecord[MAX_GUILDS];
		int  m_transmite_req_sync_at_manager_server_number;


	protected:
		BOOL Init(void);
		inline BOOL ValidCheck_GuildIndex(const int Idx) const;
		inline BOOL ValidCheck(const guild_t* pGuild) const;
		inline BOOL ValidCheck_MemberServerIndex(const int MemberServerIdx) const;

	
	protected:
		inline void GTH_SendMessage_Guild_Summary_Request_To_ManagerServer(const int Idx);

	public:
		BOOL isOK(void) const;		
		static void strcpy_null_terminate(char* destStr,const char* sourceStr,const int bytes);
		void InitWhenManagerServer(void);		
		int  Get_transmite_req_sync_at_manager_server_number(void) const;
		
	
	public:
		struct tagGuildSummarySyncRecord* GetSummaryRecordPointer(const int Idx);

	
	public:
		guild_t* GetRecordPointer(const int Idx);
		guild_t* GetRecordPointerWithoutValidCheck(const int Idx);
		int GetIndexByName(const char* szpGuildName);

		
	
	public:
		void BetweenMsGs_ManageServerDecoder(void);		
		inline void BetweenMsGs_ManagerServerDecoder_MarkChange_Req(void);
		inline void BetweenMsGs_ManageServerDecoder_Summary_Req(void);	
		inline void TransmiteAtMs_UpdateMark(const int GuildIdx,const unsigned char* pMark);		
		

	
	public:
		void BetweenMsGs_GameServerDecoder(void);		
		inline void BetweenMsGs_GameServerDecoder_Summary_Res(void);
		inline void BetweenMsGs_GameServerDecoder_Organize_Notify(void);
		inline void BetweenMsGs_GameServerDecoder_DisOrganize_Notify(void);
		inline void BetweenMsGs_GameServerDecoder_MarkChange_Notify(void);

		inline void TransmiteAtGs_SyncPacket_Organize(const int GuildIdx);		
		inline void TransmiteAtGs_SyncPacket_DisOrganize(const int GuildIdx);		
		inline void TransmiteAtGs_SyncPacket_UpdateMark(const int GuildIdx,const struct tagGuildSummarySyncRecord* pSummaryRecord);

		inline void ManagerServerProc_MarkChange(const int GuildIdx,const unsigned char* pMarkImage);		
		void	    ManagerServerProc_Organize(const int GuildIdx,const char* pName,const unsigned char* pMarkImage);
		void		ManagerServerProc_DisOrganize(const int GuildIdx);
		

	
	public:
		void BetweenClientGs_Decoder(void);
		inline void BetweenClientGs_Decoder_MarkChange_Req(void);
		inline void BetweenClientGs_Decoder_Summary_Req(void);
		inline void BetweenClientGs_Decoder_MarkImage_Req(void);	

	
	protected:
		inline void TransmiteAtClient_MarkCheck_Req(const int GuildIdx,const struct tagGuildSummarySyncRecord* pSummaryRecord);
		inline void TransmiteAtClient_DisOrganize_Notify(const int GuildIdx);
		inline void TransmiteAtClient_MarkChange_Res(
						playerCharacter_t* pPc,
						enum tagPacket_fromGsvr_toClient_MarkChange_Res::enumCode code,
						const int data) const;


	
	public:
		CGuildCtrl();
		virtual ~CGuildCtrl();

};

#endif 
