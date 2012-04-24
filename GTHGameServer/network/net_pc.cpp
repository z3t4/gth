#include "../global.h"

extern i3sizebuf_t netMessage; 
extern i3socket_t localSocket;

void GTH_SendMessage_CompleteEffect_To_AllPC( playerCharacter_t *pc, int effectIdx )
{
	
	GTH_GetEntityListInValidRange(pc->worldIdx, pc->position);

	if (g_zoneEntityListNumber < 1) 
	{
		g_logSystem->Write("Can Not Found User from Zone.(name: %s, zoneNo: %d, pos(%f, %f, %f))", pc->name, pc->zoneIdx, pc->position[0], pc->position[1], pc->position[2]);
		GTH_Zone_UpdateCurrentZone(ENTITY_PC, pc->idx, pc->worldIdx, pc->zoneIdx, pc->position);
		return;
	}

	
	for ( int i=0; i < g_zonePCListNumber; i++)
	{
		int idx = g_zonePCList[i].idx;

		if( g_pc[idx].entityType != ENTITY_PC || idx == pc->idx )
			continue;

		MSG_BeginWriting( &netMessage );
		MSG_Clear( &netMessage );
		{
			MSG_WriteByte( &netMessage, GSC_EXTEND );
			MSG_WriteByte( &netMessage, GSC_SUB_EVENT_COMPLETE_EFFECT );
			
			MSG_WriteShort( &netMessage, pc->idx );
			MSG_WriteShort( &netMessage, pc->entityType );
			MSG_WriteShort( &netMessage, effectIdx );
			
			NET_SendUnreliableMessage( &g_pc[idx].sock, &netMessage );
		}
		MSG_EndWriting(&netMessage);
	}

}

void GTH_SendMessage_PCTrade_Request( playerCharacter_t* pc, int reply, int error, int targetIdx, int type  )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PCTRADE);
		MSG_WriteByte(&netMessage, PCTRADE_REQUEST);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteShort(&netMessage, targetIdx);
			MSG_WriteByte(&netMessage, type);
			MSG_WriteString(&netMessage, g_pc[targetIdx].name );
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}
 
void GTH_SendMessage_PCTrade_Admit( playerCharacter_t* pc, int reply, int error )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PCTRADE);
		MSG_WriteByte(&netMessage, PCTRADE_ADMIT);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PCTrade_Reject( playerCharacter_t* pc, int reply, int error, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PCTRADE);
		MSG_WriteByte(&netMessage, PCTRADE_REJECT);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, type);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PCTrade_Item( playerCharacter_t* pc, int reply, int error, int isOwner, int type, int pos, item_t *item )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PCTRADE);
		MSG_WriteByte(&netMessage, PCTRADE_ITEM);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, isOwner);
			MSG_WriteByte(&netMessage, type);
			MSG_WriteByte(&netMessage, pos);
			MSG_WriteItem(&netMessage, item);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}

		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PCTrade_Exit( playerCharacter_t* pc, int reply, int error )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PCTRADE);
		MSG_WriteByte(&netMessage, PCTRADE_EXIT);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PCTrade_Ok( playerCharacter_t* pc, int reply, int error, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PCTRADE);
		MSG_WriteByte(&netMessage, PCTRADE_OK);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, type);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PCTrade_Cancel( playerCharacter_t* pc, int reply, int error, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PCTRADE);
		MSG_WriteByte(&netMessage, PCTRADE_CANCEL);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, type);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PCTrade_Confirm( playerCharacter_t* pc, int reply, int error, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PCTRADE);
		MSG_WriteByte(&netMessage, PCTRADE_CONFIRM);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, type);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		if( reply == 0 )
		{
			GTH_UndoPCTrade( pc, 1 );
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_PCTrade_SE( playerCharacter_t *pc, int reply, int error, int type, int se )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PCTRADE);
		MSG_WriteByte(&netMessage, PCTRADE_SE);
		
		MSG_WriteByte(&netMessage, reply);
		if( reply )
		{
			MSG_WriteByte(&netMessage, type);
			MSG_WriteLong(&netMessage, se);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}




