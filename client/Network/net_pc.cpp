 #include "../global_def.h"
#include "..\GonryunBattlePractice\GonryunBattlePractice.h"
extern i3socket_t gsSocket;	
extern i3sizebuf_t netMessage; 






int GTH_SendMessage_Request_PCInfo( int type, int skillType, int curPage )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_PCINFO );
		MSG_WriteByte(&netMessage, type );
		
		switch( type )
		{
		case PCINFO_NONE :
			break;
		case PCINFO_BASIC :
			
			
			break;
		}
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Reply_PCInfo()
{
	myCharacterInfo_t *pc = g_cgv.myCharacterInfo;
	

	
	pc->calMaxRA = MSG_ReadLong();
	pc->calMaxSA = MSG_ReadShort();
	pc->curRA = MSG_ReadLong();
	pc->curSA = MSG_ReadShort();

	
	pc->calForce = MSG_ReadShort();
	pc->calSoulpower = MSG_ReadShort();
	pc->calAgility = MSG_ReadShort();
	pc->calVitality = MSG_ReadShort();
	
	
	pc->calPhyMinDmg = MSG_ReadShort();
	pc->calPhyMaxDmg = MSG_ReadShort();
	pc->calMagMinDmg = MSG_ReadShort();
	pc->calMagMaxDmg = MSG_ReadShort();

	pc->calCriticalRate = MSG_ReadFloat();

	pc->calDefensePower = MSG_ReadShort();
	pc->calPositiveRes = MSG_ReadShort();
	pc->calNegativeRes = MSG_ReadShort();
	pc->calHitPower = MSG_ReadShort();
	pc->calAvoidPower = MSG_ReadShort();
	pc->calAttackDelay = MSG_ReadShort();

	pc->calVelocity = MSG_ReadFloat();
	
	pc->calAttackLength = MSG_ReadShort();

	pc->atkPowerRACost = MSG_ReadByte();
	pc->atkPowerSACost = MSG_ReadByte();
	pc->atkBowSACost = MSG_ReadByte();

	
	pc->maxChargeRA = MSG_ReadShort();
	pc->maxChargeSA = MSG_ReadShort();
	pc->onceRecoveryRA = MSG_ReadShort();
	pc->onceRecoverySA = MSG_ReadShort();
	pc->curChargeSE = MSG_ReadLong();
	pc->curChargeRA = MSG_ReadShort();
	pc->curChargeSA = MSG_ReadShort();
	
	pc->curItemWeight = MSG_ReadShort();
	pc->calItemWeight = MSG_ReadShort();

	
	float ov_skill_length_rate = MSG_ReadFloat();

	
	pc->calDecSaUse = MSG_ReadFloat();
	return 1;
}






int GTH_SendMessage_PCTrade_RequestBatting( char*pname )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_REQUEST);
		MSG_WriteString(&netMessage, pname);
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}


int GTH_ProcessMessage_PCTrade_RequestBatting()
{

	char name[128+1]="", szTemp[256+1]="";
		
	int idxOnServer = MSG_ReadShort();
	BOOL	btype = MSG_ReadByte();
	strncpy( name, MSG_ReadString(),128);
	name[128]=NULL;
		
	if( FALSE == btype )
		{
			
			
			sprintf( szTemp, g_LPACK.GetMassage(9,194), name);
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, 0, IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING );
			g_ifMng->m_pgonryunBattleBattingwindow->SetTradeInfo( idxOnServer, name );
		}
		else
		{
			

		
#if defined _GTH_CHINESE_VERSION || _GTH_TAIWAN_VERSION
			sprintf( szTemp, g_LPACK.GetMassage(0,58), name );
#else
			sprintf( szTemp, "%s%s", name, g_LPACK.GetMassage(0,58) );
#endif		
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_CANCEL, IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING_WATEREPLAY, 10000.0f );
			g_ifMng->m_pgonryunBattleBattingwindow->SetTradeInfo( idxOnServer, name );
		}

	return 1;
}


int GTH_SendMessage_PCTrade_AdmitBatting()
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_RESULT);
		MSG_WriteShort(&netMessage, g_ifMng->m_pgonryunBattleBattingwindow->m_otherIdxOnServer );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_AdmitBatting()
{
	if( g_ifMng->m_msgType == IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING || 
		g_ifMng->m_msgType == IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING_WATEREPLAY )
	{
		g_ifMng->ExitMessage();
	}

		g_ifMng->m_pgonryunBattleBattingwindow->InitTrade();

	return 1;
}

int GTH_SendMessage_PCTrade_RejectBatting()
{
	
	

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_REJECT);
		MSG_WriteShort(&netMessage, g_ifMng->m_pgonryunBattleBattingwindow->m_otherIdxOnServer );
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_RejectBatting()
{


	char szTemp[256]="";
	
	
	


	BOOL type = (BOOL)MSG_ReadByte();
	if( TRUE == type )
		{
			
			g_cgv.saveMenuTime = 5000.0f;
		
			sprintf( szTemp, g_LPACK.GetMassage(9,134), g_ifMng->m_pgonryunBattleBattingwindow->m_otherName );
			g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 125, 100) );
		}
		else
		{
			sprintf( szTemp, g_LPACK.GetMassage(9,134), g_ifMng->m_pgonryunBattleBattingwindow->m_otherName );
			g_ifMng->AddSysMessage( szTemp, D3DCOLOR_ARGB(255, 255, 125, 100) );
		}
	

	if( g_ifMng->m_msgType == IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING || 
		g_ifMng->m_msgType == IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING_WATEREPLAY )
	{
		g_ifMng->ExitMessage();
	}

	return 1;
}
int GTH_SendMessage_PCTrade_ItemBatting( int idx, int pos )
{
	if( !GTH_LockSending() ) return 1;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_ITEM);
			
		MSG_WriteShort(&netMessage, idx );
		MSG_WriteByte(&netMessage, pos );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_ItemBatting()
{
	GTH_UnlockSending();



	item_t item;
	int isOwner = MSG_ReadByte();
	int	type = MSG_ReadByte();
	int	pos = MSG_ReadByte();
	int beforeidx = g_cgv.myCharacterInfo->m_imyTrade[pos];
		if( isOwner )
		{			
			switch( type )
			{
				
			case 0 :
			if( 0 > g_cgv.myCharacterInfo->m_imyTrade[pos]){
				g_cgv.myCharacterInfo->m_imyTrade[pos] = g_cgv.myCharacterInfo->mouseInventory;
				g_cgv.myCharacterInfo->mouseInventory = -1;
			
			} else{
				
				g_GonryunBattlePractic.SendFailItemAdd();
			}
			
				break;
				
			case 1 :
				g_cgv.myCharacterInfo->mouseInventory = g_cgv.myCharacterInfo->m_imyTrade[pos];
				g_cgv.myCharacterInfo->m_imyTrade[pos] = -1;
				break;
				
			case 2 :
			{
				int	itemIdx = g_cgv.myCharacterInfo->mouseInventory;
				g_cgv.myCharacterInfo->mouseInventory = g_cgv.myCharacterInfo->m_imyTrade[pos];
				g_cgv.myCharacterInfo->m_imyTrade[pos] = itemIdx;
			}
		
			break;
		case 3 :
			{
			
				MSG_ReadItem( &item );
				
				g_cgv.myCharacterInfo->mouseInventory = (int)MSG_ReadShort();
				GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->mouseInventory]);
				GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->m_imyTrade[pos]]);
	
				g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->m_imyTrade[pos]] = item;
				g_cgv.myCharacterInfo->mouseInventory = -1;
			
			}
				break;
			}
		}
		else
		{
			switch( type )
			{
				
			case 0 :
				MSG_ReadItem( &item );
				memcpy( &g_cgv.myCharacterInfo->m_itradeItem[pos], &item, sizeof( item_t ) );
				g_cgv.myCharacterInfo->m_iotherTrade[pos] = pos;
			
				break;
				
			case 1 :
				memset( &g_cgv.myCharacterInfo->m_itradeItem[pos], 0, sizeof( item_t ) );
				g_cgv.myCharacterInfo->m_itradeItem[pos].itemTableIdx = -1;
				g_cgv.myCharacterInfo->m_iotherTrade[pos] = -1;
				break;
				
			case 2 :
				MSG_ReadItem( &item );
				memcpy( &g_cgv.myCharacterInfo->m_itradeItem[pos], &item, sizeof( item_t ) );
				g_cgv.myCharacterInfo->m_iotherTrade[pos] = pos;
		
				break;
			}
		}
	return 1;
}

