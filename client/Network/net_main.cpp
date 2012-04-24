#include "../global_def.h"
#include "../game/GolryunBattle/GolryunBattle.h"
#include "../game/GolryunBattle/GolryunBattle_Decoder.h"


#include "..\GM(Game Master)\CGMCtrl.h"
extern CGMCtrl* gcpGMCtrl;







i3socket_t msSocket;	
i3socket_t gsSocket;	


extern unsigned int g_LastnetTime;
extern unsigned int g_netTime;
extern i3sizebuf_t netMessage; 
extern i3packet_t packetBuffer;








 
#ifdef	_DEBUG
	int	CHECK_PROBLEN_FLAG		=	false;
#else
	int	CHECK_PROBLEN_FLAG		=	true;
#endif
	

#ifdef _TERRA
#include "..\Terra ICT\CTerraCtrl.h"
extern CTerraCtrl gcTerraCtrl;
#endif






int GTH_Network_StartUp()
{
	if ( !NET_OpenNetwork() )
		return 0;

	return 1;
}

void GTH_Network_ShutDown()
{
	NET_CloseSocket(&msSocket);
	NET_CloseSocket(&gsSocket);
	NET_CloseNetwork();
}

int GTH_Network_OpenGameServerSocket()
{
	if ( !NET_OpenSocketForClient(&gsSocket, g_cgv.gameServerIP, g_cgv.gameServerPort) ) 	
	{
		char cErrorMsg[256];
		
		
		wsprintf(cErrorMsg, g_LPACK.GetMassage(0,38), WSAGetLastError()); 
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,39), cErrorMsg, 1, IF_MESSAGE_EXIT_CLIENT );

		return 0;
	}

	return 1;
}

void GTH_Network_InitNetTime()
{
	NET_GetNetTime();

	
	gsSocket.lastReceiveTime = g_netTime;
	g_LastnetTime = g_netTime;

} 



	


void GTH_Network_CheckProblemSocket(i3socket_t *sock)
{
	

	
	if (sock->resendSequence >= MAX_RESENDING_TIMES)
	{
		if( CHECK_PROBLEN_FLAG )
		{
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,40),  1, IF_MESSAGE_EXIT_CLIENT );
			g_cgv.onGameServer = false;
			return;
		}
	}

	if (g_cgv.onGameServer && (sock->lastReceiveTime - g_LastnetTime) >= 6000)
	{
		GTH_SendMessage_ReliablePing( sock );
		g_LastnetTime = sock->lastReceiveTime;



		g_SkillFile.DebugOutString("PINGTIME 最后时间 %d ,误差 %d\n",g_LastnetTime,sock->lastReceiveTime - g_LastnetTime);
		
		return;
	}



	

	
	


}



int	GTH_Network_GetMessage(i3socket_t *sock)
{
	struct sockaddr_in	readaddr;
	int					length;
	int					ret = 0;

	length = NET_ReadPacket(&sock->socket, (byte *)&packetBuffer, &readaddr);

	
	
	if (length == 0 || length < NET_HEADERSIZE || length > NET_DATAGRAMSIZE ) 
	{
		if( NET_ControlRecvBank( sock ) ) return PacketAnalysis_Start;
		return PacketAnalysis_None;
	}

	ret = NET_ControlPacket(sock, &packetBuffer);

	return ret;
}

int GTH_ListenPacket()
{
	int command;
	int state;

	g_netTime = NET_GetNetTime();


	
	GTH_Network_CheckProblemSocket(&gsSocket);

	while(1)
	{
		state = GTH_Network_GetMessage(&gsSocket); 

		if (state < PacketAnalysis_Skip)
			return 0;
				
		if (state != PacketAnalysis_Start)
			continue;

		MSG_BeginReading();
		command = MSG_ReadByte();



		if (command!=GSC_SYNCMESSAGE)
		{
			int i=0;
			i++;
		}

		switch (command)
		{

		
		
		

		case GSC_EXTEND:
			GTH_ProcessMessage_Extend();
			break;

		
		
		

		case EXTEND_SECOND:
			GTH_ProcessMessage_Extend_Second();
			break;


		
		
		

		case GSC_SYNCMESSAGE:
			GTH_SyncCharacter();
			break;

		case GSC_CHATMESSAGE:       
			GTH_ProcessChatMessage();
			break;

		case GSC_CHATMESSAGE_WHISPER:       
			GTH_ProcessChatMessage_Whisper();
			break;

		case GSC_EVENT_IDLE:
			GTH_ProcessEventMessage_Idle();
			break;

		case GSC_EVENT_WALK:
			GTH_ProcessEventMessage_Walk();
			break;
		
		case GSC_EVENT_WALKLEFT:
			GTH_ProcessEventMessage_WalkLeft();
			break;
		
		case GSC_EVENT_WALKRIGHT:
			GTH_ProcessEventMessage_WalkRight();
			break;
		
		case GSC_EVENT_BACK:
			GTH_ProcessEventMessage_Back();
			break;
			
		case GSC_EVENT_BACKLEFT:
			GTH_ProcessEventMessage_BackLeft();
			break;
			
		case GSC_EVENT_BACKRIGHT:
			GTH_ProcessEventMessage_BackRight();
			break;
			
		case GSC_EVENT_RUN:
			GTH_ProcessEventMessage_Run();
			break;

		case GSC_EVENT_RUNLEFT:
			GTH_ProcessEventMessage_RunLeft();
			break;

		case GSC_EVENT_RUNRIGHT:
			GTH_ProcessEventMessage_RunRight();
			break;

		case GSC_EVENT_TURNLEFT:
			GTH_ProcessEventMessage_TurnLeft();
			break;

		case GSC_EVENT_TURNRIGHT:
			GTH_ProcessEventMessage_TurnRight();
			break;

		case GSC_EVENT_MOVELEFT:
			GTH_ProcessEventMessage_MoveLeft();
			break;

		case GSC_EVENT_MOVERIGHT:
			GTH_ProcessEventMessage_MoveRight();
			break;

		case GSC_EVENT_ATTACK:
			GTH_ProcessEventMessage_Attack();
			break;

		case GSC_EVENT_DAMAGE:
			GTH_ProcessEventMessage_Damage();
			break;

		case GSC_EVENT_DIE:
			GTH_ProcessEventMessage_Die();
			break;

		case GSC_EVENT_RESPAWN:
			GTH_ProcessEventMessage_Respawn();
			break;

		case GSC_EVENT_SIT :
			GTH_ProcessEventMessage_Sit();
			break;

		case GSC_EVENT_STAND :
			GTH_ProcessEventMessage_Stand();
			break;


		case GSC_EVENT_SKILL :
			GTH_ProcessEventMessage_Skill();
			break;

			
		case GSC_EVENT_SKILLCASTING :
			GTH_ProcessEventMessage_SkillCasting();
			break;


		case GSC_AUTORECOVERY :
			GTH_ProcessMessage_AutoRecovery();
			break;

		case GSC_REPLY_EPACKUSE:
			GTH_ProcessMessage_ReplyEpackUse();
			break;

		case GSC_TRANSPORTWORLDREP:
			GTH_ProcessMessage_TransportRep();
			break;

		case GSC_CONNECTREP:        
			GTH_ConnectReplyFromGameServer();
			break;

		case GSC_CHANGEWORLD:
			GTH_ProcessMessage_ChangeWorld();
			break;

		case GSC_CHANGESERVER:
			GTH_ChangeServer();
			break;

		case GSC_EVENT_MOUSEWALK:
			GTH_ProcessEventMessage_MouseWalk();
			break;

		case GSC_EVENT_MOUSERUN:
			GTH_ProcessEventMessage_MouseRun();
			break;

		case GSC_LOOTVALUE :
			GTH_ProcessMessage_LootValue();
			break;
		case GSC_ADDMULTIITEMOBJECT :
			GTH_ProcessMessage_AddMultiItemObject();
			break;

		case GSC_ADDITEMOBJECT:
			GTH_ProcessMessage_AddItemObject();
			break;

		case GSC_SYNCITEMOBJECT:
			GTH_ProcessMessage_SyncItemObject();
			break;

		case GSC_DELETEITEMOBJECT:
			GTH_ProcessMessage_DeleteItemObject();
			break;

		case GSC_REPLY_GETINVENTORYITEM:
			GTH_ProcessMessage_ReplyGetInventoryItem();
			break;

		case GSC_REPLY_GETEQUIPITEM:
			GTH_ProcessMessage_ReplyGetEquipItem();
			break;

		case GSC_REPLY_DROPITEM:
			GTH_ProcessMessage_ReplyDropItem();
			break;

		case GSC_REPLY_PICKUPITEM:
			GTH_ProcessMessage_ReplyPickUpItem();
			break;

		
		
		case GSC_NPCINFO :
			GTH_ProcessMessage_NPCInfo();
			break;

		case GSC_REPLY_NPCCHAT :
			GTH_ProcessMessage_ReplyNpcChat();
			break;
		case GSC_REPLY_QUITNPCCHAT :
			GTH_ProcessMessage_ReplyQuitNpcChat();
			break;
		case GSC_NPCMESSAGE :
			GTH_ProcessMessage_NpcMessage();
			break;

		case GSC_REPLY_WEAPONCHANGE :
			GTH_ProcessMessage_WeaponChange();
			break;
		case GSC_LEVELUP :
			GTH_ProcessMessage_LevelUp();
			break;
		
		case GSC_REPLY_GENCHANGE :
			GTH_ProcessMessage_ReplyGenChange();
			break;
		
		case GSC_REPLY_STATUSUP :
			GTH_ProcessMessage_ReplyStatusUp();
			break;
		
		case GSC_REPLY_STORE :
			GTH_ProcessMessage_ReplyStore();
			break;
		
		case GSC_REPLY_BUYITEM :
			GTH_ProcessMessage_ReplyBuyItem();
			break;
		case GSC_REPLY_SELLITEM :
			GTH_ProcessMessage_ReplySellItem();
			break;
		
		case GSC_REPLY_BINDING :
			GTH_ProcessMessage_ReplyBinding();
			break;
		
		case GSC_REPLY_EPACKSTORE :
			GTH_ProcessMessage_ReplyEpackStore();
			break;

		case GSC_REPLY_REPAIR :
			GTH_ProcessMessage_ReplyRepairItem();
			break;
		
		case GSC_REPLY_DESTROY_ITEM :
			GTH_ProcessMessage_ReplyDestroyItem();
			break;

		case GSC_ADDITEMINV :	
			GTH_ProcessMessage_AddItemInv();
			break;
		case GSC_DELETEITEMINV :
			GTH_ProcessMessage_DeleteItemInv();
			break;
			
		case GSC_QUESTSTATUS_INFO :
			GTH_ProcessMessage_QuestStatusInfo();
			break;
		case GSC_QUEST_STEP :
			GTH_ProcessMessage_QuestStep();
			break;
		case GSC_QUEST_ITEM_INFO :
			GTH_ProcessMessage_QuestItemInfo();
			break;

		
		case GSC_AUTOQUESTSTATUS_INFO :
			GTH_ProcessMessage_AutoQuestStatusInfo();
			break;
		
		case GSC_ANSWER_AUTOEFFECT :
			GTH_ProcessMessage_AutoQuestIsValid();
			break;		

			
		case GC_RELIABLE_PING:
		case GC_UNRELIABLE_PING:
			break;

		case GSC_SETSHUTDOWNTIMER:
			GTH_ProcessMessage_SetShutdownTimer();
			break;

		case GSC_NOTICEMESSAGE:
			GTH_ProcessMessage_NoticeMessage();
			break;

		case GSC_CALLGAMEMASTER:
			GTH_ProcessMessage_CallGameMaster();
			break;

		case GSC_REQUESTSUMMONS:
			GTH_ProcessMessage_RequestSummons();
			break;

		case GSC_REPLYSUMMONS:
			GTH_ProcessMessage_ReplySummons();
			break;

		case GSC_REPLY_PCTRADE :
			GTH_ProcessMessage_PCTrade();
			break;

		case GSC_REPLY_PARTY :
			GTH_ProcessMessage_Party();
			break;

		case GSC_NOTFOUNDPC:
			GTH_ProcessMessage_NotFoundPC();
			break;

		case GSC_REPLY_CHEATCODE :
			GTH_ProcessMessage_ReplyCheatCode();
			break;

		case GSC_REPLY_SKILLLEVELUP :
			GTH_ProcessMessage_Reply_SkillLevelUp();
			break;

		case GSC_REPLY_BUY_SKILL :
			GTH_ProcessMessage_Reply_BuySkill();
			break;

		case GSC_REPLY_LEARN_SKILL :
			GTH_ProcessMessage_Reply_LearnSkill();
			break;

		case GSC_REPLY_ADDSKILL :
			GTH_ProcessMessage_Reply_AddSkill();
			break;

		case GSC_AFFECT :
			GTH_ProcessMessage_Affect();
			break;

		case GSC_REPLY_PCINFO :
			GTH_ProcessMessage_Reply_PCInfo();
			break;

		case GSC_REPLY_USE_PORTAL :
			GTH_ProcessMessage_Reply_UsePortal();
			break;
		
		case GSC_REPLY_SAVE_PORTAL_POSITION :
			GTH_ProcessMessage_Reply_SavePortalPosition();
			break;

		case GSC_QUESTLOGIC :
			GTH_ProcessMessage_QuestLogic();
			break;

		case GSC_REPLY_ITEMSYSTEM :
			GTH_ProcessMessage_ItemSystem();
			break;

		case GSC_REPLY_FRIENDS:
			GTH_ProcessMessage_Friends();
			break;

		
		case GSC_EVENT_MONSTER_SKILL:
			GTH_ProcessEventMessage_MonsterSkill();
			break;
		
		case GSC_EVENT_MONSTER_SKILLCASTING:
			GTH_ProcessEventMessage_MonsterSkillCasting();
			break;

		
		case GSC_GUILD_REPLY:
			GTH_ProcessMessage_Guild_Reply();
			break;

		
		case GSC_GUILD_REQUEST:
			GTH_ProcessMessage_Guild_Request();
			break;

		
		case GSC_EVENT_TELEPORT:
			GTH_ProcessEventMessage_Teleport();
			break;

		case GSC_EVENT_BEFORE_TELEPORT:
			GTH_ProcessEventMessage_Before_Teleport();
			
			break;

			
			
		case GSC_CSAUTH :

			
			break;

			
		case GSC_REPLY_USE_POTION:
			GTH_ProcessMessage_Reply_UsePotion();
			break;
			 
			
		case GSC_CHATTINGMSG :
			GTH_ProcessMessage_ChattingMsg();
			break;





			     
			
		case GSC_ANSWER_CHATBAN_FLAG:
			{
				
				g_cgv.myCharacterInfo->m_bChatBan_Flag=true;
				
			}
		
			break;
			
		case GSC_ANSWER_CHATBAN_START:
			
				
			
				GTH_ProcessChatBan_AnswerStart();
			
		
			
			break;
		
		case GSC_ANSWER_CHATBAN_CANCEL:
			{
			
				GTH_ProcessChatBan_AnswerCancel();
			}
			
			break;
		
		case GSC_ANSWER_GMCHATBAN_CANCEL:
		   {
			   GTH_ProcessChatBan_AnswerGMCancel();
		   }
			break;
		
		case GSC_ANSWER_GMCHATBAN_START:
			{
				GTH_ProcessChatBan_AnswerGMStart();
			}
			break;
		case GSC_ANSWER_CHAOVALUE:
			{
				GTH_ProcessMessage_AnswerChaosValue();
			}
			break;
		case GSC_SEND_ADDCHAOSINFO:
			{
				GTH_ProcessMessage_SendAddChaosInfo();
			}
			break;
		case GSC_SEND_DECCHAOSINFO:
			{

			}
			break;
	
	
			
		default :
			{
				char mismatchInfoStr[1024];
				sprintf( mismatchInfoStr, "%s(Protocal : %d)", g_LPACK.GetMassage(0,41), command );
				
				
				ErrorLog( mismatchInfoStr );
			}
			break;
		}
	}

	return 1;
}



