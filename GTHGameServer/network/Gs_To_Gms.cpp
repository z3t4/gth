



#include "../Global.h"
#include "Gs_To_Gms.h"
#include "Gms_To_Gs.h"

CGs_To_Gms g_GsToGms;

extern CItem_Scroll_Proc		g_Item_Scroll_Proc;





CGs_To_Gms::CGs_To_Gms()
{

}

CGs_To_Gms::~CGs_To_Gms()
{

}

void CGs_To_Gms::Decoder(playerCharacter_t* pPlayer)
{
	short PacketID =MSG_ReadShort();

	switch(PacketID){
	case ITEM_SUMMON_SYSTEM:
			g_Item_Scroll_Proc.GSDecoder();
			break;

	}
}


int  CGs_To_Gms::Send_GMS(int SystemType,int mainType, int subType,char *FromPlayerName,char *TargetPlayerName,int gameServerNo,int CheckTarget)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GS_TO_GMS_PACKET);

		MSG_WriteShort(&netMessage, SystemType);
		MSG_WriteShort(&netMessage, mainType);
		MSG_WriteShort(&netMessage, subType);

		MSG_WriteString(&netMessage, FromPlayerName);
		MSG_WriteString(&netMessage, TargetPlayerName);
	
		if ( TRUE == g_config.isManager)
		{
			entityPCList_t entityPC;

			
			
			if(enumCheckType::CHECK_PLAYER1 == CheckTarget)
			{
				int FindIdx =  g_entityListMng.FindPCList( FromPlayerName, &entityPC );				
				if(-1 == FindIdx) return -1;
			}
			
			else if(enumCheckType::CHECK_PLAYER2 == CheckTarget)
			{
				int FindIdx =  g_entityListMng.FindPCList( TargetPlayerName, &entityPC );				
				if(-1 == FindIdx) return -1;
			}
			else
			{
				return -1;
			}

			if(g_config.gameServerNo == entityPC.serverIdx)
			{
				
				
				
				
				return 0;
			}

			if(0 > entityPC.serverIdx  || entityPC.serverIdx >= MAX_MEMBER_SERVER)
			{
				char txt[256];
				wsprintf(txt,"Send_GMS :: 0 > entityPC.serverIdx  || entityPC.serverIdx >= MAX_MEMBER_SERVER mainType %d, subType %d, %s -> %s ",mainType,subType,FromPlayerName,TargetPlayerName);
				TRACE(txt);
				return 0;
			}
				

			NET_SendUnreliableMessage(&g_memberServer[entityPC.serverIdx].sock, &netMessage);
		} 
		else
		{
			MSG_WriteShort(&netMessage, gameServerNo);
			NET_SendUnreliableMessage(&localSocket, &netMessage);
		}		
	}
	MSG_EndWriting(&netMessage);
	return 0;
}




int  CGs_To_Gms::Send_GMS(int SystemType,int mainType, int subType,long value,char *FromPlayerName,char *TargetPlayerName,int gameServerNo,int CheckTarget)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GS_TO_GMS_PACKET);

		MSG_WriteShort(&netMessage, SystemType);
		MSG_WriteShort(&netMessage, mainType);
		MSG_WriteShort(&netMessage, subType);

		MSG_WriteLong(&netMessage,   value);

		MSG_WriteString(&netMessage, FromPlayerName);
		MSG_WriteString(&netMessage, TargetPlayerName);
	
		if ( TRUE == g_config.isManager)
		{
			entityPCList_t entityPC;

			
			
			if(enumCheckType::CHECK_PLAYER1 == CheckTarget)
			{
				int FindIdx =  g_entityListMng.FindPCList( FromPlayerName, &entityPC );				
				if(-1 == FindIdx) return -1;
			}
			
			else if(enumCheckType::CHECK_PLAYER2 == CheckTarget)
			{
				int FindIdx =  g_entityListMng.FindPCList( TargetPlayerName, &entityPC );				
				if(-1 == FindIdx) return -1;
			}
			else
			{
				return -1;
			}

			if(g_config.gameServerNo == entityPC.serverIdx)
			{
				
				
				
				
				return 0;
			}
			
			if(0 > entityPC.serverIdx  || entityPC.serverIdx >= MAX_MEMBER_SERVER)
			{
				char txt[256];
				wsprintf(txt,"Send_GMS :: 0 > entityPC.serverIdx  || entityPC.serverIdx >= MAX_MEMBER_SERVER mainType %d, subType %d, %s -> %s ",mainType,subType,FromPlayerName,TargetPlayerName);
				TRACE(txt);
				return 0;
			}				

			NET_SendUnreliableMessage(&g_memberServer[entityPC.serverIdx].sock, &netMessage);
		} 
		else
		{
			MSG_WriteShort(&netMessage, gameServerNo);
			NET_SendUnreliableMessage(&localSocket, &netMessage);
		}		
	}
	MSG_EndWriting(&netMessage);
	return 0;
}



