


#include "../Global.h"
#include "Gs_To_Cc.h"

CGs_To_Cc g_GsToCc;





CGs_To_Cc::CGs_To_Cc()
{

}

CGs_To_Cc::~CGs_To_Cc()
{

}


void CGs_To_Cc::Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, SystemType);
		MSG_WriteShort(&netMessage, mainType);		
		MSG_WriteShort(&netMessage, subType);		
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CGs_To_Cc::Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType,char *TargetPlayerName)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, SystemType);
		MSG_WriteShort(&netMessage, mainType);		
		MSG_WriteShort(&netMessage, subType);	
		MSG_WriteString(&netMessage, TargetPlayerName);	

		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CGs_To_Cc::Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType,char *requestPlayerName,char *TargetPlayerName)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		MSG_WriteShort(&netMessage, SystemType);
		MSG_WriteShort(&netMessage, mainType);		
		MSG_WriteShort(&netMessage, subType);	
		
		MSG_WriteString(&netMessage, requestPlayerName);	
		MSG_WriteString(&netMessage, TargetPlayerName);	

		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}



void CGs_To_Cc::Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType,int val, char *TargetPlayerName)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		
		MSG_WriteShort(&netMessage, SystemType);
		MSG_WriteShort(&netMessage, mainType);		
		MSG_WriteShort(&netMessage, subType);	

		MSG_WriteLong(&netMessage, val);	

		MSG_WriteString(&netMessage, TargetPlayerName);	

		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CGs_To_Cc::Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType,int val, char *requestPlayerName,char *TargetPlayerName)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		
		MSG_WriteShort(&netMessage, SystemType);
		MSG_WriteShort(&netMessage, mainType);		
		MSG_WriteShort(&netMessage, subType);	

		MSG_WriteLong(&netMessage, val);	

		MSG_WriteString(&netMessage, requestPlayerName);
		MSG_WriteString(&netMessage, TargetPlayerName);	

		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CGs_To_Cc::Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType,int worldIdx,vec3_t position,char *requestPlayerName,char *TargetPlayerName)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		
		MSG_WriteShort(&netMessage, SystemType);
		MSG_WriteShort(&netMessage, mainType);		
		MSG_WriteShort(&netMessage, subType);	
		
		MSG_WriteByte(&netMessage,		worldIdx);
		MSG_WritePosition(&netMessage,  position);

		MSG_WriteString(&netMessage, requestPlayerName);	
		MSG_WriteString(&netMessage, TargetPlayerName);	

		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void CGs_To_Cc::Send(playerCharacter_t* pPlayer,int SystemType,int mainType, int subType,int val, int worldIdx,vec3_t position,char *requestPlayerName,char *TargetPlayerName)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, EXTEND_SECOND);
		
		MSG_WriteShort(&netMessage, SystemType);
		MSG_WriteShort(&netMessage, mainType);		
		MSG_WriteShort(&netMessage, subType);	
		
		MSG_WriteLong(&netMessage, val);

		MSG_WriteByte(&netMessage,		worldIdx);
		MSG_WritePosition(&netMessage,  position);

		MSG_WriteString(&netMessage, requestPlayerName);	
		MSG_WriteString(&netMessage, TargetPlayerName);	

		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}