void GTH_SendMessage_Party_Error( playerCharacter_t* pc )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY );
		MSG_WriteByte(&netMessage, PARTY_ERROR );
		
		MSG_WriteByte(&netMessage, pc->serverPartyErrorFlag );
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Party_Request_Organize( playerCharacter_t* pc, int reply, int error, int idx, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		MSG_WriteByte(&netMessage, PARTY_REQUEST_ORGANIZE);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteShort(&netMessage, idx);
			MSG_WriteByte(&netMessage, type);
			MSG_WriteString(&netMessage, g_pc[idx].name );
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Party_Admit_Organize( playerCharacter_t* pc, int reply, int error, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		MSG_WriteByte(&netMessage, PARTY_ADMIT_ORGANIZE);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, type);
			MSG_WriteString(&netMessage, pc->organizerName);
			MSG_WriteChar(&netMessage, pc->organizeServer);
			MSG_WriteLong(&netMessage, pc->organizeTime);
			
			MSG_WriteByte(&netMessage, pc->memberNumber);
			MSG_WriteByte(&netMessage, pc->_partyRandomInfo_t.itemDivideType);
			MSG_WriteByte(&netMessage, pc->canJoin);
			
			MSG_WriteShort(&netMessage, pc->partyIndex);
			MSG_WriteByte(&netMessage, pc->isLeader);
			
			MSG_WriteMember(&netMessage, &pc->member[0] );
			MSG_WriteMember(&netMessage, &pc->member[1] );
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Party_Reject_Organize( playerCharacter_t* pc, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		MSG_WriteByte(&netMessage, PARTY_REJECT_ORGANIZE);
		
		MSG_WriteByte(&netMessage, type);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Party_Request_Join( playerCharacter_t* pc, int reply, int error, int idx, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		MSG_WriteByte(&netMessage, PARTY_REQUEST_JOIN);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteShort(&netMessage, idx);
			MSG_WriteByte(&netMessage, type);
			
			MSG_WriteString(&netMessage, g_pc[idx].name );
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Party_Admit_Join( playerCharacter_t* pc, int reply, int error, int isNewMember )
{
	int i;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		MSG_WriteByte(&netMessage, PARTY_ADMIT_JOIN);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, isNewMember);
			
			MSG_WriteString( &netMessage, pc->organizerName );
			MSG_WriteChar( &netMessage, pc->organizeServer );
			MSG_WriteLong( &netMessage, pc->organizeTime );
			MSG_WriteByte( &netMessage, pc->memberNumber );
			MSG_WriteByte( &netMessage, pc->_partyRandomInfo_t.itemDivideType );
			MSG_WriteByte( &netMessage, pc->canJoin );
			MSG_WriteShort( &netMessage, pc->partyIndex );
			MSG_WriteByte( &netMessage, pc->isLeader );
			
			for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
			{
				if( pc->member[i].serverIndex == -1 ) continue;
				MSG_WriteMember( &netMessage, &pc->member[i] );
			}
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Party_Reject_Join( playerCharacter_t* pc, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		MSG_WriteByte(&netMessage, PARTY_REJECT_JOIN);
		
		MSG_WriteByte(&netMessage, type);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Party_Out( playerCharacter_t* pc, int reply, int error, int isForced, int outMemberIdx, int newLeaderIdx, int reqFlag )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		
		if( isForced )
			MSG_WriteByte(&netMessage, PARTY_FORCEDOUT );
		else 
			MSG_WriteByte(&netMessage, PARTY_OUT );
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, reqFlag);
			
			if( !reqFlag )
			{
				MSG_WriteByte(&netMessage, outMemberIdx);
				MSG_WriteByte(&netMessage, newLeaderIdx);
			}
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Party_Disorganize( playerCharacter_t *pc )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		MSG_WriteByte(&netMessage, PARTY_DISORGANIZE );
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void GTH_SendMessage_Party_SendAll( playerCharacter_t *pc, int reply )
{
	int i;
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		MSG_WriteByte(&netMessage, PARTY_SENDALL);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply )
		{
			MSG_WriteString( &netMessage, pc->organizerName );
			MSG_WriteChar( &netMessage, pc->organizeServer );
			MSG_WriteLong( &netMessage, pc->organizeTime );
			MSG_WriteByte( &netMessage, pc->memberNumber );
			MSG_WriteByte( &netMessage, pc->_partyRandomInfo_t.itemDivideType );
			MSG_WriteByte( &netMessage, pc->canJoin );
			MSG_WriteShort( &netMessage, pc->partyIndex );
			MSG_WriteByte( &netMessage, pc->isLeader );
			
			for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
			{
				if( pc->member[i].serverIndex == -1 ) continue;
				MSG_WriteMember( &netMessage, &pc->member[i] );
			}
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Party_Update( playerCharacter_t *pc, int memberIdx, int newLeaderIdx, int active, int pcIdx, int level, int worldIdx )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		
		MSG_WriteByte(&netMessage, PARTY_UPDATE );
		
		MSG_WriteByte(&netMessage, memberIdx);
		MSG_WriteByte(&netMessage, newLeaderIdx);
		MSG_WriteByte(&netMessage, active);
		MSG_WriteByte(&netMessage, level);
		MSG_WriteShort(&netMessage, worldIdx );
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Party_CanJoin( playerCharacter_t *pc, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		MSG_WriteByte(&netMessage, PARTY_CANJOIN );
		
		MSG_WriteByte(&netMessage, type);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Party_ItemDivideType( playerCharacter_t *pc, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		MSG_WriteByte(&netMessage, PARTY_ITEMDIVIDETYPE );
		
		MSG_WriteByte(&netMessage, type);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Party_ChatMessage( playerCharacter_t *pc, char* fromName, char *message )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PARTY);
		MSG_WriteByte(&netMessage, PARTY_CHATMESSAGE );
		
		MSG_WriteString(&netMessage, fromName);
		MSG_WriteString(&netMessage, message);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}





void GTH_SendMessage_Reply_SkillLevelUp( playerCharacter_t *pc, int reply, int error, int skillIdx, int genType )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_SKILLLEVELUP);
		
		MSG_WriteByte(&netMessage, reply );
		
		if( !reply )
		{
			MSG_WriteByte(&netMessage, error );
			NET_SendMessage(&pc->sock, &netMessage);
		}
		else
		{
			MSG_WriteByte(&netMessage, skillIdx );
			MSG_WriteLong(&netMessage, pc->skill[skillIdx].iLevel );
			MSG_WriteByte(&netMessage, genType );
			MSG_WriteLong(&netMessage, pc->genCapability[genType] );
			
			CSkilTable *pSkillTable = g_SkillTableDB.FindDB(pc->skill[skillIdx].tableIdx);
			if(pSkillTable!=NULL)
			{
				g_logSystem->Write( "Skill Level Up : %s(%s),Skill Table Index %d .. Level %d(GenType %d) Level UP", 
					pc->name, pc->userID,pc->skill[skillIdx].tableIdx, pc->skill[skillIdx].level.gen[genType], genType );
			}
			else
			{
				g_logSystem->Write( "Skill Level Up : %s(%s), %s Level %d(GenType %d) Level UP", 
					pc->name, pc->userID,"Error!pSkillTable==NULL", pc->skill[skillIdx].level.gen[genType], genType );				
			}


			NET_SendMessage(&pc->sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Reply_AddSkill( playerCharacter_t *pc, int skillNumber, int skillIdx[] )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		
		MSG_WriteByte(&netMessage, GSC_REPLY_ADDSKILL);
		
		MSG_WriteByte(&netMessage, skillNumber );
		
		for( int i = 0; i < skillNumber; i ++ )
		{
			MSG_WriteByte(&netMessage, skillIdx[i] );
			MSG_WriteShort(&netMessage, pc->skill[skillIdx[i]].tableIdx );
			MSG_WriteLong(&netMessage,  pc->skill[skillIdx[i]].iLevel );
		}
		NET_SendMessage(&pc->sock, &netMessage);	
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Reply_Learn_Skill( playerCharacter_t *pc, int reply, int error, int invenPos, int skillIdx )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_LEARN_SKILL);
		MSG_WriteByte(&netMessage, reply);
		if( !reply )
		{
			MSG_WriteByte(&netMessage, error);
		}
		else
		{
			MSG_WriteByte(&netMessage, invenPos);
			MSG_WriteByte(&netMessage, skillIdx);
			MSG_WriteShort(&netMessage, pc->skill[skillIdx].tableIdx);
			MSG_WriteLong(&netMessage, pc->skill[skillIdx].iLevel);
		}
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Reply_InitSkill( playerCharacter_t* pc )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_CHEATCODE);
		MSG_WriteByte(&netMessage, CHEATCODE_INITSKILL);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void GTH_SendMessage_Reply_PCInfo( playerCharacter_t *pc )
{
	if( !pc->ready ) return;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_PCINFO );
		
		MSG_WriteLong( &netMessage, (int)pc->calMaxRA );
		MSG_WriteShort( &netMessage, (int)pc->calMaxSA );
		MSG_WriteLong( &netMessage, (int)pc->curRA );
		MSG_WriteShort( &netMessage, (int)pc->curSA );
				
		MSG_WriteShort( &netMessage, GetStausWeight( pc, STATUS_FORCE ) );
		MSG_WriteShort( &netMessage, GetStausWeight( pc, STATUS_SOULPOWER ) );
		MSG_WriteShort( &netMessage, GetStausWeight( pc, STATUS_AGILITY ) );
		MSG_WriteShort( &netMessage, GetStausWeight( pc, STATUS_VITALITY ) );
				
		
		MSG_WriteShort( &netMessage, pc->calPhyMinDmg );
		MSG_WriteShort( &netMessage, pc->calPhyMaxDmg );
		MSG_WriteShort( &netMessage, pc->calMagMinDmg );
		MSG_WriteShort( &netMessage, pc->calMagMaxDmg );
		
		MSG_WriteFloat( &netMessage, pc->calCriticalRate );
		MSG_WriteShort( &netMessage, pc->calDefensePower );
		
		MSG_WriteShort( &netMessage, pc->calPositiveRes );
		MSG_WriteShort( &netMessage, pc->calNegativeRes );
		MSG_WriteShort( &netMessage, pc->calHitPower );
		MSG_WriteShort( &netMessage, pc->calAvoidPower );
		MSG_WriteShort( &netMessage, pc->calAttackDelay );
		
		MSG_WriteFloat( &netMessage, pc->calVelocity );
		MSG_WriteShort( &netMessage, pc->calAttackLength );
		
		MSG_WriteByte( &netMessage, pc->atkPowerRACost );
		MSG_WriteByte( &netMessage, pc->atkPowerSACost );
		MSG_WriteByte( &netMessage, pc->atkBowSACost );
		
		
		MSG_WriteShort( &netMessage, pc->maxChargeRA );
		MSG_WriteShort( &netMessage, pc->maxChargeSA );
		MSG_WriteShort( &netMessage, pc->onceRecoveryRA );
		MSG_WriteShort( &netMessage, pc->onceRecoverySA );
		MSG_WriteLong( &netMessage, pc->curChargeSE );


		
		MSG_WriteShort( &netMessage,(int) pc->curChargeRA );
		MSG_WriteShort( &netMessage,(int) pc->curChargeSA );
		

		
		MSG_WriteShort( &netMessage, pc->curItemWeight );
		MSG_WriteShort( &netMessage, pc->calItemWeight );
		
		
		MSG_WriteFloat( &netMessage, pc->optValue[OV_SKILL_LENGTH_RATE] );
		MSG_WriteFloat( & netMessage, pc->optValue[OV_DECSA_USE] );
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void GTH_SendMessage_Reply_Use_Portal( playerCharacter_t *pc, int reply, int error, int invenPos, int isChangeServer )
{



	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_USE_PORTAL);
		MSG_WriteByte(&netMessage, reply);
		
		if( reply )
		{
			MSG_WriteShort(&netMessage, pc->idx);
			MSG_WriteByte(&netMessage, pc->entityType);
			
			
			MSG_WriteByte(&netMessage, isChangeServer);
			
			if( !isChangeServer )
			{
				MSG_WriteByte(&netMessage, pc->worldIdx);
				MSG_WritePosition(&netMessage, pc->position);
				MSG_WriteFloat(&netMessage, pc->angles[1]);
				MSG_WriteLong(&netMessage, (int)pc->curRA);
				MSG_WriteShort(&netMessage, (int)pc->curSA);
				
				MSG_WriteByte(&netMessage, invenPos);
			}
			else
			{
				MSG_WriteByte(&netMessage, invenPos);
			}
			
			NET_SendMessage(&pc->sock, &netMessage);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
			NET_SendMessage(&pc->sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Reply_Use_Potion(playerCharacter_t* pc, int error, int invenPos)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_USE_POTION);		
		MSG_WriteByte(&netMessage, invenPos);							
		
		MSG_WriteByte(&netMessage, error);

		NET_SendMessage(&pc->sock, &netMessage);		
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Reply_SavePortalPosition( playerCharacter_t *pc, int reply, int error )
{
	
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_SAVE_PORTAL_POSITION);
		MSG_WriteByte(&netMessage, reply);
		if( reply )
		{
			MSG_WriteShort( &netMessage, pc->skillVariable[0] );		
			MSG_WriteShort( &netMessage, pc->skillVariable[1] );		
			MSG_WriteShort( &netMessage, pc->skillVariable[2] );		
			MSG_WriteShort( &netMessage, pc->skillVariable[3] );		
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void GTH_ProcessMessage_DelQuestStatus( playerCharacter_t *pc )
{
	int	idx = MSG_ReadShort();

	
	GTH_ClearQuestStatus( pc, idx, 0 );
}

void GTH_SendMessage_QuestStatusInfo( playerCharacter_t *pc, int type, int questIdx )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_QUESTSTATUS_INFO );
		
		
		
		
		MSG_WriteByte(&netMessage, type);
		MSG_WriteShort(&netMessage, questIdx);
		
		MSG_WriteByte(&netMessage, pc->questNumber );
		
		for( int i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
		{
			MSG_WriteByte( &netMessage, pc->questStatus[i].idx );
			MSG_WriteShort( &netMessage,pc->questStatus[i].questIdx );
			MSG_WriteLong( &netMessage, pc->questStatus[i].clearTime );
			MSG_WriteLong( &netMessage, pc->questStatus[i].limitTime );
			
			MSG_WriteString( &netMessage, pc->questStatus[i].partyOrganizerName );
			MSG_WriteChar( &netMessage, pc->questStatus[i].partyOrganizeServer );
			MSG_WriteLong( &netMessage, pc->questStatus[i].partyOrganizeTime );
			
			for( int j = 0; j < MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD; j ++ )
			{
				MSG_WriteLong( &netMessage, pc->questStatus[i].dField[j] );
			}
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}



void GTH_SendMessage_AutoQuestStatusInfo( playerCharacter_t *pc, int type, int questIdx )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_AUTOQUESTSTATUS_INFO );
		
		
		
		
		MSG_WriteByte(&netMessage, type);
		MSG_WriteShort(&netMessage,questIdx);
		
		MSG_WriteByte(&netMessage, pc->questNumber );
		
	
		for( int i = 0; i <MAX_COUNT_QUEST_STATUS; i ++ )
			{
				MSG_WriteByte( &netMessage, pc->questStatus[i].idx );
				MSG_WriteShort( &netMessage,pc->questStatus[i].questIdx );
				MSG_WriteLong( &netMessage, pc->questStatus[i].clearTime );
				MSG_WriteLong( &netMessage, pc->questStatus[i].limitTime );
				
			
			
			
				
				for( int j = 0; j < MAX_COUNT_QUEST_STATUS_DYNAMIC_FIELD; j ++ )
				{
					MSG_WriteLong( &netMessage, pc->questStatus[i].dField[j] );
				}

				
				if(pc->questStatus[i].idx !=-1 && pc->questStatus[i].questIdx !=-1)
				{
					ShowLogInfo("autoqueststatusInfo: userID:%s,questIdx:%d,type:%d",pc->name,pc->questStatus[i].dField[0],type);
				}
			}
	
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void GTH_SendMessage_QuestStep(playerCharacter_t *pc, int questIdx, int questStep, int completeFlag )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_QUEST_STEP );
		MSG_WriteShort( &netMessage, questIdx );
		MSG_WriteShort( &netMessage, questStep );
		MSG_WriteByte( &netMessage, completeFlag );
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_QuestItemInfo( playerCharacter_t *pc, int itemTableIdx, int itemNumber )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_QUEST_ITEM_INFO );
		
		MSG_WriteShort( &netMessage, itemTableIdx );
		MSG_WriteShort( &netMessage, itemNumber );
		
		for( int i = 0; i < MAX_COUNT_QUEST_INVENTORY; i ++ )
		{
			MSG_WriteLong(&netMessage, pc->questInventory[i].iItem );
		}
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}





void GTH_SendMessage_ItemSystem_DepotOpen( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, DEPOT_OPEN );
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_DepotGetItem( playerCharacter_t *pc, int reply, int error, int depotPos )
{
	int itemIdx;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, DEPOT_GETITEM );
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, depotPos);
			
			if( reply == 5 )
			{
				MSG_WriteByte(&netMessage, pc->mouseInventory);
				itemIdx = pc->depot[depotPos];
				MSG_WriteItem( &netMessage, &pc->item[itemIdx] );
				MSG_WriteItem( &netMessage, &pc->item[pc->mouseInventory] );
			}
		}
		else
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_DepotItem( playerCharacter_t *pc, int reply, int error, int depotPos )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, DEPOT_ITEM );
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, depotPos);
			MSG_WriteLong(&netMessage, pc->curChargeSE );
		}
		else
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);

	
	GTH_PC_Calculate( pc );
}

