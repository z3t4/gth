




#include "../global.h"
CItem_Scroll_Proc		g_Item_Scroll_Proc;

#include "../Tools/CTools.h"
extern CTools* gcpTools;

#include "..\GolryunBattle\CGolryunBattle.h"
extern CGolryunBattle* gcpGolryunBattle;

#include "..\GonryunPracticeBattle\CGonryunPracticeBattle.h"
#include "..\GonryunPracticeBattle\CGonryunPractice_ProcessCtrl.h"
#include "..\GonryunPracticeBattle\CGonryunPractice_WaiterCtrl.h"
extern CGonryunPracticeBattle	g_GonryunPracticeBattle;											

#include "../network/Gs_To_Gms.h"
extern CGs_To_Gms g_GsToGms;

#include "../network/Gms_To_Gs.h"
extern CGms_To_Gs g_GmsToGs;

extern CItemTableData gItemTableData;

#include "../network/Gs_To_Cc.h"
extern CGs_To_Cc g_GsToCc;


#include "../LogEvent.h"
extern CLogEvent gLogEvent;






CItem_Scroll_Proc::CItem_Scroll_Proc()
{

}

CItem_Scroll_Proc::~CItem_Scroll_Proc()
{

}

void CItem_Scroll_Proc::Decoder(playerCharacter_t* pPlayer)
{
	short PacketID =MSG_ReadShort();
	switch(PacketID){
		case enumPacketID::CC_REQUEST_PC_SUMMON:
			ClientRequest_PC_Summon(pPlayer);
			break;

		case enumPacketID::CC_REPLY_PC_SUMMON:
			Client_Reply_PC_Summon(pPlayer);
			break;

		
		
		

		case enumPacketID::CC_STATE_MES:
			Client_State_Mes(pPlayer);
			break;

		case enumPacketID::CC_ERROR_MES:
			Client_Error_Mes(pPlayer);
			break;


		
		
		

		case enumPacketID::CC_REQUEST_PC_MOVE_SUMMON:
			ClientRequest_PC_Move_Summon(pPlayer);
			break;

		case enumPacketID::CC_REPLY_PC_MOVE_SUMMON:
			Client_Reply_PC_Move_Summon(pPlayer);
			break;

		case enumPacketID::CC_REDAY_PC_MOVE_SUMMON:
			Client_Ready_PC_Move_Summon(pPlayer);
			break;

	}
}

int  CItem_Scroll_Proc::GSDecoder()
{
	int mainType			= MSG_ReadShort();

	switch(mainType)
	{
	case CC_REQUEST_PC_SUMMON:
		{
			OtherServer_CleintRequest_PC_Summon();
		}
		break;

	case CC_REPLY_PC_SUMMON:
		{
			Otherserver_from_Reply_Pc_Summon();
		}
		break;
		
	
	
	
	
	case GSC_ERROR_MES:
		GsErrorMes();
		break;


	case GSC_STATE_MES:
		GsStateMes();
		break;

	
	
	

	case CC_REQUEST_PC_MOVE_SUMMON:
		{
			OtherServer_CleintRequest_PC_Move_Summon();
		}
		break;

	case CC_REPLY_PC_MOVE_SUMMON:
		{
			Otherserver_from_Reply_Pc_Move_Summon();
		}
		break;

	}

	return 1;
}






bool CItem_Scroll_Proc::CheckInvalidArea(playerCharacter_t* pPlayer)
{
	
	if(false == pPlayer->alive)
	{
		return false;
	}

	
	if(false == pPlayer->ready)
	{
		return false;
	}

	if(pPlayer->m_WorldChangeSystem.m_SystemMode != WorldChangeSystem::enumWorldChangeMode::WORLD_CHANGESYSTEM_NONE)
	{
		return false;
	}

	
	if(tagGolryunBattle::Golryun_Battle_Map_Index == pPlayer->worldIdx)
	{
		return false;
	}	

	
	CGonryunPractice_ProcessCtrl* pProcess = g_GonryunPracticeBattle.GetProcessCtrl();
	if(playerCharacter_t::tagGonryunBattlePractice::MEMBERSHIP_NONE != pPlayer->GonryunBattlePractice.MemberShip)
	{
		return false;
	}

	
	if( DAN_BATTLEMAP_NO == pPlayer->worldIdx )
	{
		return false;
	}

	return true;	
}