int GTH_SendMessage_PCTrade_OkBatting( int idx )
{

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_OK);
		
	
		MSG_WriteShort(&netMessage, idx );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_OkBatting()
{
	BOOL type = (BOOL)MSG_ReadByte();
	g_ifMng->m_pgonryunBattleBattingwindow->SetReady(type,TRUE);
	return 1;
}

int GTH_SendMessage_PCTrade_CancelBatting( int idx )
{

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_CANCEL);
		
	
		MSG_WriteShort(&netMessage, idx );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_CancelBatting()
{


	BOOL btype = (BOOL)MSG_ReadByte();
		if(TRUE == btype ) 
	{
			g_ifMng->m_pgonryunBattleBattingwindow->SetReady( 0, FALSE );
			g_ifMng->m_pgonryunBattleBattingwindow->SetReady( 1, FALSE );
			
		}
		else 
		{
			g_ifMng->m_pgonryunBattleBattingwindow->SetReady( 0, FALSE );
			g_ifMng->m_pgonryunBattleBattingwindow->SetReady( 1, FALSE );
	}

	return 1;
}

int GTH_SendMessage_PCTrade_ExitBatting()
{

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_EXIT);
		
		MSG_WriteShort(&netMessage, g_ifMng->m_pgonryunBattleBattingwindow->m_otherIdxOnServer );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}



int GTH_ProcessMessage_PCTrade_ExitBatting()
{
	g_ifMng->m_pgonryunBattleBattingwindow->CancelTrade();
	return 1;
}

int GTH_SendMessage_PCTrade_ConfirmBatting( int idx, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_CONFIRM);
		
		MSG_WriteShort(&netMessage, idx );
		
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_ConfirmBatting()
{
	int	type = MSG_ReadByte();
	switch( type )
	{
		
	case 0 :
		{
			for( int i = 0; i < MAX_TRADE_SIZE; i ++ )
			{
				
				g_GonryunBattlePractic.m_itradeItemtemp[i] =g_cgv.myCharacterInfo->m_itradeItem[i];
				g_GonryunBattlePractic.m_iotherTradetemp[i] =g_cgv.myCharacterInfo->m_iotherTrade[i];
				g_GonryunBattlePractic.m_imyTradetemp[i]=g_cgv.myCharacterInfo->m_imyTrade[i];
				
				
			}
		
			g_GonryunBattlePractic.m_myTradeSEtemp=g_ifMng->m_pgonryunBattleBattingwindow->m_myTradeSE;
			g_GonryunBattlePractic.m_otherTradeSEtemp=g_ifMng->m_pgonryunBattleBattingwindow->m_otherTradeSE;
			g_ifMng->m_pgonryunBattleBattingwindow->Enable(FALSE);
			g_ifMng->m_itemWin->Enable(false);
		}
		break;
		
	case 1 :
		g_ifMng->m_pgonryunBattleBattingwindow->SetConfirm( true );
		break;
		
		
	case 2 :
		g_ifMng->m_pgonryunBattleBattingwindow->ConfirmTrade();
		break;
		
		
	case 3 :
		g_ifMng->m_pgonryunBattleBattingwindow->ConfirmTradeDefeat();
		break;
	case 4:
		
		g_ifMng->m_pgonryunBattleBattingwindow->CancelTrade();
		break;
	
	case 5:			
		break;
	}

	
	CGonryunBattlePractice::enumBattingErrorCode error = static_cast<CGonryunBattlePractice::enumBattingErrorCode>(MSG_ReadByte());
	switch(error) {
	case CGonryunBattlePractice::enumBattingErrorCode::ERROR_None:
		break;
	case CGonryunBattlePractice::enumBattingErrorCode::ERROR_Weight_Over:
		if(5 == type)
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,536));  
		else g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),  g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,504));
		
		break;
	case CGonryunBattlePractice::enumBattingErrorCode::ERROR_NotEnough_InventorySpace:

		if(5 == type)
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,537));  
		else g_ifMng->SetMessage( g_LPACK.GetMassage(0,322),g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,505));
		
		break;
	}
		

	
	return 1;
}

int GTH_SendMessage_PCTrade_SEBatting( int idx, int se )
{

	
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_EXTEND );
		MSG_WriteByte(&netMessage, CC_REQUEST_GONRYUNBATTLEPRACTICE );
		MSG_WriteByte(&netMessage, BATTLE_PRACTICE_BATTINGTRADE_SE);
		
		MSG_WriteShort(&netMessage, idx );
		MSG_WriteLong(&netMessage, se );
		NET_SendMessage(&gsSocket, &netMessage); 
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_PCTrade_SEBatting()
{

		
	
	BOOL btype = MSG_ReadByte();
	long se = MSG_ReadLong();
	
	if(TRUE ==  btype ) g_ifMng->m_pgonryunBattleBattingwindow->m_myTradeSE = se;
		else g_ifMng->m_pgonryunBattleBattingwindow->m_otherTradeSE = se;
	
	CGonryunBattlePractice::enumBattingErrorCode error = static_cast<CGonryunBattlePractice::enumBattingErrorCode>(MSG_ReadByte());

	switch( error )
		{



	case CGonryunBattlePractice::ERROR_Over_BattingNake :
			
		g_ifMng->AddSysMessage(
			g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,114),
			D3DCOLOR_ARGB(255, 255, 125, 100) );
			break;
		}
	
	
	return 1;
}






int GTH_SendMessage_HotKey( int curIdx )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_HOTKEY );
		MSG_WriteByte(&netMessage, curIdx);
		
		MSG_WriteByte(&netMessage, g_cgv.myCharacterInfo->hotkeyType[curIdx] );
		MSG_WriteChar(&netMessage, g_cgv.myCharacterInfo->hotkeyIdx[curIdx] );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_SendMessage_Request_UsePortal( int invenPos )
{
	if( !GTH_LockSending() ) return false;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_USE_PORTAL );
		MSG_WriteByte(&netMessage, invenPos);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}


int GTH_ProcessMessage_Reply_UseMasicBook()
{
	GTH_UnlockSending();

	int reply, error;

	reply = MSG_ReadByte();

	if( reply )
	{
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_CONSUMPTIVE_0 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,225), g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,503));
			break;
		}
		
		return false;
	}
	return true;
}