int GTH_ProcessMessage_Extend()
{
	int command;

	command = MSG_ReadByte();
	switch( command )
	{
		#ifdef _TERRA
		case GSC_EXTEND_TERRA:
			gcTerraCtrl.Decoder();
			break;
		#endif
			
		case GSC_SUB_EVENT_SOCIAL_SHOUT:
		case GSC_SUB_EVENT_SOCIAL_BOW:
			GTH_ProcessEventMessage_SocialAnimaton(command);
			break;
			
		case GSC_SUB_EVENT_COMPLETE_EFFECT:
			GTH_ProcessEventMessage_CompleteEffect();
			break;
			
		case GSC_MAILSYSTEM :
			GTH_ProcessMessage_MailSystem();
			break;
			
			
		case GSC_WORLDCHATTING :
			GTH_ProcessMessage_WorldChatting();
			break;

		case GSC_DAN_BATTLE :															
			g_DanBattle.RecvDanBattle();
			break;
					
		case GSC_ANSWER_GONRYUNBATTLEPRACTICE:
			{
				g_GonryunBattlePractic.RecvDanBattle();
			}			
			break;
			

		
		case GSC_GB_BATTLE :														
			g_GolryunBattle_Decoder.RecvGolryunBattle();
			break;
		

			
		case GSC_REPLY_CHRISTMAS_EVENT:	
			g_ChristMasEvent.GTH_Process_ChristMas_Event();
			break;


		
		case  GSC_POSTSYSTEM:
			{								
				g_PostFunc.GTH_ProcessMessage_RecvPostSystem();
			}break;			

		case  GSC_MAILBOX:
			{								
				g_PostFunc.GTH_ProcessMessage_RecvMailBox();
			}break;		
		

			
		case GSC_REPLY_USE_EPACK_CHARG_SOUL:
			GTH_ProcessMessage_EpackChargSoul();
			break;


			
		case GSC_EXTEND_SEND_USE_MASIC_BOOK:
			GTH_ProcessMessage_Reply_UseMasicBook();
			break;
		
		
		case GSC_EXTEND_REPLY_USE_MASIC_BOOK :
			GTH_ProcessMessage_Reply_MasicBook_RepairItem();
			break;

		
		case GSC_EXTEND_EVENT_SKILL_ERROR:
			GTH_ProcessMessage_Repl_SkillError();
			break;


		
		case GSC_WORLDCHATTING_COUNT:
			GTH_ProcessMessage_WorldChatting_Count();
			break;

		 
		case GSC_WORLD_CHANGE:
			GTH_WorldChangeSystem_Process();
			break;
		 
			
			

	}

	return 1;
}













#include "..\Terra ICT\CTerraLpack.h"	

void Decoder_SC_DISCONNECT(void)
{
	int MsgIdx = 0;
	MsgIdx = MSG_ReadLong();
	switch ( MsgIdx )
	{
		
	case 0:
		g_ifMng->SetMessage( 
			"HACKSHIELD",
			g_LPACK.GetMessage(LPACK_TYPE_TERRA,CTerraLpack::enumMessageType::type_HackShield,8),
			MSG_BUTTONTYPE_OK, 
			IF_MESSAGE_EXIT_CLIENT );
		break;

	case 1:
		g_ifMng->SetMessage( 
			"HACKSHIELD",
			g_LPACK.GetMessage(LPACK_TYPE_TERRA,CTerraLpack::enumMessageType::type_HackShield,9),
			MSG_BUTTONTYPE_OK, 
			IF_MESSAGE_EXIT_CLIENT );
		break;

	case 2:
		g_ifMng->SetMessage( 
			"HACKSHIELD",
			g_LPACK.GetMessage(LPACK_TYPE_TERRA,CTerraLpack::enumMessageType::type_HackShield,10),
			MSG_BUTTONTYPE_OK, 
			IF_MESSAGE_EXIT_CLIENT );
		break;		 
		
	}
	g_ifMng->SetMessage( 
		g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), 
		g_LPACK.GetMassage(LPACK_TYPE_NORMAL,40),  
		MSG_BUTTONTYPE_OK, 
		IF_MESSAGE_EXIT_CLIENT );
	g_cgv.onGameServer = false;
}











