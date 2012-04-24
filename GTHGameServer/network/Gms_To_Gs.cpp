



#include "../Global.h"
#include "Gms_To_Gs.h"


CGms_To_Gs g_GmsToGs;
extern CItem_Scroll_Proc		g_Item_Scroll_Proc;





CGms_To_Gs::CGms_To_Gs()
{

}

CGms_To_Gs::~CGms_To_Gs()
{

}

void CGms_To_Gs::Decoder(playerCharacter_t* pPlayer)
{
	short PacketID =MSG_ReadShort();

	switch(PacketID){
	case ITEM_SUMMON_SYSTEM:
		g_Item_Scroll_Proc.GSDecoder();
		break;
	}
}



int  CGms_To_Gs::Send_GS(int SystemType,int mainType, int subType,char *FromPlayerName,char *TargetPlayerName,int gameServerNo)
{	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GMS_TO_GS_PACKET);

	
		MSG_WriteShort(&netMessage, SystemType);
		MSG_WriteShort(&netMessage, mainType);
		MSG_WriteShort(&netMessage, subType);

		MSG_WriteString(&netMessage, FromPlayerName);
		MSG_WriteString(&netMessage, TargetPlayerName);

		MSG_WriteShort(&netMessage, gameServerNo);

		if(g_config.gameServerNo == gameServerNo)
		{
			
			
			return 0;
		}

		if(0 > gameServerNo || gameServerNo >= MAX_MEMBER_SERVER)
		{
			char txt[256];
			wsprintf(txt,"Send_GS :: 0 > gameServerNo || gameServerNo >= MAX_MEMBER_SERVER mainType %d, subType %d, %s -> %s ",mainType,subType,FromPlayerName,TargetPlayerName);
			TRACE(txt);
			return 0;
		}

		NET_SendUnreliableMessage(&g_memberServer[gameServerNo].sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
	return 0;
}


