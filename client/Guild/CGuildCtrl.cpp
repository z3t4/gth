



#include "..\global_def.h"
#include "CGuildCtrl.h"
#include "Shlwapi.h"




CGuildCtrl::CGuildCtrl()
{
	m_bOK=FALSE;


	if(FALSE == Init()) return;
	
	

	m_bOK=TRUE;

}





CGuildCtrl::~CGuildCtrl()
{

}





inline BOOL CGuildCtrl::Init(void)
{
	

	return TRUE;
	
}






void CGuildCtrl::Reset(void)
{

	m_SummaryRecordMap.clear();
}






BOOL CGuildCtrl::Prepare(void)
{
	
	
	


	
	if(FALSE == g_ifMng->m_guildMarkMng->PreparePactMarkBuffers(CIFPMManager::MAXDRAWNUMperRENDER)){	
		
		
		OutputDebugString("error");
		return FALSE;
	}

	if(false == g_ifMng->m_guildMarkMng->RestoreDeviceObjects()){
		OutputDebugString("error");
		return FALSE;
	}

	return TRUE;
}







BOOL CGuildCtrl::isOK(void) const
{
	return m_bOK; 
}




inline void CGuildCtrl::TransmitePackete_Summary_Req(const int guildIdx) const
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_GUILD );
		MSG_WriteByte(&netMessage, GUILD_SYNC );
		MSG_WriteByte(&netMessage,enumClientGsvr::fromClient_toGsvr_Summary_Req);
		MSG_WriteShort( &netMessage, guildIdx );
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );

}







inline void CGuildCtrl::Decoder_Summary_Res(void)
{
	int GuildIdx=MSG_ReadShort();
	std::map< int, struct tagSummaryRecord>::iterator itor = m_SummaryRecordMap.find(GuildIdx);
	if(itor == m_SummaryRecordMap.end()) return;


	StrCpyN((itor->second).Name,MSG_ReadString(),GUILDNAMESTRING);
	(itor->second).Name[GUILDNAMESTRING]=NULL;


	(itor->second).Mark.bActive=TRUE;
	(itor->second).Mark.ChangeCnt = MSG_ReadLong();


	unsigned char MarkImage[GUILDMARKSIZE];
	for(int i=0; i < GUILDMARKSIZE; i++) MarkImage[i]=MSG_ReadByte();

	(itor->second).Sync.bComplete=TRUE;



	if(false == g_ifMng->m_guildMarkMng->INSERTorCHANGE_PactMarks(GuildIdx,MarkImage)){
	
		
		OutputDebugString("error");
	}
	if(false == g_ifMng->m_guildMarkMng->UpdateGuildTexture()){
		OutputDebugString("error");
	}

}




inline void CGuildCtrl::Decoder_MarkCheck_Req(void)
{
	int GuildIdx=MSG_ReadShort();
	std::map< int, struct tagSummaryRecord>::iterator itor = m_SummaryRecordMap.find(GuildIdx);
	if(itor == m_SummaryRecordMap.end()) return;


	if((itor->second).Mark.ChangeCnt != MSG_ReadLong()){
		(itor->second).Mark.bActive=FALSE;
	}

}




inline void CGuildCtrl::Decoder_MarkImage_Res(void)
{
	int GuildIdx=MSG_ReadShort();
	std::map< int, struct tagSummaryRecord>::iterator itor = m_SummaryRecordMap.find(GuildIdx);
	if(itor == m_SummaryRecordMap.end()) return;


	(itor->second).Mark.ChangeCnt = MSG_ReadLong();
	(itor->second).Mark.bActive=TRUE;

	unsigned char MarkImage[GUILDMARKSIZE];
	for(int i=0; i < GUILDMARKSIZE; i++) MarkImage[i]=MSG_ReadByte();


	if(FALSE == g_ifMng->m_guildMarkMng->INSERTorCHANGE_PactMarks(GuildIdx,MarkImage)){
		
		assert(FALSE);		
	}
	if(false == g_ifMng->m_guildMarkMng->UpdateGuildTexture()){
		assert(FALSE);		
	}
}






inline void CGuildCtrl::Decoder_DisOrganize_Notify(void)
{
	int GuildIdx=MSG_ReadShort();
	std::map< int, struct tagSummaryRecord>::iterator itor = m_SummaryRecordMap.find(GuildIdx);
	if(itor == m_SummaryRecordMap.end()) return;


	m_SummaryRecordMap.erase(itor);
}