int GTH_ProcessMessage_Reply_UsePortal()
{
	GTH_UnlockSending();

	int i;
	int reply, error;
	int idx, numCharacter;
	Fx_CHARACTER_t *character;
	int entityType;
	int worldIdx, invenPos, itemIdx, isChangeServer;
	item_t *item;
	vec3_t position;

	reply = MSG_ReadByte();

	if( reply )
	{
		
		
		if ( g_ifMng->m_guildArenaWin->m_ReadyBattleFlag == g_ifMng->m_guildArenaWin->STATE_BATTLE)
		{		
			g_ifMng->m_guildArenaInfoWin->Enable(false);
			g_ifMng->m_guildArenaWin->ClearRegTeams();
			g_ifMng->m_miniMapWin->Enable();
			
			g_ifMng->m_guildArenaWin->m_ReadyBattleFlag = g_ifMng->m_guildArenaWin->STATE_NONE;
		}
		
		
		idx = MSG_ReadShort();
		entityType = MSG_ReadByte();

		isChangeServer = MSG_ReadByte();
		
		character = &g_charManager->m_Characters[0];
		numCharacter = g_charManager->m_iNumCharacters;
		
		for (i=0; i < numCharacter; i++, character++) 
		{
			if (entityType == character->entityType && idx == character->idxOnServer)
				break;
		}
		
		if (i >= numCharacter) return 0; 
		
		
		if ( character->idxOnServer == g_cgv.myPCIdxOnServer && 
			character->entityType == ENTITY_PC )
		{
			if( !isChangeServer )
			{
				worldIdx = MSG_ReadByte();
				
				if( worldIdx != character->worldIdx )	
				{
					position[0] = (float)MSG_ReadShort();
					position[1] = (float)MSG_ReadShort();
					position[2] = (float)MSG_ReadShort();
					
					g_cgv.syncCharacter[0].worldIdx = worldIdx;
					VectorCopy(g_cgv.syncCharacter[0].position, position);
					g_cgv.syncCharacter[0].angles[YAW] =  MSG_ReadFloat();
					g_cgv.syncCharacter[0].curRA = MSG_ReadLong();
					g_cgv.syncCharacter[0].curSA = MSG_ReadShort();
					g_cgv.syncCharacter[0].event = GTH_EV_CHAR_RESPAWN;
					invenPos = MSG_ReadByte();
					
					itemIdx = g_cgv.myCharacterInfo->inventory[invenPos];
					item = &g_cgv.myCharacterInfo->item[itemIdx];
					if( item->durability > 0 )
					{
						item->durability --;
					}
					else
					{
						GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
						g_cgv.myCharacterInfo->inventory[invenPos] = -1;
					}
					character->event = GTH_EV_CHAR_IDLE;
					
					GTH_PrepareChangeWorld();
				}
				else
				{
					MSG_ReadPosition( character->position );
					character->angles[ YAW ] = MSG_ReadFloat();
					character->curRA = MSG_ReadLong();
					character->curSA = MSG_ReadShort();
					invenPos = MSG_ReadByte();
					itemIdx = g_cgv.myCharacterInfo->inventory[invenPos];
					item = &g_cgv.myCharacterInfo->item[itemIdx];
					if( item->durability > 0 )
					{
						item->durability --;
					}
					else
					{
						GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
						g_cgv.myCharacterInfo->inventory[invenPos] = -1;
					}				
					character->event = GTH_EV_CHAR_RESPAWN;
				}
			}
			else
			{
				invenPos = MSG_ReadByte();
				itemIdx = g_cgv.myCharacterInfo->inventory[invenPos];
				item = &g_cgv.myCharacterInfo->item[itemIdx];
				if( item->durability > 0 )
				{
					item->durability --;
				}
				else
				{
					GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
					g_cgv.myCharacterInfo->inventory[invenPos] = -1;
				}
			}
		}
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_CONSUMPTIVE_0 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,392) );
			break;
		case ERROR_CONSUMPTIVE_1 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,393) );
			break;
		}
	}

	return 1;
}


int GTH_SendMessage_Request_SavePortalPosition()
{
	
	if( g_SkillLogic.GTH_HaveSkill( 86 ) < 0 )
	{
		
		g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 454 ), GTHCOLOR_ERRORMESSAGE );
		return( false );
	}
	
	if( !GTH_LockSending() ) return false;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_SAVE_PORTAL_POSITION );
		
		MSG_WriteByte(&netMessage, 0);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Reply_SavePortalPosition()
{
	GTH_UnlockSending();

	int reply, error;
	int worldIdx;
	int posx, posy, posz;

	reply = MSG_ReadByte();

	if( reply )
	{
		worldIdx = MSG_ReadShort();
		posx = MSG_ReadShort();
		posy = MSG_ReadShort();
		posz = MSG_ReadShort();

		g_cgv.myCharacterInfo->skillVariable[0] = worldIdx;
		g_cgv.myCharacterInfo->skillVariable[1] = posx;
		g_cgv.myCharacterInfo->skillVariable[2] = posy;
		g_cgv.myCharacterInfo->skillVariable[3] = posz;

		
		g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 455 ) );
		return 1;
	}
	else
	{
		error = MSG_ReadByte();

		switch( error )
		{
		case ERROR_SAVEPORTALPOSITION_0 :
			
			g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 456 ), GTHCOLOR_ERRORMESSAGE );
			break;
		}
	}

	return 1;
}