int CItem_Scroll_Proc::ClientRequest_PC_Summon(playerCharacter_t* pPlayer)
{
	int InventoryItemPos	= MSG_ReadShort();

	char targetPlayerName[NAMESTRING+1];
	sstrncpy( targetPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	targetPlayerName[NAMESTRING]=NULL;

	
	item_t* pItem = gcpTools->GetMyInventoryItem(pPlayer ,InventoryItemPos);
	if(NULL == pItem)
	{
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,enumErrorCode::ERROR_ITEM_NOT_HAVE,pPlayer->name,targetPlayerName);
		return 0;
	}

	
	itemTable_t* pItemTable = gcpTools->GetItemTablePointer(pItem->itemTableIdx);
	if(NULL == pItemTable)
	{
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,enumErrorCode::ERROR_INVALID_ITEM,pPlayer->name,targetPlayerName);
		return 0;
	}

	int UseItemType = gItemTableData.GetUseItemType(pItemTable);
	if(CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_PC_SUMMON != UseItemType) 
	{
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,enumErrorCode::ERROR_NOT_SUMMON_ITEM,pPlayer->name,targetPlayerName);
		return 0;
	}

	
	if(false == CheckInvalidArea(pPlayer))
	{
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,enumErrorCode::ERROR_REQUEST_PLAYER_NOT_USE_AREA,pPlayer->name,targetPlayerName);
		return 0;
	}

	
	
	playerCharacter_t*  pTargetPlayer = gcpTools->GetPlayerRecordPointer(targetPlayerName);
	
	if(NULL != pTargetPlayer)
	{
		
		if(false == CheckInvalidArea(pTargetPlayer))
		{
			g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,enumErrorCode::ERROR_TARGET_PLAYER_NOT_USE_AREA,pPlayer->name,targetPlayerName);
			return 0;
		}

		
		
		g_GsToCc.Send(pTargetPlayer,ITEM_SUMMON_SYSTEM,GSC_REQUEST_PC_SUMMON, 0,InventoryItemPos,pPlayer->worldIdx,pPlayer->position,pPlayer->name,targetPlayerName);

		return 0;
	}

	
	int Result = g_GsToGms.Send_GMS(
		ITEM_SUMMON_SYSTEM,
		CC_REQUEST_PC_SUMMON,
		0,
		InventoryItemPos,

		pPlayer->worldIdx,
		pPlayer->position,

		pPlayer->name,
		targetPlayerName,
		g_config.gameServerNo,
		CGs_To_Gms::enumCheckType::CHECK_PLAYER2);

	
	if ( -1 == Result )
	{
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,enumErrorCode::ERROR_NULL_TARGET_PLAYER,pPlayer->name,targetPlayerName);
		return 0;
	}

	
	return 0;
}



