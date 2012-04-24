




#include "..\global.h"
#include "CGuildCtrl.h"
#include "Shlwapi.h"





CGuildCtrl::CGuildCtrl()
{
	m_bOK=FALSE;
	m_transmite_req_sync_at_manager_server_number=0;

	if(FALSE == Init()) return;

	m_bOK=TRUE;
}






CGuildCtrl::~CGuildCtrl()
{

}






BOOL CGuildCtrl::isOK(void) const
{
	return m_bOK; 
}





inline BOOL CGuildCtrl::
ValidCheck_MemberServerIndex(const int MemberServerIdx) const
{
	if(0 > MemberServerIdx) return FALSE;
	if(MAX_MEMBER_SERVER <= MemberServerIdx) return FALSE;
	if(!g_memberServer[MemberServerIdx].active) return FALSE;

	return TRUE;
}










inline void CGuildCtrl::BetweenMsGs_ManageServerDecoder_Summary_Req(void)
{
	int MemberServerIdx = MSG_ReadByte();
	if(FALSE == ValidCheck_MemberServerIndex(MemberServerIdx)) return;

	int GuildIdx             = MSG_ReadShort();
	if(FALSE == ValidCheck_GuildIndex(GuildIdx)) return;

	struct tagGuildSummarySyncRecord* pSummaryRecord=GetSummaryRecordPointer(GuildIdx);
	if(NULL == pSummaryRecord){
		MSG_BeginWriting(&netMessage);
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
			MSG_WriteByte(&netMessage, GUILD_SYNC);
			MSG_WriteByte(&netMessage, enumGsvrMsvr::fromMsvr_toGsvr_Summary_Res);
			MSG_WriteShort(&netMessage, GuildIdx);
			MSG_WriteByte(&netMessage, FALSE);			
			NET_SendUnreliableMessage(&g_memberServer[MemberServerIdx].sock, &netMessage);
		}
		MSG_EndWriting(&netMessage);
		return;
	}


	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SYNC);
		MSG_WriteByte(&netMessage, enumGsvrMsvr::fromMsvr_toGsvr_Summary_Res);

		MSG_WriteShort(&netMessage, GuildIdx);
		MSG_WriteByte(&netMessage, TRUE);
		MSG_WriteString(&netMessage,pSummaryRecord->Name);
		MSG_WriteLong(&netMessage, pSummaryRecord->Mark.ChangeDate);
		for( int i=0; i<GUILDMARKSIZE; i++ ) MSG_WriteByte(&netMessage,pSummaryRecord->Mark.Image[i]);		
		NET_SendUnreliableMessage(&g_memberServer[MemberServerIdx].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	return;
}









void CGuildCtrl::BetweenMsGs_ManageServerDecoder(void)
{	
	switch(MSG_ReadByte()){

		case enumGsvrMsvr::fromGsvr_toMsvr_MarkChange_Req:
			BetweenMsGs_ManagerServerDecoder_MarkChange_Req();
			break;


		case enumGsvrMsvr::fromGsvr_toMsvr_Summary_Req:		
			BetweenMsGs_ManageServerDecoder_Summary_Req();
			break;		
	}
}






inline void CGuildCtrl::BetweenMsGs_GameServerDecoder_Summary_Res(void)
{
	int GuildIdx = MSG_ReadShort();
	if(FALSE == ValidCheck_GuildIndex(GuildIdx)) return;


	struct tagGuildSummarySyncRecord* pSummaryRecord=&m_SummarySyncRecord[GuildIdx];
	pSummaryRecord->Sync.bComplete=TRUE;
	pSummaryRecord->bActive = MSG_ReadByte();
	if(FALSE == pSummaryRecord->bActive) return;

	StrCpyN(pSummaryRecord->Name,MSG_ReadString(),GUILDNAMESTRING);
	pSummaryRecord->Name[GUILDNAMESTRING]=NULL;

	pSummaryRecord->Mark.ChangeDate = MSG_ReadLong();
	for(int i=0; i<GUILDMARKSIZE; i++) pSummaryRecord->Mark.Image[i]=MSG_ReadByte();

	return;
}