void GTH_SendMessage_ItemSystem_DepotNak( playerCharacter_t *pc, int reply, int error )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, DEPOT_NAK );
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteLong(&netMessage, pc->curChargeSE);
			MSG_WriteLong(&netMessage, pc->depotSE );
		}
		else
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}






void GTH_SendMessage_ItemSystem_PrecocityOpen( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, PRECOCITY_OPEN );
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_ItemSystem_PrecocityItem( playerCharacter_t *pc, int reply, int error, int pos )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, PRECOCITY_ITEM);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, pos);
			MSG_WriteShort( &netMessage, pc->mouseInventory );
			MSG_WriteShort( &netMessage, pc->precocityInventory[pos] );
			MSG_WriteLong( &netMessage, pc->precocityTime[pos] );
		}	
		else
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_PrecocityComplete( playerCharacter_t *pc, int pos )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, PRECOCITY_COMPLETE);
		
		MSG_WriteByte(&netMessage, pos);
		MSG_WriteItem(&netMessage, &pc->item[pc->precocityInventory[pos]]);
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}






void GTH_SendMessage_ItemSystem_EventCraftOpen( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EVENT_CRAFT_OPEN);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_EventCraftClose( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EVENT_CRAFT_CLOSE);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}
 
void GTH_SendMessage_ItemSystem_EventCraftItem( playerCharacter_t *pc, int reply, int error, int posType, int pos )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EVENT_CRAFT_ITEM);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, reply);
			
			MSG_WriteChar(&netMessage, posType);
			MSG_WriteByte(&netMessage, pos);
			
			MSG_WriteShort( &netMessage, pc->mouseInventory );
			MSG_WriteShort( &netMessage, pc->craftSourceInventory[pos] );
		}	
		else
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_EventCraftPrepare( playerCharacter_t *pc, int reply, int error )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EVENT_CRAFT_PREPARE);
		
		MSG_WriteByte(&netMessage, reply);
		
		if(! reply )
		{
			MSG_WriteByte( &netMessage, error );
		}

		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_EventCraftComplete( playerCharacter_t *pc, int reply, int error ,int craftIdx)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EVENT_CRAFT_COMPLETE);
		
		MSG_WriteByte(&netMessage, reply);
		MSG_WriteByte(&netMessage, craftIdx);
				
		if( !reply )
		{
			MSG_WriteByte( &netMessage, error );			
		}
		else
		{
			MSG_WriteLong(&netMessage, g_curPC->curChargeSE);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);

	if( reply )
	{
		GTH_SendMessage_CompleteEffect_To_AllPC( pc, 35 );		
	}

}

void GTH_SendMessage_Reply_Load_EventCraftItem( playerCharacter_t *pc, short sEventIdx , int n1PointCnt, int n3PointCnt,int nTotalPoint )
{

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, EVENT_CRAFT_LOADEVENT);
		
		MSG_WriteShort(&netMessage, sEventIdx);
		
		MSG_WriteLong(&netMessage, n1PointCnt);
		MSG_WriteLong(&netMessage, n3PointCnt);
		MSG_WriteLong(&netMessage, nTotalPoint);

		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}