int  CItem_Scroll_Proc::GsStateMes()
{
	int SubType			= MSG_ReadShort();
	
	switch(SubType) {
		
	case STATE_MSGBOX_POPUP_SUCC:
	case STATE_MSGBOX_POPUP_FAIL:
		{
			int InventoryItemPos		= MSG_ReadLong();

			
			char requestPlayerName[NAMESTRING+1];
			sstrncpy( requestPlayerName, MSG_ReadString(), NAMESTRING - 1 );
			requestPlayerName[NAMESTRING]=NULL;

			char targetPlayerName[NAMESTRING+1];
			sstrncpy( targetPlayerName, MSG_ReadString(), NAMESTRING - 1 );
			targetPlayerName[NAMESTRING]=NULL;

			playerCharacter_t*  pRequestPlayer = gcpTools->GetPlayerRecordPointer(requestPlayerName);
			if(NULL == pRequestPlayer) return 0;

			
			if(STATE_MSGBOX_POPUP_SUCC == SubType)
			{
				
				gcpTools->DeleteItemFromMyInventory(pRequestPlayer, InventoryItemPos);
			}			
			
			g_GsToCc.Send(pRequestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,SubType,InventoryItemPos,requestPlayerName,targetPlayerName);
		}
		break;

	case STATE_TARGET_PLAYER_REFUSE:
	case STATE_TARGET_PLAYER_OK:
		{
			
			char requestPlayerName[NAMESTRING+1];
			sstrncpy( requestPlayerName, MSG_ReadString(), NAMESTRING - 1 );
			requestPlayerName[NAMESTRING]=NULL;

			char targetPlayerName[NAMESTRING+1];
			sstrncpy( targetPlayerName, MSG_ReadString(), NAMESTRING - 1 );
			targetPlayerName[NAMESTRING]=NULL;

			playerCharacter_t*  pRequestPlayer = gcpTools->GetPlayerRecordPointer(requestPlayerName);
			if(NULL == pRequestPlayer) return 0;

			g_GsToCc.Send(pRequestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,SubType,requestPlayerName,targetPlayerName);
		}
		break;

	case STATE_TARGET_PLAYER_MOVE_REFUSE:
	case STATE_TARGET_PLAYER_MOVE_OK:
		{
			int worldIdx    = MSG_ReadByte();
			vec3_t position;
			MSG_ReadPosition(position);

			
			char requestPlayerName[NAMESTRING+1];
			sstrncpy( requestPlayerName, MSG_ReadString(), NAMESTRING - 1 );
			requestPlayerName[NAMESTRING]=NULL;

			char targetPlayerName[NAMESTRING+1];
			sstrncpy( targetPlayerName, MSG_ReadString(), NAMESTRING - 1 );
			targetPlayerName[NAMESTRING]=NULL;

			playerCharacter_t*  pRequestPlayer = gcpTools->GetPlayerRecordPointer(requestPlayerName);
			if(NULL == pRequestPlayer) return 0;

			g_GsToCc.Send(pRequestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,SubType,worldIdx,position,requestPlayerName,targetPlayerName);
		}
		break;

	}

	return 1;
}