void CGuildCtrl::BetweenMsGs_GameServerDecoder(void)
{
	switch(MSG_ReadByte()){
		case enumGsvrMsvr::fromMsvr_toGsvr_MarkChange_Notify:		
			BetweenMsGs_GameServerDecoder_MarkChange_Notify();
			break;

		
		case enumGsvrMsvr::fromMsvr_toGsvr_DisOrganize_Notify:
			BetweenMsGs_GameServerDecoder_DisOrganize_Notify();
			break;

		case enumGsvrMsvr::fromMsvr_toGsvr_Summary_Res:		
			BetweenMsGs_GameServerDecoder_Summary_Res();
			break;

		case enumGsvrMsvr::fromMsvr_toGsvr_Organize_Notify:		
			BetweenMsGs_GameServerDecoder_Organize_Notify();
			break;
	}
}









inline void CGuildCtrl::
TransmiteAtClient_MarkChange_Res(
	playerCharacter_t* pPc,
	enum tagPacket_fromGsvr_toClient_MarkChange_Res::enumCode code,
	const int data) const
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SYNC);
		MSG_WriteByte(&netMessage, enumClientGsvr::fromGsvr_toClient_MarkChange_Res);		
		MSG_WriteByte(&netMessage, code);
		MSG_WriteLong(&netMessage, data);
		
		NET_SendMessage(&(pPc->sock),&netMessage);
		
	}
	MSG_EndWriting(&netMessage);



}





inline void CGuildCtrl::TransmiteAtClient_DisOrganize_Notify(const int GuildIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SYNC);
		MSG_WriteByte(&netMessage, enumClientGsvr::fromGsvr_toClient_DisOrganize_Notify);
		MSG_WriteShort(&netMessage, GuildIdx);		


		for (int online_pc_number=0,pcIdx = 0; (pcIdx < MAX_PCS) && (online_pc_number < g_pcNumber); pcIdx++){			
			if( (!g_pc[pcIdx].active) || (!g_pc[pcIdx].ready)) continue;
		
			NET_SendMessage(&g_pc[pcIdx].sock,&netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}






inline void CGuildCtrl::BetweenMsGs_GameServerDecoder_DisOrganize_Notify(void)
{
	int GuildIdx = MSG_ReadShort();
	if(FALSE == ValidCheck_GuildIndex(GuildIdx)) return;

	struct tagGuildSummarySyncRecord* pSummaryRecord=&m_SummarySyncRecord[GuildIdx];

	pSummaryRecord->bActive=FALSE;

	TransmiteAtClient_DisOrganize_Notify(GuildIdx);	
}





void CGuildCtrl::
ManagerServerProc_DisOrganize(const int GuildIdx)
{
	if(FALSE == ValidCheck_GuildIndex(GuildIdx)) return;
	struct tagGuildSummarySyncRecord* pSummaryRecord=GetSummaryRecordPointer(GuildIdx);
	if(NULL == pSummaryRecord) return;	

	pSummaryRecord->bActive=FALSE;

	
	TransmiteAtGs_SyncPacket_DisOrganize(GuildIdx);

	
	TransmiteAtClient_DisOrganize_Notify(GuildIdx);
}







inline void CGuildCtrl::TransmiteAtGs_SyncPacket_DisOrganize(const int GuildIdx)
{

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SYNC);
		MSG_WriteByte(&netMessage, enumGsvrMsvr::fromMsvr_toGsvr_DisOrganize_Notify);
		MSG_WriteShort(&netMessage, GuildIdx);

		
		for(int hMemberServerIdx=1; hMemberServerIdx < MAX_MEMBER_SERVER; hMemberServerIdx++){
			if(!g_memberServer[hMemberServerIdx].active ) continue;					
			NET_SendUnreliableMessage(&g_memberServer[hMemberServerIdx].sock, &netMessage);					
		}
		
	}
	MSG_EndWriting(&netMessage);
}