void GTH_SendMessage_ItemSystem_CraftOpen( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, CRAFT_OPEN);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_NpcInventoryOpen( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, AUTOQUESTSCROLL_NPCINVENTORYOPEN);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_NpcInventoryClose( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, AUTOQUESTSCROLL_NPCINVENTORYCLOSE);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_CraftClose( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, CRAFT_CLOSE);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_CraftItem( playerCharacter_t *pc, int reply, int error, int posType, int pos )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, CRAFT_ITEM);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, reply);
			
			MSG_WriteChar(&netMessage, posType);
			MSG_WriteByte(&netMessage, pos);
			
			MSG_WriteShort( &netMessage, pc->mouseInventory );
			if( posType == 0 ) MSG_WriteShort( &netMessage, pc->craftSourceInventory[pos] );
			else MSG_WriteShort( &netMessage, pc->craftResultInventory[pos] );
		}	
		else
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_AutoQuestCraftItem( playerCharacter_t *pc, int reply, int error, int posType, int pos )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, AUTOQUESTSCROLL_NPCINVENTORYITEM);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, reply);
			
			MSG_WriteChar(&netMessage, posType);
			MSG_WriteByte(&netMessage, pos);
			
			MSG_WriteShort( &netMessage, pc->mouseInventory );
			if( posType == 0 ) MSG_WriteShort( &netMessage,  pc->npcAutoSourceInventory[pos]  );
			else MSG_WriteShort( &netMessage, pc->npcAutoSourceInventory[pos] );
		}	
		else
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_CraftPrepare( playerCharacter_t *pc, int reply, int error, int resultItemIdx[], int nak, float pro ,int craftIdx )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, CRAFT_PREPARE);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply )
		{
			MSG_WriteShort( &netMessage, resultItemIdx[0] );
			MSG_WriteShort( &netMessage, resultItemIdx[1] );
			MSG_WriteLong( &netMessage, nak );
			MSG_WriteFloat( &netMessage, pro );
			MSG_WriteLong( &netMessage,craftIdx);			
		}
		else
		{
			MSG_WriteByte( &netMessage, error );
			MSG_WriteLong( &netMessage, nak );
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_ItemSystem_AutoCraftPrepare( playerCharacter_t *pc, int &reply, int &error)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, AUTOQUESTSCROLL_NPCINVENTORYPREPARE);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( !reply )
		{
			MSG_WriteByte( &netMessage, error );
		}
	
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_CraftComplete( playerCharacter_t *pc, int reply, int error, int nak )
{
	int success = false;
	item_t item1, item2;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, CRAFT_COMPLETE);
		
		MSG_WriteByte(&netMessage, reply);
		
		InitItem( &item1 );
		InitItem( &item2 );
		
		if( reply )
		{
			if( pc->craftResultInventory[0] >= 0 )
			{
				memcpy( &item1, &pc->item[pc->craftResultInventory[0]], sizeof( item_t ) );
				success = true;
			}
			if( pc->craftResultInventory[1] >= 0 )
			{
				memcpy( &item1, &pc->item[pc->craftResultInventory[1]], sizeof( item_t ) );
				success = true;
			}
			MSG_WriteByte( &netMessage, success );
			MSG_WriteLong( &netMessage, pc->curChargeSE );
			
			MSG_WriteItem( &netMessage, &item1 );
			MSG_WriteItem( &netMessage, &item2 );
		}
		else
		{
			MSG_WriteByte( &netMessage, error );
			MSG_WriteLong( &netMessage, nak );
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);

	if( reply )
	{
		if( success )
			GTH_SendMessage_CompleteEffect_To_AllPC( pc, 35 );
		else
			GTH_SendMessage_CompleteEffect_To_AllPC( pc, 36 );
	}

}





void GTH_SendMessage_ItemSystem_BoothSellOpen( playerCharacter_t *pc, int reply, int error )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_SELL_OPEN );
		
		if( !reply )
		{
			MSG_WriteByte(&netMessage, error );
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}




void GTH_SendMessage_ItemSystem_BoothSellItem( playerCharacter_t *pc, int reply, int pos, int  ErrorIdx )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_SELL_ITEM );		
		MSG_WriteShort(&netMessage, reply);
		if ( reply )
		{
			MSG_WriteShort(&netMessage, pc->mouseInventory);		
			MSG_WriteShort(&netMessage, pos);
			MSG_WriteShort(&netMessage, pc->boothInventory[pos].itemIdx);
			MSG_WriteLong(&netMessage, pc->boothInventory[pos].price);
		}
		else		
			MSG_WriteLong(&netMessage, ErrorIdx);				

		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}



void GTH_SendMessage_ItemSystem_BoothSellStart( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_SELL_START );
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_BoothSellClose( playerCharacter_t *pc, int type )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_SELL_CLOSE );
		
		
		MSG_WriteByte(&netMessage, type );
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_BoothBuyOpen( playerCharacter_t *pc, int reply, int error, playerCharacter_t *targetPC, int type )
{
	int itemIdx;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_BUY_OPEN );
		MSG_WriteByte(&netMessage, reply );
		
		if( !reply )
		{
			MSG_WriteByte(&netMessage, error );
			NET_SendMessage(&pc->sock, &netMessage);
		}
		else
		{
			MSG_WriteByte(&netMessage, type );
			
			if( !type )
			{
				
				MSG_WriteString(&netMessage, targetPC->name );
				MSG_WriteString(&netMessage, targetPC->boothName );
				MSG_WriteString(&netMessage, targetPC->boothInfo );
				
				for( int i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
				{
					itemIdx = targetPC->boothInventory[i].itemIdx;
					MSG_WriteShort(&netMessage, itemIdx);
					if( itemIdx >= 0 )
					{
						MSG_WriteShort(&netMessage, i);
						MSG_WriteItem(&netMessage, &targetPC->item[itemIdx] );
						MSG_WriteLong(&netMessage, targetPC->boothInventory[i].price);
					}
				}
			}
			
			else
			{
				MSG_WriteString(&netMessage, targetPC->name );
			}
			
			NET_SendMessage(&pc->sock, &netMessage, TRUE);
		}
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_BoothBuyItem( playerCharacter_t *pc, int reply, int error, int pos, int itemCount, int type )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_BUY_ITEM );
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply )
		{
			MSG_WriteByte(&netMessage, type);
			MSG_WriteShort(&netMessage, pos);
			MSG_WriteShort(&netMessage, itemCount);
			MSG_WriteLong(&netMessage, pc->curChargeSE);
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);

	GTH_PC_Calculate( pc );
}