#include "..\GM(Game Master)\CGMCtrl.h"
extern CGMCtrl* gcpGMCtrl;
int GTH_ProcessMessage_Extend_Second()
{	

	enum enumExtendSecondPacket SecondPackaet;	
	SecondPackaet = static_cast<enum enumExtendSecondPacket>(MSG_ReadShort());

	
	
	switch( SecondPackaet )
	{		
	case GMsystem:
		gcpGMCtrl->Decoder();
		break;

	case WEATHER_SYSTEM:
		g_WeatherSystem.GTH_Process_Weather_System();
		break;			

		
	case ITEM_REPARE:
		GTH_ProcessMessageReply_RepairItemConfirm();
		break;

	case HELPER_SYSTEM:
		g_HelperSystem.GTH_ProcessMessage_RecvHelpSystem();
		break;

	
	case GIFTCARD:
		g_GiftCard.ProcessGiftCard();
		break;

		
		
	
	case EXP_EVENT:
		g_ExpRateMng.ProcessMessage();

	case TERRA:
		#ifdef _TERRA
		gcTerraCtrl.Decoder();
		#endif
		break;

		
	case CASH_MALL_ITEM:
		GTH_DecodeCashMallItem();		
		break;

	
	case GAMBLE_SYSTEM:
		GambleSystemDecode();
		break;
		
	case SC_DISCONNECT:
		Decoder_SC_DISCONNECT();
		break;
		
	
	case ITEM_EXTEND_SYSTEM:
		GTH_ItemExtendSystemDecode();
		break;

	
	case ITEM_SUMMON_SYSTEM:
		g_ifMng->m_Item_ScrollWin->Decoder();
		break;
		
	
	case NEW_DAN_BATTLE:
		GTH_DecodeDanBattleSystem();
		break;

	case CHAO_POINT_EVENT:
		GTH_Chao_point_event();
		break;

	default:
		return 0;
	}	
	return 1;
}




void GTH_SendMessage_ConnectGameServer()
{
	

	
	g_GonryunBattlePractic.m_ieffectMode=PCB_EFF_NONE;
	g_GonryunBattlePractic.m_ibattleMode=PCB_NONE;

	if( !g_cgv.sendGameServerConnectFlag ) 
		return;

    MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_CONNECT_GAMESERVER);
		MSG_WriteString(&netMessage, g_cgv.clientID);
		MSG_WriteByte(&netMessage, g_cgv.selectedGroupIdx);
		MSG_WriteByte(&netMessage, g_cgv.savedData[g_cgv.selectedGameDataIdx].characterID);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

int GTH_ConnectReplyFromGameServer()
{
	unsigned int time;
	int j, k;

	time = MSG_ReadLong();  

	g_cgv.syncCharacterNumber = 1;
	g_cgv.syncCharacter[0].entityType = ENTITY_PC;

	g_cgv.syncCharacter[0].idxOnServer = MSG_ReadShort();

	g_cgv.myPCIdxOnServer = g_cgv.syncCharacter[0].idxOnServer;

	g_cgv.syncCharacter[0].tableIdx = MSG_ReadByte();

	g_cgv.syncCharacter[0].skinPart[0] = MSG_ReadByte();
	g_cgv.syncCharacter[0].skinPart[1] = MSG_ReadByte();
	g_cgv.syncCharacter[0].skinPart[2] = MSG_ReadByte();
	g_cgv.syncCharacter[0].skinPart[3] = MSG_ReadByte();

	g_cgv.syncCharacter[0].equipNumber = MSG_ReadByte();
	for (j=0; j<g_cgv.syncCharacter[0].equipNumber; j++)
		g_cgv.syncCharacter[0].equipment[j] = MSG_ReadShort();

	g_cgv.syncCharacter[0].worldIdx = MSG_ReadByte();

	g_cgv.syncCharacter[0].position[0] = (float)MSG_ReadShort();
	g_cgv.syncCharacter[0].position[1] = (float)MSG_ReadShort();
	g_cgv.syncCharacter[0].position[2] = (float)MSG_ReadShort();

	g_cgv.syncCharacter[0].angles[ YAW ] = MSG_ReadFloat();

	g_cgv.syncCharacter[0].event = (i3characterEvent_t) MSG_ReadByte();
	g_cgv.syncCharacter[0].curRA = MSG_ReadLong();
	g_cgv.syncCharacter[0].curSA = MSG_ReadShort();
	g_cgv.syncCharacter[0].calMaxRA = MSG_ReadLong();
	g_cgv.syncCharacter[0].calMaxSA = MSG_ReadShort();

	g_cgv.syncCharacter[0].speed = MSG_ReadFloat();
	strcpy(g_cgv.syncCharacter[0].name, MSG_ReadString()); 
	g_cgv.syncCharacter[0].nameColorIdx = MSG_ReadByte();
	g_cgv.syncCharacter[0].guildIdx = MSG_ReadShort();
	g_cgv.GM.bChatAllow = MSG_ReadByte();
	assert(NULL != gcpGMCtrl);
	assert(NULL != gcpGMCtrl->m_cpPKTimeCtrl);
	(gcpGMCtrl->m_cpPKTimeCtrl)->SetPK(MSG_ReadLong());

	
	g_HelperSystem.m_MyPointInfo.iMyPoint = MSG_ReadByte();

	
	
	
	
	
	
	
	for( j = 0; j < MAX_INVENTORY_SIZE; j ++ )
		g_cgv.myCharacterInfo->inventory[j] = -1;
	
	for( j = 0; j < MAX_DEPOT_SIZE; j ++ )
		g_cgv.myCharacterInfo->depot[j] = -1;
	
	for( j = 0; j < MAX_EQUIPMENT; j ++ )
		g_cgv.myCharacterInfo->equipment[j] = -1;

	for( j = 0; j < MAX_PRECOCITY_SIZE; j ++ )
		g_cgv.myCharacterInfo->precocityInventory[j] = -1;
				
	g_cgv.myCharacterInfo->mouseInventory = -1;
	
	for (j = 0; j < MAX_NUMBER_OF_OWNITEM; j++)
	{
		g_cgv.myCharacterInfo->item[j].itemTableIdx = -1;
		for( k = 0; k < MAX_NUMBER_OF_ITEM_OPTION; k ++ )
		{
			g_cgv.myCharacterInfo->item[j].optionIdx[k] = -1;
		}
	}
	g_cgv.myCharacterInfo->itemNumber = MSG_ReadByte();

	for( j = 0; j < MAX_INVENTORY_SIZE; j ++ )
		g_cgv.myCharacterInfo->inventory[j] = MSG_ReadChar();

	for ( j = 0; j < MAX_DEPOT_SIZE; j ++ )
		g_cgv.myCharacterInfo->depot[j] = MSG_ReadChar();

	for (j=0; j<MAX_EQUIPMENT; j++)
		g_cgv.myCharacterInfo->equipment[j] = MSG_ReadChar();

	g_cgv.myCharacterInfo->mouseInventory = MSG_ReadChar();

	for( j = 0; j < MAX_PRECOCITY_SIZE; j ++ )
	{
		g_cgv.myCharacterInfo->precocityInventory[j] = MSG_ReadChar();
		g_cgv.myCharacterInfo->precocityTime[j] = MSG_ReadLong();
	}

	
	

	
	for( j = 0; j < g_cgv.myCharacterInfo->itemNumber; j++)
	{
		int itemIdx = MSG_ReadByte();
		item_t *item = &g_cgv.myCharacterInfo->item[itemIdx];

		strcpy( item->name, MSG_ReadString() );
		item->itemTableIdx = MSG_ReadShort();
		item->optionNumber = MSG_ReadByte();

		for( k = 0; k < item->optionNumber; k ++ )
			item->optionIdx[k] = MSG_ReadShort();

		for( k = 0; k < item->optionNumber; k ++ )
		{
			item->optionValue[k] = MSG_ReadShort();
		}

		item->durability = MSG_ReadShort();
		item->reqLevel = MSG_ReadByte();
		item->experience = MSG_ReadLong();
		item->timer = MSG_ReadLong();

			
		item->ItemExtendInfo.UseFlag = MSG_ReadLong();
		
		item->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType = 
			static_cast<ITEM_SPAWN_INFO::enumItemSpawnType>(MSG_ReadLong());
		
		strncpy(item->ItemExtendInfo.ItemSpawnInfomation.data, 
			MSG_ReadString(), item_t::MAX_SIZE_OF_ITEM_SPAWN_DATA);
		
		item->ItemExtendInfo.ItemSpawnInfomation.data[item_t::MAX_SIZE_OF_ITEM_SPAWN_DATA -1] = NULL;
		item->ItemExtendInfo.AllowUseTime	= MSG_ReadLong();	
		item->ItemExtendInfo.CalclateUseDateToSec	= MSG_ReadLong();	
	}
	
	g_cgv.myCharacterInfo->GM.Chat.RemainBanTime = MSG_ReadLong();
	
	
	int flag = MSG_ReadLong();
	gGameConfigFlag.SetFlag(flag);

	
	g_cgv.myCharacterInfo->m_nMemberShipType = MSG_ReadByte();

	
	if(gGameConfigFlag.m_bWorld_Chat_Limit)
	{
		g_cgv.myCharacterInfo->WorldChatting.lDate			= MSG_ReadLong();
		g_cgv.myCharacterInfo->WorldChatting.iMaxCount		= MSG_ReadLong();
		g_cgv.myCharacterInfo->WorldChatting.iDecreaseCount = MSG_ReadLong();
	}
	

	
	if(gGameConfigFlag.m_bPrecocity_Time)
	{
		g_cgv.myCharacterInfo->m_fItemPrecocityTimeRatio	= MSG_ReadFloat();
	}
	//lucky 2012 new GM system
	g_cgv.syncCharacter->gameMaster = MSG_ReadLong();
	//end
	GTH_SetWorldInfo();

	
	
	g_cgv.syncCharacter[0].guildIdx = -1;
	memset( g_cgv.syncCharacter[0].guildName, 0, GUILDNAMESTRING );

	
	g_ifMng->m_itemWin->ClearLockItems();

	
	g_NewDanBattleSystem.InitDanBattleInfo();
	g_ifMng->m_DanWarBattleWin->Enable(FALSE);	


	


	return 1;
}