int  CItem_Scroll_Proc::GsErrorMes()
{
	int SubType			= MSG_ReadShort();

	
	char requestPlayerName[NAMESTRING+1];
	sstrncpy( requestPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	requestPlayerName[NAMESTRING]=NULL;

	char targetPlayerName[NAMESTRING+1];
	sstrncpy( targetPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	targetPlayerName[NAMESTRING]=NULL;

	switch(SubType) {
		
	case ERROR_NULL_TARGET_PLAYER:
	case ERROR_TARGET_PLAYER_NOT_USE_AREA:
		{
			playerCharacter_t*  pRequestPlayer = gcpTools->GetPlayerRecordPointer(requestPlayerName);
			if(NULL == pRequestPlayer) return 0;

			g_GsToCc.Send(pRequestPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,SubType,requestPlayerName,targetPlayerName);
		}
		break;

		
	case ERROR_NULL_REQUEST_PLAYER:
	case ERROR_REQUEST_PLAYER_NOT_USE_AREA:
		{
			playerCharacter_t*  pTargetPlayer = gcpTools->GetPlayerRecordPointer(targetPlayerName);
			if(NULL == pTargetPlayer) return 0;

			g_GsToCc.Send(pTargetPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,SubType,requestPlayerName,targetPlayerName);
		}
		break;

	}

	return 1;
}

int  CItem_Scroll_Proc::OtherServer_CleintRequest_PC_Summon()
{
	int SubType			= MSG_ReadShort();
	int InventoryItemPos = MSG_ReadLong();

	int worldIdx    = MSG_ReadByte();
	vec3_t position;
	MSG_ReadPosition(position);

	
	char requestPlayerName[NAMESTRING+1];
	sstrncpy( requestPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	requestPlayerName[NAMESTRING]=NULL;

	char targetPlayerName[NAMESTRING+1];
	sstrncpy( targetPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	targetPlayerName[NAMESTRING]=NULL;

	
	playerCharacter_t*  pTargetPlayer = gcpTools->GetPlayerRecordPointer(targetPlayerName);
	if(NULL == pTargetPlayer)
	{
		
		g_GsToGms.Send_GMS(
			ITEM_SUMMON_SYSTEM,
			GSC_ERROR_MES,
			enumErrorCode::ERROR_NULL_TARGET_PLAYER,
			requestPlayerName,
			targetPlayerName,
			g_config.gameServerNo,
			CGs_To_Gms::enumCheckType::CHECK_PLAYER1);

		return 0;

		
		return 0;
	}

	
	
	if(false == CheckInvalidArea(pTargetPlayer))
	{
		g_GsToGms.Send_GMS(
			ITEM_SUMMON_SYSTEM,
			GSC_ERROR_MES,
			enumErrorCode::ERROR_TARGET_PLAYER_NOT_USE_AREA,
			requestPlayerName,
			targetPlayerName,
			g_config.gameServerNo,	
			CGs_To_Gms::enumCheckType::CHECK_PLAYER1);		
		return 0;
	}

	
	
	g_GsToCc.Send(pTargetPlayer,ITEM_SUMMON_SYSTEM,GSC_REQUEST_PC_SUMMON, 0,InventoryItemPos,worldIdx,position,requestPlayerName,targetPlayerName);

	return 0;
}

int CItem_Scroll_Proc::Client_Error_Mes(playerCharacter_t*  pPlayer)
{
	int ReplyID				= MSG_ReadShort();

	
	char requestPlayerName[NAMESTRING+1];
	sstrncpy( requestPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	requestPlayerName[NAMESTRING]=NULL;

	char targetPlayerName[NAMESTRING+1];
	sstrncpy( targetPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	targetPlayerName[NAMESTRING]=NULL;


	switch(ReplyID) {
	case ERROR_NULL_TARGET_PLAYER:
	case ERROR_TARGET_PLAYER_NOT_USE_AREA:
		{
			
			playerCharacter_t* requestPlayer = gcpTools->GetPlayerRecordPointer(requestPlayerName);
			if(NULL == requestPlayer) 
			{
				g_GsToGms.Send_GMS(
					ITEM_SUMMON_SYSTEM,
					CC_ERROR_MES,
					ReplyID,
					requestPlayerName,
					targetPlayerName,
					g_config.gameServerNo,
					CGs_To_Gms::enumCheckType::CHECK_PLAYER1);

				return 0;
			}
			else
			{
				g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES, ReplyID,requestPlayerName,targetPlayerName);
			}
		}
		break;

	case ERROR_NULL_REQUEST_PLAYER:
	case ERROR_REQUEST_PLAYER_NOT_USE_AREA:
		{
			
			playerCharacter_t* targetPlayer = gcpTools->GetPlayerRecordPointer(targetPlayerName);
			if(NULL == targetPlayer) 
			{
				g_GsToGms.Send_GMS(
					ITEM_SUMMON_SYSTEM,
					CC_ERROR_MES,
					ReplyID,
					requestPlayerName,
					targetPlayerName,
					g_config.gameServerNo,
					CGs_To_Gms::enumCheckType::CHECK_PLAYER2);

				return 0;
			}
			else
			{
				g_GsToCc.Send(targetPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES, ReplyID,requestPlayerName,targetPlayerName);
			}
		}
		break;
	}

	return 0;
}

int CItem_Scroll_Proc::Client_State_Mes(playerCharacter_t*  pPlayer)
{
	int ReplyID				= MSG_ReadShort();
	int InventoryItemPos	= MSG_ReadLong();

	
	char requestPlayerName[NAMESTRING+1];
	sstrncpy( requestPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	requestPlayerName[NAMESTRING]=NULL;

	switch(ReplyID) {
	case STATE_MSGBOX_POPUP_SUCC:
		{
			
			playerCharacter_t*  requestPlayer = gcpTools->GetPlayerRecordPointer(requestPlayerName);
			if(NULL == requestPlayer) 
			{
				
				g_GsToGms.Send_GMS(
					ITEM_SUMMON_SYSTEM,
					GSC_STATE_MES,
					enumStateCode::STATE_MSGBOX_POPUP_SUCC,
					InventoryItemPos,
					requestPlayerName,
					pPlayer->name,		
					g_config.gameServerNo,
					CGs_To_Gms::enumCheckType::CHECK_PLAYER1);	
				return 0;
			}
			
			
			
			gcpTools->DeleteItemFromMyInventory(requestPlayer, InventoryItemPos);
			g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,STATE_MSGBOX_POPUP_SUCC,InventoryItemPos,requestPlayerName,pPlayer->name);
		}
		break;
	case STATE_MSGBOX_POPUP_FAIL:
		{
			
			
			playerCharacter_t*  requestPlayer = gcpTools->GetPlayerRecordPointer(requestPlayerName);
			if(NULL == requestPlayer) 
			{
				g_GsToGms.Send_GMS(
					ITEM_SUMMON_SYSTEM,
					GSC_STATE_MES,
					enumStateCode::STATE_MSGBOX_POPUP_FAIL,
					InventoryItemPos,
					requestPlayerName,
					pPlayer->name,		
					g_config.gameServerNo,
					CGs_To_Gms::enumCheckType::CHECK_PLAYER1);	
				return 0;
			}

			g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,enumStateCode::STATE_MSGBOX_POPUP_FAIL,InventoryItemPos,requestPlayerName,pPlayer->name);
		}
		break;
	}
	return 0;
}


int CItem_Scroll_Proc::Client_Reply_PC_Summon(playerCharacter_t* pPlayer)
{
	int ReplyID			= MSG_ReadShort();

	int worldIdx    = MSG_ReadByte();
	vec3_t position;
	MSG_ReadPosition(position);

	
	char requestPlayerName[NAMESTRING+1];
	sstrncpy( requestPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	requestPlayerName[NAMESTRING]=NULL;


	
	if(false == CheckInvalidArea(pPlayer))
	{
		
		g_GsToGms.Send_GMS(
			ITEM_SUMMON_SYSTEM,
			GSC_ERROR_MES,
			enumErrorCode::ERROR_TARGET_PLAYER_NOT_USE_AREA,
			requestPlayerName,
			pPlayer->name,
			g_config.gameServerNo,
			CGs_To_Gms::enumCheckType::CHECK_PLAYER1);	

		
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES, enumErrorCode::ERROR_TARGET_PLAYER_NOT_USE_AREA,requestPlayerName,pPlayer->name);		
		return 0;
	}
	
	
	if(enumTargetPlayerReply::REPLY_OK == ReplyID)
	{		
		GTH_PC_SetSummonsInfo(pPlayer,requestPlayerName,worldIdx,position,tagPcSummonTypedef::enumPcSummonType::PC_SUMMON_MOVE_ITEM);
		ReplySummons(pPlayer);
		
	}

	
	playerCharacter_t*  requestPlayer = gcpTools->GetPlayerRecordPointer(requestPlayerName);
	if(NULL == requestPlayer) 
	{
		switch(ReplyID) {
		
		
		case REPLY_OK:
			{
				g_GsToGms.Send_GMS(
					ITEM_SUMMON_SYSTEM,
					CC_REPLY_PC_SUMMON,
					enumTargetPlayerReply::REPLY_OK,
					requestPlayerName,
					pPlayer->name,
					g_config.gameServerNo,
					CGs_To_Gms::enumCheckType::CHECK_PLAYER1);	
				return 0;
			}
			break;

		
		case REPLY_REFUSE:
				g_GsToGms.Send_GMS(
					ITEM_SUMMON_SYSTEM,
					CC_REPLY_PC_SUMMON,
					enumTargetPlayerReply::REPLY_REFUSE,
					requestPlayerName,
					pPlayer->name,
					g_config.gameServerNo,
					CGs_To_Gms::enumCheckType::CHECK_PLAYER1);

				return 0;
			break;
		}
		return 0;
	}

	
	if(enumTargetPlayerReply::REPLY_REFUSE == ReplyID)
	{
		
		g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,enumStateCode::STATE_TARGET_PLAYER_REFUSE,requestPlayer->name,pPlayer->name);
		return  0;
	}

	
	g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,enumStateCode::STATE_TARGET_PLAYER_OK,requestPlayer->name,pPlayer->name);

	return 0;
}

int  CItem_Scroll_Proc::Otherserver_from_Reply_Pc_Summon()
{
	int SubType			= MSG_ReadShort();

	char requestPlayerName[NAMESTRING+1];
	sstrncpy( requestPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	requestPlayerName[NAMESTRING]=NULL;

	char targetPlayerName[NAMESTRING+1];
	sstrncpy( targetPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	targetPlayerName[NAMESTRING]=NULL;

	
	playerCharacter_t*  requestPlayer = gcpTools->GetPlayerRecordPointer(requestPlayerName);
	if(NULL == requestPlayer) 
	{
		
		return 0;
	}
	

	if(enumTargetPlayerReply::REPLY_REFUSE == SubType)
	{
		
		g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,enumStateCode::STATE_TARGET_PLAYER_REFUSE,requestPlayer->name,targetPlayerName);
		return  0;
	}

	
	g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,enumStateCode::STATE_TARGET_PLAYER_OK,requestPlayer->name,targetPlayerName);

	return 1;
}










int CItem_Scroll_Proc::ClientRequest_PC_Move_Summon(playerCharacter_t* pPlayer)
{
	int InventoryItemPos	= MSG_ReadShort();

	char targetPlayerName[NAMESTRING+1];
	sstrncpy( targetPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	targetPlayerName[NAMESTRING]=NULL;

	
	item_t* pItem = gcpTools->GetMyInventoryItem(pPlayer ,InventoryItemPos);
	if(NULL == pItem)
	{
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,enumErrorCode::ERROR_ITEM_NOT_HAVE,pPlayer->name,targetPlayerName);
		return 0;
	}

	
	itemTable_t* pItemTable = gcpTools->GetItemTablePointer(pItem->itemTableIdx);
	if(NULL == pItemTable)
	{
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,enumErrorCode::ERROR_INVALID_ITEM,pPlayer->name,targetPlayerName);
		return 0;
	}

	int UseItemType = gItemTableData.GetUseItemType(pItemTable);
	if(CItemTableData::ItemTable_FluxDataType::USE_ITEM_FOR_PC_MOVE_SUMMON != UseItemType) 
	{
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,enumErrorCode::ERROR_NOT_MOVE_SUMMON_ITEM,pPlayer->name,targetPlayerName);
		return 0;
	}

	
	if(false == CheckInvalidArea(pPlayer))
	{
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,enumErrorCode::ERROR_REQUEST_PLAYER_NOT_USE_AREA,pPlayer->name,targetPlayerName);
		return 0;
	}


	
	
	playerCharacter_t*  pTargetPlayer = gcpTools->GetPlayerRecordPointer(targetPlayerName);
	
	if(NULL != pTargetPlayer)
	{
		
		if(false == CheckInvalidArea(pTargetPlayer))
		{
			g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,enumErrorCode::ERROR_TARGET_PLAYER_NOT_USE_AREA,pPlayer->name,targetPlayerName);
			return 0;
		}

		
		
		g_GsToCc.Send(pTargetPlayer,ITEM_SUMMON_SYSTEM,GSC_REQUEST_PC_MOVE_SUMMON, 0,InventoryItemPos,pPlayer->name,targetPlayerName);

		return 0;
	}

	
	int Result = g_GsToGms.Send_GMS(
		ITEM_SUMMON_SYSTEM,
		CC_REQUEST_PC_MOVE_SUMMON,
		0,
		InventoryItemPos,
		pPlayer->name,
		targetPlayerName,
		g_config.gameServerNo,
		CGs_To_Gms::enumCheckType::CHECK_PLAYER2);

	
	if ( -1 == Result )
	{
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES,enumErrorCode::ERROR_NULL_TARGET_PLAYER,pPlayer->name,targetPlayerName);
		return 0;
	}

	
	return 0;
}