void GTH_SendMessage_ItemSystem_BoothBuyClose( playerCharacter_t *pc, int reply, int error, int type, char* targetName )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_BUY_CLOSE );
		
		MSG_WriteByte(&netMessage, reply);
		if( reply )
		{
			MSG_WriteByte(&netMessage, type);
			if( type )
			{
				MSG_WriteString( &netMessage, targetName );
			}
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_ItemSystem_BoothBuyUpdate( playerCharacter_t *pc, playerCharacter_t *seller )
{
	int itemIdx;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, BOOTH_BUY_UPDATE );
		
		for( int i = 0; i < MAX_BOOTH_INVENTORY_SIZE; i ++ )
		{
			itemIdx = seller->boothInventory[i].itemIdx;
			MSG_WriteShort(&netMessage, itemIdx);
			if( itemIdx >= 0 )
			{
				MSG_WriteShort(&netMessage, i);
				MSG_WriteItem(&netMessage, &seller->item[itemIdx] );
				MSG_WriteLong(&netMessage, seller->boothInventory[i].price);
			}
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_SendMessage_ItemSystem_UpgradeOpen( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, UPGRADE_OPEN);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_UpgradeClose( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, UPGRADE_CLOSE);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_UpgradeItem( playerCharacter_t *pc, int reply, int error, int posType, int pos )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, UPGRADE_ITEM);
		
		if( reply > 0 )
		{
			MSG_WriteByte(&netMessage, reply);
			
			MSG_WriteChar(&netMessage, posType);
			MSG_WriteByte(&netMessage, pos);
			
			MSG_WriteShort( &netMessage, pc->mouseInventory );
			if( posType == 0 ) MSG_WriteShort( &netMessage, pc->upgradeSourceInventory[pos] );
			else MSG_WriteShort( &netMessage, pc->upgradeResultInventory );
		}	
		else
		{
			MSG_WriteByte(&netMessage, reply);
			MSG_WriteByte(&netMessage, error);
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_UpgradePrepare( playerCharacter_t *pc, int reply, int error, int resultItemIdx, int nak, float pro, float vanishPro, float downGradePro, int currentGrade, int nextGrade, int optionIdx )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, UPGRADE_PREPARE);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply )
		{
			MSG_WriteShort( &netMessage, resultItemIdx );
			MSG_WriteLong( &netMessage, nak );
			MSG_WriteFloat( &netMessage, pro );
			MSG_WriteFloat( &netMessage, vanishPro );
			MSG_WriteFloat( &netMessage, downGradePro );
			MSG_WriteShort( &netMessage, currentGrade );
			MSG_WriteShort( &netMessage, nextGrade );
			
			MSG_WriteShort( &netMessage, optionIdx );
		}
		else
		{
			MSG_WriteByte( &netMessage, error );
			MSG_WriteLong( &netMessage, nak );
			MSG_WriteShort( &netMessage, currentGrade );
			MSG_WriteShort( &netMessage, nextGrade );
			
			MSG_WriteShort( &netMessage, resultItemIdx );
			MSG_WriteShort( &netMessage, optionIdx );
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_UpgradeComplete( playerCharacter_t *pc, int reply, int error, int nak, int success, int resultItemIdx, int optionIdx, int nextGrade )
{
	item_t item;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, UPGRADE_COMPLETE);
		
		MSG_WriteByte(&netMessage, reply);
		
		InitItem( &item );
		
		if( reply )
		{
			MSG_WriteByte( &netMessage, success );
			MSG_WriteLong( &netMessage, pc->curChargeSE );
			
			
			MSG_WriteShort( &netMessage, resultItemIdx );
			MSG_WriteShort( &netMessage, optionIdx );
			MSG_WriteShort( &netMessage, nextGrade );
			
			MSG_WriteShort( &netMessage, pc->upgradeResultInventory );
			
			if( pc->upgradeResultInventory >= 0 )
			{
				memcpy( &item, &pc->item[pc->upgradeResultInventory], sizeof( item_t ) );
				
				MSG_WriteItem( &netMessage, &item );
			}
		}
		else
		{
			MSG_WriteByte( &netMessage, error );
			
			MSG_WriteShort( &netMessage, resultItemIdx );
			MSG_WriteShort( &netMessage, optionIdx );
			
			MSG_WriteLong( &netMessage, nak );
		}
		
		
		NET_SendMessage(&pc->sock, &netMessage, TRUE);
	}
	MSG_EndWriting(&netMessage);

	if( reply )
	{
		if( success )
			GTH_SendMessage_CompleteEffect_To_AllPC( pc, 37 );
		else
			GTH_SendMessage_CompleteEffect_To_AllPC( pc, 38 );
	}
}


void GTH_SendMessage_ItemSystem_DeletePrepare( playerCharacter_t *pc, int reply, int error, int optionIdx )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, DELETE_PREPARE);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply )
		{
			MSG_WriteShort( &netMessage, pc->upgradeSourceInventory[0] );
			MSG_WriteByte( &netMessage, optionIdx );
		}
		else
		{
			MSG_WriteByte( &netMessage, error );
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_ItemSystem_DeleteComplete( playerCharacter_t *pc, int reply, int error, int success, int optionIdx )
{
	item_t item;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, DELETE_COMPLETE);
		
		MSG_WriteByte(&netMessage, reply);
		
		InitItem( &item );
		
		if( reply )
		{
			MSG_WriteByte( &netMessage, success );
			MSG_WriteLong( &netMessage, pc->curChargeSE );
			MSG_WriteByte( &netMessage, optionIdx );
			MSG_WriteShort( &netMessage, pc->upgradeResultInventory );
			
			if( pc->upgradeResultInventory >= 0 )
			{
				memcpy( &item, &pc->item[pc->upgradeResultInventory], sizeof( item_t ) );
				
				MSG_WriteItem( &netMessage, &item );
			}
		}
		else
		{
			MSG_WriteByte( &netMessage, error );
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void GTH_SendMessage_Friends_Invite ( playerCharacter_t * pc, int reply, int error, int idx, int type )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_FRIENDS);
		MSG_WriteByte(&netMessage, FRIENDS_INVITE);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply > 0 )
		{
			MSG_WriteShort(&netMessage, idx);
			MSG_WriteByte(&netMessage, type);
			MSG_WriteString(&netMessage, g_pc[idx].name );
		}
		else
		{
			MSG_WriteByte(&netMessage, error);
		}
		
		if ( pc->ready )
		{
			NET_SendMessage(&pc->sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Friends_Added ( playerCharacter_t *pc, char *name, int level, int job, int worldIdx )
{
	MSG_BeginWriting ( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte ( &netMessage, GSC_REPLY_FRIENDS );
		MSG_WriteByte ( &netMessage, FRIENDS_ADD );
		
		MSG_WriteByte ( &netMessage, level );
		MSG_WriteByte ( &netMessage, job );
		MSG_WriteByte ( &netMessage, worldIdx );
		MSG_WriteString ( &netMessage, name );
		
		if ( pc->ready )
		{
			NET_SendMessage ( &pc->sock, &netMessage );
		}
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Friends_Reject ( playerCharacter_t *pc, int idx )
{
	MSG_BeginWriting ( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte ( &netMessage, GSC_REPLY_FRIENDS );
		MSG_WriteByte ( &netMessage, FRIENDS_REJECT );
		MSG_WriteShort ( &netMessage, idx );
		
		if ( pc->ready )
		{
			NET_SendMessage ( &pc->sock, &netMessage );
		}
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Friends_Remove ( playerCharacter_t *pc, char *name )
{
	MSG_BeginWriting ( &netMessage );
	MSG_Clear( &netMessage );
	{
		
		MSG_WriteByte ( &netMessage, GSC_REPLY_FRIENDS );
		MSG_WriteByte ( &netMessage, FRIENDS_REMOVE );
		MSG_WriteString ( &netMessage, name );
		
		if ( pc->ready )
		{
			NET_SendMessage ( &pc->sock, &netMessage );
		}
	}
	MSG_EndWriting(&netMessage);
}






void GTH_SendMessage_Guild_Organize_Request_To_Client( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_GUILD_REQUEST );
		MSG_WriteByte( &netMessage, GUILD_ORGANIZE );
		
		if( pc->ready )
		{
			NET_SendMessage( &pc->sock, &netMessage );
			

		}
	}
	MSG_EndWriting(&netMessage);
}






void GTH_SendMessage_Guild_Organize_Reply_To_Client( playerCharacter_t *pc, int result, int error, int RemainTime )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_GUILD_REPLY );
		MSG_WriteByte( &netMessage, GUILD_ORGANIZE );
		
		MSG_WriteShort( &netMessage, result );
		MSG_WriteShort( &netMessage, error );
		MSG_WriteShort( &netMessage, pc->rankPoint );
		MSG_WriteLong( &netMessage, pc->curChargeSE );
		MSG_WriteString( &netMessage, pc->guildName );
		
		MSG_WriteLong( &netMessage, RemainTime );		
		
		if( pc->ready )
		{
			NET_SendMessage( &pc->sock, &netMessage );
			

		}
	}
	MSG_EndWriting(&netMessage);
}





void GTH_SendMessage_Guild_Disorganize_Reply_To_Client( playerCharacter_t *pc, int result, int error )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_GUILD_REPLY );
		MSG_WriteByte( &netMessage, GUILD_DISORGANIZE );
		
		MSG_WriteShort( &netMessage, result );
		MSG_WriteShort( &netMessage, error );
		MSG_WriteShort( &netMessage, pc->rankPoint );
		
		if( pc->ready )
		{
			NET_SendMessage( &pc->sock, &netMessage );
			

		}
	}
	MSG_EndWriting(&netMessage);
}