int GTH_SendMessage_DisconnectGameServer(BOOL bForceQuit_fromGM) 
{
	
	g_GonryunBattlePractic.m_ieffectMode=PCB_EFF_NONE;
	g_GonryunBattlePractic.m_ibattleMode=PCB_NONE;
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_DISCONNECT_GAMESERVER);
		MSG_WriteByte(&netMessage,bForceQuit_fromGM);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}


void GTH_SendMessage_ReliablePing( i3socket_t *sock )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GC_RELIABLE_PING);
		NET_SendMessage(sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_UnreliablePing( i3socket_t *sock )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GC_UNRELIABLE_PING);
		NET_SendUnreliableMessage(sock, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

int GTH_SendMessage_ReadyToPlay()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_READYTOPLAY);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendMessage_TransportWorld(int worldIdx, int gateIdx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_TRANSPORTWORLD);
		MSG_WriteByte(&netMessage, worldIdx);
		MSG_WriteByte(&netMessage, gateIdx);
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );
	return 1;
}



int	GTH_SendChatMessage(char *chatMessage, int type)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_CHATMESSAGE);
		MSG_WriteByte(&netMessage, type);		
		MSG_WriteString(&netMessage, chatMessage);	
		NET_SendUnreliableMessage(&gsSocket, &netMessage);		
	}
	MSG_EndWriting( &netMessage );

	if( type == IF_CHATMSG_TYPE_DAN )
	{
		return 1;
	}
	
	Fx_CHARACTER_t *character;

	character = &g_charManager->m_Characters[0];

	strcpy( character->chatMsg, chatMessage );
	character->chatMsgTime = g_timer.GetAppMilliTime();

	return 1;
}

int	GTH_SendChatMessage_Whisper(char *name, char *chatMessage)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_CHATMESSAGE_WHISPER);
		MSG_WriteString(&netMessage, name);
		MSG_WriteString(&netMessage, chatMessage);
		
		NET_SendUnreliableMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

#define		MAX_ALLOWED_LATENCY				4000
#define		MAX_ALLOWED_AVE_LATENCY			2000
#define		MAX_ALLOWED_OVER_LATENCY_COUNT	4

int	GTH_CheckSpeedHack	()
{
	float	elapsedTime;

	g_cgv.lastSyncTime		=	g_cgv.currentSyncTime;
	g_cgv.currentSyncTime	=	g_timer.GetAppMilliTime ();
	
	if	(	g_cgv.lastSyncTime	>	0.0	)
	{
		elapsedTime	=	g_cgv.currentSyncTime - g_cgv.lastSyncTime;
		if	(	elapsedTime	<	0.0f	)
		{
			return	false;
		}

		if	(	elapsedTime	>	MAX_ALLOWED_LATENCY	)
		{
			++g_cgv.syncOverAverageCount;
			if	(	g_cgv.syncOverAverageCount	>=	MAX_ALLOWED_OVER_LATENCY_COUNT	)
			{
				return	false;
			}
		}

		if	(	g_cgv.syncSampleCount	<	MAX_SYNC_SAMPLE_COUNT	)
		{
			g_cgv.syncTimeList [g_cgv.syncSampleCount]	=	elapsedTime;
			++g_cgv.syncSampleCount;
		}
		else
		{
			float	average	=	g_cgv.syncAverage * MAX_SYNC_SAMPLE_COUNT - g_cgv.syncTimeList [0] + elapsedTime;
			memcpy ( &g_cgv.syncTimeList [0], &g_cgv.syncTimeList [1], sizeof(float) * ( MAX_SYNC_SAMPLE_COUNT - 1 ) );
			g_cgv.syncTimeList [MAX_SYNC_SAMPLE_COUNT - 1]	=	elapsedTime;
			g_cgv.syncAverage	=	average / (float)MAX_SYNC_SAMPLE_COUNT;

			if	(	g_cgv.syncAverage	>	MAX_ALLOWED_AVE_LATENCY	)
			{
				return	false;
			}
		}
	}

	return	true;
}









void GTH_SyncCharacter_PC_One(syncCharacter_t& syncChar)
{
	syncChar.entityType =  ENTITY_PC;
	syncChar.idxOnServer = MSG_ReadShort();
	syncChar.tableIdx = MSG_ReadByte();
	syncChar.pcJob = MSG_ReadByte();			

	
	syncChar.aliveState = MSG_ReadByte();

	
	
	syncChar.isTransform = MSG_ReadByte();
	if( !syncChar.isTransform ){
		syncChar.skinPart[0] = MSG_ReadByte();
		syncChar.skinPart[1] = MSG_ReadByte();
		syncChar.skinPart[2] = MSG_ReadByte();
		syncChar.skinPart[3] = MSG_ReadByte();
		
		syncChar.equipNumber = MSG_ReadByte();
		for (int idx=0; idx < syncChar.equipNumber; idx++)
			syncChar.equipment[idx] = MSG_ReadShort();
	}else{
		syncChar.transformEntityIdx = MSG_ReadShort();
		syncChar.transformType = MSG_ReadByte();
	}

	MSG_ReadPosition( syncChar.position );
	syncChar.angles[ YAW ] = MSG_ReadFloat();

	syncChar.event = (i3characterEvent_t) MSG_ReadByte();
	syncChar.curRA = MSG_ReadLong();
	syncChar.curSA = MSG_ReadShort();
	syncChar.calMaxRA = MSG_ReadLong();
	syncChar.calMaxSA = MSG_ReadShort();

	syncChar.speed = MSG_ReadFloat();

	
	syncChar.boothState = MSG_ReadByte();
	
	if( syncChar.boothState == 2 ){
		strncpy( syncChar.boothName, MSG_ReadString() ,syncCharacter_t::maxbytes_boothName);
		syncChar.boothName[syncCharacter_t::maxbytes_boothName-1]=NULL;
	}

	strncpy(syncChar.name, MSG_ReadString(),NAMESTRING); 
	syncChar.name[NAMESTRING-1]=NULL;

	
	syncChar.nameColorIdx = MSG_ReadByte();

	syncChar.waitingRoom = MSG_ReadByte();
	syncChar.joinNum = MSG_ReadShort();

	
	syncChar.numStaticEffects = MSG_ReadByte();
	assert(syncChar.numStaticEffects < MAX_NUM_STATIC_EFFECT);

	for(int idx = 0; idx < syncChar.numStaticEffects; idx++){
		syncChar.staticEffectIndex[idx] = MSG_ReadShort();
	}

	
	syncChar.generalSpeed = MSG_ReadByte();

	
	
	syncChar.chaosPoint = MSG_ReadShort();			
	syncChar.pvpMode = MSG_ReadByte();

	syncChar.guildIdx = MSG_ReadShort();
	strncpy( syncChar.guildName, MSG_ReadString(),GUILDNAMESTRING);
	syncChar.guildName[GUILDNAMESTRING-1]=NULL;

	
	syncChar.hide	= MSG_ReadByte();

	
	syncChar.nDanBattleIdx = MSG_ReadShort();
	//lucky 2012 new GM system
	syncChar.gameMaster = MSG_ReadLong();
	//end


}