void GTH_ProcessMessage_QuestStatusInfo()
{
	int i, j, type, questIdx;
	char szTemp[256];

	
	int prevQuestScrollIdx, afterQuestScrollIdx, questStatusIdx;
	
	for( i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
	{
		
		
		if ( g_cgv.myCharacterInfo->questStatus[i].questIdx == QUEST_SCROLL_INDEX )
		{
			questStatusIdx = i;
			prevQuestScrollIdx = g_cgv.myCharacterInfo->questStatus[i].dField[0];
		}

		memset( &g_cgv.myCharacterInfo->questStatus[i], 0, sizeof( questStatus_t ) );
		g_cgv.myCharacterInfo->questStatus[i].questIdx = -1;
	}

	
	
	
	type = MSG_ReadByte();
	questIdx = MSG_ReadShort();

	g_cgv.myCharacterInfo->questNumber = MSG_ReadByte();
	for( i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
	{
		g_cgv.myCharacterInfo->questStatus[i].idx = MSG_ReadByte();
		g_cgv.myCharacterInfo->questStatus[i].questIdx = MSG_ReadShort();
		g_cgv.myCharacterInfo->questStatus[i].clearTime = MSG_ReadLong();
		g_cgv.myCharacterInfo->questStatus[i].limitTime = MSG_ReadLong();
		strcpy( g_cgv.myCharacterInfo->questStatus[i].partyOrganizerName, MSG_ReadString() );
		g_cgv.myCharacterInfo->questStatus[i].partyOrganizeServer = MSG_ReadChar();
		g_cgv.myCharacterInfo->questStatus[i].partyOrganizeTime = MSG_ReadLong();
		
		for( j = 0; j < MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD; j ++ )
		{
			g_cgv.myCharacterInfo->questStatus[i].dField[j] = MSG_ReadLong();
		}

		
		
		if ( type == 1 && g_cgv.myCharacterInfo->questStatus[i].questIdx == QUEST_SCROLL_INDEX )
		{
			questStatusIdx = i;
			afterQuestScrollIdx = g_cgv.myCharacterInfo->questStatus[i].dField[0];
		}

	}

	
	
	if ( questIdx == QUEST_SCROLL_INDEX )
	{
		switch( type )
		{
		case 0 :	
			{
				sprintf( szTemp, g_LPACK.GetMassage( 0, 4 ), g_questscrollTable[prevQuestScrollIdx].name );
			}
			break;
		case 1 :	
			{
				sprintf( szTemp, g_LPACK.GetMassage( 0, 3 ), g_questscrollTable[afterQuestScrollIdx].name );
			}
			break;
		case 2:		
			{
				sprintf( szTemp, g_LPACK.GetMassage( 0, 20 ), g_questscrollTable[prevQuestScrollIdx].name );
			}
			break;
		
		case 3:		
			{
				sprintf( szTemp, g_LPACK.GetMassage( 0, 738 ), 
						g_questscrollTable[prevQuestScrollIdx].name,
						g_cgv.myCharacterInfo->questStatus[questStatusIdx].dField[2], 
						g_cgv.myCharacterInfo->questStatus[questStatusIdx].dField[3]	
						);
			}
			break;
			
		default :
			return;
		}

		g_ifMng->AddSysMessage( szTemp, GTHCOLOR_QUESTMESSAGE );

		return;
	}

	switch( type )
	{
	case 0 :	
		sprintf( szTemp, g_LPACK.GetMassage( 0, 4 ), g_LPACK.GetMassage( LPACK_TYPE_QUESTINFO, CLPack_Quest::TYPE_QUESTNAME, questIdx ) );
		break;
	case 1 :	
		sprintf( szTemp, g_LPACK.GetMassage( 0, 3 ), g_LPACK.GetMassage( LPACK_TYPE_QUESTINFO, CLPack_Quest::TYPE_QUESTNAME, questIdx ) );
		break;
	case 2 :	
		sprintf( szTemp, g_LPACK.GetMassage( 0, 20 ), g_LPACK.GetMassage( LPACK_TYPE_QUESTINFO, CLPack_Quest::TYPE_QUESTNAME, questIdx ) );
		break;

	default :
		return;
	}

	g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );

}




void GTH_ProcessMessage_AutoQuestStatusInfo()
{
	int i, j, type, questIdx, questStatusIdx;
	char szTemp[256];

	for( i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
	{
		if ( g_cgv.myCharacterInfo->questStatus[i].questIdx == AUTOQUESTSCROLL_ITEM_INDEX )
		{
			questStatusIdx = i;
			questIdx = g_cgv.myCharacterInfo->questStatus[i].dField[0];
		}

		memset( &g_cgv.myCharacterInfo->questStatus[i], 0, sizeof( questStatus_t ) );
		g_cgv.myCharacterInfo->questStatus[i].questIdx = -1;
	}

	
	
	
	type = MSG_ReadByte();
	questIdx = MSG_ReadShort();

	g_cgv.myCharacterInfo->questNumber = MSG_ReadByte();
	for( i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
	{
		g_cgv.myCharacterInfo->questStatus[i].idx = MSG_ReadByte();
		g_cgv.myCharacterInfo->questStatus[i].questIdx = MSG_ReadShort();
		g_cgv.myCharacterInfo->questStatus[i].clearTime = MSG_ReadLong();
		g_cgv.myCharacterInfo->questStatus[i].limitTime = MSG_ReadLong();




		
		for( j = 0; j < MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD; j ++ )
		{
			g_cgv.myCharacterInfo->questStatus[i].dField[j] = MSG_ReadLong();
		}

		if ( type == 1 && g_cgv.myCharacterInfo->questStatus[i].questIdx == AUTOQUESTSCROLL_ITEM_INDEX )
		{
			questStatusIdx = i;
			questIdx = g_cgv.myCharacterInfo->questStatus[i].dField[0];
		}
	}

	switch( type )
	{
	case 0 :	
		{
			sprintf( szTemp, g_LPACK.GetMassage( 0, 4 ), g_autoquestscrollTable[questIdx].name );
		}
		break;
	case 1 :	
		{
			sprintf( szTemp, g_LPACK.GetMassage( 0, 3 ), g_autoquestscrollTable[questIdx].name );
		}
		break;
	case 2:		
		{
			sprintf( szTemp, g_LPACK.GetMassage( 0, 20 ), g_autoquestscrollTable[questIdx].name );
			
			g_ifMng->m_autoQuestWin->m_questIdx=questIdx;
		}
		break;
	
	case 3:		
		{
			sprintf( szTemp, g_LPACK.GetMassage( 0, 738 ), 
					g_autoquestscrollTable[questIdx].name,
					g_cgv.myCharacterInfo->questStatus[questStatusIdx].dField[2], 
					g_cgv.myCharacterInfo->questStatus[questStatusIdx].dField[3]	
					);
		}
		break;
		
	default :
		return;
	}

	g_ifMng->AddSysMessage( szTemp, GTHCOLOR_QUESTMESSAGE );

}




void GTH_ProcessMessage_AutoQuestIsValid()
{
	int type, reply;

	type = MSG_ReadByte();
	reply = MSG_ReadByte();

	
	if(reply == 1)
	{
		for( int i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
		{
			if(g_ifMng->m_winControl[i]->State())
			{
				g_ifMng->m_winControl[i]->SetBeforeState(true);
				g_ifMng->m_winControl[i]->Enable(false);
			}		
		}

		g_ifMng->m_autoQuestWin->Enable(true);
		g_ifMng->m_autoQuestWin->m_curTick = g_timer.GetAbsoluteTime();
		g_ifMng->m_autoQuestWin->m_fAngle = 0.0f;
		g_ifMng->m_autoQuestWin->m_eType = AUTO_WIDE_START;	
		g_ifMng->m_autoQuestWin->m_eMsgtype = AUTO_SCROLL_START;
		
		bool aaa = g_ifMng->m_questscrollWin->State();
	}	
	
	
	else g_ifMng->m_autoQuestWin->m_iautoflag=-1;
}

void GTH_ProcessMessage_QuestStep()
{
	int questIdx, step, completeFlag;
	questIdx = MSG_ReadShort();
	step = MSG_ReadShort();
	completeFlag = MSG_ReadByte();
	
	g_cgv.myCharacterInfo->questVar[questIdx].var.step = step;
	g_cgv.myCharacterInfo->questVar[questIdx].var.isComplete = completeFlag;
}

void GTH_ProcessMessage_QuestItemInfo()
{
	char szTemp[256];
	int itemTableIdx, itemNumber;

	itemTableIdx = MSG_ReadShort();
	itemNumber = MSG_ReadShort();

	if( itemNumber > 0 && itemTableIdx >= 0 )
	{
		sprintf( szTemp, g_LPACK.GetMassage(0,404), g_itemTable[itemTableIdx].name );
		g_ifMng->AddSysMessage( szTemp );
	}
	else if( itemNumber < 0 && itemTableIdx >= 0 )
	{
		sprintf( szTemp, g_LPACK.GetMassage(0,405), g_itemTable[itemTableIdx].name );
		g_ifMng->AddSysMessage( szTemp );
	}

	for( int i = 0; i < MAX_COUNT_QUEST_INVENTORY; i ++ )
	{
		g_cgv.myCharacterInfo->questInventory[i].iItem = MSG_ReadLong();
	}
}

void GTH_SendMessage_DeleteQuestStatus( int questIdx )
{
	if( questIdx < 0 ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_DELETE_QUESTSTATUS );
		MSG_WriteShort(&netMessage, questIdx);
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}








void GTH_SendMessage_ItemSystem_PrecocityItem( int pos )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, PRECOCITY_ITEM );
		MSG_WriteShort(&netMessage, pos);
	
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_ItemSystem_PrecocityOpen()
{
	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_precocityWin->Enable();
	g_ifMng->m_itemWin->Enable();
	g_ifMng->SetFocus( g_ifMng->m_precocityWin );
}

void GTH_ProcessMessage_ItemSystem_PrecocityItem()
{
	GTH_UnlockSending();

	int reply, error;
	int pos;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		pos = MSG_ReadByte();
		g_cgv.myCharacterInfo->mouseInventory = MSG_ReadShort();
		g_cgv.myCharacterInfo->precocityInventory[pos] = MSG_ReadShort();
		g_cgv.myCharacterInfo->precocityTime[pos] = MSG_ReadLong();
	}
	else
	{
		error = MSG_ReadByte();
	}
}

void GTH_ProcessMessage_ItemSystem_PrecocityComplete()
{
	int pos;
	int itemTableIndex, outComeItemTableIdx;
	item_t item;
	char szTemp[256];

	GTH_InitItem( &item );

	pos = MSG_ReadByte();
	MSG_ReadItem( &item );

	itemTableIndex = g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->precocityInventory[pos]].itemTableIdx;
	outComeItemTableIdx = item.itemTableIdx;

	
	memcpy( &g_cgv.myCharacterInfo->item[g_cgv.myCharacterInfo->precocityInventory[pos]], &item, sizeof( item_t ) );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 600 ), g_itemTable[itemTableIndex].name, g_itemTable[outComeItemTableIdx].name );
	g_ifMng->AddSysMessage( szTemp );
}