void GTH_SendMessage_Guild_Invite_Request_To_Client( playerCharacter_t *pc, int result, int error, int who, int	RemainTime )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_INVITE_REQUEST);
		
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, who);
		
		MSG_WriteLong(&netMessage,  RemainTime);
		
		
		if( who == 0 )	
			MSG_WriteString(&netMessage, pc->tempGuildMateName);
		else			
			MSG_WriteString(&netMessage, pc->guildName);
		
		if( pc->ready )
		{
			NET_SendMessage(&pc->sock, &netMessage);
			

		}
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Invite_Cancel_To_Client( playerCharacter_t *pc, int error, int who, char *name )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_INVITE_CANCEL);
		
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, who);
		
		
		MSG_WriteString(&netMessage, name);
		
		if( pc->ready )
		{
			NET_SendMessage(&pc->sock, &netMessage);
			

		}
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Invite_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_INVITE_REPLY);
		
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, who);
		MSG_WriteString(&netMessage, name);
		
		if( pc->ready )
		{
			NET_SendMessage(&pc->sock, &netMessage);
			

		}
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Secede_Reply_To_Client( playerCharacter_t *pc, int result, int error )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SECEDE);
		
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, pc->rankPoint);
		
		if( pc->ready )
		{
			NET_SendMessage(&pc->sock, &netMessage);
			

		}
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Banish_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_BANISH);
		
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, who);
		MSG_WriteShort(&netMessage, pc->rankPoint);
		MSG_WriteString(&netMessage, name);
		
		if( pc->ready )
		{
			NET_SendMessage(&pc->sock, &netMessage);
			

		}
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Change_Master_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_CHANGEMASTER);
		
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, who);
		MSG_WriteString(&netMessage, name);
		
		if( pc->ready )
		{
			NET_SendMessage(&pc->sock, &netMessage);
			

		}
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Add_SubMaster_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_ADDSUBMASTER);
		
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, who);
		MSG_WriteString(&netMessage, name);
		
		if( pc->ready )
		{
			NET_SendMessage(&pc->sock, &netMessage);
			

		}
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Add_FactionMaster_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_ADDFACTIONMASTER);
		
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, who);
		MSG_WriteString(&netMessage, name);
		
		if( pc->ready )
		{
			NET_SendMessage(&pc->sock, &netMessage);
			

		}
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Delete_Master_Reply_To_Client( playerCharacter_t *pc, int error, char *name )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_DELETE_MASTER);
		
		MSG_WriteShort(&netMessage, error);
		MSG_WriteString(&netMessage, name);
		
		if( pc->ready )
		{
			NET_SendMessage(&pc->sock, &netMessage);
			

		}
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Remove_SubMaster_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_REMOVESUBMASTER);
		
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, who);
		MSG_WriteString(&netMessage, name);
		
		if( pc->ready )
		{
			NET_SendMessage(&pc->sock, &netMessage);
			

		}
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Remove_FactionMaster_Reply_To_Client( playerCharacter_t *pc, int result, int error, int who, char *name )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_REMOVEFACTIONMASTER);
		
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		MSG_WriteShort(&netMessage, who);
		MSG_WriteString(&netMessage, name);
		
		if( pc->ready )
		{
			NET_SendMessage(&pc->sock, &netMessage);
			

		}
	}
	MSG_EndWriting(&netMessage);
}








void GTH_SendMessage_Guild_GuildMark_Reply_To_Client(
	playerCharacter_t*	pc,
	const int			guildIdx, 
	const int			markChnageCnt, 
	unsigned char*		pMarkImage)
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_GUILDMARK);
		
		MSG_WriteShort(&netMessage, guildIdx);
		MSG_WriteLong(&netMessage, markChnageCnt);
		
		for( int i=0; i<GUILDMARKSIZE; i++ )
			MSG_WriteByte(&netMessage, pMarkImage[i]);
		
		NET_SendMessage(&pc->sock, &netMessage);
		

	}
	MSG_EndWriting(&netMessage);
}













void GTH_SendMessage_Guild_Subscribe_Reply_To_Client( playerCharacter_t *pc, int result, int error, char *guildName )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_SUBSCRIBE);
		
		MSG_WriteShort(&netMessage, result);
		MSG_WriteShort(&netMessage, error);
		
		if( result != -1 )
			MSG_WriteString(&netMessage, guildName);
		
		NET_SendMessage(&pc->sock, &netMessage);
		

	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_Update_To_Client( playerCharacter_t *pc, guild_t* guild, int update, int rank, int rankIdx )
{
	int i;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_GUILD_REPLY);
		MSG_WriteByte(&netMessage, GUILD_UPDATE);
		
		MSG_WriteShort(&netMessage, guild->idx);
		MSG_WriteLong(&netMessage, update);
		MSG_WriteLong(&netMessage, rank);
		MSG_WriteLong(&netMessage, rankIdx);
		
		int addRankIdx = 0;
		
		if( rank == GUILD_SUBMASTER )
			addRankIdx = MAX_GUILD_MASTERS;
		else if( rank == GUILD_FACTIONMASTER )
			addRankIdx = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS;
		else if( rank == GUILD_MATE )
			addRankIdx = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS;
		else
			addRankIdx = 0;
		
		if( ( update & GUILD_IDX ) != 0 )
		{
			MSG_WriteShort(&netMessage, guild->idx);
		}
		
		if( ( update & GUILD_SERVERGROUPID ) != 0 )
		{
			MSG_WriteShort(&netMessage, guild->serverGroupID);
		}
		
		if( ( update & GUILD_NAME ) != 0 )
		{
			MSG_WriteString(&netMessage, guild->name);
		}
		
		if( ( update & GUILD_MARK ) != 0 )
		{
			
			
		}
		
		if( ( update & GUILD_MARKUPDATETIME ) != 0 )
		{
			MSG_WriteLong(&netMessage, guild->markUpdateTime);
		}
		
		if( ( update & GUILD_NOTICE ) != 0 )
		{
			MSG_WriteString(&netMessage, guild->notice);
		}
		
		if( ( update & GUILD_FAMEPOINT ) != 0 )
		{
			MSG_WriteLong(&netMessage, guild->famePoint);
		}
		
		if( ( update & GUILD_EXPPOINT ) != 0 )
		{
			MSG_WriteLong(&netMessage, guild->expPoint);
		}
		
		if( ( update & GUILD_RAIDPOINT ) != 0 )
		{
			MSG_WriteLong(&netMessage, guild->raidPoint);
		}
		
		if( ( update & GUILD_PVPPOINT ) != 0 )
		{
			MSG_WriteLong(&netMessage, guild->pvpPoint);
		}
		
		if( ( update & GUILD_HUNTERPOINT ) != 0 )
		{
			MSG_WriteLong(&netMessage, guild->hunterPoint);
		}
		
		if( ( update & GUILD_LEVEL ) != 0 )
		{
			MSG_WriteShort(&netMessage, guild->level);
		}
		
		if( ( update & GUILD_MASTERNAME ) != 0 )
		{
			MSG_WriteString(&netMessage, guild->masterName);
		}
		
		if( ( update & GUILD_SUBMASTERNAME ) != 0 )
		{
			if( rankIdx == -1)
			{
				for( i=0; i<MAX_GUILD_SUBMASTERS; i++ )
					MSG_WriteString(&netMessage, guild->subMasterName[i]);
			}
			else
				MSG_WriteString(&netMessage, guild->subMasterName[rankIdx]);
		}
		
		if( ( update & GUILD_FACTIONMASTERNAME ) != 0 )
		{
			if( rankIdx == -1)
			{
				for( i=0; i<MAX_GUILD_FACTIONMASTERS; i++ )
					MSG_WriteString(&netMessage, guild->factionMasterName[i]);
			}
			else
				MSG_WriteString(&netMessage, guild->factionMasterName[rankIdx]);
		}
		
		if( ( update & GUILD_MATENAME ) != 0 )
		{
			if( rankIdx == -1)
			{
				for( i=0; i<MAX_GUILD_MATES; i++ )
					MSG_WriteString(&netMessage, guild->mateName[i]);
			}
			else
				MSG_WriteString(&netMessage, guild->mateName[rankIdx]);
		}
		
		if( ( update & GUILD_MATEAUTHORITY ) != 0 )
		{
			if( rankIdx == -1)
			{
				for( i=0; i<MAX_GUILD_MATES; i++ )
					MSG_WriteByte(&netMessage, guild->mateAuthority[i]);
			}
			else
				MSG_WriteByte(&netMessage, guild->mateAuthority[rankIdx]);
		}
		
		if( ( update & GUILD_MATELASTCONNECTTIME ) != 0 )
		{
			if( rankIdx == -1)
			{
				for( i=0; i<MAX_GUILD_MATES; i++ )
					MSG_WriteLong(&netMessage, guild->mateLastConnectTime[i]);
			}
			else
				MSG_WriteLong(&netMessage, guild->mateLastConnectTime[rankIdx]);
		}
		
		if( ( update & GUILD_MASTERNOTE ) != 0 )
		{
			if( rankIdx == -1)
			{
				
				
			}
			else
				MSG_WriteString(&netMessage, guild->masterNote[rankIdx]);
		}
		
		if( ( update & GUILD_USERLEVEL ) != 0 )
		{
			if( rankIdx == -1 )
			{
				int max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;
				
				for( i=0; i<max; i++ )
					MSG_WriteLong(&netMessage, guild->userLevel[i]);
			}
			else
				MSG_WriteLong(&netMessage, guild->userLevel[rankIdx+addRankIdx]);
		}
		
		if( ( update & GUILD_USERJOB ) != 0 )
		{
			if( rankIdx == -1 )
			{
				int max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;
				
				for( i=0; i<max; i++ )
					MSG_WriteLong(&netMessage, guild->userJob[i]);
			}
			else
				MSG_WriteLong(&netMessage, guild->userJob[rankIdx+addRankIdx]);
		}
		
		if( ( update & GUILD_USERLOCATION ) != 0 )
		{
			if( rankIdx == -1 )
			{
				int max = MAX_GUILD_MASTERS+MAX_GUILD_SUBMASTERS+MAX_GUILD_FACTIONMASTERS+MAX_GUILD_MATES;
				
				for( i=0; i<max; i++ )
					MSG_WriteLong(&netMessage, guild->userLocation[i]);
			}
			else
				MSG_WriteLong(&netMessage, guild->userLocation[rankIdx+addRankIdx]);
		}
		
		if( pc->ready )
		{
			
			
			
			
			
			
			
			
			NET_SendMessage(&pc->sock, &netMessage);
			

		}
	}
	MSG_EndWriting(&netMessage);
}




