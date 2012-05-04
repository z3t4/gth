



#include "../global.h"





CAttack::CAttack()
{

}

CAttack::~CAttack()
{

}
void GTH_PC_Attack(playerCharacter_t* pc, int charEvent)
{
	message_t message;
	int itemIdx;
	
	
	if ( GTH_IsEnableHaveDamage(pc) == FALSE)
		return;
	


	
	pc->resultValid = 0;
	pc->recoveryTime = (float)pc->calAttackDelay;

	if( pc->targetIdx < 0 || pc->targetType < 0 )
	{
		return;
	}
	
	if( pc->targetType == ENTITY_PC )
	{
		
		if(pc->idx == pc->targetIdx) return;
		
		
		
		if( pc->targetingType == TARGETING_NORMAL || pc->targetingType == TARGETING_SAFE )
		{
			AddTargetList( pc, g_pc[pc->targetIdx].name );
		}
	}

	PC_GetAttackComboType( pc );
	AI_MakeMessage( pc, &message, MESSAGE_NORMAL_ATTACK );
	if( pc->attackComboType == ATTACK_COMBO_TYPE_NEAR_RANGE )
	{
		message.data[MSG_ATKDATA_PHYSIC_DMG] = (int)( (float)message.data[MSG_ATKDATA_PHYSIC_DMG] * 0.6f );
	}
	
	
	
	AI_SendMessage( &message,NULL );

	itemIdx = GTH_PC_DecreaseDurability( pc, 0, message.data[2] );

	if( (i3characterEvent_t)charEvent == GTH_EV_CHAR_ATTACK )
		GTH_SendPCEventMessage_Attack( pc, true, pc->resultValid, itemIdx );

	pc->event = GTH_EV_CHAR_SKIP;
}

#include "../Tools/CTools.h"
extern CTools* gcpTools;
void GTH_ProcessEventMessage_Attack()
{
	float angle;
	int damageAttackFlag, charEvent;

	g_curPC->event = GTH_EV_CHAR_ATTACK;
	g_curPC->targetIdx = MSG_ReadShort(); 
	g_curPC->targetType = MSG_ReadByte();

	damageAttackFlag = MSG_ReadByte();

	MSG_ReadPosition( g_curPC->position );

	angle = MSG_ReadFloat();
	g_curPC->angles[ YAW ] = angle;

	charEvent = MSG_ReadByte();
	//lucky 2012 no range hack patch 
 	if (g_curPC->calAttackLength != MSG_ReadShort())
	{
		g_logSystem->WriteToHackingLog("[HACKING_SKILL] UserID:[%s]  char:[%s] ·Tryed to No range attak with normal attack.",g_curPC->userID,g_curPC->name);
		return;
	}
	//end
	//lucky 2012 speed hack
	bool speedhack = true;
	if (g_curPC->calAttackDelay != MSG_ReadLong())
 	{
		speedhack = false;
 	}
	//end	
	if ( g_curPC->targetType == ENTITY_NPC )
		return;
	//lucky 2012 speedhack patch
	if ( speedhack == false ) 
	{
		g_logSystem->WriteToHackingLog("[SPEED_HACKING] UserID:[%s] char:[%s] Tryed to speedhack ( using memory editing tools ).", g_curPC->userID, g_curPC->name); 
		return;
	}
	//end
	
	if ( g_curPC->targetType == ENTITY_PC )
	{
		if( g_curPC->targetIdx >= 0 )
		{
			if( !( g_curPC->pvpMode && g_pc[g_curPC->targetIdx].pvpMode ) )
			{
				
				if ( g_curPC->worldIdx == DAN_BATTLEMAP_NO)
				{
					if ( g_pDanBattleSys->GetStatus() != DBTL_STATUS_START &&
						g_pDanBattleSys->GetStatus() != DBTL_STATUS_EVENT)
						return;						
				}
				
				

				
				
				if ( (g_curPC->DanBattleInfo.nDanBattleIdx >= 0 ) && 
				 ( g_curPC->DanBattleInfo.nBattleMapIdx == g_curPC->worldIdx ) ){
					if (FALSE == g_NewDanBattleSystem.IsDanBattleEnemy(g_curPC->idx, g_curPC->targetIdx))
						return;

				}
				
				else{			
						if ( IsPKDisableWorld( g_curPC->worldIdx ) ) 
							return;
						
					

					playerCharacter_t* pTargetPC = gcpTools->GetPlayerRecordPointer(g_curPC->targetIdx);
					if ( pTargetPC != NULL)
						if ( pTargetPC->DanBattleInfo.nDanBattleIdx >= 0) 	return;
				}				
			}
		}
	}

	if( damageAttackFlag )
	{
		GTH_PC_Attack( g_curPC , charEvent);

		
		
		for(int i = 0; i < MAX_AFFECT; i++ )
		{
			if( g_curPC->affect[i].skillTableIdx == 44)
			{
				GTH_DeleteToggle( g_curPC, ENTITY_PC, g_curPC->affect[i].skillTableIdx );
				break;
			}
		}
	}
	else
	{
		PC_GetAttackComboType( g_curPC );
		GTH_SendPCEventMessage_Attack( g_curPC, 0, 0, 0 );
	}
}