void GTH_SyncCharacter_PC(BOOL& bMyCharacterSync,int& characterNumber,const int pcNumber)
{
	bMyCharacterSync=FALSE;

	syncCharacter_t tempSyncChar;
	syncCharacter_t* pSyncChar=NULL;
	int pcIdx;

	
	for(int num=0; num < pcNumber; num++){

		memset(&tempSyncChar,0x00,sizeof(syncCharacter_t ));
		GTH_SyncCharacter_PC_One(tempSyncChar);


	
		if(tempSyncChar.idxOnServer == g_cgv.myPCIdxOnServer){
			pcIdx=0;
	
			bMyCharacterSync=TRUE;
		}else{
	
			pcIdx = characterNumber;
			if(pcIdx < 0) continue;
			if(pcIdx >= MAX_CHARACTERS) continue;

			characterNumber++;
		}



		pSyncChar=&g_cgv.syncCharacter[pcIdx];
		
		pSyncChar->entityType	=	tempSyncChar.entityType;
		pSyncChar->idxOnServer	=	tempSyncChar.idxOnServer;
		pSyncChar->tableIdx		=	tempSyncChar.tableIdx;		
		pSyncChar->pcJob		=	tempSyncChar.pcJob;		
		pSyncChar->aliveState	=	tempSyncChar.aliveState;		
		pSyncChar->isTransform	=	tempSyncChar.isTransform;


		if( !pSyncChar->isTransform )
		{
			pSyncChar->skinPart[0] = tempSyncChar.skinPart[0];
			pSyncChar->skinPart[1] = tempSyncChar.skinPart[1];
			pSyncChar->skinPart[2] = tempSyncChar.skinPart[2];
			pSyncChar->skinPart[3] = tempSyncChar.skinPart[3];
			
			pSyncChar->equipNumber = tempSyncChar.equipNumber;
			for (int j=0; j<pSyncChar->equipNumber; j++)
				pSyncChar->equipment[j] = tempSyncChar.equipment[j];
		}
		else
		{
			pSyncChar->transformEntityIdx = tempSyncChar.transformEntityIdx;
			pSyncChar->transformType = tempSyncChar.transformType;
		}

		pSyncChar->position[0] = tempSyncChar.position[0];
		pSyncChar->position[1] = tempSyncChar.position[1];
		pSyncChar->position[2] = tempSyncChar.position[2];


		
		pSyncChar->angles[ YAW ] = tempSyncChar.angles[YAW];

		pSyncChar->event = tempSyncChar.event;
		pSyncChar->curRA = tempSyncChar.curRA;
		pSyncChar->curSA = tempSyncChar.curSA;
		pSyncChar->calMaxRA = tempSyncChar.calMaxRA;
		pSyncChar->calMaxSA = tempSyncChar.calMaxSA;

		pSyncChar->speed = tempSyncChar.speed;

		
		pSyncChar->boothState = tempSyncChar.boothState;
		
		if( pSyncChar->boothState == 2 ){
			strncpy( pSyncChar->boothName,tempSyncChar.boothName, syncCharacter_t::maxbytes_boothName);
			pSyncChar->boothName[syncCharacter_t::maxbytes_boothName-1]=NULL;
		}


		strncpy(pSyncChar->name, tempSyncChar.name,NAMESTRING);
		pSyncChar->name[NAMESTRING-1]=NULL;

		pSyncChar->nameColorIdx = tempSyncChar.nameColorIdx;

		pSyncChar->waitingRoom = tempSyncChar.waitingRoom;
		pSyncChar->joinNum = tempSyncChar.joinNum;

		
		pSyncChar->numStaticEffects = tempSyncChar.numStaticEffects;
		for(int j = 0; j < pSyncChar->numStaticEffects; j ++ )
		{
			pSyncChar->staticEffectIndex[j] = tempSyncChar.staticEffectIndex[j];
		}

		
		pSyncChar->generalSpeed = tempSyncChar.generalSpeed;
		

		pSyncChar->chaosPoint = tempSyncChar.chaosPoint;
		pSyncChar->pvpMode = tempSyncChar.pvpMode;

		pSyncChar->guildIdx = tempSyncChar.guildIdx;
		strncpy( pSyncChar->guildName, tempSyncChar.guildName,GUILDNAMESTRING);
		pSyncChar->guildName[GUILDNAMESTRING-1]=NULL;

		pSyncChar->hide	= tempSyncChar.hide;		

		
		pSyncChar->nDanBattleIdx = tempSyncChar.nDanBattleIdx;	
		//lucky 2012 new GM SYSTEM
		pSyncChar->gameMaster		=	tempSyncChar.gameMaster;
		//end

	}	
}


















void GTH_SyncCharacter_Monster_One(syncCharacter_t& syncChar)
{
	syncChar.entityType =  ENTITY_MONSTER;
	syncChar.idxOnServer = MSG_ReadShort();
	syncChar.tableIdx = MSG_ReadByte(); 
	syncChar.aliveState = MSG_ReadByte(); 

	MSG_ReadPosition(syncChar.position);  
	syncChar.angles[ YAW ] = MSG_ReadFloat();
	syncChar.event = (i3characterEvent_t) MSG_ReadByte();
	syncChar.curRA = MSG_ReadLong();
	syncChar.curSA = MSG_ReadShort();
	syncChar.calMaxRA = MSG_ReadLong();
	syncChar.calMaxSA = MSG_ReadShort();

	syncChar.speed = MSG_ReadFloat(); 
	syncChar.equipNumber = (int)MSG_ReadFloat();

	if( syncChar.equipNumber > 0 )
	{

	}
	
	syncChar.numStaticEffects = MSG_ReadByte();
	for(int j = 0; j < syncChar.numStaticEffects; j ++ )
	{
		syncChar.staticEffectIndex[j] = MSG_ReadShort();
	}

	syncChar.generalSpeed = MSG_ReadByte();

	
	syncChar.specialMonster = MSG_ReadByte();
}



void GTH_SyncCharacter_Monster(int& characterNumber,const int monsterNumber)
{

	syncCharacter_t tempMonster;
	syncCharacter_t* pMonster=NULL;

	
	for(int num=0; num < monsterNumber; num++)
	{


		memset(&tempMonster,0x00,sizeof(syncCharacter_t));
		GTH_SyncCharacter_Monster_One(tempMonster);


		if(characterNumber < 0) continue;
		if(characterNumber >= MAX_CHARACTERS) continue;
		pMonster = &g_cgv.syncCharacter[characterNumber];
		characterNumber++;


		pMonster->entityType =  ENTITY_MONSTER;
		pMonster->idxOnServer = tempMonster.idxOnServer;
		pMonster->tableIdx = tempMonster.tableIdx; 
		pMonster->aliveState = tempMonster.aliveState;

		pMonster->position[0] = tempMonster.position[0];
		pMonster->position[1] = tempMonster.position[1];
		pMonster->position[2] = tempMonster.position[2];

		pMonster->angles[ YAW ] = tempMonster.angles[YAW];
		pMonster->event = tempMonster.event;
		pMonster->curRA = tempMonster.curRA;
		pMonster->curSA = tempMonster.curSA;
		pMonster->calMaxRA = tempMonster.calMaxRA;
		pMonster->calMaxSA = tempMonster.calMaxSA;

		pMonster->speed = tempMonster.speed; 
		pMonster->equipNumber = tempMonster.equipNumber;

		if( pMonster->equipNumber > 0 )
		{

		}
		
		pMonster->numStaticEffects = tempMonster.numStaticEffects;
		for(int j = 0; j < pMonster->numStaticEffects; j ++ )
		{
			pMonster->staticEffectIndex[j] = tempMonster.staticEffectIndex[j];
		}

		pMonster->generalSpeed = tempMonster.generalSpeed;

		
		pMonster->specialMonster = tempMonster.specialMonster;		
	}
}






void GTH_SyncCharacter_NPC_One(syncCharacter_t& syncChar)
{
	syncChar.entityType =  ENTITY_NPC;
	syncChar.idxOnServer = MSG_ReadShort();
	syncChar.tableIdx = MSG_ReadByte(); 
	syncChar.aliveState = MSG_ReadByte(); 
	syncChar.scriptFlag = MSG_ReadShort();

	MSG_ReadPosition(syncChar.position);  
	syncChar.angles[ YAW ] = MSG_ReadFloat();
	syncChar.event = (i3characterEvent_t) MSG_ReadByte();
	syncChar.curRA = MSG_ReadLong();
	syncChar.curSA = MSG_ReadShort();

	syncChar.calMaxRA = MSG_ReadLong();
	syncChar.calMaxSA = MSG_ReadShort();

	syncChar.speed = MSG_ReadFloat(); 

	syncChar.generalSpeed = MSG_ReadByte();

	
	syncChar.numStaticEffects = 0;

	
	syncChar.specialMonster = 0;
}
		








void GTH_SyncCharacter_NPC(int& characterNumber,const int npcNumber)
{
	syncCharacter_t tempSyncChar;
	syncCharacter_t* pNPC=NULL;


	
	for(int num=0; num < npcNumber; num++){
		
		GTH_SyncCharacter_NPC_One(tempSyncChar);

		if(characterNumber < 0) continue;
		if(characterNumber >= MAX_CHARACTERS) continue;
		pNPC = &g_cgv.syncCharacter[characterNumber];
		characterNumber++;

		pNPC->entityType =  ENTITY_NPC;
		pNPC->idxOnServer = tempSyncChar.idxOnServer;
		pNPC->tableIdx = tempSyncChar.tableIdx;
		pNPC->aliveState = tempSyncChar.aliveState;
		pNPC->scriptFlag = tempSyncChar.scriptFlag;

		pNPC->position[0] = tempSyncChar.position[0];  
		pNPC->position[1] = tempSyncChar.position[1];  
		pNPC->position[2] = tempSyncChar.position[2];  

		pNPC->angles[ YAW ] = tempSyncChar.angles[YAW];
		pNPC->event = tempSyncChar.event;
		pNPC->curRA = tempSyncChar.curRA;
		pNPC->curSA = tempSyncChar.curSA;

		pNPC->calMaxRA = tempSyncChar.calMaxRA;
		pNPC->calMaxSA = tempSyncChar.calMaxSA;

		pNPC->speed = tempSyncChar.speed; 

		pNPC->generalSpeed = tempSyncChar.generalSpeed;
		
		pNPC->numStaticEffects = 0;
		
		pNPC->specialMonster = 0;
	}

}




void GTH_SyncCharacter(void)
{
	int i;
	int pcNumber, monsterNumber, npcNumber;
	int characterNumber;

	if( g_cgv.state != CLIENT_GAME_PROCESS ) return;


	GTH_CheckSpeedHack ();	

	g_cgv.latencyTime	= MSG_ReadShort();	
	pcNumber			= MSG_ReadByte();
	monsterNumber		= MSG_ReadByte();
	npcNumber			= MSG_ReadByte();
	g_cgv.syncCharacterNumber = pcNumber + monsterNumber + npcNumber;

	characterNumber = 1;	
	
	BOOL bMyCharacterSync=FALSE;

	GTH_SyncCharacter_PC(bMyCharacterSync,characterNumber,pcNumber);
	GTH_SyncCharacter_Monster(characterNumber,monsterNumber);
	GTH_SyncCharacter_NPC(characterNumber,npcNumber);



	
	
	int partyFlag, memberFlag, memberActive, memberIdx;
	int maxRA, curRA, maxSA, curSA;

	
	partyFlag = MSG_ReadByte();

	g_ifMng->m_partyWin->InitMemberInfo();

	if( partyFlag )
	{
		int n = 0;
		for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
		{			
			memberFlag = MSG_ReadByte();
			if( !memberFlag ) continue;
			
			memberIdx = MSG_ReadByte();
			memberActive = MSG_ReadByte();

			curRA = MSG_ReadLong();
			maxRA = MSG_ReadLong();
			curSA = MSG_ReadShort();
			maxSA = MSG_ReadShort();

			
			g_ifMng->m_partyWin->UpdateMemberInfo( n, memberIdx, memberActive, curRA, curSA, maxRA, maxSA );
			n ++;
		}
	}

	
	
	for( i = 0; i < MAX_PRECOCITY_SIZE; i ++ )
	{
		g_cgv.myCharacterInfo->precocityTime[i] = MSG_ReadLong();
	}

	
	int flag = MSG_ReadLong();
	gGameConfigFlag.SetFlag(flag);
	

	
	assert(g_cgv.syncCharacterNumber == (pcNumber + monsterNumber + npcNumber));
	g_charManager->GenerateCharacter(g_cgv.syncCharacter, g_cgv.syncCharacterNumber); 

	
	g_cgv.pTargetChar = g_GameTarget.GTH_GetTargetPointer( g_pApp->m_myCharacter );
	
	
	if(TRUE == bMyCharacterSync)
	{
		g_SkillLogic.SetMyCharTransform(false);
	}	
	return;
}