void GTH_SendMessage_Guild_Message_To_Client( playerCharacter_t *pc, short guildIndex, char *message )
{
	char buffer[64];

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_CHATMESSAGE);
		MSG_WriteByte(&netMessage, CHATMSG_TYPE_DAN );
		MSG_WriteString(&netMessage, pc->name);
		
		memset( buffer, 0, sizeof(buffer) );
		sprintf( buffer, "%s, %d", message, guildIndex );
		MSG_WriteString(&netMessage, buffer);
		
		if( pc->ready )
		{
			NET_SendUnreliableMessage(&pc->sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_Guild_Organize_Request_From_Client( playerCharacter_t *pc )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		
		MSG_WriteByte(&netMessage, GSC_GUILD_REQUEST);
		MSG_WriteByte(&netMessage, GUILD_ORGANIZE_REQUEST);
		
		if( pc->ready )
		{
			NET_SendUnreliableMessage(&pc->sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Guild_List_Request_From_Client( playerCharacter_t *pc )
{
	char buffer[128];

	int i;
	for( i = 0; i < MAX_GUILDS; i++ )
	{
		if( g_guild[i].name[0] != 0 && g_guild[i].idx != -1 )
		{
			MSG_BeginWriting(&netMessage);
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte(&netMessage, GSC_CHATMESSAGE);
				MSG_WriteByte(&netMessage, CHATMSG_TYPE_DAN);		
				MSG_WriteString(&netMessage, pc->name);
				
				memset( buffer, 0, sizeof(buffer) );
				sprintf( buffer, "%s, %d", g_guild[i].name, g_guild[i].idx );
				MSG_WriteString(&netMessage, buffer);
				
				if( pc->ready )
				{
					NET_SendUnreliableMessage(&pc->sock, &netMessage);
				}
			}
			MSG_EndWriting(&netMessage);
		}
	}
}


void GTH_SendMessage_Guild_ChatMessage_Reply_To_Client( playerCharacter_t *pc, char *name, char *chatMessage )
{
	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_CHATMESSAGE);
		MSG_WriteByte(&netMessage, CHATMSG_TYPE_DAN);		
		MSG_WriteString(&netMessage, name);
		MSG_WriteString(&netMessage, chatMessage);
		
		if( pc->ready )
		{
			NET_SendUnreliableMessage(&pc->sock, &netMessage);
		}
	}
	MSG_EndWriting(&netMessage);
}



bool CheckLevel( int level, int *dptList, int otherClass )
{
	bool ret = false;

	for( int i=0; i<g_questscrollTableNumber; i++ )
	{
		if ( g_questscrollTable[i].data.level == level )
		{
			for (int j=0; j<3; j++)
			{
				if ( j == otherClass )
					continue;

				ret = true;

				if ( g_questscrollTable[i].data.scrollType == j )
				{
					dptList[j] = 1;
				}
			}
		}
	}

	return ret;
}

void GTH_SendMessage_QuestScroll_ObtainOpen( playerCharacter_t *pc, int openFlag )
{
	int lvlList[3] = {0, 0, 0};
	int dptList[3] = {0, 0, 0};	

	if ( openFlag > 0 )
	{
		int otherClass = 0;

		switch ( pc->pcClass )
		{
		case GEN_MENTALITY :
		case GEN_WIND :
			otherClass = 2;
			break;
		case GEN_WATER :
		case GEN_EARTH :
			otherClass = 1;
			break;
		}

		int level;

		level = pc->level;

		for( int i=0; i<3; i++ )
		{
			if ( level<11 )
				break;
			CheckLevel( level, dptList, otherClass );
			level--;
		}

		for( i=0; i<3; i++ )
		{
			if ( dptList[i] != 0 )
			{
				int lvl = pc->level;

				for( int j=0; j<3; j++ )
				{
					if ( lvl<11 )
					break;

					for ( int k=0; k<g_questscrollTableNumber; k++ )
					{
						if ( g_questscrollTable[k].data.level == lvl && g_questscrollTable[k].data.scrollType == i )
						{
							lvlList[j] = lvl;
						}
					}
					lvl--;
				}

				break;
			}
		}

		if ( i == 3 )
			openFlag = 2;
	}

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, QUESTSCROLL_OBTAINOPEN);
		
		MSG_WriteByte(&netMessage, openFlag);
		
		MSG_WriteByte(&netMessage, lvlList[0]);
		MSG_WriteByte(&netMessage, lvlList[1]);
		MSG_WriteByte(&netMessage, lvlList[2]);
		
		MSG_WriteByte(&netMessage, dptList[0]);
		MSG_WriteByte(&netMessage, dptList[1]);
		MSG_WriteByte(&netMessage, dptList[2]);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_QuestScroll_ObtainPrepare( playerCharacter_t *pc, int reply, int error, int reqNak, int tableIdx )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, QUESTSCROLL_OBTAINPREPARE);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply )
		{
			MSG_WriteShort( &netMessage, tableIdx );
			MSG_WriteLong( &netMessage, reqNak );
		}
		else
		{
			MSG_WriteByte( &netMessage, error );
			MSG_WriteShort( &netMessage, tableIdx );
			MSG_WriteLong( &netMessage, reqNak );
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_QuestScroll_ObtainComplete( playerCharacter_t *pc, int reply, int error,int reqNak, int tableIdx )
{
	int success = false;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, QUESTSCROLL_OBTAINCOMPLETE);
		
		MSG_WriteByte(&netMessage, reply);
		
		if( reply )
		{
			MSG_WriteShort( &netMessage, tableIdx );
			MSG_WriteLong( &netMessage, pc->curChargeSE );
		}
		else
		{
			MSG_WriteByte( &netMessage, error );
			MSG_WriteShort( &netMessage, tableIdx );
			MSG_WriteLong( &netMessage, reqNak );
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_QuestScroll_ObtainClose( playerCharacter_t *pc )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, QUESTSCROLL_OBTAINCLOSE);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_QuestScrollComplete( playerCharacter_t *pc, int type, int reward, int itemNumber, int rankPoint, int nextScroll )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte( &netMessage, QUESTSCROLL_COMPLETE );
		
		MSG_WriteByte(&netMessage, type );
		MSG_WriteLong(&netMessage, reward);
		MSG_WriteShort(&netMessage, itemNumber);
		MSG_WriteShort(&netMessage, rankPoint);
		
		if( nextScroll < 0 )
			MSG_WriteByte( &netMessage, 0 );
		else
			MSG_WriteByte( &netMessage, 1 );
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	GTH_SendMessage_CompleteEffect_To_AllPC( pc, 133 );
}