inline void CGuildCtrl::BetweenMsGs_GameServerDecoder_Organize_Notify(void)
{
	int GuildIdx=MSG_ReadShort();
	if(FALSE == ValidCheck_GuildIndex(GuildIdx)) return;

	struct tagGuildSummarySyncRecord* pSummaryRecord=&m_SummarySyncRecord[GuildIdx];
	StrCpyN(pSummaryRecord->Name,MSG_ReadString(),GUILDNAMESTRING);
	pSummaryRecord->Name[GUILDNAMESTRING]=NULL;

	pSummaryRecord->Mark.ChangeDate = MSG_ReadLong();
	for(int i=0; i< GUILDMARKSIZE; i++) pSummaryRecord->Mark.Image[i]=MSG_ReadByte();



	pSummaryRecord->bActive=TRUE;
	pSummaryRecord->Sync.bComplete=TRUE;
}




void CGuildCtrl::ManagerServerProc_Organize(const int GuildIdx,const char* pName,const unsigned char* pMarkImage)
{
	if(FALSE == ValidCheck_GuildIndex(GuildIdx)) return;

	{
		struct tagGuildSummarySyncRecord* pSummarySyncRecord;
		pSummarySyncRecord = &m_SummarySyncRecord[GuildIdx];

		pSummarySyncRecord->bActive=TRUE;
		StrCpyN(pSummarySyncRecord->Name,pName,GUILDNAMESTRING);
		pSummarySyncRecord->Name[GUILDNAMESTRING]=NULL;

		memcpy(pSummarySyncRecord->Mark.Image,pMarkImage,GUILDMARKSIZE);

		

		g_guild[GuildIdx].markChangeDate=
			pSummarySyncRecord->Mark.ChangeDate = time(NULL);
			 
		pSummarySyncRecord->Sync.bComplete=TRUE;
		g_DBGameServer->SaveGuildMark(GuildIdx);
	}

	TransmiteAtGs_SyncPacket_Organize(GuildIdx);
}




inline void CGuildCtrl::TransmiteAtGs_SyncPacket_Organize(const int GuildIdx)
{
	struct tagGuildSummarySyncRecord* pSummaryRecord=GetSummaryRecordPointer(GuildIdx);
	if(NULL == pSummaryRecord) return;	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SYNC);
		MSG_WriteByte(&netMessage, enumGsvrMsvr::fromMsvr_toGsvr_Organize_Notify);


		MSG_WriteShort(&netMessage, GuildIdx);
		MSG_WriteString(&netMessage,pSummaryRecord->Name);
		MSG_WriteLong(&netMessage,pSummaryRecord->Mark.ChangeDate);
		for(int i=0; i < GUILDMARKSIZE; i++) MSG_WriteByte(&netMessage,pSummaryRecord->Mark.Image[i]);		


		
		for(int hMemberServerIdx=1; hMemberServerIdx < MAX_MEMBER_SERVER; hMemberServerIdx++){
			if(!g_memberServer[hMemberServerIdx].active ) continue;					
			NET_SendUnreliableMessage(&g_memberServer[hMemberServerIdx].sock, &netMessage);					
		}
		
	}
	MSG_EndWriting(&netMessage);
}




void CGuildCtrl::InitWhenManagerServer(void)
{
	struct tagGuildSummarySyncRecord* pSummarySyncRecord;
	guild_t* pGuild;

	for(int GuildIdx=0; GuildIdx < MAX_GUILDS; GuildIdx++){

		pGuild = &g_guild[GuildIdx];
		if(NULL == pGuild) continue;
		if(NULL == pGuild->name[0]) continue;
		
		pSummarySyncRecord = &m_SummarySyncRecord[GuildIdx];

		pSummarySyncRecord->bActive=TRUE;
		StrCpyN(pSummarySyncRecord->Name,pGuild->name,GUILDNAMESTRING);
		pSummarySyncRecord->Name[GUILDNAMESTRING]=NULL;

		memcpy(pSummarySyncRecord->Mark.Image,pGuild->mark,GUILDMARKSIZE);
		pSummarySyncRecord->Mark.ChangeDate = pGuild->markChangeDate;

		pSummarySyncRecord->Sync.bComplete=TRUE;

	}
}


int  CGuildCtrl::Get_transmite_req_sync_at_manager_server_number(void) const
{
	return m_transmite_req_sync_at_manager_server_number;
}