int  CItem_Scroll_Proc::OtherServer_CleintRequest_PC_Move_Summon()
{
	int SubType			= MSG_ReadShort();
	int InventoryItemPos = MSG_ReadLong();

	
	char requestPlayerName[NAMESTRING+1];
	sstrncpy( requestPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	requestPlayerName[NAMESTRING]=NULL;

	char targetPlayerName[NAMESTRING+1];
	sstrncpy( targetPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	targetPlayerName[NAMESTRING]=NULL;

	
	playerCharacter_t*  pTargetPlayer = gcpTools->GetPlayerRecordPointer(targetPlayerName);
	if(NULL == pTargetPlayer)
	{
		
		g_GsToGms.Send_GMS(
			ITEM_SUMMON_SYSTEM,
			GSC_ERROR_MES,
			enumErrorCode::ERROR_NULL_TARGET_PLAYER,
			requestPlayerName,
			targetPlayerName,
			g_config.gameServerNo,
			CGs_To_Gms::enumCheckType::CHECK_PLAYER1);

		return 0;

		
		return 0;
	}

	
	
	if(false == CheckInvalidArea(pTargetPlayer))
	{
		g_GsToGms.Send_GMS(
			ITEM_SUMMON_SYSTEM,
			GSC_ERROR_MES,
			enumErrorCode::ERROR_TARGET_PLAYER_NOT_USE_AREA,
			requestPlayerName,
			targetPlayerName,
			g_config.gameServerNo,	
			CGs_To_Gms::enumCheckType::CHECK_PLAYER1);		
		return 0;
	}

	
	
	g_GsToCc.Send(pTargetPlayer,ITEM_SUMMON_SYSTEM,GSC_REQUEST_PC_MOVE_SUMMON, 0,InventoryItemPos,requestPlayerName,targetPlayerName);

	return 0;
}


int  CItem_Scroll_Proc::Client_Ready_PC_Move_Summon(playerCharacter_t* pPlayer)
{
	int SubType		= MSG_ReadShort();

	int worldIdx    = MSG_ReadByte();
	vec3_t position;
	MSG_ReadPosition(position);

	char targetPlayerName[NAMESTRING+1];
	
	sstrncpy( targetPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	targetPlayerName[NAMESTRING]=NULL;

	GTH_PC_SetSummonsInfo(pPlayer,targetPlayerName,worldIdx,position,tagPcSummonTypedef::enumPcSummonType::PC_SUMMON_MOVE_ITEM);
	
	ReplySummons(pPlayer);
	return 0;
}


int CItem_Scroll_Proc::Client_Reply_PC_Move_Summon(playerCharacter_t* pPlayer)
{
	int ReplyID			= MSG_ReadShort();


	
	char requestPlayerName[NAMESTRING+1];
	sstrncpy( requestPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	requestPlayerName[NAMESTRING]=NULL;

	
	if(false == CheckInvalidArea(pPlayer))
	{
		
		g_GsToGms.Send_GMS(
			ITEM_SUMMON_SYSTEM,
			GSC_ERROR_MES,
			enumErrorCode::ERROR_TARGET_PLAYER_NOT_USE_AREA,
			requestPlayerName,
			pPlayer->name,
			g_config.gameServerNo,
			CGs_To_Gms::enumCheckType::CHECK_PLAYER1);	

		
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES, enumErrorCode::ERROR_TARGET_PLAYER_NOT_USE_AREA,requestPlayerName,pPlayer->name);		
		return 0;
	}

	
	playerCharacter_t*  requestPlayer = gcpTools->GetPlayerRecordPointer(requestPlayerName);
	if(NULL == requestPlayer) 
	{
		
		g_GsToGms.Send_GMS(
			ITEM_SUMMON_SYSTEM,
			CC_REPLY_PC_MOVE_SUMMON,
			ReplyID,

			pPlayer->worldIdx,
			pPlayer->position,

			requestPlayerName,
			pPlayer->name,
			g_config.gameServerNo,
			CGs_To_Gms::enumCheckType::CHECK_PLAYER1);	
		return 0;
	}

	
	if(enumTargetPlayerReply::REPLY_REFUSE == ReplyID)
	{
		
		g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,enumStateCode::STATE_TARGET_PLAYER_MOVE_REFUSE,
			pPlayer->worldIdx,pPlayer->position,requestPlayer->name,pPlayer->name);
		return  0;
	}

	
	if(false == CheckInvalidArea(requestPlayer))
	{
		
		g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES, enumErrorCode::ERROR_REQUEST_PLAYER_NOT_USE_AREA,
			requestPlayerName,pPlayer->name);
	

		
		g_GsToCc.Send(pPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES, enumErrorCode::ERROR_REQUEST_PLAYER_NOT_USE_AREA,
			requestPlayerName,pPlayer->name);
		return 0;
	}
	
	
	g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,enumStateCode::STATE_TARGET_PLAYER_MOVE_OK,
		pPlayer->worldIdx,pPlayer->position,requestPlayer->name,pPlayer->name);
	return 0;
}