void GTH_ProcessChatMessage()
{
	char chatMessage[CHATSTRING]; 
	char fromName[NAMESTRING];
	DWORD color;
	int type;

	type = MSG_ReadByte();
	strcpy(fromName, MSG_ReadString());
	strcpy(chatMessage, MSG_ReadString());

	switch( type )
	{
	case IF_CHATMSG_TYPE_DAN :
		color = GTHCOLOR_DANMESSAGE;
		break;
	case IF_CHATMSG_TYPE_TRADE :
		color = GTHCOLOR_TRADEMESSAGE;
		break;
	case IF_CHATMSG_TYPE_NORMAL :
		color = GTHCOLOR_CHATMESSAGE;
		break;
	case IF_CHATMSG_TYPE_WORLD :
		color = GTHCOLOR_WORLDMESSAGE;
		break;
	case IF_CHATMSG_TYPE_SYSTEM :
		color = GTHCOLOR_SYSMESSAGE;
		break;
	}

	if( type == IF_CHATMSG_TYPE_DAN || type == IF_CHATMSG_TYPE_WORLD ||
		type == IF_CHATMSG_TYPE_PARTY )
	{
		g_ifMng->AddChatMessage( chatMessage, color, fromName, type );
		return;
	}

	Fx_CHARACTER_t *character;
	int i, numCharacter;

	character = &g_charManager->m_Characters[0];
	numCharacter = g_charManager->m_iNumCharacters;

	for (i=0; i < numCharacter; i++, character++) 
	{
		if ( character->entityType != ENTITY_PC ) continue;
		if( !stricmp( fromName, character->name ) )
		{
			if( GTH_RenderGM( character ) )
			{
				strcpy( character->chatMsg, chatMessage );
				character->chatMsgTime = g_timer.GetAppMilliTime();
			}
			break;
		}
	}
	
	if( i == numCharacter ) return;

	if( GTH_RenderGM( character ) )
	{
		g_ifMng->AddChatMessage( chatMessage, color, fromName, type );
	}
}

void GTH_ProcessChatMessage_Whisper()
{
	char chatMessage[CHATSTRING]; 
	char fromName[CHATSTRING];
	char szTemp[256];

	strcpy(fromName, MSG_ReadString());
	strcpy(chatMessage, MSG_ReadString());

	
	if( strstr( fromName, "GM") != NULL )
	{
		if( !stricmp( g_LPACK.GetMassage( 0, 441 ), chatMessage ) )
		{
			GTH_SendMessage_DisconnectGameServer(TRUE);
			g_cgv.serverShutdownTimer.isComplete = true;
			g_cgv.serverShutdownTimer.active = false;

			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,308), 1, IF_MESSAGE_EXIT_CLIENT );

			return;
		}
	}

	sprintf( szTemp, "(from %s)", fromName );

	g_ifMng->AddChatMessage( chatMessage, GTHCOLOR_WHISPERMESSAGE, szTemp, IF_CHATMSG_TYPE_WHISPER );
}


#include "..\tagGCPacket.h"
int GTH_ProcessMessage_TransportRep()
{
	enum tagGCPacket_TRANSPORTWORLDREP::enumCode code;
	int worldIdx; 
	vec3_t position; 
	float yaw;

	code = (enum tagGCPacket_TRANSPORTWORLDREP::enumCode)MSG_ReadByte();
	worldIdx = MSG_ReadByte();
	
	position[0] = (float)MSG_ReadShort();
	position[1] = (float)MSG_ReadShort();
	position[2] = (float)MSG_ReadShort();

	yaw = MSG_ReadFloat();

	switch(code){		
		case tagGCPacket_TRANSPORTWORLDREP::enumCode::fail:
			
			g_cgv.flagChangeWorld = false;
			break;

		case tagGCPacket_TRANSPORTWORLDREP::enumCode::success:
			g_cgv.syncCharacter[0].worldIdx = worldIdx;
			VectorCopy(g_cgv.syncCharacter[0].position, position);
			VectorSet(g_cgv.syncCharacter[0].angles, 0, yaw, 0);
			g_cgv.syncCharacter[0].event = GTH_EV_CHAR_IDLE;

			GTH_PrepareChangeWorld();
			break;

		case tagGCPacket_TRANSPORTWORLDREP::enumCode::access_deny_because_free_user:
			g_cgv.flagChangeWorld = false;

			g_ifMng->SetMessage( 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,224), 
				MSG_BUTTONTYPE_OK, 
				IF_MESSAGE_NONE);			
			break;
		
			case tagGCPacket_TRANSPORTWORLDREP::enumCode::busy_state:
				g_cgv.flagChangeWorld = false;

				g_ifMng->SetMessage( 
					g_LPACK.GetMassage(LPACK_TYPE_NORMAL,322), 
					g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,588), 
					MSG_BUTTONTYPE_OK, 
					IF_MESSAGE_NONE);			
			break;
		
			

				
			
	}

	return 1;
}




int GTH_ChangeServer()
{
	strcpy(g_cgv.gameServerIP, MSG_ReadString());
	g_cgv.gameServerPort = MSG_ReadShort();

	NET_CloseSocket( &gsSocket );

	g_cgv.changeServerFlag = true;
	
	g_cgv.sendGameServerConnectFlag = true;
	
	GTH_Network_OpenGameServerSocket();
	
	g_ifMng->m_changeStateFlag = true;

	g_cgv.restartState = RESTART_NONE;
	g_cgv.isGmHide = false;
	g_cgv.isGmBusy = false;


	
	SetWorldChangeMode(WorldChangeSystem::WORLD_CHANGESYSTEM_NONE);

	return true;
}

int GTH_ProcessMessage_ChangeWorld()
{
	int success;
	int worldIdx; 
	vec3_t position; 
	float yaw;

	success = MSG_ReadByte();
	worldIdx = MSG_ReadByte();
	
	position[0] = (float)MSG_ReadShort();
	position[1] = (float)MSG_ReadShort();
	position[2] = (float)MSG_ReadShort();
	yaw = MSG_ReadFloat();

	g_cgv.syncCharacter[0].worldIdx = worldIdx;
	VectorCopy(g_cgv.syncCharacter[0].position, position);
	VectorSet(g_cgv.syncCharacter[0].angles, 0, yaw, 0);

	GTH_PrepareChangeWorld();

	return 1;
}






void MSG_WritePosition(i3sizebuf_t *sb, vec3_t pos)
{
	vec3_t	position;

	position[0] = pos[0] - g_pApp->m_worldBSPs[0]->m_models->bbox[0];
	position[1] = pos[1] - g_pApp->m_worldBSPs[0]->m_models->bbox[1];
	position[2] = pos[2] - ( g_pApp->m_worldBSPs[0]->m_models->bbox[2]);

	MSG_WriteShort(sb, (int)position[0]);
	MSG_WriteShort(sb, (int)position[1]);
	MSG_WriteShort(sb, (int)position[2]);
}

void MSG_ReadPosition(vec3_t v)
{
	v[0] = (float)MSG_ReadShort();
	v[1] = (float)MSG_ReadShort();
	v[2] = (float)MSG_ReadShort();

	if( g_cgv.state != CLIENT_GAME_PROCESS ) return;

	v[0] += g_pApp->m_worldBSPs[0]->m_models->bbox[0];
	v[1] += g_pApp->m_worldBSPs[0]->m_models->bbox[1];
	v[2] += ( g_pApp->m_worldBSPs[0]->m_models->bbox[2]);
}

void MSG_WriteItem(i3sizebuf_t *sb, item_t *item)
{
	int i;

	MSG_WriteString(sb, item->name);
	MSG_WriteShort(sb, item->itemTableIdx);
	MSG_WriteByte(sb, item->optionNumber);

	for (i=0; i<item->optionNumber; i++)
		MSG_WriteShort(sb, item->optionIdx[i]);

	
	
	for (i=0; i<item->optionNumber; i++)
		MSG_WriteShort(sb, item->optionValue[i]);

	MSG_WriteShort(sb, item->durability);
	MSG_WriteByte(sb, item->reqLevel);
	MSG_WriteLong(sb, item->experience);
	MSG_WriteLong(sb, item->timer);

	
	MSG_WriteLong(sb, item->ItemExtendInfo.UseFlag);
	MSG_WriteLong(sb, (int)item->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType);
	MSG_WriteString(sb, item->ItemExtendInfo.ItemSpawnInfomation.data);
	MSG_WriteLong(sb, item->ItemExtendInfo.AllowUseTime);
	MSG_WriteLong(sb, item->ItemExtendInfo.CalclateUseDateToSec);
	
	
	

}