int  CGs_To_Gms::Send_GMS(int SystemType,int mainType, int subType, int worldIdx, vec3_t position, char *FromPlayerName,char *TargetPlayerName,int gameServerNo,int CheckTarget)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GS_TO_GMS_PACKET);

		MSG_WriteShort(&netMessage, SystemType);
		MSG_WriteShort(&netMessage, mainType);
		MSG_WriteShort(&netMessage, subType);

		MSG_WriteByte(&netMessage,		worldIdx);
		MSG_WritePosition(&netMessage,  position);

		MSG_WriteString(&netMessage, FromPlayerName);
		MSG_WriteString(&netMessage, TargetPlayerName);
	
		if ( TRUE == g_config.isManager)
		{
			entityPCList_t entityPC;

			
			
			if(enumCheckType::CHECK_PLAYER1 == CheckTarget)
			{
				int FindIdx =  g_entityListMng.FindPCList( FromPlayerName, &entityPC );				
				if(-1 == FindIdx) return -1;
			}
			
			else if(enumCheckType::CHECK_PLAYER2 == CheckTarget)
			{
				int FindIdx =  g_entityListMng.FindPCList( TargetPlayerName, &entityPC );				
				if(-1 == FindIdx) return -1;
			}
			else
			{
				return -1;
			}

			if(g_config.gameServerNo == entityPC.serverIdx)
			{
				
				
				
				
				return 0;
			}
				
			if(0 > entityPC.serverIdx  || entityPC.serverIdx >= MAX_MEMBER_SERVER)
			{
				char txt[256];
				wsprintf(txt,"Send_GMS :: 0 > entityPC.serverIdx  || entityPC.serverIdx >= MAX_MEMBER_SERVER mainType %d, subType %d, %s -> %s ",mainType,subType,FromPlayerName,TargetPlayerName);
				TRACE(txt);
				return 0;
			}

			NET_SendUnreliableMessage(&g_memberServer[entityPC.serverIdx].sock, &netMessage);
		} 
		else
		{
			MSG_WriteShort(&netMessage, gameServerNo);
			NET_SendUnreliableMessage(&localSocket, &netMessage);
		}		
	}
	MSG_EndWriting(&netMessage);
	return 0;
}



int  CGs_To_Gms::Send_GMS(int SystemType,int mainType, int subType, int val, int worldIdx, vec3_t position, char *FromPlayerName,char *TargetPlayerName,int gameServerNo,int CheckTarget)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GS_TO_GMS_PACKET);

		MSG_WriteShort(&netMessage, SystemType);
		MSG_WriteShort(&netMessage, mainType);
		MSG_WriteShort(&netMessage, subType);

		MSG_WriteLong(&netMessage, val);

		MSG_WriteByte(&netMessage,		worldIdx);
		MSG_WritePosition(&netMessage,  position);

		MSG_WriteString(&netMessage, FromPlayerName);
		MSG_WriteString(&netMessage, TargetPlayerName);
	
		if ( TRUE == g_config.isManager)
		{
			entityPCList_t entityPC;

			
			
			if(enumCheckType::CHECK_PLAYER1 == CheckTarget)
			{
				int FindIdx =  g_entityListMng.FindPCList( FromPlayerName, &entityPC );				
				if(-1 == FindIdx) return -1;
			}
			
			else if(enumCheckType::CHECK_PLAYER2 == CheckTarget)
			{
				int FindIdx =  g_entityListMng.FindPCList( TargetPlayerName, &entityPC );				
				if(-1 == FindIdx) return -1;
			}
			else
			{
				return -1;
			}

			if(g_config.gameServerNo == entityPC.serverIdx)
			{
				
				
				
				
				return 0;
			}
				
			if(0 > entityPC.serverIdx  || entityPC.serverIdx >= MAX_MEMBER_SERVER)
			{
				char txt[256];
				wsprintf(txt,"Send_GMS :: 0 > entityPC.serverIdx  || entityPC.serverIdx >= MAX_MEMBER_SERVER mainType %d, subType %d, %s -> %s ",mainType,subType,FromPlayerName,TargetPlayerName);
				TRACE(txt);
				return 0;
			}

			NET_SendUnreliableMessage(&g_memberServer[entityPC.serverIdx].sock, &netMessage);
		} 
		else
		{
			MSG_WriteShort(&netMessage, gameServerNo);
			NET_SendUnreliableMessage(&localSocket, &netMessage);
		}		
	}
	MSG_EndWriting(&netMessage);
	return 0;
}