void GTH_SendMessage_AutoQuestScrollComplete( playerCharacter_t *pc, int questindex)
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte( &netMessage, AUTOQUESTSCROLL_COMPLETE );
		
		MSG_WriteByte(&netMessage, questindex );
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_QuestScroll_ComboListChange( playerCharacter_t *pc, int depart )
{
	int lvlList[3] = {0,0,0};

	int level = pc->level;

	for( int i=0; i<3; i++ )
	{
		if ( level<11 )
		break;

		for ( int j=0; j<g_questscrollTableNumber; j++ )
		{
			if ( g_questscrollTable[j].data.level == level && g_questscrollTable[j].data.scrollType == depart )
			{
				lvlList[i] = level;
			}
		}
		level--;
	}

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, QUESTSCROLL_COMBOLISTCHANGE);
		
		MSG_WriteByte(&netMessage, lvlList[0]);
		MSG_WriteByte(&netMessage, lvlList[1]);
		MSG_WriteByte(&netMessage, lvlList[2]);
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}






void GTH_SendMessage_Reply_SendMail( playerCharacter_t *pc, int errorCode, char *toName )
{
	int reply = 1, error = 0;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_EXTEND );
		MSG_WriteByte( &netMessage, GSC_MAILSYSTEM );
		MSG_WriteByte( &netMessage, MAIL_SEND );
		
		MSG_WriteString( &netMessage, toName );					
		
		if( errorCode == 0 )
		{
			MSG_WriteByte( &netMessage, reply );
		}
		else
		{
			reply	=	0;
			error	=	errorCode;
			
			MSG_WriteByte( &netMessage, reply );
			MSG_WriteByte( &netMessage, error );
		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);

	if( reply )
	{
		
		if( g_config.isManager )
		{
			GTH_MailSystem_Find_Receiver( toName, pc->name );
		}
		else
		{
			MSG_BeginWriting( &netMessage );
			MSG_Clear( &netMessage );
			{
				MSG_WriteByte( &netMessage, GSC_EXTEND );
				MSG_WriteByte( &netMessage, GSC_MAIL_REQUEST );
				MSG_WriteByte( &netMessage, MAIL_ARRIVE );
				
				MSG_WriteString( &netMessage, toName );						
				MSG_WriteString( &netMessage, pc->name );					
				
				NET_SendUnreliableMessage( &localSocket, &netMessage );
			}
			MSG_EndWriting(&netMessage);
		}
	}
}


void GTH_SendMessage_Reply_RecvMail( playerCharacter_t *pc, int mailCount, MailEntry *mail )
{
	int			i;
	MailEntry	*pMail;

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_EXTEND );
		MSG_WriteByte( &netMessage, GSC_MAILSYSTEM );
		MSG_WriteByte( &netMessage, MAIL_RECEIVE );
		
		MSG_WriteByte( &netMessage, mailCount );
		
		pMail = &mail[0];
		for( i = 0; i < mailCount; i ++, pMail ++ )
		{
			MSG_WriteByte( &netMessage, pMail->mailIdx );
			MSG_WriteString( &netMessage, pMail->fromName );
			MSG_WriteString( &netMessage, pMail->mailTitle );
			MSG_WriteString( &netMessage, pMail->mailTime );
			MSG_WriteByte( &netMessage, pMail->mailState );
			

		}
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Reply_GetMailState( playerCharacter_t *pc, int notReadNumber, int readNumber, int s_notReadNumber, int s_readNumber )
{
	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_EXTEND );
		MSG_WriteByte( &netMessage, GSC_MAILSYSTEM );
		MSG_WriteByte( &netMessage, MAIL_STATE );
		
		MSG_WriteByte( &netMessage, notReadNumber );
		MSG_WriteByte( &netMessage, readNumber );
		MSG_WriteByte( &netMessage, s_notReadNumber );
		MSG_WriteByte( &netMessage, s_readNumber );
		
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_Reply_ViewMail( playerCharacter_t *pc, MailEntry *mail )
{

	MSG_BeginWriting( &netMessage );
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte( &netMessage, GSC_EXTEND );
		MSG_WriteByte( &netMessage, GSC_MAILSYSTEM );
		MSG_WriteByte( &netMessage, MAIL_VIEW );
		
		MSG_WriteByte( &netMessage, mail->mailIdx );
		MSG_WriteString( &netMessage, mail->fromName );
		MSG_WriteString( &netMessage, mail->mailTitle );
		MSG_WriteString( &netMessage, mail->mailTime );
		MSG_WriteString( &netMessage, mail->mailStr );
		MSG_WriteByte( &netMessage, mail->mailState );
		
		NET_SendMessage(&pc->sock, &netMessage);
		

	}
	MSG_EndWriting(&netMessage);
}



void GTH_SendMessage_AutoQuestScroll_ObtainOpen( playerCharacter_t* pPlayer, const int& autoquestindex) 
{
	
	MSG_BeginWriting( &netMessage );

	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, AUTOQUESTSCROLL_OBTAINOPEN);
			MSG_WriteByte(&netMessage, autoquestindex);
		NET_SendMessage(&pPlayer->sock, &netMessage);
		}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_AutoQuest_Error(playerCharacter_t* pPlayer,const int& error)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
		{
		MSG_WriteByte(&netMessage, GSC_REPLY_ITEMSYSTEM );
		MSG_WriteByte(&netMessage, AUTOQUESTSCROL_ERROR);
			MSG_WriteByte( &netMessage, error );
		NET_SendMessage(&pPlayer->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}

void GTH_SendMessage_AutoQuestScroll_effectisvalid( playerCharacter_t *pc,int &reply,int type) 
{
	MSG_BeginWriting( &netMessage );

	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_ANSWER_AUTOEFFECT );
		MSG_WriteByte(&netMessage, type);
		
		MSG_WriteByte(&netMessage, reply);
	
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage); 
}




void GTH_SendMessage_ChatBanFlagAnswer(playerCharacter_t *pc)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_ANSWER_CHATBAN_FLAG);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage); 
}


void GTH_SendMessag_ChaosValue()
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_ANSWER_CHAOVALUE);
		NET_SendMessage(&g_curPC->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage); 
}


void GTH_SendMessage_AddChaosInfo(playerCharacter_t *pc,int chaosvalue,bool sendclass,bool sendspeed)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SEND_ADDCHAOSINFO);
		MSG_WriteLong(&netMessage,chaosvalue);
		MSG_WriteLong(&netMessage,pc->chaosPoint);
		MSG_WriteShort(&netMessage,sendclass);
		MSG_WriteShort(&netMessage,sendspeed);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_DecChaosInfo(playerCharacter_t *pc,int classtype)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_SEND_DECCHAOSINFO);
		MSG_WriteShort(&netMessage,classtype);
		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}



void GTH_SendMessage_Reply_Use_Masic_Book(playerCharacter_t *pc, int reply, int error, int invenPos)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_EXTEND_SEND_USE_MASIC_BOOK);
		MSG_WriteByte(&netMessage,reply);
		MSG_WriteByte(&netMessage,error);
		MSG_WriteByte(&netMessage,invenPos);

		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}


void GTH_SendMessage_ReplayEpack_ChargSoul(playerCharacter_t *pc, int reply, int error, int invenpos,int EpkRemainCount)
{
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, GSC_EXTEND);
		MSG_WriteByte(&netMessage, GSC_REPLY_USE_EPACK_CHARG_SOUL);
		MSG_WriteLong(&netMessage, reply);
		if ( reply == 1)
		{
			MSG_WriteByte(&netMessage, invenpos);

			
			MSG_WriteLong(&netMessage, (int)pc->curChargeRA);
			MSG_WriteLong(&netMessage, (int)pc->curChargeSA);
			

			
			MSG_WriteShort(&netMessage,EpkRemainCount);
		}
		
		else
			MSG_WriteLong(&netMessage, error);

		NET_SendMessage(&pc->sock, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}



