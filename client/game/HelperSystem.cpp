



#include "../global_def.h"





CHelperSystem::CHelperSystem()
{
	m_iGivePoint = 0;
	memset(&m_MyPointInfo, 0, sizeof(PointInfo));
	for(int i=0; i < MAX_TAKER_LIST; i++)
		memset(&m_HelperEntry[i], 0, sizeof(HelperEntry));
	m_HelperEntry->bDeleteMe = FALSE;

	
}


CHelperSystem::~CHelperSystem() { }

void CHelperSystem::GTH_ProcessMessage_RecvHelpSystem()
{
	int WhatDoYouMean = MSG_ReadShort();
	
	switch(WhatDoYouMean)
	{
	case SC_INVITE_Req_toTAKER:				  
		GTH_ProcessMessage_HelpSystem_NeedToHelper();
		break;

	case SC_INVITE_Res_toHELPER:              
		GTH_ProcessMessage_HelpSystem_DoYouWant();
		break;
		
	case SC_INVITE_Res_toTAKER:	
		GTH_ProcessMessage_HelpSystem_RequstComplete();
		break;

	case SC_REMOVE_Res_toPlayer:	
		GTH_ProcessMessage_HelpSystem_RemovePlayer();
		break;

	case SC_REMOVE_NOTIFY_toPlayer:
		GTH_ProcessMessage_HelpSystem_NotifytoRemovePlayer();
		break;

	case SC_SEND_MEMBERLIST_toAllPlayer: 
		GTH_ProcessMessage_HelpSystem_UpDateListAll();
		break;

	case SC_SEND_ADD_toPlayer:
		GTH_ProcessMessage_HelpSystem_AddToPlayer();
		break;

	case SC_LIST_Res_toServer:
		GTH_ProcessMessage_HelpSystem_UpDateListAll();
		break;

	case SC_LISTINFO_toAllPlayer:
		GTH_ProcessMessage_HelpSystem_UpDateList();
		break;

	case SC_UPDATE_POINTINFO_SEND_toPlayer:
		GTH_ProcessMessage_HelpSystem_UpDatePointInfo();
		break;

	case SC_SPAWN_Req_toTaker:
		GTH_ProcessMessage_HelpSystem_SpawnTaker();
		break;

	case SC_SPAWN_Req_toServer:
		break;

	case SC_SPAWN_Res_toTaker:
		GTH_ProcessMessage_HelpSystem_Portal();
		break;

	case SC_SPAWN_Res_toServer:
		break;

	case SC_CONFIRM_MYPOINT_Res_toPlayer:
		GTH_ProcessMessage_HelpSystem_DisplayMyPoint();
		break;
			
	case SC_CONFIRM_HELPPOINT_Res_toPlayer:
		GTH_ProcessMessage_HelpSystem_DisplayHelpPoint();
		break;
			
	case SC_CONFIRM_WEPPOINT_Res_toPlayer:
		GTH_ProcessMessage_HelpSystem_DisplayWebPoint();
		break;

	case SC_STACK_OPEN_Req_toPlayer:
		GTH_ProcessMessage_HelpSystem_StackOpen();
		break;
		
	case SC_SENDMYPOINT_toAllPlayer:
		GTH_ProcessMessage_HelpSystem_GivePointResultToTaker();
		break;
		
	case SC_SENDHELPPOINT_toHelper:
		GTH_ProcessMessage_HelpSystem_GivePointResultToHelper();
		break;

	default:
		return;
	}
}







void CHelperSystem::GTH_ProcessMessage_HelpSystem_NeedToHelper(void)
{
	char szMsg[256+1];
	char szName[NAMESTRING+1];
	
	strncpy(szName, MSG_ReadString(), NAMESTRING);
	szName[NAMESTRING]=NULL;
	
	_snprintf(szMsg, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,296), szName);
	szMsg[256]=NULL;
	
	g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,298), szMsg, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_HELPERSYSTEM_DOYOUWANTME, 600000.0f);
}