void MSG_ReadItem(item_t *item)
{
	int i;

	strcpy(item->name, MSG_ReadString());
	item->itemTableIdx = MSG_ReadShort();
	item->optionNumber = MSG_ReadByte();

	for (i=0; i<item->optionNumber; i++)
		item->optionIdx[i] = MSG_ReadShort();

	
	
	for (i=0; i<item->optionNumber; i++)
		item->optionValue[i] = MSG_ReadShort();

	item->durability = MSG_ReadShort();
	item->reqLevel = MSG_ReadByte();
	item->experience = MSG_ReadLong();
	item->timer = MSG_ReadLong();

	
	item->ItemExtendInfo.UseFlag = MSG_ReadLong();

	item->ItemExtendInfo.ItemSpawnInfomation.ItemSpawnType = 
		static_cast<ITEM_SPAWN_INFO::enumItemSpawnType>(MSG_ReadLong());

	strncpy(item->ItemExtendInfo.ItemSpawnInfomation.data, 
		MSG_ReadString(), item_t::MAX_SIZE_OF_ITEM_SPAWN_DATA);

	item->ItemExtendInfo.ItemSpawnInfomation.data[item_t::MAX_SIZE_OF_ITEM_SPAWN_DATA -1] = NULL;
	item->ItemExtendInfo.AllowUseTime	= MSG_ReadLong();	
	item->ItemExtendInfo.CalclateUseDateToSec	= MSG_ReadLong();	
	
	
}
void MSG_WriteMember(i3sizebuf_t *sb, partyMember_t *m)
{
	MSG_WriteByte(sb, m->idx);

	MSG_WriteString(sb, m->memberName);
	MSG_WriteByte(sb, m->isActive);
	MSG_WriteByte(sb, m->isLeader);

	MSG_WriteByte(sb, m->level);
	MSG_WriteByte(sb, m->gen);
	MSG_WriteByte(sb, m->serverIndex);
	MSG_WriteShort(sb, m->idxOnServer);
	MSG_WriteShort(sb, m->worldIdx);

}

void MSG_ReadMember( partyMember_t *m )
{
	m->idx = MSG_ReadByte();

	strcpy( m->memberName, MSG_ReadString() );
	m->isActive = MSG_ReadByte();
	m->isLeader = MSG_ReadByte();
	m->level = MSG_ReadByte();
	m->gen = MSG_ReadByte();
	m->serverIndex = MSG_ReadByte();
	m->idxOnServer = MSG_ReadShort();
	m->worldIdx = MSG_ReadShort();
}


void MSG_ReadAffect( affect_t *aff )
{
	aff->type = MSG_ReadByte();
	for( int i = 0; i < MAX_COUNT_AFFECT_DATA; i ++ )
	{
		aff->data[i] = MSG_ReadShort();
	}
}

void MSG_WriteAffect( i3sizebuf_t *sb, affect_t *aff )
{
	MSG_WriteByte( sb, aff->type );
	for( int i = 0; i < MAX_COUNT_AFFECT_DATA; i ++ )
	{
		MSG_WriteShort( sb, aff->data[i] );
	}
}





void GTH_Debug_PacketInfo()
{

}


int	GTH_SendMessage_UpdatePosition()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_UPDATEPOSITION);
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position);
		MSG_WriteFloat(&netMessage, g_pApp->m_myCharacter->angles[ YAW ]);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}




int	GTH_SendMessage_SystemAdimin_ShutdownServer()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMADMIN_SERVERSHUTDOWN);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

void GTH_ProcessMessage_SetShutdownTimer()
{
	int sec;

	sec = MSG_ReadShort();

	g_cgv.serverShutdownTimer.setTime = (int)g_timer.GetAppTime();
	g_cgv.serverShutdownTimer.active = true;
	g_cgv.serverShutdownTimer.countdown = sec;
	g_cgv.serverShutdownTimer.isComplete = false;
}

int	GTH_SendMessage_SystemAdimin_GoToPlayer(char *name)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMSDMIN_GOTOPLAYER);
		MSG_WriteString(&netMessage, name);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendMessage_SystemAdimin_GoToMonster(int idx)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMSDMIN_GOTOMONSTER);
		MSG_WriteShort(&netMessage, idx);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int	GTH_SendMessage_SystemAdimin_Notice(char *notice)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMSDMIN_NOTICE);
		MSG_WriteString(&netMessage, notice);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

void GTH_SendMessage_SystemAdimin_Busy()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMSDMIN_GMBUSYONOFF);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_SystemAdimin_Hide()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMSDMIN_GMHIDEONOFF);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_NoticeMessage()
{
	char notice[256];

	strcpy(notice, MSG_ReadString());
	g_ifMng->AddSysMessage( notice, GTHCOLOR_NOTICEMESSAGE );
	g_ifMng->AddNoticeMessage( notice, GTHCOLOR_NOTICEMESSAGE );
	
	
	g_ifMng->m_GmNotice = TRUE;

	
	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_WARNING_2 ), GTH_SOUND_PLAY_ONCE );
}

int	GTH_SendMessage_CallGameMaster( char *reason )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_CALLGAMEMASTER);
		
		
		
		MSG_WriteString(&netMessage, reason );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

void GTH_ProcessMessage_CallGameMaster()
{
	char fromName[NAMESTRING], reason[256];
	char msg[256];
	int worldIdx;

	strcpy(fromName, MSG_ReadString());
	strcpy(reason, MSG_ReadString());
	worldIdx = MSG_ReadShort();


	sprintf(msg, g_LPACK.GetMassage(0,187), fromName, reason, g_cgv.worldTable[worldIdx].hanName );	
	g_ifMng->AddSysMessage( msg, D3DCOLOR_ARGB( 255, 255,255,100) );

	
	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_WARNING_1 ), GTH_SOUND_PLAY_ONCE );

	g_ifMng->m_gmWin->AddCaller( fromName, reason );
}

int	GTH_SendMessage_RequestSummons(char *name, int forcedYn)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUESTSUMMONS);
		MSG_WriteString(&netMessage, name);
		MSG_WriteByte(&netMessage, forcedYn);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

int GTH_SendMessage_ReplySummons(int reply)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REPLYSUMMONS);
		MSG_WriteByte(&netMessage, reply);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
	return 1;
}

void GTH_ProcessMessage_RequestSummons()
{
	char summoner[NAMESTRING];
	char msg[256];
	int	forcedYn;

	strcpy(summoner, MSG_ReadString());
	forcedYn = MSG_ReadByte();

	
	
	if( g_ifMng->m_boothWin->State() ) return;
	
	if (forcedYn == tagPcSummonTypedef::enumPcSummonType::PC_SUMMON_FORCED)
	{
		
		sprintf(msg, g_LPACK.GetMassage(0,42), summoner);
		
		g_ifMng->RestartInterface( 1 );
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,43), msg, 1, IF_MESSAGE_SUMMONS );

	}
	
	else if (forcedYn == tagPcSummonTypedef::enumPcSummonType::PC_SUMMON_ITEM)
	{
		g_ifMng->RestartInterface( 1 );
		GTH_SendMessage_ReplySummons( 1 );
	}
	else
	{
		
		sprintf(msg, g_LPACK.GetMassage(0,44), summoner);
		
		g_ifMng->RestartInterface( 1 );
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,45), msg, 0, IF_MESSAGE_SUMMONS );
	}
}

void GTH_ProcessMessage_ReplySummons()
{
	char name[NAMESTRING];
	char msg[256];
	int reply;

	reply = MSG_ReadByte();
	strcpy(name, MSG_ReadString());

	if (reply == 1)
	{
		
		sprintf(msg, g_LPACK.GetMassage(0,188), name);		
	}
	else
	{
		
		sprintf(msg, g_LPACK.GetMassage(0,189), name);
	}

	
	g_ifMng->AddSysMessage( msg, D3DCOLOR_ARGB(255, 255, 125, 100) );	

}

int	GTH_SendMessage_RequestSummonMonster( int num )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_SYSTEMADMIN);
		MSG_WriteByte(&netMessage, SYSTEMSDMIN_REQUESTSUMMONMONSTER);
		
		MSG_WriteShort(&netMessage, g_pApp->m_myCharacter->idxOnServer);
		MSG_WriteByte(&netMessage, ENTITY_PC);
		
		MSG_WriteShort(&netMessage, g_cgv.summonMonsterIdx);
		MSG_WriteShort(&netMessage, g_cgv.summonAiType);
		
		MSG_WriteByte(&netMessage, g_pApp->m_myCharacter->worldIdx);
		MSG_WritePosition(&netMessage, g_pApp->m_myCharacter->position );
		
		MSG_WriteShort(&netMessage, (int)g_cgv.summonValidTime);
		
		MSG_WriteShort(&netMessage, num);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int	GTH_ProcessMessage_ReplySummonMonster()
{
	int reply, error;
	
	reply = MSG_ReadByte();
	error = MSG_ReadByte();

	if( !reply )
	{
		
		
		g_ifMng->AddChatMessage( g_LPACK.GetMassage(0,165), GTHCOLOR_SYSMESSAGE, g_LPACK.GetMassage(0,166) );

	}
	else
	{
		
		
		g_ifMng->AddChatMessage( g_LPACK.GetMassage(0,319), GTHCOLOR_SYSMESSAGE, g_LPACK.GetMassage(0, 166) );		
	}
	return 1;
}

int	GTH_ProcessMessage_NotFoundPC()
{
	
	
	g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,190), D3DCOLOR_ARGB(255, 255, 55, 55) );

	return 1;
}


int GTH_ProcessMessage_RejectClient()
{
	int reason;

	reason = MSG_ReadByte();

	switch ( reason )
	{
	case REJECT_OVER_MAXCLIENT :
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,384), 1, IF_MESSAGE_EXIT_CLIENT );
		
		break;
	case REJECT_VERSION_MISMATCH :
		break;
	}

	return 1;
}