int  CItem_Scroll_Proc::Otherserver_from_Reply_Pc_Move_Summon()
{
	int SubType			= MSG_ReadShort();

	int worldIdx    = MSG_ReadByte();
	vec3_t position;
	MSG_ReadPosition(position);

	char requestPlayerName[NAMESTRING+1];
	char targetPlayerName[NAMESTRING+1];

	sstrncpy( requestPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	requestPlayerName[NAMESTRING]=NULL;
	
	sstrncpy( targetPlayerName, MSG_ReadString(), NAMESTRING - 1 );
	targetPlayerName[NAMESTRING]=NULL;


	
	playerCharacter_t*  requestPlayer = gcpTools->GetPlayerRecordPointer(requestPlayerName);
	if(NULL == requestPlayer) 
	{
		
		
		g_GsToGms.Send_GMS(
			ITEM_SUMMON_SYSTEM,
			GSC_ERROR_MES,
			ERROR_NULL_REQUEST_PLAYER,
			requestPlayerName,
			targetPlayerName,
			g_config.gameServerNo,
			CGs_To_Gms::enumCheckType::CHECK_PLAYER2);

		return 0;

		
		return 0;
	}	

	if(enumTargetPlayerReply::REPLY_REFUSE == SubType)
	{
		
		g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,enumStateCode::STATE_TARGET_PLAYER_MOVE_REFUSE,worldIdx,position,requestPlayer->name,targetPlayerName);
		return  0;
	}

	
	if(false == CheckInvalidArea(requestPlayer))
	{
		
		g_GsToGms.Send_GMS(
			ITEM_SUMMON_SYSTEM,
			GSC_ERROR_MES,
			enumErrorCode::ERROR_REQUEST_PLAYER_NOT_USE_AREA,
			requestPlayerName,
			targetPlayerName,
			g_config.gameServerNo,
			CGs_To_Gms::enumCheckType::CHECK_PLAYER2);	

		
		g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_ERROR_MES, enumErrorCode::ERROR_REQUEST_PLAYER_NOT_USE_AREA,requestPlayerName,targetPlayerName);
		return 0;
	}

	
	g_GsToCc.Send(requestPlayer,ITEM_SUMMON_SYSTEM,GSC_STATE_MES,enumStateCode::STATE_TARGET_PLAYER_MOVE_OK,worldIdx,position,requestPlayer->name,targetPlayerName);
	return 1;
}