inline void CGuildCtrl::Decoder_MarkChange_Res(void)
{
	switch(MSG_ReadByte()){

		case tagPacket_fromGsvr_toClient_MarkChange_Res::error_delay_hour:
			{
				char message[_MAX_PATH];
				wsprintf(message,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,141-1),MSG_ReadLong());
				g_ifMng->AddSysMessage(message, GTHCOLOR_ERRORMESSAGE );
			}
			break;

		case tagPacket_fromGsvr_toClient_MarkChange_Res::success:
			g_ifMng->AddSysMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,140-1), GTHCOLOR_ERRORMESSAGE );
			break;

	}
}




void CGuildCtrl::Decoder(void)
{
	switch(MSG_ReadByte()){

		case enumClientGsvr::fromGsvr_toClient_MarkChange_Res:
			Decoder_MarkChange_Res();
			break;

		case enumClientGsvr::fromGsvr_toClient_DisOrganize_Notify:
			Decoder_DisOrganize_Notify();
			break;

		case enumClientGsvr::fromGsvr_toClient_MarkImage_Res:						
			Decoder_MarkImage_Res();
			break;


		case enumClientGsvr::fromGsvr_toClient_MarkCheck_Req:			
			Decoder_MarkCheck_Req();
			break;

		
		case enumClientGsvr::fromGsvr_toClient_Summary_Res:		
			Decoder_Summary_Res();
			break;
	}
}





BOOL CGuildCtrl::GetSummaryRecord(const int Idx,struct tagSummaryRecord& GuildSummaryRecord)
{
	std::map< int, struct tagSummaryRecord>::iterator itor;	
	itor	=	m_SummaryRecordMap.find(Idx);
	if(itor == m_SummaryRecordMap.end()){

		struct tagSummaryRecord TempSummaryRecord;
		memset(&TempSummaryRecord,0x00,sizeof(struct tagSummaryRecord));


		std::pair<std::map< int, struct tagSummaryRecord>::iterator,BOOL> TempPair;
		TempPair=m_SummaryRecordMap.insert(std::map< int, struct tagSummaryRecord>::value_type(Idx,TempSummaryRecord));
		if(FALSE == TempPair.second) return FALSE;
		itor = TempPair.first;

		
	}

	int curTime=timeGetTime();
	
	if(TRUE == (itor->second).Sync.bComplete){


		if(FALSE == (itor->second).Mark.bActive){

			if(TRUE == (itor->second).Mark.Request.bDo){
				int elasp= curTime - (itor->second).Mark.Request.Time;
				if(elasp > 10 * 1000) (itor->second).Mark.Request.bDo=FALSE;
			}else{
				(itor->second).Mark.Request.bDo = TRUE;
				(itor->second).Mark.Request.Time = curTime;
				TransmitePacket_MarkImage_Req(Idx);
			}
		}
		GuildSummaryRecord=itor->second;
		return TRUE;
	}
	

	
	if(TRUE == (itor->second).Sync.Request.bDo){
		
		int elasp = curTime - (itor->second).Sync.Request.Time;
		if(elasp < 10 * 1000) return FALSE;
	}

	(itor->second).Sync.Request.bDo=TRUE;
	(itor->second).Sync.Request.Time=curTime;
	
	TransmitePackete_Summary_Req(Idx);		

	return FALSE;
}





void CGuildCtrl::
TransmitePacket_MarkChange_Req(const int GuildIdx,const unsigned char* pMark) const
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_GUILD );
		MSG_WriteByte(&netMessage, GUILD_SYNC );
		MSG_WriteByte(&netMessage,enumClientGsvr::fromClient_toGsvr_MarkChange_Req);
		for(int i=0; i < GUILDMARKSIZE; i++) MSG_WriteByte(&netMessage,pMark[i]);
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );
}




void CGuildCtrl::
TransmitePacket_MarkImage_Req(const int GuildIdx) const
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_GUILD );
		MSG_WriteByte(&netMessage, GUILD_SYNC );
		MSG_WriteByte(&netMessage,enumClientGsvr::fromClient_toGsvr_MarkImage_Req);
		MSG_WriteShort(&netMessage,GuildIdx);
		
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );
}