inline void CGuildCtrl::GTH_SendMessage_Guild_Summary_Request_To_ManagerServer(const int Idx) 
{
	m_transmite_req_sync_at_manager_server_number+=1;
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_SYNC);
		MSG_WriteByte(&netMessage, enumGsvrMsvr::fromGsvr_toMsvr_Summary_Req);

		MSG_WriteByte(&netMessage,g_config.gameServerNo);
		MSG_WriteShort(&netMessage, Idx);
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




struct CGuildCtrl::tagGuildSummarySyncRecord* 
CGuildCtrl::GetSummaryRecordPointer(const int Idx)
{
	if(FALSE == ValidCheck_GuildIndex(Idx)) return NULL;
	struct tagGuildSummarySyncRecord* pSummaryRecord=&m_SummarySyncRecord[Idx];

	if(FALSE == pSummaryRecord->Sync.bComplete){

		if(TRUE == pSummaryRecord->Sync.Request.bDo){
			int cur	  = timeGetTime();
			int elasp = cur - pSummaryRecord->Sync.Request.Time;
			if(elasp > 10 * 1000) pSummaryRecord->Sync.Request.bDo=FALSE;
			return NULL;
		}	
		
		GTH_SendMessage_Guild_Summary_Request_To_ManagerServer(Idx);
		pSummaryRecord->Sync.Request.bDo=TRUE;
		pSummaryRecord->Sync.Request.Time=timeGetTime();
		return NULL;
	}


	if(FALSE == pSummaryRecord->bActive) return NULL;
	return pSummaryRecord;
}





BOOL CGuildCtrl::Init(void)
{
	
	memset(m_SummarySyncRecord,0x00,sizeof(struct tagGuildSummarySyncRecord) * MAX_GUILDS);

	return TRUE;
}





void CGuildCtrl::strcpy_null_terminate(char* destStr,const char* sourceStr,const int bytes)
{
	memcpy(destStr,sourceStr,bytes);
	destStr[bytes-1]=NULL;
	if(0 != IsDBCSLeadByte(destStr[bytes-2])) destStr[bytes-2]=NULL;	
};




inline BOOL CGuildCtrl::ValidCheck_GuildIndex(const int Idx) const
{
	if(0 > Idx) return FALSE;
	if(MAX_GUILDS <= Idx) return FALSE;

	return TRUE;
}




inline BOOL CGuildCtrl::ValidCheck(const guild_t* pGuild) const
{
	if(NULL == pGuild->name[0]) return FALSE;
	return TRUE;
}




guild_t* CGuildCtrl::GetRecordPointer(const int Idx)
{
	if(FALSE == ValidCheck_GuildIndex(Idx)) return NULL;
	if(FALSE == ValidCheck(&g_guild[Idx])) return NULL;	
	return &g_guild[Idx];
}

guild_t* CGuildCtrl::GetRecordPointerWithoutValidCheck(const int Idx)
{
	if(FALSE == ValidCheck_GuildIndex(Idx)) return NULL;	
	return &g_guild[Idx];	
}








int CGuildCtrl::GetIndexByName(const char* szpGuildName)
{
	int Idx;
	for( Idx=0; Idx < MAX_GUILDS; Idx++ ){
		if(FALSE == ValidCheck_GuildIndex(Idx)) continue;
		if(FALSE == ValidCheck(&g_guild[Idx])) continue;
		if(0 == stricmp(g_guild[Idx].name,szpGuildName)) return Idx;
	}
	return -1;
}






inline void CGuildCtrl::ManagerServerProc_MarkChange(const int GuildIdx,const unsigned char* pMarkImage)
{	
	struct tagGuildSummarySyncRecord* pSummaryRecordPointer=GetSummaryRecordPointer(GuildIdx);
	if(NULL == pSummaryRecordPointer) return;

	
	g_guild[GuildIdx].markChangeDate = 
		pSummaryRecordPointer->Mark.ChangeDate=time(NULL);
			

	memcpy(pSummaryRecordPointer->Mark.Image,pMarkImage,GUILDMARKSIZE);	
	memcpy(g_guild[GuildIdx].mark,pSummaryRecordPointer->Mark.Image,GUILDMARKSIZE);

	g_DBGameServer->SaveGuild(GuildIdx);

	
	TransmiteAtGs_SyncPacket_UpdateMark(GuildIdx,pSummaryRecordPointer);
	
	TransmiteAtClient_MarkCheck_Req(GuildIdx,pSummaryRecordPointer);
}