void CHelperSystem::GTH_ProcessMessage_HelpSystem_DoYouWant(void)
{
	if( g_ifMng->m_msgType == IF_MESSAGE_HELPERSYSTEM_DOYOUWANTME )		
			g_ifMng->ExitMessage();	

	int Code = MSG_ReadShort();
	
		switch(Code)
		{
		case 0:
			
			
			g_ifMng->ExitMessage();	
			break;

		case 1:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,386));
			break;

		case 2:
			
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322),g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,589));
			break;

		case 3:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,294));
			break;

		case 4:
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,388));
			
			break;

		case 6:
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,389));
			
			break;

		case 7:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,295));
			break;

		case 8:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,318));
			break;

		case 13: 
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,390));
			break;

		case 15:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,293));
			break;

		case 16:
			
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,590));
			
			break;
		case 17:
			
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,591) );
			break;	
		default:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,266));
			break;
		}
	
}

void CHelperSystem::GTH_ProcessMessage_HelpSystem_RequstComplete(void)
{
	if( g_ifMng->m_msgType == IF_MESSAGE_HELPERSYSTEM_DOYOUWANTME )		
			g_ifMng->ExitMessage();	
	int errorCode = MSG_ReadShort();
	
	if(errorCode)
	{
		switch(errorCode)
		{
		case 0:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,321));
			break;

		case 1:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,319));
			break;

		case 4:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,320));
			break;

		default:
			break;
		}
	}
}

void CHelperSystem::GTH_ProcessMessage_HelpSystem_RemovePlayer(void)
{
	int Code = MSG_ReadShort();

	if(Code)
	{
		switch(Code)
		{
		case 10:
			
			g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,322));
			break;

		case 14:
			char RemoveName[NAMESTRING+1];
			strncpy(RemoveName, MSG_ReadString(), NAMESTRING);
			RemoveName[NAMESTRING]=NULL;
			g_ifMng->m_HelperWin->RemoveHelper(RemoveName);
			break;

		default:
			break;
		}
	}
}


void CHelperSystem::GTH_ProcessMessage_HelpSystem_NotifytoRemovePlayer(void)
{
	char EraserName[NAMESTRING+1];
	strncpy(EraserName, MSG_ReadString(), NAMESTRING);	
	EraserName[NAMESTRING]=NULL;
	char notify[256+1];
	
	_snprintf(notify, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,323), EraserName);
	notify[256]=NULL;
	g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), notify);
	
}

void CHelperSystem::GTH_ProcessMessage_HelpSystem_AddToPlayer(void)
{
	HelperEntry	pHelperEntry;

	pHelperEntry.level		= MSG_ReadByte();
	pHelperEntry.job		= MSG_ReadByte();
	pHelperEntry.worldIdx	= MSG_ReadByte();
	strncpy(pHelperEntry.name, MSG_ReadString(), NAMESTRING);
	pHelperEntry.name[NAMESTRING]=NULL;
	m_HelperEntry->bDeleteMe = FALSE;
	g_ifMng->m_HelperWin->AddHelper(&pHelperEntry);
	
	if( g_ifMng->m_msgType == IF_MESSAGE_HELPERSYSTEM_DOYOUWANTME )		
			g_ifMng->ExitMessage();	
}

void CHelperSystem::GTH_ProcessMessage_HelpSystem_UpDateListAll(void)
{

	for(int i=0; i < MAX_TAKER_LIST; i++)
	{
		memset(&m_HelperEntry[i], 0, sizeof(HelperEntry));
		m_HelperEntry[i].bDeleteMe = FALSE;
	}
	g_ifMng->m_HelperWin->m_totalOnlineHelperCount = 0;
	g_ifMng->m_HelperWin->m_selectedHelperIdx = -1;
	g_ifMng->m_HelperWin->m_totalHelperCount = MSG_ReadByte();
	g_ifMng->m_HelperWin->m_Mode  = MSG_ReadShort();

	for(i = 0; i < g_ifMng->m_HelperWin->m_totalHelperCount; i++)
	{
		m_HelperEntry[i].level		=	MSG_ReadByte();
		m_HelperEntry[i].job		=	MSG_ReadByte();
		m_HelperEntry[i].worldIdx	=	MSG_ReadByte();
		strncpy(m_HelperEntry[i].name,  MSG_ReadString(), NAMESTRING);
		m_HelperEntry[i].name[NAMESTRING]=NULL;
		m_HelperEntry[i].bDeleteMe  = MSG_ReadByte();
		if(g_HelperSystem.m_HelperEntry[i].worldIdx != -1 && g_HelperSystem.m_HelperEntry[i].worldIdx != 255 )
			g_ifMng->m_HelperWin->m_totalOnlineHelperCount++;
	}
}