void GTH_SendMessage_ItemSystem_ExchangeClose()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EXCHANGE_CLOSE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_ExchangeItem( int battleType, int levelType, int pos )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EXCHANGE_ITEM );
		MSG_WriteShort(&netMessage, battleType);	
		MSG_WriteShort(&netMessage, levelType);	
		MSG_WriteShort(&netMessage, pos);
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_ExchangePrepare()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EXCHANGE_PREPARE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_ItemSystem_ExchangeClose()
{
	GTH_UnlockSending();

	g_ifMng->m_shareWin->Enable( false );
	g_ifMng->m_shareWin->UndoShareInventory();
}

void GTH_ProcessMessage_ItemSystem_ExchangeItem()
{
	GTH_UnlockSending();
	int reply, error;
	int pos;

	reply = MSG_ReadByte();
	if( reply > 0 )
	{
		pos = MSG_ReadByte();
		g_cgv.myCharacterInfo->mouseInventory = MSG_ReadShort();
		g_cgv.myCharacterInfo->shareInventory[pos] = MSG_ReadShort();
		g_ifMng->m_shareWin->m_iShareNak = MSG_ReadLong();
	}
	else
	{
		error = MSG_ReadByte();
	}
}

void GTH_ProcessMessage_ItemSystem_ExchangePrepare()
{
	GTH_UnlockSending();

	char szTemp[256];
	int reply, error;
	int nak;
	float pro;
	int itemIdx;
	
	reply = MSG_ReadByte();

	if( reply )
	{
		nak = MSG_ReadLong();
		pro = MSG_ReadFloat();

		
		
		int goldNum = g_ifMng->m_shareWin->m_iShareNak/1000000;
		int remain = g_ifMng->m_shareWin->m_iShareNak - (goldNum*1000000);
		g_cgv.myCharacterInfo->curChargeSE += remain;
		g_ifMng->m_shareWin->m_iShareNak = 0;
		g_ifMng->m_shareWin->Enable(false);


		sprintf( szTemp, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,457), remain);
		g_ifMng->AddSysMessage(szTemp, D3DCOLOR_ARGB(255, 55, 255, 55));

		for( int i = 0; i < MAX_SHARE_INVENTORY_SIZE; i ++ )
		{
			itemIdx = g_cgv.myCharacterInfo->shareInventory[i];

			if( itemIdx < 0 ) continue;

			GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
			g_cgv.myCharacterInfo->shareInventory[i] = -1;
		}

	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{		
			
		case ERROR_ITEMSYSTEM_2 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,604) );
			break;
		case ERROR_ITEMSYSTEM_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,35) );
			break;
		}
	}
}










void GTH_SendMessage_ItemSystem_BoothSellOpen()
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_SELL_OPEN );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_BoothSellItem( int pos, int price )
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_SELL_ITEM );
		
		MSG_WriteShort(&netMessage, pos);
		MSG_WriteLong(&netMessage, price);
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_BoothSellStart( char* boothName, char* boothInfo )
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_SELL_START );
		
		MSG_WriteString(&netMessage, boothName);
		MSG_WriteString(&netMessage, boothInfo);
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_BoothSellClose()
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_SELL_CLOSE );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_BoothBuyOpen( int idxOnServer )
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_BUY_OPEN );
		
		MSG_WriteShort(&netMessage, idxOnServer );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_BoothBuyItem( int pos, int itemCount )
{	
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_BUY_ITEM );
		
		MSG_WriteShort(&netMessage, pos);
		MSG_WriteShort(&netMessage, itemCount);
		
		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_ItemSystem_BoothBuyClose()
{
	if( !GTH_LockSending() ) return;
	
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_BUY_CLOSE );
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_ItemSystem_BoothSellOpen()
{
	GTH_UnlockSending();

	int reply, error;

	reply = MSG_ReadByte();
	if( reply )
	{
		g_ifMng->m_boothWin->InitBooth( 1 );
		
		g_ifMng->m_boothWin->Enable();
		g_ifMng->m_itemWin->Enable();
		g_ifMng->SetFocus( g_ifMng->m_boothWin );
	}
	else
	{
		error = MSG_ReadByte();
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,612) );
	}
}

























void GTH_ProcessMessage_ItemSystem_BoothSellItem()
{
	GTH_UnlockSending();
	

	int reply = MSG_ReadShort();	
	if ( FALSE == reply )
	{
		int ErrorIdx = MSG_ReadLong();
		switch(ErrorIdx )
		{				
		case 0:
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,63) );	
			break;

			
		case 1 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,62) );	
			break;
		}
		
	}
	else
	{
		int pos;
		g_cgv.myCharacterInfo->mouseInventory = MSG_ReadShort();
		
		pos = MSG_ReadShort();
		g_cgv.myCharacterInfo->boothInventory[pos].itemIdx = MSG_ReadShort();
		g_cgv.myCharacterInfo->boothInventory[pos].price = MSG_ReadLong();
	}
	
}


void GTH_ProcessMessage_ItemSystem_BoothSellStart()
{
	GTH_UnlockSending();

	
	g_ifMng->m_boothWin->SetBoothState( 1 );
}

void GTH_ProcessMessage_ItemSystem_BoothSellClose()
{
	GTH_UnlockSending();
	
	int type = MSG_ReadByte();
	
	if( type == 0 ) g_ifMng->m_boothWin->CloseBooth();
	else g_ifMng->m_boothWin->SetBoothState( 0 );
}

void GTH_ProcessMessage_ItemSystem_BoothBuyOpen()
{
	GTH_UnlockSending();

	int reply, error, type;
	char buyerName[128];

	reply = MSG_ReadByte();
	if( reply )
	{
		type = MSG_ReadByte();
		
		if( !type )
		{
			int pos, itemIdx;
			
			g_ifMng->m_boothWin->InitBooth( 0 );
			g_ifMng->m_boothWin->Enable();
			g_ifMng->m_itemWin->Enable();
			g_ifMng->SetFocus( g_ifMng->m_boothWin );

			strcpy( g_ifMng->m_boothWin->m_sellerName, MSG_ReadString() );
			strcpy( g_ifMng->m_boothWin->m_boothName, MSG_ReadString() );
			strcpy( g_ifMng->m_boothWin->m_boothInfo, MSG_ReadString() );

			for( int i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
			{
				itemIdx = MSG_ReadShort();
				if( itemIdx >= 0 )
				{
					pos = MSG_ReadShort();
					MSG_ReadItem( &g_ifMng->m_boothWin->m_boothBuyItem[pos].item );
					g_ifMng->m_boothWin->m_boothBuyItem[pos].price = MSG_ReadLong();
				}
			}
		}
		else
		{
			
			strcpy( buyerName, MSG_ReadString() );
			g_ifMng->m_boothWin->AddBuyer( buyerName );

		}
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{		
			
		case ERROR_ITEMSYSTEM_6 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,613) );
			break;
			
		case ERROR_ITEMSYSTEM_4 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,614) );
			break;
			
		case ERROR_ITEMSYSTEM_5 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,615) );
			break;
		}
	}
}