inline void CGuildCtrl::BetweenMsGs_ManagerServerDecoder_MarkChange_Req(void)
{
	int GuildIdx = MSG_ReadShort();
	unsigned char MarkImage[GUILDMARKSIZE];
	for(int i=0; i< GUILDMARKSIZE; i++) MarkImage[i]=MSG_ReadByte();

	
	
	
	ManagerServerProc_MarkChange(GuildIdx,MarkImage);
}





inline void CGuildCtrl::TransmiteAtMs_UpdateMark(const int GuildIdx,const unsigned char* pMark)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_SYNC);
		MSG_WriteByte(&netMessage,enumGsvrMsvr::fromGsvr_toMsvr_MarkChange_Req);
		
		MSG_WriteShort(&netMessage, GuildIdx);
		for(int i=0; i< GUILDMARKSIZE; i++) MSG_WriteByte(&netMessage,pMark[i]);
		
		NET_SendUnreliableMessage(&localSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




inline void CGuildCtrl::BetweenClientGs_Decoder_MarkChange_Req(void)
{
	int GuildIdx=g_curPC->guildIdx;
	if(FALSE == ValidCheck_GuildIndex(GuildIdx)) return;


	struct tagGuildSummarySyncRecord* pSummaryRecordPointer=GetSummaryRecordPointer(GuildIdx);
	if(NULL == pSummaryRecordPointer){
		TransmiteAtClient_MarkChange_Res(g_curPC,tagPacket_fromGsvr_toClient_MarkChange_Res::error_unknown,0);
		return;
	}


	int elasp_second = time(NULL) - pSummaryRecordPointer->Mark.ChangeDate;
	if(abs(elasp_second) < (g_logic.danMarkChangeDelayHour * 60 * 60)){	
		int remain_second = (g_logic.danMarkChangeDelayHour * 60 * 60) - abs(elasp_second);
		int remain_hour   = remain_second / (60 * 60);
		if(remain_hour < 1) remain_hour=1;

		TransmiteAtClient_MarkChange_Res(g_curPC,tagPacket_fromGsvr_toClient_MarkChange_Res::error_delay_hour,remain_hour);
		return;
	}

	TransmiteAtClient_MarkChange_Res(g_curPC,tagPacket_fromGsvr_toClient_MarkChange_Res::Success,0);
	

	

	unsigned char MarkImage[GUILDMARKSIZE];
	for(int i=0; i< GUILDMARKSIZE; i++) MarkImage[i]=MSG_ReadByte();


	if(g_config.isManager){
		ManagerServerProc_MarkChange(GuildIdx,MarkImage);
	}else{
		TransmiteAtMs_UpdateMark(GuildIdx,MarkImage);
	}
	
}





inline void CGuildCtrl::BetweenClientGs_Decoder_Summary_Req(void)
{
	int GuildIdx=MSG_ReadShort();
	if(FALSE == ValidCheck_GuildIndex(GuildIdx)) return;

	struct tagGuildSummarySyncRecord* pSummaryRecord=GetSummaryRecordPointer(GuildIdx);
	if(NULL == pSummaryRecord) return;


	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SYNC);
		MSG_WriteByte(&netMessage, enumClientGsvr::fromGsvr_toClient_Summary_Res);
		
		MSG_WriteShort(&netMessage, GuildIdx);
		MSG_WriteString(&netMessage, pSummaryRecord->Name);
		MSG_WriteLong(&netMessage,  pSummaryRecord->Mark.ChangeDate);		
		for( int i=0; i<GUILDMARKSIZE; i++ ) MSG_WriteByte(&netMessage,pSummaryRecord->Mark.Image[i]);
		
		NET_SendMessage(&g_curPC->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}





inline void CGuildCtrl::
BetweenClientGs_Decoder_MarkImage_Req(void)
{
	int GuildIdx= MSG_ReadShort();
	if(FALSE == ValidCheck_GuildIndex(GuildIdx)) return;

	struct tagGuildSummarySyncRecord* pSummaryRecord=GetSummaryRecordPointer(GuildIdx);
	if(NULL == pSummaryRecord) return;


	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SYNC);
		MSG_WriteByte(&netMessage, enumClientGsvr::fromGsvr_toClient_MarkImage_Res);
		
		MSG_WriteShort(&netMessage, GuildIdx);		
		MSG_WriteLong(&netMessage,  pSummaryRecord->Mark.ChangeDate);		
		for( int i=0; i<GUILDMARKSIZE; i++ ) MSG_WriteByte(&netMessage,pSummaryRecord->Mark.Image[i]);
		
		NET_SendMessage(&g_curPC->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void CGuildCtrl::BetweenClientGs_Decoder(void)
{
	switch(MSG_ReadByte()){
		case enumClientGsvr::fromClient_toGsvr_MarkImage_Req:		
			BetweenClientGs_Decoder_MarkImage_Req();
			break;
		
		case enumClientGsvr::fromClient_toGsvr_Summary_Req:
			BetweenClientGs_Decoder_Summary_Req();
			break;

		case enumClientGsvr::fromClient_toGsvr_MarkChange_Req:		
			BetweenClientGs_Decoder_MarkChange_Req();
			break;
	}
}






inline void CGuildCtrl::
TransmiteAtClient_MarkCheck_Req(const int GuildIdx,const struct tagGuildSummarySyncRecord* pSummaryRecord)
{
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SYNC);
		MSG_WriteByte(&netMessage, enumClientGsvr::fromGsvr_toClient_MarkCheck_Req);
		MSG_WriteShort(&netMessage, GuildIdx);
		MSG_WriteLong(&netMessage,pSummaryRecord->Mark.ChangeDate);




		for (int online_pc_number=0,pcIdx = 0; (pcIdx < MAX_PCS) && (online_pc_number < g_pcNumber); pcIdx++){			
			if( (!g_pc[pcIdx].active) || (!g_pc[pcIdx].ready)) continue;

			online_pc_number+=1;
			NET_SendMessage(&g_pc[pcIdx].sock,&netMessage);
		}
	}



	MSG_EndWriting(&netMessage);
}