void GTH_ReturnMasterServer( int restartflag )
{
	
	GTH_SendMessage_DisconnectGameServer(FALSE);
	NET_CloseSocket( &gsSocket );
	NET_CloseSocket( &msSocket );

	
	GTH_InitClientGlobalVariable();		
	
	
	g_cgv.state = CLIENT_GAME_PROCESS;
	g_cgv.restartState = restartflag;
	
	GTH_UnlockSending();

	
	g_charManager->m_iNumCharacters = 0;
	g_charManager->m_iNumBackupCharacters = 0; 

	
	GTH_Network_EstablishMasterServerConnection();
}
	
void GTH_ProcessMessage_ReplyCheatCode()
{
	int type, value;
	type = MSG_ReadByte();
	value = MSG_ReadLong();

	switch( type )
	{
	case CHEATCODE_SKILLPOINT :
		g_cgv.myCharacterInfo->bonusSkillPoint = value;
		break;
	case CHEATCODE_GETMAXSE :
		g_cgv.myCharacterInfo->curChargeSE = value;
		break;
	case CHEATCODE_GETGENCAPABILITY :
		g_cgv.myCharacterInfo->genCapability[g_cgv.myCharacterInfo->selectedGen] = value;
		break;
	case CHEATCODE_INITSKILL :
		{
			for( int i=0; i<MAX_NUMBER_OF_SKILL; i++ )
			{
				g_cgv.myCharacterInfo->skill[i].idx = -1;
				g_cgv.myCharacterInfo->skill[i].tableIdx = -1;
				g_cgv.myCharacterInfo->skill[i].iLevel = 0;
			}
			g_cgv.myCharacterInfo->skillNumber = 0;
		}
		break;
	
	case CHEATCODE_ITEMDROPTEST :
		{
			char szTemp[128];
			int itemTypeNumber = MSG_ReadShort();

			int resultItem[1000], resultNumber[1000];

			g_ifMng->AddSysMessage( "--------------------" , GTHCOLOR_NOTICEMESSAGE );
			
			for (int i=0; i<itemTypeNumber; i++)
			{
				resultItem[i] = MSG_ReadShort();
				resultNumber[i] = MSG_ReadShort();

				sprintf( szTemp, "%s : %d", g_itemTable[resultItem[i]].name, resultNumber[i] );
				g_ifMng->AddSysMessage( szTemp );
			}

			g_ifMng->AddSysMessage( "--------------------" , GTHCOLOR_NOTICEMESSAGE );

		}
		break;
		
	
	case CHEATCODE_CLEARINVEN:
		{
			
			g_cgv.myCharacterInfo->curItemWeight = value;

			for (int iInvenPos = 0; iInvenPos < MAX_INVENTORY_SIZE; iInvenPos++)
			{										
				int ItemIdx = g_cgv.myCharacterInfo->inventory[iInvenPos];
				if ( ItemIdx < 0 || ItemIdx >= MAX_NUMBER_OF_OWNITEM)
					continue;

				item_t *pTempItem = NULL;
				pTempItem = &g_cgv.myCharacterInfo->item[ItemIdx];

				GTH_DeleteMyItem( pTempItem);
				g_cgv.myCharacterInfo->inventory[iInvenPos] = -1;	
			}
			
		}
		break;	
	
	case CHEATCODE_DANBATTLE_PCPOINT:
		{			
			char szTemp[128];
			_snprintf( szTemp, 128, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2,  659 ),  value);
			g_ifMng->AddSysMessage(szTemp, GTHCOLOR_ITEMNOTICE_MESSAGE);
		}break;
	
	}
}






















void GTH_ProcessMessage_ChattingMsg()
{
	int		dlgType, lpackType, lpackIdx;

	dlgType		=	MSG_ReadByte();
	char strBuffer[MAX_PATH+1];

	if ( dlgType != 2)
	{	
		lpackType	=	MSG_ReadShort();
		lpackIdx	=	MSG_ReadShort();
	}
	else
	{
		_snprintf(strBuffer, MAX_PATH, MSG_ReadString());
		strBuffer[MAX_PATH] = NULL;		
	}

	switch(dlgType) 
	{
	case 0:
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), g_LPACK.GetMassage( lpackType, lpackIdx ) );
		break;
	case 1:
		g_ifMng->AddSysMessage( g_LPACK.GetMassage( lpackType, lpackIdx ) );
		break;
	case 2:
		g_ifMng->AddSysMessage( strBuffer );
		break;
	}	
}




void GTH_SendChatBan_Flag()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHATBAN_FLAG);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessChatBan_Answerflag()
{
	g_cgv.myCharacterInfo->m_bChatBan_Flag=true;
}

void GTH_ProcessChatBan_AnswerStart()
{
	char szTemp[256];
	int tempTime;
	tempTime=MSG_ReadByte();
	sprintf( szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 72 ), 
		tempTime);
	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 55, 55) );

	g_cgv.myCharacterInfo->m_dwLimite_ChatTime =(DWORD)tempTime;



}


void GTH_SendChatBanRequestMessage(char* pName, char* pLimiteTime)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		int LimiteTime= atoi(pLimiteTime);
		MSG_WriteByte(&netMessage, CC_REQUEST_CHATBAN_START);
		MSG_WriteString(&netMessage, pName);
		MSG_WriteByte(&netMessage, LimiteTime);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

}

void GTH_SendChatBan_Cancel()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHATBAN_CANCEL);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessChatBan_AnswerCancel()
{	

	g_cgv.myCharacterInfo->m_dwLimite_ChatTime		= 0.0f;

	g_cgv.myCharacterInfo->m_bChatBan_Flag			= false;


	g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,73), D3DCOLOR_ARGB(255, 255, 55, 55) );

}


void GTH_SendChatBan_GMCancel(char* pName)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_CHATBAN_GMCANCEL);
		MSG_WriteString(&netMessage, pName);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessChatBan_AnswerGMCancel()
{


	g_ifMng->AddSysMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,73), D3DCOLOR_ARGB(255, 255, 55, 55) );

}

void GTH_ProcessChatBan_AnswerGMStart()
{
	char szTemp[256];
	

	sprintf( szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 72 ), 
		(DWORD)MSG_ReadByte());
	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 55, 55) );
}


void GTH_SendMessage_SkinPart(const BOOL bSaveSQLDB)
{	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_SAVE_SKIN_PART );
		MSG_WriteByte(&netMessage, (char)bSaveSQLDB);
		MSG_WriteByte(&netMessage, g_charManager->m_Characters[0].skinPart[0]);
		MSG_WriteByte(&netMessage, g_charManager->m_Characters[0].skinPart[1]);

		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}





void GTH_ProcessMessage_AnswerChaosValue()
{

	char szTemp[512];
    memset(szTemp,0,512);
	int chaosType=GetChaosClass(g_pApp->m_myCharacter);
	switch(chaosType) {
	case CHAOS_PURE:
		sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 81 ),
			g_pApp->m_myCharacter->chaosPoint,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 93 ));

		break;
	case CHAOS_GENERAL:
		sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 81 ),
			g_pApp->m_myCharacter->chaosPoint,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 94 ));
	
		break;
	case CHAOS_DISORDER:
		sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 81 ),
			g_pApp->m_myCharacter->chaosPoint,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 95 ));

		break;
	case CHAOS_MONSTER:
		sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 81 ),
			g_pApp->m_myCharacter->chaosPoint,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 96 ));
	
		break;
	case CHAOS_ATROCITY:
		sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 81 ),
			g_pApp->m_myCharacter->chaosPoint,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 97 ));

		break;

	}

	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 55, 55) );
}


void GTH_ProcessMessage_SendAddChaosInfo()
{
	char szTemp[512];

	memset(szTemp,0,512);
	int chaosvalue=MSG_ReadLong();
	int totalchaosvalue=MSG_ReadLong();
	bool sendclass=MSG_ReadShort();
	bool sendspeed=MSG_ReadShort();
	
	sprintf( szTemp, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 82 ), 
		chaosvalue);
	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 55, 55) );

	
	if(sendclass==true)
	{
		char classtemp[512];
		memset(classtemp,0,512);
		int chaosType=GetChaosClass(g_pApp->m_myCharacter);
		switch(chaosType) {
		case CHAOS_PURE:
		
				sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 81 ),
			totalchaosvalue,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 93 ));
			break;
		case CHAOS_GENERAL:
		
				sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 81 ),
			totalchaosvalue,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 94 ));
			break;
		case CHAOS_DISORDER:
		
				sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 81 ),
			totalchaosvalue,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 95 ));
			break;
		case CHAOS_MONSTER:
		
				sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 81 ),
			totalchaosvalue,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 96 ));
			break; 
		case CHAOS_ATROCITY:
		
				sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 81 ),
			totalchaosvalue,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 97 ));
			break;
		}

		g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 55, 55) );
		memset(szTemp,0,512);
		
	
	
	}

	
}


void GTH_ProcessMessage_SendDecChaosInfo()
{
	char szTemp[512];
	memset(szTemp,0,512);
	int classtype=MSG_ReadShort();
	switch(classtype) {
	case CHAOS_PURE:

			sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 84 ),
			g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 93 ));
		break;
	case CHAOS_GENERAL:

			sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 84 ),
			g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 94 ));
		break;
	case CHAOS_DISORDER:

			sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 84 ),
			g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 95 ));
		break;
	case CHAOS_MONSTER:

			sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 84 ),
			g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 96 ));
		break;
	case CHAOS_ATROCITY:

			sprintf(szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 84 ),
			g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 97 ));
		break;
	}

	g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 55, 55) );
}








void GTH_SendMessage_ReloadLogic()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_RELOAD_LOGIC );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}



void GTH_Chao_point_event()
{
	int Point = MSG_ReadByte();
	char szTemp[128];
	_snprintf( szTemp, 128, g_LPACK.GetMassage( LPACK_TYPE_NORMAL2,  658 ),  Point);
	g_ifMng->AddSysMessage(szTemp, GTHCOLOR_ITEMNOTICE_MESSAGE);
}