int  CItem_Scroll_Proc::ReplySummons(playerCharacter_t *pc)
{
	vec3_t position;
	vec3_t savePosition;
	int saveWorldIdx;

	int worldIdx = pc->summonsInfo.worldIdx;
	vec_copy( pc->summonsInfo.position, position );

	if( pc->isTransform ) GTH_DeleteTransform( pc );

	if ( GTH_IsLocalServiceWorld(worldIdx) )
	{
		if( worldIdx == pc->worldIdx ) 
		{
			vec_copy( position, pc->position );

			
			pc->reliableWorldIdx = pc->worldIdx;
			vec_copy( pc->position, pc->reliablePosition );

			int zoneIdx = GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
			pc->zoneIdx = zoneIdx;

			GTH_SendPCEventMessage_Respawn( pc );
			GTH_SendMessage_SyncItemObject( pc );
		}
		else 
		{
			
			GTH_Zone_DeleteEntityList(pc->worldIdx, pc->zoneIdx, pc->entityType, pc->idx);

			pc->worldIdx = worldIdx;
			vec_copy( position, pc->position );

			
			pc->reliableWorldIdx = pc->worldIdx;
			vec_copy( pc->position, pc->reliablePosition );

			pc->zoneIdx = GTH_Zone_GetZoneIdx( pc->position );
			
			GTH_Zone_AddEntityList(pc->worldIdx, pc->zoneIdx, pc->entityType, pc->idx);

			GTH_SendPCEventMessage_Respawn( pc );
			pc->ready = false;

			
			g_logSystem->Write("User [%s] Move To Summonning Pos. Pos(%.2f, %.2f, %.2f), World : %d", pc->userID, pc->position[0], pc->position[1], pc->position[2], pc->worldIdx);
		}
	}
	else 
	{
		int memberIdx = GTH_FindServerWithinGroup( worldIdx );
		if (memberIdx < 0) 
		{
			g_logSystem->Write("User [%s] Summon Error(worldIdx:%d)", pc->name, pc->worldIdx);
			return 0;
		}
		else
		{
			
			saveWorldIdx = pc->worldIdx;
			vec_copy(pc->position, savePosition);

			
			pc->worldIdx = worldIdx;
			vec_copy(position, pc->position);

			pc->flagChangeServer = 1; 
			pc->changeServerWorldIdx = worldIdx;
			DB_SaveGameData( pc, SAVEGAMEDATA_QUIT);

			
			pc->worldIdx = saveWorldIdx;
			vec_copy(savePosition, pc->position);
		}
	}

	
	GTH_SendMessage_ReplySummons(1, pc->summonsInfo.summoner, pc->name);

	
	GTH_PC_InitSummonsInfo( pc );
	return 1;
}