void GTH_ProcessMessage_ItemSystem_BoothBuyItem()
{
	GTH_UnlockSending();

	int reply, error, type, pos, itemCount;

	reply = MSG_ReadByte();
	if( reply )
	{
		type = MSG_ReadByte();
		pos = MSG_ReadShort();
		itemCount = MSG_ReadShort();
		g_cgv.myCharacterInfo->curChargeSE = MSG_ReadLong();

		g_ifMng->m_boothWin->TradeItem( type, pos, itemCount );
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{		
		
		case ERROR_ITEMSYSTEM_6 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,613) );
			break;
		
		case ERROR_ITEMSYSTEM_10 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,616) );
			break;
		
		case ERROR_ITEMSYSTEM_8 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,617) );
			break;
		
		case ERROR_ITEMSYSTEM_7 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,36) );
			break;
		
		case ERROR_ITEMSYSTEM_9 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,327) );
			break;
		
		case ERROR_ITEMSYSTEM_11 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,35) );
			break;
		
		case ERROR_ITEMSYSTEM_4 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,614) );
			break;
		}
	}
}

void GTH_ProcessMessage_ItemSystem_BoothBuyClose()
{
	GTH_UnlockSending();

	int reply, error, type;
	char buyerName[128];

	reply = MSG_ReadByte();
	
	if( reply )
	{
		type = MSG_ReadByte();
		g_ifMng->m_boothWin->ExitBooth( type );

		if( type ) 
		{
			strcpy( buyerName, MSG_ReadString() );
			g_ifMng->m_boothWin->RemoveBuyer( buyerName );
		}
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{		
		case ERROR_ITEMSYSTEM_6 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,613) );
			break;
		}
	}
}







void GTH_ProcessMessage_ItemSystem_BoothBuyUpdate()
{
	int pos, itemIdx;
	
	g_ifMng->m_boothWin->InitBooth( 0 );
	for( int i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
	{
		itemIdx = MSG_ReadShort();
		if( itemIdx >= 0 )
		{
			pos = MSG_ReadShort();
			MSG_ReadItem( &g_ifMng->m_boothWin->m_boothBuyItem[pos].item );
			g_ifMng->m_boothWin->m_boothBuyItem[pos].price = MSG_ReadLong();
		}
	}
}


void GTH_SendMessage_QuestScroll_ObtainClose()
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, QUESTSCROLL_OBTAINCLOSE );
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_QuestScroll_ObtainPrepare( int depart, int level, int reward )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, QUESTSCROLL_OBTAINPREPARE );
		
		MSG_WriteByte(&netMessage, depart);
		MSG_WriteByte(&netMessage, level);
		MSG_WriteByte(&netMessage, reward);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_SendMessage_QuestScroll_ComboListChange( int depart )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, QUESTSCROLL_COMBOLISTCHANGE );
		
		MSG_WriteByte(&netMessage, depart);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_QuestScroll_ComboListChange()
{
	GTH_UnlockSending();

	int lvlList[3];
	char szTemp[16];

	lvlList[0] = MSG_ReadByte();
	lvlList[1] = MSG_ReadByte();
	lvlList[2] = MSG_ReadByte();

	
	g_ifMng->m_questscrollWin->m_comboLevel->ClearString();

	for ( int i=0; i<3; i++ )
	{
		if ( lvlList[i] > 0 )
		{
			sprintf( szTemp, "%d", lvlList[i] );
			g_ifMng->m_questscrollWin->m_comboLevel->AddString(szTemp);
		}
	}

	g_ifMng->m_questscrollWin->m_comboLevel->SetCurSel( 0 );

}

void GTH_SendMessage_QuestScroll_ObtainComplete( int depart, int level, int reward, int qsTableIdx )
{
	if( !GTH_LockSending() ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, QUESTSCROLL_OBTAINCOMPLETE );
		
		MSG_WriteByte(&netMessage, depart);
		MSG_WriteByte(&netMessage, level);
		MSG_WriteByte(&netMessage, reward);
		MSG_WriteShort(&netMessage, qsTableIdx);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_QuestScroll_ObtainOpen()
{
	int i;
	char szTemp[16];
	int lvlList[3], dptList[3];
	char szMessage[256];

	
	int openFlag = MSG_ReadByte();

	if ( openFlag == 0 )
	{
		
		sprintf( szMessage, "&c8%s&n&n&c0%s", g_monsterTable[125].name, g_LPACK.GetMassage( LPACK_TYPE_NPC,423 ) );
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,483), szMessage );
		return;
	}

	if ( openFlag == 2 )
	{
		
		sprintf( szMessage, "&c8%s&n&n&c0%s", g_monsterTable[125].name, g_LPACK.GetMassage( LPACK_TYPE_NPC,424 ) );
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,483), szMessage );
		return;
	}

	lvlList[0] = MSG_ReadByte();
	lvlList[1] = MSG_ReadByte();
	lvlList[2] = MSG_ReadByte();

	dptList[0] = MSG_ReadByte();
	dptList[1] = MSG_ReadByte();
	dptList[2] = MSG_ReadByte();

	
	g_ifMng->m_questscrollWin->m_comboDepart->ClearString();

	for ( i=0; i<3; i++ )
	{
		g_ifMng->m_questscrollWin->m_dptList[i] = dptList[i];

		if ( dptList[i] == 1 )
			g_ifMng->m_questscrollWin->m_comboDepart->AddString( g_LPACK.GetMassage( 0, 748 + i ) );

	}

	g_ifMng->m_questscrollWin->m_comboDepart->Enable( true );
	g_ifMng->m_questscrollWin->m_comboDepart->SetCurSel( 0 );

	
	g_ifMng->m_questscrollWin->m_comboLevel->ClearString();

	for ( i=0; i<3; i++ )
	{
		if ( lvlList[i] > 0 )
		{
			sprintf( szTemp, "%d", lvlList[i] );
			g_ifMng->m_questscrollWin->m_comboLevel->AddString(szTemp);
		}
	}

	g_ifMng->m_questscrollWin->m_comboLevel->Enable( true );
	g_ifMng->m_questscrollWin->m_comboLevel->SetCurSel( 0 );

	
	g_ifMng->m_questscrollWin->m_comboReward->ClearString();
	g_ifMng->m_questscrollWin->m_comboReward->AddString( g_LPACK.GetMassage( 0, 766 ) );
	g_ifMng->m_questscrollWin->m_comboReward->AddString( g_LPACK.GetMassage( 0, 767 ) );
	g_ifMng->m_questscrollWin->m_comboReward->AddString( g_LPACK.GetMassage( 0, 768 ) );
	g_ifMng->m_questscrollWin->m_comboReward->AddString( g_LPACK.GetMassage( 0, 769 ) );
	g_ifMng->m_questscrollWin->m_comboReward->AddString( g_LPACK.GetMassage( 0, 770 ) );
	g_ifMng->m_questscrollWin->m_comboReward->Enable( true );
	g_ifMng->m_questscrollWin->m_comboReward->SetCurSel( 0 );

	
	g_ifMng->m_questscrollWin->InitWriting();
	g_ifMng->m_chatWin->Enable( false );
	g_ifMng->m_questscrollWin->Enable();
	g_ifMng->SetFocus( g_ifMng->m_questscrollWin );
}