void CGuildCtrl::BetweenMsGs_GameServerDecoder_MarkChange_Notify(void)
{
	int GuildIdx = MSG_ReadShort();
	if(FALSE == ValidCheck_GuildIndex(GuildIdx)) return;

	struct tagGuildSummarySyncRecord* pSummaryRecord=GetSummaryRecordPointer(GuildIdx);
	if(NULL == pSummaryRecord) return;

	g_guild[GuildIdx].markChangeDate =
		pSummaryRecord->Mark.ChangeDate = MSG_ReadLong();
	

	
	for( int i=0; i<GUILDMARKSIZE; i++ ) 
		pSummaryRecord->Mark.Image[i]=MSG_ReadByte();


	TransmiteAtClient_MarkCheck_Req(GuildIdx,pSummaryRecord);

	
}




inline void CGuildCtrl::TransmiteAtGs_SyncPacket_UpdateMark(const int GuildIdx,const struct tagGuildSummarySyncRecord* pSummaryRecord)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SYNC);
		MSG_WriteByte(&netMessage, enumGsvrMsvr::fromMsvr_toGsvr_MarkChange_Notify);

		MSG_WriteShort(&netMessage, GuildIdx);		
		MSG_WriteLong(&netMessage, pSummaryRecord->Mark.ChangeDate);
		for( int i=0; i<GUILDMARKSIZE; i++ ) MSG_WriteByte(&netMessage,pSummaryRecord->Mark.Image[i]);		


		
		for(int hMemberServerIdx=1; hMemberServerIdx < MAX_MEMBER_SERVER; hMemberServerIdx++){
			if(!g_memberServer[hMemberServerIdx].active ) continue;					
			NET_SendUnreliableMessage(&g_memberServer[hMemberServerIdx].sock, &netMessage);					
		}		
	}
	MSG_EndWriting(&netMessage);

}