void CHelperSystem::GTH_ProcessMessage_HelpSystem_UpDateList(void)
{
	HelperEntry Entry;
	
	Entry.level		=	MSG_ReadByte();
	Entry.job		=	MSG_ReadByte();
	Entry.worldIdx	=	MSG_ReadByte();
	strncpy(Entry.name,  MSG_ReadString(), NAMESTRING);
	Entry.name[NAMESTRING]=NULL;
	g_ifMng->m_HelperWin->UpdateHelper(&Entry);
}

void CHelperSystem::GTH_ProcessMessage_HelpSystem_UpDatePointInfo(void)
{
	m_MyPointInfo.iMyPoint			= MSG_ReadLong();
	m_MyPointInfo.iHelpPoint		= MSG_ReadLong();
	m_MyPointInfo.iTotalHelpPoint	= MSG_ReadLong();
}

void CHelperSystem::GTH_ProcessMessage_HelpSystem_SpawnTaker(void)
{
	char SpawnName[NAMESTRING+1];
	char notify[256+1];
	strncpy(SpawnName, MSG_ReadString(), NAMESTRING);
	SpawnName[NAMESTRING]=NULL;
	
	_snprintf(notify,256,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,324), SpawnName);
	notify[256]=NULL;
	g_ifMng->SetMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,316), notify, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_HELPERSYSTEM_DOYOUWANTPORTAL, 600000.0f);
	g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), notify);
}


void CHelperSystem::GTH_ProcessMessage_HelpSystem_Portal(void)
{
	int reply = MSG_ReadByte();
	if(reply)	
	{
		char PortalTargetCharacterName[NAMESTRING+1];

		strncpy(PortalTargetCharacterName, MSG_ReadString(), NAMESTRING);
		PortalTargetCharacterName[NAMESTRING]=NULL;
	}
}

void CHelperSystem::GTH_ProcessMessage_HelpSystem_DisplayMyPoint(void)
{
	char szTemp[256+1];
	g_HelperSystem.m_MyPointInfo.iMyPoint = MSG_ReadLong();
	
	_snprintf(szTemp,256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,330), g_HelperSystem.m_MyPointInfo.iMyPoint);
	szTemp[256]=NULL;
	g_ifMng->AddSysMessage(szTemp);
}

void CHelperSystem::GTH_ProcessMessage_HelpSystem_DisplayHelpPoint(void)
{
	char szTemp[256+1];
	g_HelperSystem.m_MyPointInfo.iHelpPoint = MSG_ReadLong();
	
	_snprintf(szTemp,256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,325), g_HelperSystem.m_MyPointInfo.iHelpPoint);
	szTemp[256]=NULL;
	g_ifMng->AddSysMessage(szTemp);
}

void CHelperSystem::GTH_ProcessMessage_HelpSystem_DisplayWebPoint(void)
{
	g_HelperSystem.m_MyPointInfo.iWebPoint = MSG_ReadLong();
}


void CHelperSystem::GTH_ProcessMessage_HelpSystem_StackOpen(void)
{
	
}

void CHelperSystem::GTH_ProcessMessage_HelpSystem_GivePointResultToTaker(void)
{
	char szTemp[256+1];
	char TakerName[NAMESTRING+1];
	char HelperName[NAMESTRING+1];
	strncpy(TakerName, MSG_ReadString(), NAMESTRING);
	TakerName[NAMESTRING]=NULL;
	strncpy(HelperName, MSG_ReadString(), NAMESTRING);
	HelperName[NAMESTRING]=NULL;
	int delta_HelpPoint = MSG_ReadLong();
	
	_snprintf(szTemp,256,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,306),TakerName,HelperName,delta_HelpPoint);
	szTemp[256]=NULL;
	GTH_SendMessage_WorldChatting(szTemp, TRUE);
}