void GTH_ProcessMessage_AutoQuestScroll_ObtainOpen()
{
	Fx_CHARACTER_t *character = &g_charManager->m_Characters[0];

	int	tableIdx = MSG_ReadByte();
	g_ifMng->m_autoQuestWin->m_questIdx = tableIdx;

	for(int i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
	{
		if(g_ifMng->m_winControl[i]->State())
		{
			g_ifMng->m_winControl[i]->SetBeforeState(true);
			g_ifMng->m_winControl[i]->Enable(false);
		}	
	}

	g_ifMng->m_autoQuestWin->Enable(true);
	g_ifMng->m_autoQuestWin->m_curTick = g_timer.GetAbsoluteTime();
	g_ifMng->m_autoQuestWin->m_fAngle = 0.0f;
	g_ifMng->m_autoQuestWin->m_eMsgtype = AUTO_SCROLL_ORDER;	

	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;	
	myChar->angles[YAW] += 180;
	VectorCopy( g_camera.m_angles, myChar->angles );
	g_camera.m_angles[YAW] = myChar->angles[YAW] + 90.0f;
	g_camera.m_distance = -185;
}





void GTH_SendMessage_AutoQuestScroll_Progress(int msgProgress)
{


	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_DELETE_AUTOEFFECT );
		MSG_WriteByte(&netMessage, msgProgress );
			
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );
}

void GTH_ProcessMessage_QuestScroll_ObtainClose()
{
	GTH_UnlockSending();

	g_ifMng->m_questscrollWin->InitWriting();
	g_ifMng->m_questscrollWin->Enable( false );
}

void GTH_ProcessMessage_QuestScroll_ObtainPrepare()
{
	GTH_UnlockSending();

	char szTemp[256], szNak[32];
	int reply, error;
	int tableIdx, nak;
	
	reply = MSG_ReadByte();

	if( reply )
	{
		tableIdx = MSG_ReadShort();
		nak = MSG_ReadLong();

		strcpy( szNak, g_ifMng->GetNumberString( nak ) );
		sprintf( szTemp, g_LPACK.GetMassage( 0,756 ), g_questscrollTable[tableIdx].name, szNak );
		
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_QUESTSCROLL_OBTAIN_PREPARE );
		g_ifMng->m_questscrollWin->m_okBtn->SetButtonState( IF_BTN_TYPE_DISABLE );
		g_ifMng->m_questscrollWin->m_selectedQSTableIdx = tableIdx;
	}
	else
	{
		error = MSG_ReadByte();
		tableIdx = MSG_ReadShort();
		nak = MSG_ReadLong();

		strcpy( szNak, g_ifMng->GetNumberString( nak ) );

		switch( error )
		{		
			
		case ERROR_ITEMSYSTEM_1 :
			sprintf( szTemp, g_LPACK.GetMassage(0,853), g_questscrollTable[tableIdx].name, szNak );
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp );
			break;
			
		case ERROR_ITEMSYSTEM_2 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,854) );
			break;
		case ERROR_ITEMSYSTEM_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,35) );
			break;
		case ERROR_ITEMSYSTEM_18 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,758) );
			break;
		case ERROR_ITEMSYSTEM_19 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,759) );
			break;
		case ERROR_ITEMSYSTEM_20 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,754) );
			break;

		}
	}
	
}

void GTH_ProcessMessage_QuestScroll_ObtainComplete()
{
	GTH_UnlockSending();

	char szTemp[256], szNak[32];
	int tableIdx, reply, error, nak;
	
	reply = MSG_ReadByte();

	if( reply )
	{
		tableIdx = MSG_ReadShort();
		g_cgv.myCharacterInfo->curChargeSE = MSG_ReadLong();

		sprintf( szTemp, g_LPACK.GetMassage(0,765), g_questscrollTable[tableIdx].name );
		g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OK, IF_MESSAGE_QUESTSCROLL_OBTAIN_COMPLETE );

	}
	else
	{
		error = MSG_ReadByte();
		tableIdx = MSG_ReadShort();
		nak = MSG_ReadLong();

		strcpy( szNak, g_ifMng->GetNumberString( nak ) );

		switch( error )
		{		
			
		case ERROR_ITEMSYSTEM_1 :
			sprintf( szTemp, g_LPACK.GetMassage(0,853), g_questscrollTable[tableIdx].name, szNak );
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp );
			break;
			
		case ERROR_ITEMSYSTEM_2 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,854) );
			break;
		case ERROR_ITEMSYSTEM_3 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,35) );
			break;
		case ERROR_ITEMSYSTEM_18 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,758) );
			break;
		case ERROR_ITEMSYSTEM_19 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,759) );
			break;
		case ERROR_ITEMSYSTEM_20 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,754) );
			break;

		}
	}

	g_ifMng->m_questscrollWin->m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
}




void GTH_ProcessMessage_QuestScrollComplete()
{
	int type, reward, itemNumber, rankPoint, nextScroll;
	char szTemp[256], szType[128];
	type = MSG_ReadByte();
	reward = MSG_ReadLong();
	itemNumber = MSG_ReadShort();
	rankPoint = MSG_ReadShort();
	nextScroll = MSG_ReadByte();

	int addRankPoint = 	rankPoint - g_cgv.myCharacterInfo->rankPoint;
	g_cgv.myCharacterInfo->rankPoint = rankPoint;

	switch( type )
	{
	case QS_REWARD_GENCAP :	
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_GENCAP, reward, 0, szType );
		break;

	case QS_REWARD_NAK :	
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_NAK, reward, 0, szType );
		break;
		
	case QS_REWARD_EXP :	
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_EXP, reward, 0, szType );
		break;

	case QS_REWARD_GENEXP :	
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_GENEXP, reward, 0, szType );
		break;

	case QS_REWARD_ITEM :	
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_ITEM, reward, itemNumber, szType );
		break;
	}

	sprintf( szTemp, g_LPACK.GetMassage( 0, 739 ), szType );
	g_ifMng->AddSysMessage( szTemp, GTHCOLOR_LOOTMESSAGE );

	
	sprintf( szTemp, g_LPACK.GetMessage(9,22), addRankPoint );
	g_ifMng->AddSysMessage( szTemp, GTHCOLOR_LOOTMESSAGE );

	g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
								133, 
								g_pApp->m_myCharacter->position,
								NULL, 
								true, 
								g_pApp->m_myCharacter->entityType, 
								g_pApp->m_myCharacter->idxOnServer,
								0.0f, 
								0.0f, 
								0.0f, 
								-1.0f,
								false );

	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_QUESTSCROLL_DROP ), GTH_SOUND_PLAY_ONCE );

	
	if ( nextScroll )
	{
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,847), GTHCOLOR_QUESTMESSAGE );
	}

}