void CHelperSystem::GTH_ProcessMessage_HelpSystem_GivePointResultToHelper(void)
{
	char szTemp[256+1];
	char Name[NAMESTRING+1];
	strncpy(Name, MSG_ReadString(), NAMESTRING);
	Name[NAMESTRING]=NULL;
	int delta_HelpPoint = MSG_ReadLong();
	
	_snprintf(szTemp,256,g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,307),delta_HelpPoint);
	szTemp[256]=NULL;
	g_ifMng->AddSysMessage(szTemp);
}





void CHelperSystem::GTH_SendMessage_HelpSystem_DoYouWant(void)
{
	MSG_BeginWriting(&netMessage);
	
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage,	CS_INVITE_Req_fromHELPER);		
		
		MSG_WriteString(&netMessage, g_cgv.pTargetChar->name);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CHelperSystem::GTH_SendMessage_HelpSystem_AnswerYesOrNo(int Answer)
{
	MSG_BeginWriting(&netMessage);
	
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage,	CS_INVITE_Res_fromTAKER);		

		MSG_WriteShort(&netMessage, Answer);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CHelperSystem::GTH_SendMessage_HelpSystem_RemovePlayer(char *DeleteHelperName)
{
	MSG_BeginWriting(&netMessage);
	
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage,	CS_REMOVE_Req_fromPlayer);	

		MSG_WriteString(&netMessage, DeleteHelperName);
		MSG_WriteString(&netMessage, g_cgv.myCharacterInfo->name);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CHelperSystem::GTH_SendMessage_HelpSystem_GivePointToHelper(int GivePoint, char *HelperName)
{
	MSG_BeginWriting(&netMessage);
	
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage,	CS_GIVEPOINT_Req_toHelper);		

		MSG_WriteLong(&netMessage, m_iGivePoint);
		MSG_WriteString(&netMessage, HelperName);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}



void CHelperSystem::GTH_SendMessage_HelpSystem_SpawnTaker(char *SpawnHelperName)
{
	MSG_BeginWriting(&netMessage);

	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage,	CS_SPAWN_Req_fromHelper);		

		MSG_WriteString(&netMessage, SpawnHelperName);
	
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CHelperSystem::GTH_SendMessage_HelpSystem_SpawnMeAnswer(int YesOrNo)
{
	MSG_BeginWriting(&netMessage);
	
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage,	CS_SPAWN_Res_fromTaker);		
		
		MSG_WriteByte(&netMessage, YesOrNo);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CHelperSystem::GTH_SendMessage_HelpSystem_DisplayMyPoint(void)
{
	MSG_BeginWriting(&netMessage);

	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);

		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage, CS_CONFIRM_MYPOINT_Req_fromPlayer);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CHelperSystem::GTH_SendMessage_HelpSystem_DisplayHelpPoint(void)
{
	MSG_BeginWriting(&netMessage);
	
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage, CS_CONFIRM_HELPPOINT_Req_fromPlayer);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CHelperSystem::GTH_SendMessage_HelpSystem_DisplayWebPoint(void)
{
	MSG_BeginWriting(&netMessage);
	
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage, CS_CONFIRM_WEPPOINT_Req_fromPlayer);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void CHelperSystem::GTH_SendMessage_HelpSystem_GiveMeListAll(void)
{
	MSG_BeginWriting(&netMessage);
	
	MSG_Clear(&netMessage);
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, HELPER_SYSTEM);
		MSG_WriteShort(&netMessage, CS_MemBerList_Req_fromPlayer);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

BOOL CHelperSystem::IsChild(char* TarketName)
{
	for( int nIdx = 0; nIdx < g_ifMng->m_HelperWin->m_totalHelperCount; nIdx++){
		
			if( 0 == strncmp(m_HelperEntry[nIdx].name,TarketName,NAMESTRING) ) return TRUE;
	}

	return FALSE;
}