void GTH_ProcessMessage_AutoQuestScrollComplete()
{
	int questIdx;
	char szTemp[256], szType[128];
	questIdx = MSG_ReadByte();	

	int exp = g_autoquestscrollTable[questIdx].data.reward.exp;
	if(exp == -1)
		exp = 0;
	else
	{
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_EXP, exp, 0, szType );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 739 ), szType );
		g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
	}

	int genExp = g_autoquestscrollTable[questIdx].data.reward.genExp;
	if(genExp == -1)
		genExp = 0;
	else
	{
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_GENEXP, genExp, 0, szType );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 739 ), szType );
		g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
	}

	int genCap = g_autoquestscrollTable[questIdx].data.reward.genCap;
	if(genCap == -1)
		genCap = 0;
	else
	{
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_GENCAP, genCap, 0, szType );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 739 ), szType );
		g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
	}

	
	int fame = g_autoquestscrollTable[questIdx].data.reward.fame;
	if(fame == -1)
		fame = 0;
	else{
		g_cgv.myCharacterInfo->rankPoint += fame;
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_FAME, fame, 0, szType );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 739 ), szType );
		g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
		
	}
	


	int nak = g_autoquestscrollTable[questIdx].data.reward.nak;
	if(nak == -1)
		nak = 0;
	else
	{
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_NAK, nak, 0, szType );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 739 ), szType );
		g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
	}

	int itemIdx = g_autoquestscrollTable[questIdx].data.reward.itemIdx;
	int itemNo = g_autoquestscrollTable[questIdx].data.reward.itemNo;

	if(itemIdx != -1)
	{
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_ITEM, itemIdx, itemNo, szType );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 739 ), szType );
		g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
	}				
	
	
	if(g_autoquestscrollTable[questIdx].data.reward.regItemIndex0 !=-1)
	{
		int itemindex=g_autoquestscrollTable[questIdx].data.reward.regItemIndex0;
		int itemcount=g_autoquestscrollTable[questIdx].data.reward.regItemNo0;
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_ITEM, itemindex, itemcount, szType );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 739 ), szType );
		g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
	}
	
	if(g_autoquestscrollTable[questIdx].data.reward.regItemIndex1 !=-1)
	{
		int itemindex=g_autoquestscrollTable[questIdx].data.reward.regItemIndex1;
		int itemcount=g_autoquestscrollTable[questIdx].data.reward.regItemNo1;
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_ITEM, itemindex, itemcount, szType );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 739 ), szType );
		g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
	}
	
	if(g_autoquestscrollTable[questIdx].data.reward.regItemIndex2 !=-1)
	{
		int itemindex=g_autoquestscrollTable[questIdx].data.reward.regItemIndex2;
		int itemcount=g_autoquestscrollTable[questIdx].data.reward.regItemNo2;
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_ITEM, itemindex, itemcount, szType );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 739 ), szType );
		g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
	}
	
	if(g_autoquestscrollTable[questIdx].data.reward.regItemIndex3 !=-1)
	{
		int itemindex=g_autoquestscrollTable[questIdx].data.reward.regItemIndex3;
		int itemcount=g_autoquestscrollTable[questIdx].data.reward.regItemNo3;
		g_ifMng->m_questWin->MakeRewardString( QS_REWARD_ITEM, itemindex, itemcount, szType );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 739 ), szType );
		g_ifMng->AddSysMessage( szTemp, GTHCOLOR_NOTICEMESSAGE );
	}
	
	

	int nextScroll = g_autoquestscrollTable[questIdx].data.reward.nextScroll;

	for( int i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
	{
		if(g_ifMng->m_winControl[i]->State())
		{
			g_ifMng->m_winControl[i]->SetBeforeState(true);
			g_ifMng->m_winControl[i]->Enable(false);
		}		
	}

	g_ifMng->m_autoQuestWin->Enable(true);
	g_ifMng->m_autoQuestWin->m_curTick = g_timer.GetAbsoluteTime();
	g_ifMng->m_autoQuestWin->m_fAngle = 0.0f;
	g_ifMng->m_autoQuestWin->m_eMsgtype = AUTO_SCROLL_COMPLETE;

	Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;	
	myChar->angles[YAW] += 180;
	VectorCopy( g_camera.m_angles, myChar->angles );
	g_camera.m_angles[YAW] = myChar->angles[YAW] + 90.0f;
	g_camera.m_distance = -185;
	

	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_QUESTSCROLL_DROP ), GTH_SOUND_PLAY_ONCE );

	if ( nextScroll != -1)
	{
		g_ifMng->AddSysMessage( g_LPACK.GetMassage(0,847), GTHCOLOR_QUESTMESSAGE );
	}

}






int GTH_SendMessage_Request_UsePotion(int invenPos)
{
	MSG_BeginWriting( &netMessage);	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte	( &netMessage, CC_REQUEST_USE_POTION);
		MSG_WriteByte	( &netMessage, invenPos);
		
		MSG_WriteShort(&netMessage, g_cgv.saveTargetIdx);	
		MSG_WriteByte(&netMessage, g_cgv.saveTargetType);

		
		NET_SendMessage(&gsSocket, &netMessage, TRUE);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}








int	GTH_ProcessMessage_Reply_UsePotion()
{	
	int			invenPos;	
	int			itemIdx;
	int			error;

	item_t		*item	= NULL;
	invenPos	= MSG_ReadByte();
	error		= MSG_ReadByte();

	if(error != 0)
	{
		switch(error)
		{
		
		case ERROR_CONSUMPTIVE_0:
			g_ifMng->SetMessage(
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,491));
			break;

		
		case ERROR_CONSUMPTIVE_NOT_USE_NPC:		
			g_ifMng->SetMessage(
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,492));
			break;

		
		case ERROR_CONSUMPTIVE_NOT_SKILL_ITEM:
			g_ifMng->SetMessage(
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,493));
			break;

		
		case ERROR_CONSUMPTIVE_NOT_LEVEL:		
			g_ifMng->SetMessage(
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,494));

			break;

		
		case ERROR_CONSUMPTIVE_NOT_HAVE_ITEM:	
			g_ifMng->SetMessage(
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,495));
			break;

		
		case ERROR_CONSUMPTIVE_NULL_TARGET_PLAYER :
			g_ifMng->SetMessage(
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,496));
			break;

		
		case ERROR_CONSUMPTIVE_ALIVE_TARGET_PLAYER:	
			g_ifMng->SetMessage(
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,497));
			break;

		
		case ERROR_CONSUMPTIVE_ONLY_USE_SELF:
			g_ifMng->SetMessage(
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,498));
			break;

		
		case ERROR_CONSUMPTIVE_ONLY_USE_OTHER:
			g_ifMng->SetMessage(
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,499));
			break;

		
		case ERROR_CONSUMPTIVE_ONLY_USE_PC:
			g_ifMng->SetMessage(
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL,322), 
				g_LPACK.GetMessage(LPACK_TYPE_NORMAL2,500));

			break;

		}

		return 0;
	}

	if ( invenPos < 0 )		return 0;

	itemIdx		= g_cgv.myCharacterInfo->inventory[invenPos];
	if ( itemIdx < 0 )		return 0;

	item		= &g_cgv.myCharacterInfo->item[itemIdx];
	if ( item == NULL)		return 0;

	
	if( item->durability > 0 )	
	{
		item->durability --;	
	}
	
	else	
	{			
		GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
		g_cgv.myCharacterInfo->inventory[invenPos] = -1;

	}

	
	
	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_PC_RECOVERY_SA ), GTH_SOUND_PLAY_ONCE );
	return 1;

	
}





void GTH_SendMessage_Request_ChaosValue()
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, CC_REQUEST_CHAOVALUE );
		NET_SendMessage( &gsSocket, &netMessage );
	}
	MSG_EndWriting( &netMessage );

}







int GTH_SendMessage_Request_MasicBook(int invenPos)
{
	MSG_BeginWriting( &netMessage);	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte	( &netMessage, CC_REQUEST_USE_MASIC_BOOK);
		MSG_WriteByte	( &netMessage, invenPos);
		NET_SendMessage	( &gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}






void GTH_SendMessage_Request_EpackChargSoul(int invenPos)
{
	MSG_BeginWriting( &netMessage);	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte	( &netMessage, CC_EXTEND);
		MSG_WriteByte	( &netMessage, CC_REQUEST_USE_EPACK_CHARG_SOUL);
		MSG_WriteByte	( &netMessage, invenPos);
		NET_SendMessage	( &gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

}




